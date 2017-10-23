#include "LocalApplication.h"
#define MAX_FILE_SIZE 2000000000

Define_Module(LocalApplication);

LocalApplication::~LocalApplication(){
}

void LocalApplication::initialize(){

	std::ostringstream osStream;
	cMessage *waitToExecuteMsg;

		// Set the moduleIdName
		osStream << "LocalApplication." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		SimcanAPI::initialize();

		// App Module parameters
		inputDataSize  = (int) par ("inputDataSize") * MB;
		outputDataSize  = (int) par ("outputDataSize") * MB;
		MIs  = par ("MIs");
		iterations  = par ("iterations");
		currentIteration = 1;
		
		const char *newInputFile = par ("inputFile");
		inputFile = newInputFile;

		const char *newOutputFile = par ("outputFile");
		outputFile = newOutputFile;

		// Service times		
		total_service_IO = total_service_CPU = 0.0;
		startServiceIO = endServiceIO = 0.0;
		endServiceCPU = startServiceCPU = 0.0;
		readOffset = writeOffset = 0;
		
		// Boolean variables
		executeCPU = executeRead = executeWrite = false;

		// Show initial parameters
		showStartedModule ("InputData:%d - OutputData:%d - MIs:%d - Iterations:%d", inputDataSize, outputDataSize, MIs, iterations);

		// Create SM_WAIT_TO_EXECUTE message for delaying the execution of this application
		waitToExecuteMsg = new cMessage (SM_WAIT_TO_EXECUTE.c_str());
	    scheduleAt (simTime().dbl()+startDelay, waitToExecuteMsg);
}


void LocalApplication::finish(){

	// Finish the super-class
    SimcanAPI::finish();
}

void LocalApplication::processSelfMessage (cMessage *msg){

	if (!strcmp (msg->getName(), SM_WAIT_TO_EXECUTE.c_str())){
	
		// Delete msg!
		delete (msg);
	
		// Starting time...
		simStartTime = simTime();
		runStartTime = time (NULL);
						
		// Init...		
		startServiceIO = simTime();
		executeIOrequest (true, false);
	}
	else
		showErrorMessage ("Unknown self message [%s]", msg->getName());
}

void LocalApplication::processRequestMessage (SIMCAN_Message *sm){
	showErrorMessage ("This module does not recieve request messages!");
}


void LocalApplication::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_App_CPU_Message *sm_cpu;
	bool isError;	
	std::ostringstream osStream;
		// Init...				
		sm_io = dynamic_cast<SIMCAN_App_IO_Message *>(sm);
		sm_cpu = dynamic_cast<SIMCAN_App_CPU_Message *>(sm);
		isError = false;
								
		

		// IO Message?
		if (sm_io != NULL){
			
			// Get time!
			endServiceIO = simTime();
			
			// Read response!
			if (sm_io->getOperation() == SM_READ_FILE){

				// All ok!
				if (sm_io->getResult() == SIMCAN_OK){
					executeCPU = true;					
					executeWrite = false;
					executeRead = false;
					delete (sm);
				}
				
				// File not found!
				else if (sm_io->getResult() == SIMCAN_FILE_NOT_FOUND){
					osStream << "File not found!";
					isError = true;
				}		
				
				// File not found!
				else if (sm_io->getResult() == SIMCAN_DATA_OUT_OF_BOUNDS){					
					executeCPU = true;
				}		
				
				// Unknown result!
				else{
					osStream << "Unknown result value:" << sm_io->getResult();
					isError = true;
				}		
			}
					
			
			// Write response!
			else if (sm_io->getOperation() == SM_WRITE_FILE){

				// All ok!
				if (sm_io->getResult() == SIMCAN_OK){
					executeCPU = false;
					executeWrite = false;
					executeRead = true;
					currentIteration++;
					delete (sm);
				}
				
				// File not found!
				else if (sm_io->getResult() == SIMCAN_FILE_NOT_FOUND){
					osStream << "File not found!";
					isError = true;
				}		
				
				// File not found!
				else if (sm_io->getResult() == SIMCAN_DISK_FULL){
					osStream << "Disk full!";
					isError = true;
				}		
				
				// Unknown result!
				else{
					osStream << "Unknown result value:" << sm_io->getResult();
					isError = true;
				}			
			}
			
			// Unknown I/O operation
			else{
				osStream << "Unknown received response message";	
				isError = true;
			}			
			

			// Increase total time for I/O
			total_service_IO = total_service_IO + ((endServiceIO.dbl() - startServiceIO.dbl()));
		}
				
	
		// Response came from CPU system		
		else if (sm_cpu != NULL){
			
			// Get time!
			endServiceCPU = simTime ();
				
			// CPU!
			if (sm_cpu->getOperation() == SM_CPU_EXEC){
				executeCPU = false;
				executeWrite = true;
				executeRead = false;
				delete (sm);
			}
			
			// Unknown CPU operation
			else{
				osStream << "Unknown received response message";	
				isError = true;
			}
			
			// Increase total time for I/O
			total_service_CPU = total_service_CPU.dbl() + (endServiceCPU.dbl() - startServiceCPU.dbl());
		}
		
		// Wrong response message!
		else{
			
			osStream << "Unknown received response message";	
			isError = true;
		}

			// Error?
			if (isError){
				
				showErrorMessage ("Error in response message:%s. %s", 
									osStream.str().c_str(),
									sm_io->contentsToString(true).c_str());
			}
								
			// CPU?
			else if (executeCPU){
					
				// Execute CPU!
				executeCPUrequest ();								
			}
			
			// IO?
			else if ((executeRead) || (executeWrite)){

				if ((executeRead) && (currentIteration > iterations))
					printResults();
				else
					executeIOrequest(executeRead, executeWrite);
			}
				
			// Inconsistency error!		
			else
				showErrorMessage ("Inconsistency error!!!! :%s. %s", 
									osStream.str().c_str(),
									sm->contentsToString(true).c_str());	
} 


void LocalApplication::executeIOrequest(bool executeRead, bool executeWrite){

	// Reset timer!
	startServiceIO = simTime();

	// Executes I/O operation
	if (executeRead){
		
		if ((readOffset+(inputDataSize))>=MAX_FILE_SIZE)
			readOffset = 0;

		if (DEBUG_LocalApplication)
			showDebugMessage ("[%d/%d] Executing (Read) Offset:%d; dataSize:%d", currentIteration, iterations, readOffset,  inputDataSize);
		
		SIMCAN_request_read (inputFile.c_str(), readOffset, inputDataSize);
		readOffset += (inputDataSize);

	}
	else{
		
		if ((writeOffset+(outputDataSize))>=MAX_FILE_SIZE)
			writeOffset = 0;

		if (DEBUG_LocalApplication)
		    showDebugMessage ("[%d/%d] Executing (Write) Offset:%d; dataSize:%d", currentIteration, iterations, writeOffset,  outputDataSize);
		
		SIMCAN_request_write (outputFile.c_str(), writeOffset, outputDataSize);
		writeOffset += (outputDataSize);
	}

}


void LocalApplication::executeCPUrequest(){

	// Debug?
	if (DEBUG_LocalApplication)
		showDebugMessage ("[%d/%d] Executing (CPU) MIs:%d", currentIteration, iterations, MIs);

	// Reset timer!	
	startServiceCPU = simTime ();
	SIMCAN_request_cpu(MIs);
}


void LocalApplication::printResults (){

	// End time
    simEndTime = simTime();
    runEndTime = time (NULL);    
    const char *testID = par ("testID");
    //showResultMessage ("App [%s]", moduleIdName.c_str());
    //showResultMessage ("Time I/O:%f - Time CPU:%f", total_service_IO.dbl(), total_service_CPU.dbl());
    
    //showResultMessage ("Real run-time:%f", difftime (runEndTime,runStartTime));
    //showResultMessage ("Simulation time:%f", (simEndTime - simStartTime).dbl());

    printf ("#___#:%s:%f;%f;%f\n", testID, total_service_IO.dbl(), total_service_CPU.dbl(), (simEndTime.dbl() - simStartTime.dbl()));
}


