#include "ParallelFileSystem.h"

Define_Module (ParallelFileSystem);


#define PVFS_AVG 0.000045
#define PVFS_STD_DEV 0.00003


ParallelFileSystem::~ParallelFileSystem(){
	
	SMS_pfs->clear();
	delete (SMS_pfs);
	connections.clear();
}


void ParallelFileSystem::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "Parallel_FS." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Get the strideSize
		strideSizeKB = par ("strideSizeKB");
		clientAppIndex = par ("clientAppIndex");

		// Parse serversFile and fill the servers vector!
		parseServersFile();

		// Debug?
		showStartedModule (" %s ", serverListToString().c_str());

		// Creates the SMS object
		SMS_pfs = new SMS_PFS (strideSizeKB, numberOfServers);

		// Init gate IDs
		fromVMGate = gate ("fromVM");
		toVMGate = gate ("toVM");
		fromIORGate = gate ("fromIOR");
		toIORGate = gate ("toIOR");
		
		// Create the latency message and flush message
	    latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());	    
	    pendingMessage = NULL;
}


void ParallelFileSystem::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* ParallelFileSystem::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromVMGate){
			if (toVMGate->getNextGate()->isConnected()){
				return (toVMGate);
			}
		}

		// If msg arrive from Inputs
		else if (msg->getArrivalGate()==fromIORGate){
			if (toIORGate->getNextGate()->isConnected()){
				return (toIORGate);
			}
		}

	// If gate not found!
	return NULL;
}


void ParallelFileSystem::processCurrentRequestMessage (){

	SIMCAN_Message *sm;
	cMessage *unqueuedMessage;

		// If exists enqueued requests
		if (!queue.empty()){

			// Pop!
			unqueuedMessage = (cMessage *) queue.pop();

			// Dynamic cast!
			sm = check_and_cast<SIMCAN_Message *>(unqueuedMessage);
						
			// Process
			processRequestMessage (sm);
		}
		else
			processSubRequests();
}


void ParallelFileSystem::processSelfMessage (cMessage *msg){	

	// Is a pending message?
	if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){


		// There is a pending message...
		if (pendingMessage != NULL){

			if ((DEBUG_DETAILED_Parallel_FS) && (DEBUG_Parallel_FS))
				showDebugMessage ("End of PFS processing! Pending request:%s ", pendingMessage->contentsToString(DEBUG_MSG_Parallel_FS).c_str());
				
			cancelEvent (msg);
			
			sendRequestMessage (pendingMessage, toIORGate);						
		}

		// No pending message waiting to be sent...
		else			
			showErrorMessage ("There is no pending message waiting to be sent! :(");		
	}

	else
		showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void ParallelFileSystem::processRequestMessage (SIMCAN_Message *sm){

	int i;
	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_Message *strippedRequest;
	SIMCAN_App_IO_Message *sm_sub;


		// Casting to debug!
		sm_io = check_and_cast<SIMCAN_App_IO_Message *>(sm);

		// Close operation! Just send the response message!
		if (sm_io->getOperation() == SM_CLOSE_FILE){

		 	if (DEBUG_Parallel_FS)
				showDebugMessage ("Processing Request (Close). %s",									
									sm_io->contentsToString(DEBUG_MSG_Parallel_FS).c_str());

			// Answer message is sent back...
			sm_io->setIsResponse (true);

			// Send response
			sendResponseMessage (sm_io);
		}

		// The rest operations require communication with servers...
		else{

			// Set PFS client index in App vector
			sm_io->setNextModuleIndex(clientAppIndex);

			// Now this request is remote!
			sm_io->setRemoteOperation (true);

			// Splits incomming request!
			SMS_pfs->splitRequest (sm_io);

			// Verbose mode? Show detailed request
			if (DEBUG_Parallel_FS)
				showDebugMessage ("Processing request:%s from message:%s", 
									SMS_pfs->requestToString(sm_io, DEBUG_SMS_Parallel_FS).c_str(),
									sm_io->contentsToString(DEBUG_MSG_Parallel_FS).c_str());			

			// Show the complete SMS vector?
			showSMSContents(DEBUG_ALL_SMS_Parallel_FS);
			
			// Process next request!
			processSubRequests();
		}
}


void ParallelFileSystem::processSubRequests (){

	SIMCAN_App_IO_Message *sm_io;		
	double currentLatency;
	
	
		// Currently processing?
		if (latencyMessage->isScheduled()){

			if (DEBUG_Parallel_FS)
				showDebugMessage ("Currently processing... current subRequest must wait!");

			return;
		}

		// Init...
		pendingMessage = NULL;
		currentLatency = 0.0;

		// Get the first subrequest!
		sm_io = SMS_pfs->getFirstSubRequest();

		// There is, at least, one subRequest...
		if (sm_io != NULL){
			
			// Pop subrequest!
			sm_io = SMS_pfs->popSubRequest();

			// Send..
			pendingMessage = sm_io;
			
			currentLatency = fabs (normal(PVFS_AVG, PVFS_STD_DEV));
			scheduleAt (currentLatency+simTime(), latencyMessage);
		}
}


void ParallelFileSystem::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *requestMsg;
	cMessage *parentRequest;


		// Parent request
		parentRequest = sm->getParentRequest();

		// SubRequest arrives...
		SMS_pfs->arrivesSubRequest (sm, parentRequest);

		// Casting!
		requestMsg = check_and_cast <SIMCAN_App_IO_Message*> (parentRequest);

		// Verbose mode? Show detailed request
		if (DEBUG_DETAILED_Parallel_FS)
			showDebugMessage ("Processing response:%s from message:%s", 
							SMS_pfs->requestToString(parentRequest, DEBUG_SMS_Parallel_FS).c_str(),
							requestMsg->contentsToString(DEBUG_MSG_Parallel_FS).c_str());

		// If all request have arrived... erase request and send back the response!
		if (SMS_pfs->arrivesAllSubRequest (parentRequest)){

			// Debug!			
			if ((DEBUG_DETAILED_Parallel_FS) && (DEBUG_Parallel_FS))
				showDebugMessage ("Arrives all subRequest. %s", SMS_pfs->requestToString(parentRequest, DEBUG_SMS_Parallel_FS).c_str());

			// Removes the request object!
			SMS_pfs->removeRequest (parentRequest);

			// Show complete SMS
			showSMSContents(DEBUG_ALL_SMS_Parallel_FS);

			// Now is a Response Message
			requestMsg->setIsResponse (true);

			// Update the mesage length!
			requestMsg->updateLength();

			// Sends the message
			sendResponseMessage (requestMsg);
		}
}


void ParallelFileSystem::parseServersFile (){

	int i, result;
	CfgServers *serverCfg;
	FileConfigManager * fileCfgMng;
	cModule *fileConfigModule;
	connector newServer;
		
			
		// Storage node with local fileConfig?
		fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
		
		// Not storage node!
		if (fileConfigModule == NULL){
			
			// Single node using a global file config
			fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
			// Not a global file Config
			if (fileConfigModule == NULL){
				
				// Racked compute node. Get file config from Rack!		
				fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
				// Wrong packaging configuration!
				if (fileConfigModule == NULL)
					showErrorMessage ("Wrong packaging configuration. Can not find FileConfigManager");
			}
		}	
		
		fileCfgMng = check_and_cast<FileConfigManager *>(fileConfigModule);
		serverCfg = fileCfgMng->getServerPfsCfg();
		
		if (serverCfg != NULL){

			// Get the number of servers
			numberOfServers=serverCfg->getNumServers();
	
			// Store info in current class
			for (i=0; i<numberOfServers; i++){
	
				newServer.address = serverCfg->getHostName (i);
				newServer.port = serverCfg->getPort (i);
				newServer.id = serverCfg->getId (i);
				connections.push_back (newServer);
			}
		}
}


string ParallelFileSystem::serverListToString (){

	std::ostringstream osStream;
	int i;

	osStream << "Server list...(" << numberOfServers << " servers)" << endl;

		for (i=0; i<connections.size(); i++){
			osStream << "  Connection[" << i << "] Address:" << connections[i].address <<
						"  Port:" << connections[i].port <<
						"  ID:" << connections[i].id << endl;
		}

	return osStream.str();
}


void ParallelFileSystem::showSMSContents (bool showContents){

	int i;
	int numRequests;
	
		if ((showContents) && (DEBUG_Parallel_FS)){
			
			// Get the number of requests
			numRequests = SMS_pfs->getNumberOfRequests();
	
			// Debug...
			showDebugMessage ("Showing the complete SMS vector... (%d requests)", numRequests);
	
			// Get all requests
			for (i=0; i<numRequests; i++)
				showDebugMessage (" Request[%d]:%s ", i, SMS_pfs->requestToStringByIndex(i).c_str());
		}
}





