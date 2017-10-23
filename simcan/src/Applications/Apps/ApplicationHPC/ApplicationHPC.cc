#include "ApplicationHPC.h"

#define SHOW_DEBUG_LOG 0

#define CHECK_MEMORY_HPC_APP 0

#define HPCAPP_INIT 0
#define HPCAPP_MASTER_READING 1
#define HPCAPP_MASTER_DELIVERING 2
#define HPCAPP_MASTER_WAITING_RESULTS 3
#define HPCAPP_MASTER_WRITTING 4

#define HPCAPP_WORKER_RECEIVING 11
#define HPCAPP_WORKER_READING 12
#define HPCAPP_WORKER_ALLOCATING 13
#define HPCAPP_WORKER_PROCESSING 14
#define HPCAPP_WORKER_WRITTING 15
#define HPCAPP_WORKER_SENDING_BACK 16

#define METADATA_MSG_SIZE 64
#define HPCAPP_MAX_SIZE_PER_INPUT_FILE (3500*MB)


Define_Module(ApplicationHPC);

ApplicationHPC::~ApplicationHPC(){
}

void ApplicationHPC::initialize(){

	std::ostringstream osStream;	
	unsigned int dataResult_MB;
	unsigned int dataToWorkerSet_MB;
	unsigned int previousData_input_MB;
    unsigned int previousData_output_MB;

    unsigned int totalDataPerWorker_MB;
    unsigned int totalResultPerWorker_MB;
    unsigned int numWorkers;
    unsigned int numCoordinators;
    unsigned int numSlices;

	
		// Get the workers per set
		workersSet = par ("workersSet");

		// Init the super-class
		AppMPI_Base::initialize();	
		
		// Set the moduleIdName
		osStream << "ApplicationHPC." << getId();
		moduleIdName = osStream.str();
		
		// Get parameters from module
		dataSet_MB = par ("dataSet");
		sliceToWorkers_MB = par ("sliceToWorkers");
		sliceToCoordinator_MB = par ("sliceToCoordinator");
		sliceCPU = par ("sliceCPU");
		workersRead = par ("workersRead");
		workersWrite = par ("workersWrite");
		showLog = par ("showLog");
		offsetRead = offsetWrite = 0;
		currentInputFile = currentOutputFile = 0;
		createdResultsFile = false;
		dataSetOffset = 0;
		activeWorkers = 0;

		const char *newInputFile = par ("inputFilePrefix");
		inputFilePrefix = newInputFile;

		const char *newOutputFile = par ("outputFilePrefix");
		outputFilePrefix = newOutputFile;

		// Calculate number of workers and coordinators
		numWorkers = numProcesses - (numProcesses/workersSet);
        numCoordinators = numProcesses/workersSet;

		// Calculate data processed by each worker and resultSet
        numSlices = (dataSet_MB%sliceToWorkers_MB)==0 ? dataSet_MB/sliceToWorkers_MB : (dataSet_MB/sliceToWorkers_MB)+1;
        dataResult_MB = numSlices * sliceToCoordinator_MB;
		totalDataPerWorker_MB = ((numSlices%numWorkers)==0 ? numSlices/numWorkers : (numSlices/numWorkers)+1) * sliceToWorkers_MB;
		totalResultPerWorker_MB = ((numSlices%numWorkers)==0 ? numSlices/numWorkers : (numSlices/numWorkers)+1) * sliceToCoordinator_MB;


		// Coordinator
		if (isCoordinator(processID)){

            // Calculate portion of dataSet processed by each coordinator
            dataToWorkerSet_MB = dataSet_MB/numCoordinators;
            initDatasetOffset = dataToWorkerSet_MB*getNumPreviousCoordinators(processID);

            // Last coordinator
            if ((processID+workersSet) == numProcesses)
                endDatasetOffset = dataSet_MB-1;
            else
                endDatasetOffset = initDatasetOffset+(dataToWorkerSet_MB-1);

            // Set current offset in the data-set
            dataSetOffset = initDatasetOffset;

            // Calculate offset in files
            previousData_output_MB = getNumPreviousCoordinators(processID) * (workersSet-1) * totalResultPerWorker_MB;

            currentInputFile = initDatasetOffset / (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB);
            currentOutputFile = previousData_output_MB / (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB);

            offsetRead = (initDatasetOffset % (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB)) * MB;
            offsetWrite = (previousData_output_MB % (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB)) * MB;
		}

		// Worker
		else{
            if (workersRead){
                previousData_input_MB = getNumPreviousWorkers(processID) * totalDataPerWorker_MB;
                currentInputFile = previousData_input_MB / (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB);
                offsetRead = (previousData_input_MB % (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB)) * MB;
            }

            if (workersWrite){
                previousData_output_MB = getNumPreviousWorkers(processID) * totalResultPerWorker_MB;
                currentOutputFile = previousData_output_MB / (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB);
                offsetWrite = (previousData_output_MB % (HPCAPP_MAX_SIZE_PER_INPUT_FILE / MB)) * MB;
            }
		}

		// Set file names
		sprintf (inputFileName, "%s_%d.dat", inputFilePrefix.c_str(), currentInputFile);
		sprintf (outFileName, "%s_%d.dat", outputFilePrefix.c_str(), currentOutputFile);


		// Show global information
		if ((showLog) && (processID == 0)){

		    printf ("Showing global data values before launching the application:\n\n");

		    printf ("dataSet:%d MB - resultSet:%d MB - #workers:%d - #coord:%d - #slices:%d\n\n",
		                dataSet_MB,
		                dataResult_MB,
		                numWorkers,
		                numCoordinators,
		                numSlices);
		}


		// Shows process configuration
		if (showLog){

		    // Coordinator
		    if (isCoordinator(processID))
		        printf ("processID:%d(Coor) - initData:%d - endData:%d - inputFile:%s - readOffset:%d - outputFile:%s - writeOffset:%d\n",
                            processID,
                            initDatasetOffset,
                            endDatasetOffset,
                            inputFileName,
                            offsetRead,
                            outFileName,
                            offsetWrite);

		    // Worker
		    else
		        printf ("processID:%d(Worker) - inputFile:%s - readOffset:%d - outputFile:%s - writeOffset:%d\n",
		                    processID,
		                    inputFileName,
                            offsetRead,
                            outFileName,
                            offsetWrite);
		}


		// Init state
		currentState = HPCAPP_INIT;
		totalIO = ioStart = ioEnd = 0.0;
		totalNET = netStart = netEnd = 0.0;
		totalCPU = cpuStart = cpuEnd = 0.0;
}

void ApplicationHPC::finish(){

	// Finish the super-class
	AppMPI_Base::finish();
}

void ApplicationHPC::processSelfMessage (cMessage *msg){
		
	// Execute!!!
	if (!strcmp (msg->getName(), SM_WAIT_TO_EXECUTE.c_str())){

        if (DEBUG_ApplicationMatrix)
            showDebugMessage ("[%d] Procesing self message!", processID);

		// Delete msg!
		cancelAndDelete (msg);

		// Start simulation time
		simStartTime = simTime();

		// Starting time...
		runStartTime = time (NULL);

		// Execute!
		calculateNextState ();
	}

	// Establish connection with server...
	else if (!strcmp (msg->getName(), SM_WAIT_TO_CONNECT.c_str())){

		// Delete message
		cancelAndDelete (msg);

		// Establish all connections...			
		establishAllConnections();
	}	

	else
		showErrorMessage ("Unknown self message [%s]", msg->getName());
}

void ApplicationHPC::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_NET_Message *sm_net;
	SIMCAN_App_CPU_Message *sm_cpu;
	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_App_MEM_Message *sm_mem;

		// Try to cast to SIMCAN_App_NET_Message
		sm_net = dynamic_cast<SIMCAN_App_NET_Message *>(sm);

		// NET call response...
		if (sm_net != NULL){
			processNetCallResponse (sm_net);						
		}
		// Try to cast to SIMCAN_App_IO_Message
		else{
			
			// Try to cast to SIMCAN_App_IO_Message
			sm_io = dynamic_cast<SIMCAN_App_IO_Message *>(sm);
	
			// NET call response...
			if (sm_io != NULL){
				processIOCallResponse (sm_io);						
			}			
			
			// Try to cast to SIMCAN_App_CPU_Message
			else{
				
				// Try to cast to SIMCAN_App_CPU_Message
				sm_cpu = dynamic_cast<SIMCAN_App_CPU_Message *>(sm);
		
				// NET call response...
				if (sm_cpu != NULL){
					processCPUCallResponse (sm_cpu);						
				}

				// Try to cast to SIMCAN_App_MEM_Message
				else{

					// Try to cast to SIMCAN_App_CPU_Message
					sm_mem = dynamic_cast<SIMCAN_App_MEM_Message *>(sm);

					// NET call response...
					if (sm_mem != NULL){
						processMEMCallResponse (sm_mem);
					}

					// Unknown message type
					else{
						showErrorMessage ("Unknown message type as response :%s", sm->contentsToString(true).c_str());
					}
				}
			}			
		}			
}

void ApplicationHPC::calculateNextState (){

	// Coordinator process?
	if (isCoordinator(processID)){
		
		// Init state... read data!
		if (currentState == HPCAPP_INIT){
			
			if ((!createdResultsFile) && (!workersWrite)){
				
				totalResponses = 1;
				
				if (SHOW_DEBUG_LOG)
					printf ("INIT - Process ID %d - Creating results file %s!\n", processID, outFileName);
				
				createdResultsFile = true;
				SIMCAN_request_create (outFileName);
			}
			
			else{
				currentState = HPCAPP_MASTER_READING;
				totalResponses = workersSet-1;
				activeWorkers = 0;
				
				if (SHOW_DEBUG_LOG)
				    printf ("(%f) -> [%d:%d] Process ID %d - State:%s\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, stateToString(currentState).c_str());

				ioStart = simTime();

				readInputData();
			}				
		}
		
		// Read finished! Start delivering data among workers..
		else if (currentState == HPCAPP_MASTER_READING){

		    // There are pending requests...
			if (totalResponses > 0){
				readInputData();
			}
			
			// All requests have been performed
			else{

				ioEnd = simTime();
				totalIO = totalIO.dbl() + (ioEnd.dbl() - ioStart.dbl());
			
				currentState = HPCAPP_MASTER_DELIVERING;	
				
				if (SHOW_DEBUG_LOG)
					printf ("(%f) -> [%d:%d] Process ID %d - State:%s\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, stateToString(currentState).c_str());

				netStart = simTime();

				deliverData();
			}
		}
		
		// Wait for results...
		else if (currentState == HPCAPP_MASTER_DELIVERING){
			
			currentState = HPCAPP_MASTER_WAITING_RESULTS;
			
			// Init responses
			totalResponses = activeWorkers;
			
			if (SHOW_DEBUG_LOG)
				printf ("(%f) -> [%d:%d] Process ID %d - State:%s - Waiting for %d responses\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, stateToString(currentState).c_str(), totalResponses);
			
			// Receiving results...
			receivingResults();			
		}	
		
		// Waiting for results...
		else if (currentState == HPCAPP_MASTER_WAITING_RESULTS){
									
			// Init responses
			totalResponses--;
			
			if (SHOW_DEBUG_LOG)
				printf ("(%f) -> [%d:%d] Process ID %d - Waiting for results. %d responses left!\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, totalResponses);
			
			// Check the number of responses received...
			if (totalResponses==0){
				
				netEnd = simTime ();
				totalNET = totalNET.dbl() + (netEnd.dbl()  - netStart.dbl());

				currentState = HPCAPP_MASTER_WRITTING;
				totalResponses = activeWorkers;
				
				if (SHOW_DEBUG_LOG)
					printf ("(%f) -> [%d:%d] Process ID %d - State:%s\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, stateToString(currentState).c_str());
						
				ioStart = simTime();

				writtingData();			
			}
			else
				receivingResults();					
		}		
		
		// Writting results...
		else if (currentState == HPCAPP_MASTER_WRITTING){
			
			// There are pending requests...
			if (totalResponses > 0){
				writtingData();
			}
			
			// All requests have been performed
			else{	

			    if (SHOW_DEBUG_LOG)
                    printf ("(%f) -> [%d:%d] Process ID %d - State:END ITERATION\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID);

				ioEnd = simTime();
				totalIO = totalIO.dbl() + (ioEnd.dbl() - ioStart.dbl());

				currentState = HPCAPP_INIT;		
				nextIteration();
			}			
		}			
	}	
			
	// Worker processes
	else{
		
		// Init state... read data!
		if (currentState == HPCAPP_INIT){
			
			if ((!createdResultsFile) && (workersWrite)){
				
				totalResponses = 1;
				
				if (SHOW_DEBUG_LOG)
					printf ("(%f) -> Process ID %d - Creating results file %s!\n", simTime().dbl(), processID, outFileName);
					
				createdResultsFile = true;
				SIMCAN_request_create (outFileName);
			}
			
			else{
				currentState = HPCAPP_WORKER_RECEIVING;
				
				if (SHOW_DEBUG_LOG)
					printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

				netStart = simTime();
				receiveInputData();			
			}
		}
		
		// Worker starts processing CPU
		else if (currentState == HPCAPP_WORKER_RECEIVING){

			currentState = HPCAPP_WORKER_READING;
			
			if (SHOW_DEBUG_LOG)
                printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

			netEnd = simTime();
			totalNET = totalNET.dbl() + (netEnd.dbl()  - netStart.dbl());

			ioStart = simTime();

			readInputDataWorkers();
		}

		// Worker starts allocating memory...
		else if (currentState == HPCAPP_WORKER_READING){

			currentState = HPCAPP_WORKER_ALLOCATING;

			if (SHOW_DEBUG_LOG)
                printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

			ioEnd = simTime();
			totalIO = totalIO.dbl() + (ioEnd.dbl() - ioStart.dbl());

			allocating();
		}

				
		// Worker starts to processing CPU
		else if (currentState == HPCAPP_WORKER_ALLOCATING){
			
			currentState = HPCAPP_WORKER_PROCESSING;
			
			if (SHOW_DEBUG_LOG)
                printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

			cpuStart = simTime();

			processingCPU();
		}	
		
		// Workers send data to master process
		else if (currentState == HPCAPP_WORKER_PROCESSING){
			
			currentState = HPCAPP_WORKER_WRITTING;

			if (SHOW_DEBUG_LOG)
                printf ("Process ID %d - State:%s\n", processID, stateToString(currentState).c_str());

			cpuEnd = simTime();
			totalCPU = totalCPU.dbl() + (cpuEnd.dbl() - cpuStart.dbl());

			ioStart = simTime();
			writtingDataWorkers();
		}
		
		// Workers send data to master process
		else if (currentState == HPCAPP_WORKER_WRITTING){
			
			currentState = HPCAPP_WORKER_SENDING_BACK;

			if (SHOW_DEBUG_LOG)
                printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

			ioEnd = simTime();
			totalIO = totalIO.dbl() + (ioEnd.dbl() - ioStart.dbl());
			netStart = simTime();

			sendResults();
		}	
		
		
		// Workers send data to master process
		else if (currentState == HPCAPP_WORKER_SENDING_BACK){

			netEnd = simTime();
			totalNET = totalNET.dbl() + (netEnd.dbl()  - netStart.dbl());

			if (SHOW_DEBUG_LOG)
                printf ("(%f) -> Process ID %d - State:END ITERATION\n", simTime().dbl(), processID);

			currentState = HPCAPP_INIT;
			nextIteration();
		}			
	}
}

void ApplicationHPC::nextIteration (){

    // Coordinator
    if (isCoordinator(processID)){

        // End of processing?
        if (dataSetOffset >= endDatasetOffset){
            showResults();
        }
        else
            calculateNextState ();
    }

    // Worker
    else
        calculateNextState ();
}

void ApplicationHPC::continueExecution (){
		
	calculateNextState ();
}

void ApplicationHPC::readInputData(){

	// Coordinator process performs READ!
	if (!workersRead){
										
		if (showLog)
			printf ("(%f) -> [%d:%d] Process ID %d - Reading file:%s - Offset:%u - Size:%u bytes\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, inputFileName, offsetRead, sliceToWorkers_MB*MB);
			
			// Reading data for each worker process
			SIMCAN_request_read (inputFileName, offsetRead, sliceToWorkers_MB*MB);
			
			// Calculating next offset
			if ((offsetRead+(sliceToWorkers_MB*MB)) > HPCAPP_MAX_SIZE_PER_INPUT_FILE){
				currentInputFile++;
				sprintf (inputFileName, "%s_%d.dat", inputFilePrefix.c_str(), currentInputFile);
				offsetRead = 0;
			}
			else{
				offsetRead += sliceToWorkers_MB*MB;
			}

			// Update processed MB in dataSet
			dataSetOffset += sliceToWorkers_MB;
			activeWorkers++;

			// Data-set has been totally processed
			if (dataSetOffset >= endDatasetOffset)
			    totalResponses = 1;
	}
	
	// Coordinator process does not READ data
	else{
		totalResponses = 0;
		calculateNextState ();
	}
}

void ApplicationHPC::readInputDataWorkers(){
		
	// Coordinator process performs READ!
	if (workersRead){
		
		// Init
		totalResponses = 1;
			
		if (showLog)
			showDebugMessage ("Process ID %d - Reading file:%s - Offset:%u - Size:%u bytes", processID, inputFileName, offsetRead, sliceToWorkers_MB*MB);
		
		// Reading data for each worker process
		SIMCAN_request_read (inputFileName, offsetRead, sliceToWorkers_MB*MB);
		
		// Calculating next offset
		if ((offsetRead+(sliceToWorkers_MB*MB)) > HPCAPP_MAX_SIZE_PER_INPUT_FILE){
			currentInputFile++;
			sprintf (inputFileName, "%s_%d.dat", inputFilePrefix.c_str(), currentInputFile);
			offsetRead = 0;
		}
		else
			offsetRead += sliceToWorkers_MB*MB;
	}
	
	// Coordinator process does not READ data
	else{
		totalResponses = 0;
		calculateNextState ();
	}
}

void ApplicationHPC::allocating(){

	SIMCAN_request_allocMemory (1, 0);
}

void ApplicationHPC::deliverData(){
	
	int i;	
	unsigned int dataSize;
	unsigned int remainingSlices;
			
		// Coordinator Process READ!
		if (!workersRead){
			dataSize = sliceToWorkers_MB*MB;
		}

		// Workers READ
		else{
		    remainingSlices = ((endDatasetOffset-dataSetOffset)%sliceToWorkers_MB)==0 ? (endDatasetOffset-dataSetOffset)/sliceToWorkers_MB : ((endDatasetOffset-dataSetOffset)/sliceToWorkers_MB)+1;

		    if (remainingSlices<(workersSet-1))
		        activeWorkers = remainingSlices;
		    else
		        activeWorkers = workersSet-1;

			dataSize = METADATA_MSG_SIZE;
		}
	
		// Send data to each process
		for (i=(processID+1); i<=(processID+activeWorkers); i++){

		    if (showLog)
                printf ("(%f) -> [%d:%d] Process ID %d - Delivering data (%d bytes) to worker process %d\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, dataSize, i);

			mpi_send (i, dataSize);
		}

		// Update offset in dataSet
		if (workersRead)
		    dataSetOffset += (sliceToWorkers_MB*activeWorkers);
		
		// Wai for responses...
		calculateNextState();						
}

void ApplicationHPC::receivingResults(){

    if (!workersWrite){
        if (showLog)
            printf ("(%f) -> [%d:%d] Process ID %d - Receiving data (%d bytes) from any worker process\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, sliceToCoordinator_MB*MB);

        mpi_recv (MPI_ANY_SENDER, sliceToCoordinator_MB*MB);
    }
    else{
        mpi_recv (MPI_ANY_SENDER, METADATA_MSG_SIZE);
    }
}

void ApplicationHPC::receiveInputData(){
	
	if (!workersRead){

	    if (showLog)
	        printf ("(%f) -> Process ID %d - Receiving data (%d bytes) from coordinator %d\n", simTime().dbl(), processID, sliceToWorkers_MB*MB, getMyCoordinatorID(processID));

		mpi_recv (getMyCoordinatorID(processID), sliceToWorkers_MB*MB);
	}
	else
		mpi_recv (getMyCoordinatorID(processID), METADATA_MSG_SIZE);
}

void ApplicationHPC::processingCPU(){
	
	if (showLog)
		printf ("(%f) -> Process ID %d - Processing CPU\n", simTime().dbl(), processID);
	
	SIMCAN_request_cpu (sliceCPU);
}

void ApplicationHPC::writtingData(){
		
		// Coordinator process performs WRITE!
		if (!workersWrite){
				
			if (showLog)
				printf ("(%f) -> [%d:%d] Process ID %d - Writing file:%s - Offset:%u - Size:%u\n", simTime().dbl(), dataSetOffset, endDatasetOffset, processID, outFileName, offsetWrite, sliceToCoordinator_MB*MB);
			
			// Reading data for each worker process
			SIMCAN_request_write (outFileName, offsetWrite, sliceToCoordinator_MB*MB);
			
			// Calculating next offset
			if ((offsetWrite+(sliceToCoordinator_MB*MB)) > HPCAPP_MAX_SIZE_PER_INPUT_FILE){
				currentOutputFile++;
				sprintf (outFileName, "%s_%d.dat", outputFilePrefix.c_str(), currentOutputFile);
				offsetWrite = 0;
			}
			else
				offsetWrite += sliceToCoordinator_MB*MB;
		}
		
		// Coordinator process does not WRITE data
		else{
			totalResponses = 0;
			calculateNextState ();
		}
}

void ApplicationHPC::writtingDataWorkers(){
	
	// Coordinator process performs WRITE!
	if (workersWrite){
		
		if (DEBUG_ApplicationHPC)
			printf ("(%f) -> Process ID %d - State:%s\n", simTime().dbl(), processID, stateToString(currentState).c_str());

		// Init
		totalResponses = 1;		
			
		if (showLog)
			printf ("(%f) -> Process ID %d - Writing file:%s - Offset:%u - Size:%u bytes\n", simTime().dbl(), processID, outFileName, offsetRead, sliceToCoordinator_MB*MB);
		
		// Reading data for each worker process
		SIMCAN_request_write (outFileName, offsetWrite, sliceToCoordinator_MB*MB);
		
		// Calculating next offset
		if ((offsetWrite+(sliceToCoordinator_MB*MB)) > HPCAPP_MAX_SIZE_PER_INPUT_FILE){
			currentOutputFile++;
			sprintf (outFileName, "%s_%d.dat", outputFilePrefix.c_str(), currentOutputFile);
			offsetWrite = 0;
		}
		else
			offsetWrite += sliceToCoordinator_MB*MB;
	}
	
	// Coordinator process does not WRITE data
	else{
		totalResponses = 0;
		calculateNextState ();
	}
}

void ApplicationHPC::sendResults(){
	
	unsigned int dataSize;
	
		// Coordinator Process WRITE!
		if (!workersWrite)
			dataSize = sliceToCoordinator_MB*MB;
		else
			dataSize = METADATA_MSG_SIZE;
			
		if (showLog)
			printf ("(%f) -> Process ID %d - Sending results to coordinator %d (%d bytes)\n", simTime().dbl(), processID, getMyCoordinatorID(processID), dataSize);

		// Send data to each process			
		mpi_send (getMyCoordinatorID(processID), dataSize);
	
	calculateNextState ();	
}

void ApplicationHPC::processNetCallResponse (SIMCAN_App_NET_Message *responseMsg){

	// Create connection response...
	if (responseMsg->getOperation () == SM_CREATE_CONNECTION){

		// Set the established connection.
		setEstablishedConnection (responseMsg);		

		delete (responseMsg);
	}

	else
		showErrorMessage ("Unknown NET call response :%s", responseMsg->contentsToString(true).c_str());
}

void ApplicationHPC::processIOCallResponse (SIMCAN_App_IO_Message *responseMsg){
	
	bool correctResult;
	std::ostringstream osStream;
	
		// Init...
		correctResult = false;		

		// Check the results...
		if (responseMsg->getOperation() == SM_READ_FILE){
			
			// Check state!
			if ((currentState != HPCAPP_MASTER_READING) && (currentState != HPCAPP_WORKER_READING))
				throw cRuntimeError("State error! Must be HPCAPP_MASTER_READING\n");

			// All ok!
			if (responseMsg->getResult() == SIMCAN_OK){
				correctResult = true;
			}

			// File not found!
			else if (responseMsg->getResult() == SIMCAN_FILE_NOT_FOUND){
				osStream << "File not found!";
				correctResult = false;
			}

			// File not found!
			else if (responseMsg->getResult() == SIMCAN_DATA_OUT_OF_BOUNDS){
				osStream << "Request out of bounds!";
				correctResult = false;
			}

			// Unknown result!
			else{
				osStream << "Unknown result value:" << responseMsg->getResult();
				correctResult = false;
			}
		}

		// Write response!
		else if (responseMsg->getOperation() == SM_WRITE_FILE){
			
			// Check state!
			if ((currentState != HPCAPP_MASTER_WRITTING) && (currentState != HPCAPP_WORKER_WRITTING))
				throw cRuntimeError("State error! Must be HPCAPP_MASTER_WRITTING\n");

			// All ok!
			if (responseMsg->getResult() == SIMCAN_OK){
				correctResult = true;
			}

			// File not found!
			else if (responseMsg->getResult() == SIMCAN_FILE_NOT_FOUND){
				osStream << "File not found!";
				correctResult = false;
			}

			// File not found!
			else if (responseMsg->getResult() == SIMCAN_DISK_FULL){
				osStream << "Disk full!";
				correctResult = false;
			}

			// Unknown result!
			else{
				osStream << "Unknown result value:" << responseMsg->getResult();
				correctResult = false;
			}
		}
		
		// Create response!
		else if (responseMsg->getOperation() == SM_CREATE_FILE){
			
			// All ok!
			if (responseMsg->getResult() == SIMCAN_OK){
				correctResult = true;
			}
			
			// File not found!
			else if (responseMsg->getResult() == SIMCAN_DISK_FULL){
				osStream << "Disk full!";
				correctResult = false;
			}		
			
			// Unknown result!
			else{
				osStream << "Unknown result value:" << responseMsg->getResult();
				correctResult = false;
			}		
		}
		
		else{
			
			// Error in response message...
			showErrorMessage ("Error! Unknown IO operation:%s. %s",
							  osStream.str().c_str(),
							  responseMsg->contentsToString(false).c_str());
		}
		
		
		// Error in response message?
		if (!correctResult){			
			showErrorMessage ("Error in response message:%s. %s",
							  osStream.str().c_str(),
							  responseMsg->contentsToString(false).c_str());
		
			throw cRuntimeError("Error in response message\n");		
		}
		
		// All OK!				  
		else{

			totalResponses--;			
			
			if (DEBUG_ApplicationHPC)
				showDebugMessage ("Process ID %d - Arrives I/O response. Left:%d", processID, totalResponses);
			
			// Error in synchronization phase?
			if (totalResponses < 0)
				throw cRuntimeError("Synchronization error!\n");		
		
			calculateNextState ();
		}
		
	delete (responseMsg);		
}

void ApplicationHPC::processCPUCallResponse (SIMCAN_App_CPU_Message *responseMsg){
	
	delete (responseMsg);
	calculateNextState ();	
}

void ApplicationHPC::processMEMCallResponse (SIMCAN_App_MEM_Message *responseMsg){

	delete (responseMsg);
	calculateNextState ();
}

string ApplicationHPC::stateToString(int state){
	
	string stringState;	
	
		if (state == HPCAPP_INIT)
			stringState = "HPCAPP_INIT";
			
		else if (state == HPCAPP_MASTER_READING)
			stringState = "HPCAPP_MASTER_READING";
			
		else if (state == HPCAPP_MASTER_DELIVERING)
			stringState = "HPCAPP_MASTER_DELIVERING";
			
		else if (state == HPCAPP_MASTER_WAITING_RESULTS)
			stringState = "HPCAPP_MASTER_WAITING_RESULTS";				

		else if (state == HPCAPP_MASTER_WRITTING)
			stringState = "HPCAPP_MASTER_WRITTING";				
			
		else if (state == HPCAPP_WORKER_RECEIVING)
			stringState = "HPCAPP_WORKER_RECEIVING";
			
		else if (state == HPCAPP_WORKER_READING)
			stringState = "HPCAPP_WORKER_READING";
			
		else if (state == HPCAPP_WORKER_PROCESSING)
			stringState = "HPCAPP_WORKER_PROCESSING";
			
		else if (state == HPCAPP_WORKER_WRITTING)
			stringState = "HPCAPP_WORKER_WRITTING";
			
		else if (state == HPCAPP_WORKER_SENDING_BACK)
			stringState = "HPCAPP_WORKER_SENDING_BACK";

		else if (state == HPCAPP_WORKER_ALLOCATING)
					stringState = "HPCAPP_WORKER_ALLOCATING";
							
		else stringState = "Unknown state";		

	return stringState;
}

void ApplicationHPC::showResults (){

	// End time
    simEndTime = simTime();
    runEndTime = time (NULL);

    if (isCoordinator(processID))
          printf ("RESULTS -> App [%s] - Rank MASTER:%d - SimTime:%f - RealTime:%f\n",
                                                                                     moduleIdName.c_str(),
                                                                                     processID,
                                                                                     (simEndTime-simStartTime).dbl(),
                                                                                     difftime (runEndTime,runStartTime));



//    if (isCoordinator(processID))
//    	printf ("RESULTS -> App [%s] - Rank MASTER:%d - SimTime:%f - RealTime:%f - IO:%f  NET:%f  CPU:%f",
//																								   moduleIdName.c_str(),
//																								   processID,
//    																							   (simEndTime-simStartTime).dbl(),
//    																							   difftime (runEndTime,runStartTime),
//    																							   totalIO.dbl(),
//    																							   totalNET.dbl(),
//    																							   totalCPU.dbl());
    //else
//        printf ("RESULTS ->  App [%s] - Rank:%d - Simulation time:%f - Real execution time:%f - IO:%f  NET:%f  CPU:%f",
//    																									   moduleIdName.c_str(),
//    																									   processID,
//    	    																							   (simEndTime-simStartTime).dbl(),
//    	    																							   difftime (runEndTime,runStartTime),
//    	    																							   totalIO.dbl(),
//    	    																							   totalNET.dbl(),
//    	    																							   totalCPU.dbl());
    //const char *testID = par ("testID");
    //printf ("#___#:%s:%03d:%f;%f;%f;%f\n", testID, processID, totalIO.dbl(), totalCPU.dbl(), totalNET.dbl(), (simEndTime.dbl() - simStartTime.dbl()));
    //printf ("#___#:%s:%f;%f;%f;%f\n", testID, totalIO.dbl(), totalCPU.dbl(), totalNET.dbl(), (simEndTime - simStartTime).dbl());
}


