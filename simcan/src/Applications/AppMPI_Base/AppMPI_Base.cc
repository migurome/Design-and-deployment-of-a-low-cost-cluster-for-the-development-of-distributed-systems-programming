#include "AppMPI_Base.h"


AppMPI_Base::~AppMPI_Base(){	
	cancelAndDelete (computingDelayMessage);
}


void AppMPI_Base::initialize(){

	std::ostringstream osStream;

		// Super init
	    SimcanAPI::initialize();

	    // Init...
	    processID = 0;
		
		// Get module parameters...
		allToAllConnections = par ("allToAllConnections");
		waitingForSend = false;
		workersSet = par ("workersSet");
		
		 // Process ID
        myRank = par ("myRank");

		// Check delays!
		if (startDelay <= connectionDelay){
			showErrorMessage ("Error! parameter startDelay must be > connectionDelay");
		}

		// Check
		if (workersSet < 0){
			showErrorMessage ("Param workersSet must be > 0!");
		}

		// Parse machine file
		parseMachinesFile ();

		// Create the computing delay message
		computingDelayMessage = new cMessage ("computing-message");
		
		// Create MPI calls objects
		currentMPICall = new MPI_Call();
		currentMPICall->clear();		
		lastSynchroMPICall = new MPI_Call();
		lastSynchroMPICall->clear();

		// Init barriers...
		barriers = new int [numProcesses];
		initBarriers ();		
		
		// Create WAIT_TO_CONNECT_MESSAGE and wait connectionDelay to sent it!
		cMessage *waitToConnectMsg = new cMessage (SM_WAIT_TO_CONNECT.c_str());
	    scheduleAt (simTime() + connectionDelay, waitToConnectMsg);

		// Create SM_WAIT_TO_EXECUTE and wait startDelay to sent it!
		cMessage *waitToExecuteMsg = new cMessage (SM_WAIT_TO_EXECUTE.c_str());
	    scheduleAt (simTime() + startDelay, waitToExecuteMsg);

	    // Show info?
	    if (DEBUG_MPIApp)
	        showDebugMessage ("ProcessID:%d - myRank:%d - WorkerSet:%d -  %s ", processID, myRank, workersSet, mpiCommunicationsToString().c_str());
}


void AppMPI_Base::finish(){
	
    SimcanAPI::finish();
}


void AppMPI_Base::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_MPI_Message *sm_mpi;			

		// Try to cast to SIMCAN_App_NET_Message
		sm_mpi = dynamic_cast<SIMCAN_MPI_Message *>(sm);

		// NET call response...
		if (sm_mpi != NULL){
			processMPICallRequest (sm_mpi);						
		}
		
		else
			showErrorMessage ("Unknown message type as request :%s", sm->contentsToString(true).c_str());	
}


void AppMPI_Base::processMPICallRequest (SIMCAN_MPI_Message *sm_mpi){
	
	if (DEBUG_MPIApp)
	    showDebugMessage ("[AppMPI_Base] Process ID:%d - Rank %d - Incoming call %s", processID, myRank, sm_mpi->contentsToString(DEBUG_MSG_MPIApp).c_str());
		
		// MPI_SEND... enqueue and process...
		if (sm_mpi->getOperation() == MPI_SEND){

			// Inserts a new MPI call into queue
			insertMPICall (sm_mpi);

			// Free mem.
			delete (sm_mpi);

			// Synchrionize with current request!
			synchronize (MPI_SEND);  //*** WARNING ***//

			//if (waitingForSend){
			//	waitingForSend = false;
			//	continueExecution();
			//}
		}

		// MPI_SCATTER... enqueue and process...
		else if (sm_mpi->getOperation() == MPI_SCATTER){

			// Inserts a new MPI call into queue
			insertMPICall (sm_mpi);
			
			// Free mem.
			delete (sm_mpi);

			// Synchrionize with current request!
			synchronize (MPI_SCATTER);
		}

		// MPI_BCAST... enqueue and process...
		else if (sm_mpi->getOperation() == MPI_BCAST){

			// Inserts a new MPI call into queue
			insertMPICall (sm_mpi);

			// Free mem.
			delete (sm_mpi);

			// Synchrionize with current request!
			synchronize (MPI_BCAST);
		}

		// MPI_BARRIER_DOWN
		else if (sm_mpi->getOperation() == MPI_BARRIER_DOWN){
			mpi_barrier_down (sm_mpi);
		}

		// MPI_BARRIER_UP
		else if (sm_mpi->getOperation() == MPI_BARRIER_UP){
			mpi_barrier_up ();
			delete (sm_mpi);
		}

		// MPI_GATHER
		else if (sm_mpi->getOperation() == MPI_GATHER){
			mpi_gather_arrives (sm_mpi);
			delete (sm_mpi);
		}
		
		else
			showErrorMessage ("Unknown MPI operation :%s", sm_mpi->contentsToString(true).c_str());
}


void AppMPI_Base::parseMachinesFile (){

	int i, currentProcessID;
	CfgMPI *mpiCfg;
	FileConfigManager * fileCfgMng;
	cModule *fileConfigModule;

	//printf (" --- Parsing machinefiles --- [Rank:%d]\n", myRank);

	    // Init...
	    numProcesses = currentProcessID = 0;

		// Node with local fileConfig?
		fileConfigModule = getParentModule()->getParentModule()->getSubmodule("fileConfig");

		// Not local fileConfig in the node... :(
		if (fileConfigModule == NULL){

			// Single getNode(without packaging) using a global file config
			fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");

			// Not a global file Config
			if (fileConfigModule == NULL){

				// Racked node. Get file config from Rack!		
				fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
				// Wrong packaging configuration!
				if (fileConfigModule == NULL)
					showErrorMessage ("Wrong packaging configuration. Can not find FileConfigManager");
			}
		}		

		// Get the corresponding servers...
		fileCfgMng = check_and_cast<FileConfigManager *>(fileConfigModule);
		mpiCfg = fileCfgMng->getMPIEnv();
		
		if (mpiCfg != NULL){

		    // Get the root process?
            rootProcessID = 0;

            // Get number of processes
            numProcesses = mpiCfg->getNumProcessesByApp(appInstance);

			// No servers file!
			if (mpiCfg->getTotalNumProcesses() == 0){
				showErrorMessage ("There aren't processes in the IPs file!\n");
			}

			// No process for this app
			else if (numProcesses == 0){
			    showErrorMessage ("There aren't processes for the application: %s", appInstance.c_str());
			}

			// Wrong number
			else if (workersSet > numProcesses){
				showErrorMessage ("Parameter workersSet must be less than total number of processes!\n");
			}

			// Wrong number
			else if ((workersSet != 0) && ((numProcesses%workersSet) != 0)){
			        showErrorMessage ("Parameter workersSet [%d] must be multiple of the total number of processes [%d]!",workersSet, numProcesses);
			}

			// Everything seems to be OK before establishing communications between processes...
			else{

				// For each process.. calculate masters and listening for incoming connections
				for (i=0; i<mpiCfg->getTotalNumProcesses(); i++){

				    // Match app instance?
				    if(appInstance == mpiCfg->getAppInstanceByIndex(i)){

				        // My process ID?
				        if (myRank == mpiCfg->getRankByIndex(i))
				            processID = currentProcessID;

				        // Update process ID
				        currentProcessID++;

				        // Insert into the ranks vector
				        ranksVector.push_back(mpiCfg->getRankByIndex(i));
					
                        // Own process... listen connections
                        if (myRank == mpiCfg->getRankByIndex(i)){

                            if (DEBUG_MPIApp)
                                showDebugMessage ("Process [ID:%d - Rank:%d] listening connection on %s:%d", processID, myRank, mpiCfg->getHostNameByIndex(i).c_str(), mpiCfg->getPortByIndex(i));

                            // Add listening connection
                            SIMCAN_request_createListenConnection (mpiCfg->getPortByIndex(i));
                        }
				    }
				}

				// Check for root
				if (ranksVector[rootProcessID] != mpiCfg->getRootProcessByApp(appInstance))
				    throw cRuntimeError("[AppMPI_Base::parseMachinesFile] Wrong root process ID!\n");


				// Create connections
				for (i=0; i<ranksVector.size(); i++){

				    // Not own process...
				    if (getRank(i) != myRank){

                        // There is no set of workers. Only one master process!
                        if (workersSet == 0){

                            // Establish connection with all processes
                            if (allToAllConnections){
                                addNewConnection (mpiCfg->getHostNameByIndex(getRank(i)), mpiCfg->getPortByIndex(getRank(i)), getRank(i));
                            }

                            // Worker process connects with Root!
                            else if ((i == rootProcessID) && (processID != rootProcessID)){
                                addNewConnection (mpiCfg->getHostNameByIndex(getRank(i)), mpiCfg->getPortByIndex(getRank(i)), getRank(i));
                            }

                            // Root process connects with a worker
                            else if (processID == rootProcessID){
                                addNewConnection (mpiCfg->getHostNameByIndex(getRank(i)), mpiCfg->getPortByIndex(getRank(i)), getRank(i));
                            }
                        }

                        // There are sets of workers!
                        else{

                            // Worker process establishes connection with its master process
                            if (getMyCoordinatorID(processID) == i){
                                addNewConnection (mpiCfg->getHostNameByIndex(getRank(i)), mpiCfg->getPortByIndex(getRank(i)), getRank(i));
                            }

                            // Coordinator process establish connection with one of its workers
                            else if (isThisWorkerInMyRange(processID, i)){
                                addNewConnection (mpiCfg->getHostNameByIndex(getRank(i)), mpiCfg->getPortByIndex(getRank(i)), getRank(i));
                            }
                        }
				    }
				}
			}
		}

		else
			showErrorMessage ("NULL object CfgTracePlayerMPI");
}





int AppMPI_Base::getRank (unsigned int procID){

    return ranksVector.at(procID);
}


bool AppMPI_Base::isCoordinator (int procID){

    bool result;

        // Init...
        result = false;

            // No workers set defined!
            if (workersSet == 0){

                if (procID == rootProcessID)
                    result = true;
                else
                    result = false;
            }

            // Workers set defined
            else{

                if ((procID < 0) || (procID >= numProcesses))
                    throw cRuntimeError("[AppMPI_Base::isCoordinator] Process index not found!\n");
                else
                    result = (procID%workersSet) == 0;
            }

	return result;
}


int AppMPI_Base::getMyCoordinatorID (int procID){

    int masterRank;

        // Init...
        masterRank = -1;

        // No workers set defined!
        if (workersSet == 0)
            masterRank = rootProcessID;

        // Workers set defined
        else{

            if ((procID < 0) || (procID >= numProcesses))
                throw cRuntimeError("[AppMPI_Base::getMyCoordinatorID] Process index not found [ID:%d]!\n", procID);
            else{
                masterRank = (procID/workersSet) * workersSet;
            }
        }

        return masterRank;
}


int AppMPI_Base::getNumPreviousWorkers (int procID){

    int result;

        // Coordinator process
        if (isCoordinator (procID)){
            result = (procID/workersSet) * (workersSet-1);
        }

        // Worker process
        else{
            result = procID - ((procID/workersSet)+1);
        }

    return result;
}


int AppMPI_Base::getNumPreviousCoordinators (int procID){

    int result;

        // Coordinator process
        if (isCoordinator (procID)){
            result = (procID/workersSet);
        }

        // Worker process
        else{
            result = (procID/workersSet) + 1;
        }

        return result;
}

bool AppMPI_Base::isThisWorkerInMyRange (int masterID, int procID){

    bool result;

        // Init...
        result = false;

        // No workers set defined
        if (workersSet == 0){

            if (masterID != rootProcessID)
                result = false;
            else
                result = true;
        }

        // Workers set defined
        else{

            // Coordinator is ... master! ;)
            if (isCoordinator(masterID)){

                if ((masterID < 0) || (masterID >= numProcesses))
                    throw cRuntimeError("[AppMPI_Base::isThisWorkerInMyRange] Coordinator process not found [%d]\n", masterID);
                else if ((procID < 0) || (procID >= numProcesses))
                    throw cRuntimeError("[AppMPI_Base::isThisWorkerInMyRange] Worker process not found [%d]\n", procID);
                else if ((procID > masterID) && (procID < (masterID+workersSet)))
                    result = true;
                else
                    result = false;
            }
            else{
                result = false;
            }
        }

        return result;
}






void AppMPI_Base::mpi_send (unsigned int procID, int bufferSize){

	SIMCAN_MPI_Message *sm_mpi;
	int rankReceiver;

	    // Get rank!
	    rankReceiver = getRank(procID);
				
		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_SEND);
		currentMPICall->setSender (myRank);
		currentMPICall->setReceiver (rankReceiver);
		currentMPICall->setBufferSize (bufferSize);
		
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] ProcessID:%d - Rank %d - MPI_SEND to %d... %s ", processID, myRank, rankReceiver, currentMPICall->toString().c_str());

		// Create a message and add the corresponding parameters
		sm_mpi = new SIMCAN_MPI_Message ();
		sm_mpi->setIsResponse (false);
		sm_mpi->setRemoteOperation (false);
		sm_mpi->setOperation (currentMPICall->getCall());		
		sm_mpi->setSourceRank (myRank);
		sm_mpi->setDestRank (rankReceiver);
		sm_mpi->setSize (bufferSize);

		if (isProcessInThisNode(rankReceiver)){
			//printf ("----- Process %d sending to process %d in the same node\n", myRank, rankReceiver);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH);
		}
		else{
			//printf ("----- Process %d sending to process %d in ANOTHER node\n", myRank, rankReceiver);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH + bufferSize);
		}

		// Send...
		SIMCAN_request_sendDataToNetwork (sm_mpi, rankReceiver);		
		
		// Next call...
		currentMPICall->clear();		
}



void AppMPI_Base::mpi_send (SIMCAN_MPI_Message *sm_mpi){

    int rankReceiver;

        // Get ranks
        rankReceiver = getRank(sm_mpi->getDestRank());

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_SEND);
		currentMPICall->setSender (myRank);
		currentMPICall->setReceiver (rankReceiver);
		currentMPICall->setBufferSize (sm_mpi->getSize());
		currentMPICall->setSequence (sm_mpi->getSequence());

		// Set MPI_Message attributes
		sm_mpi->setIsResponse (false);
		sm_mpi->setRemoteOperation (false);
		sm_mpi->setOperation (currentMPICall->getCall());
		sm_mpi->setSourceRank (myRank);
		sm_mpi->setDestRank (rankReceiver);


		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Process ID %D - Rank %d executing MPI_SEND... %s ", processID, myRank, currentMPICall->toString().c_str());


		if (isProcessInThisNode(rankReceiver)){
			//printf ("----- Process %d sending to process %d in the same node\n", myRank, rankReceiver);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH);
		}
		else{
			//printf ("----- Process %d sending to process %d in ANOTHER node\n", myRank, rankReceiver);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH + sm_mpi->getSize());
		}

		// Send...
		SIMCAN_request_sendDataToNetwork (sm_mpi, rankReceiver);

		// Next call...
		currentMPICall->clear();
}



bool AppMPI_Base::isProcessInThisNode (unsigned int rankProcess){

	cModule *appMod;
	cModule* currentApp;
	int currentIndex;
	int size;
	bool found;
	int currentRank;


		// Init
		appMod = getParentModule()->getParentModule()->getSubmodule("appModule", 0);
		size = appMod->size();
		currentIndex = 0;
		found = false;

		//printf ("..... Examining vector of %d elements\n", size);

		while ((!found) && (currentIndex<size)){

			currentApp = getParentModule()->getParentModule()->getSubmodule("appModule", currentIndex)->getSubmodule("app");

			if (currentApp->hasPar("myRank")){

				currentRank = currentApp->par("myRank");

				if (rankProcess == currentRank)
					found=true;
			}

		currentIndex++;
	}

	return found;
}



SIMCAN_MPI_Message * AppMPI_Base::mpi_recv (unsigned int procID, int bufferSize){

	SIMCAN_MPI_Message *result;
	int rankSender;

	    // Get rank
	    if (procID == MPI_ANY_SENDER)
	        rankSender = MPI_ANY_SENDER;
	    else
	        rankSender = getRank (procID);

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_RECV);
		currentMPICall->setSender (rankSender);
		currentMPICall->setReceiver (myRank);
		currentMPICall->setBufferSize (bufferSize);

		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] ProcessID: %d - Rank %d - MPI_RECV from %d... %s ", processID, myRank, rankSender, currentMPICall->toString().c_str());

		// Synchronize with the corresponding send
		result = synchronize(MPI_RECV);
		
		//if (result == NULL)
			//waitingForSend = true;

		return result;
}



void AppMPI_Base::mpi_barrier (){

	SIMCAN_MPI_Message *sm_mpi;

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_BARRIER);
		currentMPICall->setSender (myRank);
		currentMPICall->setReceiver (getRank(rootProcessID));
		
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Process ID %d - Rank %d executing BARRIER... %s ", processID, myRank, currentMPICall->toString().c_str());

		// MASTER does Barrier Down!
		if (processID == rootProcessID){
			barriers [processID] = MPI_BARRIER_DOWN;
			checkBarriers();
		}

		// A slave process makes barrier down!
		else{

			// Create a message and add the corresponding parameters
			sm_mpi = new SIMCAN_MPI_Message ();
			sm_mpi->setIsResponse (false);
			sm_mpi->setRemoteOperation (true);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH);
			sm_mpi->setOperation (MPI_BARRIER_DOWN);		
			sm_mpi->setSourceRank (myRank);
			sm_mpi->setDestRank (getRank(rootProcessID));

			// Send...
			SIMCAN_request_sendDataToNetwork (sm_mpi, getRank(rootProcessID));
		}
}



void AppMPI_Base::mpi_barrier_up (){
	
	if (DEBUG_MPIApp)
		showDebugMessage ("Process ID [%d] BARRIER_UP!", processID);

	// Init currentMPICall records...
	currentMPICall->clear();

	// Continue execution
	continueExecution ();
}



void AppMPI_Base::mpi_barrier_down (SIMCAN_MPI_Message *sm_mpi){

	int senderRank, senderID;

		// Sender
		senderRank = sm_mpi->getSourceRank();
		senderID = getProcessIndexByRank(senderRank);

		// Sender ID not found!
		if (senderID == -1)
		    throw cRuntimeError("[AppMPI_Base::mpi_barrier_down] Sender ID not found for process Rank %d!\n", senderRank);

		// Delete current MPI message
		delete (sm_mpi);
		
		// Check process rank!
		if ((senderID >= numProcesses) || (senderID < 0))
			showErrorMessage ("Wrong process ID number: %d", senderID);

		// Checks barriers
		barriers [senderID] = MPI_BARRIER_DOWN;
		
		if (DEBUG_MPIApp)
			showDebugMessage ("Coordinator process [ID:%d - Rank:%d] BARRIER_DOWN process ID [%d]!",
							  processID,
							  myRank, 
							  senderID);
		
		checkBarriers();
}



void AppMPI_Base::initBarriers (){
	
	int i;
	
		for (i=0; i<numProcesses; i++)
			barriers[i] = MPI_BARRIER_UP;		
}



void AppMPI_Base::checkBarriers (){

	int currentProcess;
	bool allDown;
	SIMCAN_MPI_Message *sm_mpi;


		// Init...
		allDown = true;
		currentProcess = 0;

		// Check if all process made BARRIER_DOWN
		while ((currentProcess<numProcesses) && (allDown)){

			if (barriers[currentProcess] == MPI_BARRIER_UP)
				allDown = false;
			else
				currentProcess++;
		}

		// Up barriers...
		if (allDown){
			
			if (DEBUG_MPIApp)
				showDebugMessage ("Coordinator process has received all BARRIERS!");

			// Re-Init barriers!
			initBarriers ();			

			// Broadcast ACKs
			for (currentProcess=1; currentProcess<numProcesses; currentProcess++){

				// Creates BARRIER_UP message
				sm_mpi = new SIMCAN_MPI_Message ();
				sm_mpi->setIsResponse (false);
				sm_mpi->setRemoteOperation (false);
				sm_mpi->setByteLength (MSG_INITIAL_LENGTH);
				sm_mpi->setOperation (MPI_BARRIER_UP);		
				sm_mpi->setSourceRank (myRank);
				sm_mpi->setDestRank (getRank(currentProcess));

				// Search the process's socket!
				SIMCAN_request_sendDataToNetwork (sm_mpi, getRank(currentProcess));
			}
			
			// Init currentMPICall records...
			currentMPICall->clear();

			// Continue execution
			continueExecution ();
		}
}


void AppMPI_Base::mpi_bcast (unsigned int rootID, int bufferSize){

	SIMCAN_MPI_Message *sm_mpi;
	int i;
	unsigned int maxBcastProcesses;
	unsigned int rootRank, destProcessRank;

	    // Get rank
	    rootRank = getRank (rootID);
	    destProcessRank = -1;

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_BCAST);
		currentMPICall->setSender (rootRank);
		currentMPICall->setRoot (rootRank);
		currentMPICall->setReceiver (myRank);
		currentMPICall->setBufferSize (bufferSize);
		
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Process ID:%d - Rank %d executing BCAST... %s ", processID,  myRank, currentMPICall->toString().c_str());

		maxBcastProcesses = (calculateBcastMax () * 2) - 1;
		//maxBcastProcesses = numProcesses/2;

		// If I am root... send messages to each process (bcast)
		if (rootRank==myRank){

			for (i=0; i<numProcesses; i++){

			    // Get destination rank
			    destProcessRank = getRank(i);

				// Send to all process except myself!
				if (destProcessRank != myRank){

					// Create a message and add the corresponding parameters
					sm_mpi = new SIMCAN_MPI_Message ();
					sm_mpi->setIsResponse (false);
					sm_mpi->setRemoteOperation (false);
					
					if (i < maxBcastProcesses)					
						sm_mpi->setByteLength (MSG_INITIAL_LENGTH+bufferSize);
					else
						sm_mpi->setByteLength (MSG_INITIAL_LENGTH);
					
					sm_mpi->setOperation (MPI_BCAST);		
					sm_mpi->setSourceRank (rootRank);
					sm_mpi->setDestRank (destProcessRank);
					sm_mpi->setSize (currentMPICall->getBufferSize());	
					
					// Send...
					SIMCAN_request_sendDataToNetwork (sm_mpi, destProcessRank);
				}
			}
			
			// Next call...
			currentMPICall->clear();
			continueExecution();
		}

		// Slave process wait to recieve scatter messages!
		else
			synchronize(MPI_BCAST);
}



void AppMPI_Base::mpi_scatter (unsigned int rootID, int bufferSize){

	SIMCAN_MPI_Message *sm_mpi;
	int i;
	unsigned int rootRank, destProcessRank;

	    // Get rank
	    rootRank = getRank (rootID);
	    destProcessRank = -1;

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_SCATTER);
		currentMPICall->setSender (rootRank);
		currentMPICall->setRoot (rootRank);
		currentMPICall->setReceiver (myRank);
		currentMPICall->setBufferSize (bufferSize);		
		
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Process ID:%d - Rank %d executing SCATTER... %s ", processID, myRank, currentMPICall->toString().c_str());
			

		// If I am root... send messages to each process (scatter)
		if (rootRank==myRank){

			for (i=0; i<numProcesses; i++){

			    // Get destination rank
			    destProcessRank = getRank(i);

				// Send to all process except myself!
				if (destProcessRank!=myRank){

					// Create a message and add the corresponding parameters
					sm_mpi = new SIMCAN_MPI_Message ();
					sm_mpi->setOperation (MPI_SCATTER);
					sm_mpi->setIsResponse (false);
					sm_mpi->setRemoteOperation (false);
					sm_mpi->setByteLength (MSG_INITIAL_LENGTH+bufferSize);					
					sm_mpi->setSourceRank (rootRank);
					sm_mpi->setDestRank (destProcessRank);
					sm_mpi->setSize (currentMPICall->getBufferSize());				
							
					// Send...
					SIMCAN_request_sendDataToNetwork (sm_mpi, destProcessRank);
				}
			}
			
			// Next call...
			currentMPICall->clear();
			continueExecution();
		}

		// Slave process wait to recieve scatter messages!
		else
			synchronize(MPI_SCATTER);
}



void AppMPI_Base::mpi_gather (unsigned int rootID, int bufferSize){

	SIMCAN_MPI_Message *sm_mpi;
	unsigned int rootRank;

	    // Get rank
	    rootRank = getRank (rootID);

		// Sets currentMPICall records...
		currentMPICall->setCall (MPI_GATHER);
		currentMPICall->setSender (myRank);
		currentMPICall->setReceiver (rootRank);
		currentMPICall->setRoot (rootRank);
		currentMPICall->setBufferSize (bufferSize);
		currentMPICall->setPendingACKs (numProcesses-1);
				
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Process ID:%d - Rank %d executing GATHER... %s ", processID, myRank, currentMPICall->toString().c_str());
			

		// If I am not root... send messages to root (gather)
		if (rootRank!=myRank){

			// Create a message and add the corresponding parameters
			sm_mpi = new SIMCAN_MPI_Message ();
			sm_mpi->setOperation (MPI_GATHER);
			sm_mpi->setIsResponse (false);
			sm_mpi->setRemoteOperation (false);
			sm_mpi->setByteLength (MSG_INITIAL_LENGTH+bufferSize);	
							
			sm_mpi->setSourceRank (myRank);
			sm_mpi->setDestRank (rootRank);
			sm_mpi->setSize (currentMPICall->getBufferSize());

			// Send...
			SIMCAN_request_sendDataToNetwork (sm_mpi, rootRank);
			
			// next call
			currentMPICall->clear();
			continueExecution ();
		}
}



void AppMPI_Base::mpi_gather_arrives (SIMCAN_Message *sm){

	// Checks if waiting for GATHER Ack
	if (currentMPICall->getCall() != MPI_GATHER)
		showErrorMessage ("Expected MPI_Gather! Received:%s", 
			callToString(currentMPICall->getCall()).c_str());		

	// Update pending Acks
	currentMPICall->arrivesACK();

	// If there is no pending ACKs... process next MPI Call..
	if (currentMPICall->getPendingACKs() == 0){
		
		// Debug...
		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Rank [ID:%d - Rank:%d] All GATHER received...", processID, myRank);
		
		currentMPICall->clear();
		continueExecution();
	}
}



SIMCAN_MPI_Message * AppMPI_Base::synchronize(int incomingCall){

	MPI_Call *searchedCall;		
	
	unsigned int searchCall;
	unsigned int searchSender;
	int searchSize;
	SIMCAN_MPI_Message * result;
		
												
		// Init...
		searchCall = MPI_NO_VALUE;
		searchSender = MPI_NO_VALUE;
		searchSize = 0;
		searchedCall = NULL;
		result = NULL;


		// This process has not executed the corresponding operation yet!
		if (currentMPICall->getCall() == MPI_NO_VALUE){
			return result;
		}			
		
		// Current MPI Call is MPI_RECV... search a MPI_SEND!
		else if (currentMPICall->getCall() == MPI_RECV){			
			searchCall = MPI_SEND;
			searchSender = currentMPICall->getSender();
			searchSize = currentMPICall->getBufferSize();
		}
		
		
		// Current MPI Call is MPI_BCAST!
		else if (currentMPICall->getCall() == MPI_BCAST){
		  	searchCall = MPI_BCAST;
			searchSender = currentMPICall->getRoot();
			searchSize = currentMPICall->getBufferSize();						
		}
		
		
		// Current MPI Call is MPI_SCATTER 
		else if	(currentMPICall->getCall() == MPI_SCATTER){			
			searchCall = MPI_SCATTER;
			searchSender = currentMPICall->getSender();
			searchSize = currentMPICall->getBufferSize();				
		}	
				
		
		// No synch call!
		//else
			//showErrorMessage ("Unknown call for synchronizing!!! %s", callToString (currentMPICall->getCall()).c_str());			
		
		// Debug!		
		if (DEBUG_MPIApp)
			showDebugMessage ("Process [ID:%d - Rank:%d] Synchronizing... Searching %s - Sender:%u - Size:%d. MPI Call list:%s",
                                processID,
                                myRank,
                                callToString (searchCall).c_str(),
                                searchSender,
                                searchSize,
                                incomingMPICallsToString().c_str());
		
		// Search for the incoming call...
		searchedCall = searchMPICall (searchCall, searchSender, searchSize);			
			
		// MPI incoming call found!
		if (searchedCall != NULL){
			
			if (DEBUG_MPIApp)
				showDebugMessage ("Process [ID:%d - Rank:%d] Synch found!!! %s with incoming %s!",
				                    processID,
									myRank,
									callToString (currentMPICall->getCall()).c_str(),
									callToString (searchedCall->getCall()).c_str());			

			// Set the last synchronized call
			if (currentMPICall->getSender() == MPI_ANY_SENDER)
				currentMPICall->setSender(searchedCall->getSender());

			currentMPICall->copyMPICall(lastSynchroMPICall);

			// Get parameters
			result = new SIMCAN_MPI_Message ();
			result->setSourceRank(currentMPICall->getSender());
			result->setDestRank(currentMPICall->getReceiver());
			result->setFileName(currentMPICall->getFileName().c_str());
			result->setOffset(currentMPICall->getOffset());
			result->setSequence(searchedCall->getSequence());
			result->setResult(searchedCall->getResult());

			// Clean MPI Call
			currentMPICall->clear();
			
			// Delete current incoming call
			deleteMPICall (searchedCall);

			// Continue execution
			//if (incomingCall == MPI_SEND)
			continueExecution();
		}

		return result;
}



void AppMPI_Base::insertMPICall (SIMCAN_MPI_Message *sm_mpi){

	MPI_Call *newCall;

		// Create new call
		newCall = new MPI_Call ();				
		newCall->setCall (sm_mpi->getOperation());
		newCall->setSender (sm_mpi->getSourceRank());
		newCall->setReceiver (myRank);
		newCall->setFileName (sm_mpi->getFileName());
		newCall->setOffset (sm_mpi->getOffset());
		newCall->setBufferSize (sm_mpi->getSize());
		newCall->setSequence (sm_mpi->getSequence());
		newCall->setResult (sm_mpi->getResult());

		if (DEBUG_MPIApp)
			showDebugMessage ("[AppMPI_Base] Inserting call in queue of rank [ID:%d - Rank:%d] %s", processID, myRank, newCall->toString().c_str());
		
		// If collective call... set root process!
		if ((newCall->getCall() == MPI_SCATTER) || (newCall->getCall() == MPI_BCAST))
			newCall->setRoot (sm_mpi->getSourceRank());		

		// Insert on list
		mpiCallList.push_back (newCall);
}



MPI_Call* AppMPI_Base::searchMPICall(unsigned int call, unsigned int sender, int size){

	MPI_Call *auxCall;
	bool found;
	list <MPI_Call*>::iterator listIterator;

		// Init...
		found = false;
		auxCall = NULL;

		// Search the corresponding call...
		for (listIterator=mpiCallList.begin(); ((listIterator!=mpiCallList.end()) && (!found)); listIterator++){

			// Any sender?
			if (sender == MPI_ANY_SENDER){
				
				if (((*listIterator)->getCall() == call) && ((*listIterator)->getBufferSize() == size)){
					found = true;
					auxCall = *listIterator;
				}				
			}
			
			
			// Specific sender!
			else{
				
				if ( ((*listIterator)->getCall() == call) && ((*listIterator)->getSender() == sender) && ((*listIterator)->getBufferSize() == size) ){
					found = true;
					auxCall = *listIterator;
				}				
			}			
		}

	return auxCall;
}



void AppMPI_Base::deleteMPICall(MPI_Call* removedCall){

	mpiCallList.remove (removedCall);	
}



string AppMPI_Base::incomingMPICallsToString (){

	std::ostringstream osStream;
	list <MPI_Call*>::iterator listIterator;
	
		if (mpiCallList.empty())
			osStream << "Empty!";

		// Search the corresponding call...
		for (listIterator=mpiCallList.begin(); (listIterator!=mpiCallList.end()); listIterator++){
			
			osStream << endl;
			osStream << (*listIterator)->toString();
			osStream << endl;			
		}
		
	return osStream.str();
}



string AppMPI_Base::callToString (int call){
	
	string result;
		
		if (call == MPI_NO_VALUE)
			result =  "MPI_NO_VALUE";
		else if (call == MPI_BARRIER_UP)
			result =  "MPI_BARRIER_UP";
		else if (call == MPI_BARRIER_DOWN)
			result =  "MPI_BARRIER_DOWN";
		else if (call == MPI_SEND)
			result =  "MPI_SEND";
		else if (call == MPI_RECV)
			result =  "MPI_RECV";
		else if (call == MPI_BARRIER)
			result =  "MPI_BARRIER";
		else if (call == MPI_BCAST)
			result =  "MPI_BCAST";
		else if (call == MPI_SCATTER)
			result =  "MPI_SCATTER";
		else if (call == MPI_GATHER)
			result =  "MPI_GATHER";
		else if (call == MPI_FILE_OPEN)
			result =  "MPI_FILE_OPEN";
		else if (call == MPI_FILE_CLOSE)
			result =  "MPI_FILE_CLOSE";
		else if (call == MPI_FILE_DELETE)
			result =  "MPI_FILE_DELETE";
		else if (call == MPI_FILE_READ)
			result =  "MPI_FILE_READ";
		else if (call == MPI_FILE_WRITE)
			result =  "MPI_FILE_WRITE";
		else if (call == MPI_FILE_CREATE)
			result =  "MPI_FILE_CREATE";
		else
			result = "UNKNOWN_MPI_CALL";
		
	return result;
}



void AppMPI_Base::addNewConnection (string hostName, unsigned int port, unsigned int rank){

	mpiConnector newMpiConnector;

		// Congigure the new connection
		newMpiConnector.destAddress = hostName;
		newMpiConnector.destPort = port;
		newMpiConnector.idConnector = rank;

		 if (DEBUG_MPIApp)
		     showDebugMessage ("Adding connection from process [ID:%d - Rank:%d] to process [Rank:%d]@%s:%d\n", processID, myRank, rank, hostName.c_str(), port);

		// Add the connection to the connection vector
		mpiConnections.push_back (newMpiConnector);
}



void AppMPI_Base::establishAllConnections (){

	int i;		
	
		// Establish all connections
		for (i=0; i<mpiConnections.size(); i++){										
		
			if (DEBUG_MPIApp)
				showDebugMessage ("Process [ID:%d - Rank:%d] establishing connection with process [Rank:%d]d@%s:%d", processID, myRank, mpiConnections[i].idConnector, mpiConnections[i].destAddress.c_str(), mpiConnections[i].destPort);
		
						
				SIMCAN_request_createConnection (mpiConnections[i].destAddress, 
												 mpiConnections[i].destPort, 
												 mpiConnections[i].idConnector);
		}
}



string AppMPI_Base::mpiCommunicationsToString (){

	std::ostringstream osStream;
	int i;

		osStream.str("");

		// Connection list enable?
		osStream << "Communications configured for processing with [ID:" << processID << " - Rank:" << myRank << "]" << endl;

			for (i=0; i<mpiConnections.size(); i++){
				osStream << "  Connection[" << i << "] IP:" << mpiConnections[i].destAddress <<
							"  Port:" << mpiConnections[i].destPort <<
							"  ID Conn:" << mpiConnections[i].idConnector <<	endl;
			}		

	return osStream.str();
}



unsigned int AppMPI_Base::calculateBcastMax (){

	int maxProcesses;
	unsigned int i;
	bool found;
	
		found = false;	
		maxProcesses = 0;
		i=1;
		
			while ((!found) && (i<=numProcesses)){
		
				maxProcesses = (int) pow (2, i);
				
				if (maxProcesses >= numProcesses)
					found = true;
				else
					i++;
			}

		if (!found)
			showErrorMessage ("Number not found!!!");

		return i;
}



MPI_Call* AppMPI_Base::getLastSynchronizedCall (){

	return (lastSynchroMPICall);
}



int AppMPI_Base::getProcessIndexByRank (unsigned int processRank){

    bool found;
    int i, index;

        // Init...
        index = i = 0;
        found = false;

        // Search...
        while ((i<ranksVector.size()) && (!found)){

            if (ranksVector[i] == processRank)
                found = true;
            else
                i++;
        }

        if (found)
            index = i;
        else
            index = -1;

    return index;
}


