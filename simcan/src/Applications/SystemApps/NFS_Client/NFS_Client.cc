#include "NFS_Client.h"


Define_Module (NFS_Client);


NFS_Client::~NFS_Client(){
	
	// Remove all requests
	SMS_nfs->clear();	
	delete (SMS_nfs);		
}


void NFS_Client::initialize(){
			
	cMessage *waitToConnectMsg;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "NFS_Client." << getId();
		moduleIdName = osStream.str();
		
		// Super-init
		SimcanAPI::initialize();
		
		// Get module params		 	    
	    destAddress = (const char*) par("connectAddress");    	    	
    	destPort = par("connectPort");
		useServersFile = par ("useServersFile");
		connectionDelay = par ("connectionDelay");
		requestSizeKB = par ("requestSizeKB");
		
		// Check parameters!
		if (requestSizeKB < 0)
			showErrorMessage ("Request size must be > 0");
				
		// Show all communication links...
		showStartedModule (" %s ", connectionsToString(true).c_str());

		// Creates the SMS object
		SMS_nfs = new SMS_NFS (requestSizeKB);

	    // Create WAIT_TO_CONNECT_MESSAGE...
		waitToConnectMsg = new cMessage (SM_WAIT_TO_CONNECT.c_str());
	    scheduleAt (simTime() + connectionDelay, waitToConnectMsg);
}


void NFS_Client::finish(){

	// Finish the super-class
    SimcanAPI::finish();
}


void NFS_Client::processSelfMessage (cMessage *msg){

	int result;
		

		// Establish connection with server...
		if (!strcmp (msg->getName(), SM_WAIT_TO_CONNECT.c_str())){
			
			// Set up a connection using a server file
			if (useServersFile){
				
				// Parse the nfs_servers file (get connections info from file)
				result = createConnectionsWithServers ();
				
				if (result == SIMCAN_ERROR)
					showErrorMessage ("Error loading servers file!");			
				else
					showStartedModule ("Servers file found! Using data in servers file to establish connections!");						
			}
			
			// Set up a connection with application parameters
			else{
				SIMCAN_request_createConnection (destAddress,								  
							      destPort,
							      DEFAULT_SERVER_ID);
								      
				showStartedModule ("Setting up a connection using local module parameters!");
			}
			
			delete (msg);
		}
}


void NFS_Client::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_Message *subRequest;
	SIMCAN_App_IO_Message *sm_io;
	int index;
		
		
		// Search the corresponding connection
		index = searchConnectionById (sm->getCommId());
		
		if (index == NOT_FOUND)
			showErrorMessage ("Server with ID=%d NOT found! %s", sm->getCommId(), sm->contentsToString(true).c_str());
				
		// Generate the corresponding subrequests
		SMS_nfs->splitRequest (sm);

		// Show complete SMS
		showSMSContents (DEBUG_SMS_NFS_Client);

		// Cast request!
		sm_io = check_and_cast<SIMCAN_App_IO_Message *>(sm);

		// Verbose mode? Show detailed request		
		if (DEBUG_NFS_Client)
			showDebugMessage ("Processing original request:%s from message:%s", 
								SMS_nfs->requestToString(sm_io, DEBUG_MSG_NFS_Client).c_str(),
								sm_io->contentsToString(DEBUG_MSG_NFS_Client).c_str());		

		// Send subRequest to server!!!
		subRequest = SMS_nfs->popNextSubRequest (sm);

		// There is a subRequest!
		if (subRequest != NULL){

			// Cast subRequest!
			sm_io = check_and_cast<SIMCAN_App_IO_Message *>(subRequest);

			if (DEBUG_NFS_Client)			
				showDebugMessage ("Processing subRequest: %s", sm_io->contentsToString(DEBUG_MSG_NFS_Client).c_str());								

			// Send packet!
			SIMCAN_request_sendDataToNetwork (sm_io, sm_io->getCommId());
		}
		else
			showErrorMessage ("First subRequest not found!");
}


void NFS_Client::processResponseMessage (SIMCAN_Message *sm){
	
	SIMCAN_App_NET_Message *sm_net;
	SIMCAN_App_IO_Message *sm_io;	
	
		
		// Try to cast to SIMCAN_App_NET_Message
		sm_net = dynamic_cast<SIMCAN_App_NET_Message *>(sm);

		// NET call response...
		if (sm_net != NULL){
			//printf("---> NFS_Client NET %s\n", sm_net->contentsToString(true).c_str());
			processNetCallResponse (sm_net);			
		}			
		
		// Not a NET call... try cast to I/O call.
		else{
			
			// Cast to SIMCAN_App_NET_Message
			sm_io = dynamic_cast<SIMCAN_App_IO_Message *>(sm);
			
			// IO call response...
			if (sm_io != NULL){
				//printf("---> NFS_Client NET %s\n", sm_net->contentsToString(true).c_str());
				processIOCallResponse (sm_io);
			}
				
			else
				showErrorMessage ("Unknown response message: %s", sm->contentsToString(true).c_str());			
		}	
}


int NFS_Client::createConnectionsWithServers (){

	int i, result;
	CfgServers *serverCfg;
	FileConfigManager * fileCfgMng;
	cModule *fileConfigModule;
	
		// Init...
		result = SIMCAN_OK;
			
		// Storage node with local fileConfig?
		fileConfigModule = getParentModule()->getParentModule()->getSubmodule("fileConfig");
		
		// Not storage node!
		if (fileConfigModule == NULL){

			// Racked compute node. Get file config from Rack!
			fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");

			// Wrong packaging configuration!
			if (fileConfigModule == NULL)
				showErrorMessage ("Wrong packaging configuration. Can not find FileConfigManager");
		}	
		

		// Get the corresponding servers...
		fileCfgMng = check_and_cast<FileConfigManager *>(fileConfigModule);
		serverCfg = fileCfgMng->getServerNfsCfg();
		
		if (serverCfg != NULL){

			// No servers file!
			if (serverCfg->getNumServers() == 0)
				result = SIMCAN_ERROR;
			
			else{
	
				// Store info in current class
				for (i=0; i<serverCfg->getNumServers(); i++){
										
					if (DEBUG_NFS_Client)
						showDebugMessage ("Creating connection with:%s:%d", serverCfg->getHostName(i).c_str(), serverCfg->getPort (i)); 
																   
		
					SIMCAN_request_createConnection (serverCfg->getHostName (i),									  
												      serverCfg->getPort (i),
												      serverCfg->getId(i));
				}
			}
		}
		else{
			result = SIMCAN_ERROR;
		}

	return result;
}


void NFS_Client::showSMSContents (bool showContents){

	int i;
	int numRequests;
	
		if (showContents){

			// Get the number of requests
			numRequests = SMS_nfs->getNumberOfRequests();
	
			showDebugMessage ("Showing the complete SMS vector... (%d requests)", numRequests);
	
			// Get all requests
			for (i=0; i<numRequests; i++)			
				showDebugMessage (" Request[%d]:%s ", i, SMS_nfs->requestToStringByIndex(i).c_str());
		}
}


void NFS_Client::processNetCallResponse (SIMCAN_App_NET_Message *sm_net){
	
	// Create connection response...
	if (sm_net->getOperation () == SM_CREATE_CONNECTION){
		
		// Set the established connection.
		setEstablishedConnection (sm_net);		
		
		delete (sm_net);
	}
	
	else
		showErrorMessage ("Unknown NET call response :%s", sm_net->contentsToString(true).c_str());	
}


void NFS_Client::processIOCallResponse (SIMCAN_App_IO_Message *sm_io){	
	
	SIMCAN_App_IO_Message *requestMsg;
	SIMCAN_App_IO_Message *nextSubRequest;
	cMessage *parentRequest;		
		
		
		// Parent request
		parentRequest = sm_io->getParentRequest();

		// SubRequest arrives...
		SMS_nfs->arrivesSubRequest (sm_io, parentRequest);

		// Verbose mode? Show detailed request
		if (DEBUG_NFS_Client)
			showDebugMessage ("Arrives subRequest of:%s", SMS_nfs->requestToString(parentRequest, DEBUG_MSG_NFS_Client).c_str());
		

		// If all request have arrived... erase request and send back the response!
		if (SMS_nfs->arrivesAllSubRequest(parentRequest)){

			if (DEBUG_NFS_Client)
				showDebugMessage ("Arrives all subRequest. %s", SMS_nfs->requestToString(parentRequest, DEBUG_MSG_NFS_Client).c_str());

			// Removes the request object!
			SMS_nfs->removeRequest (parentRequest);

			// Show complete SMS
			showSMSContents (DEBUG_SMS_NFS_Client);

			// Casting!
			requestMsg = check_and_cast <SIMCAN_App_IO_Message*> (parentRequest);

			// Now is a Response Message
			requestMsg->setIsResponse (true);

			// Update the mesage length!
			requestMsg->updateLength ();

			// Sends the message
			sendResponseMessage (requestMsg);
		}

		// Send the next subRequest to server!
		else{

			nextSubRequest = check_and_cast <SIMCAN_App_IO_Message*> (SMS_nfs->popNextSubRequest (parentRequest));

			// Debug..
			if (DEBUG_NFS_Client)
				showDebugMessage ("Sending next subRequest: %s", nextSubRequest->contentsToString(DEBUG_MSG_NFS_Client).c_str());
								
			SIMCAN_request_sendDataToNetwork (nextSubRequest, nextSubRequest->getCommId());			
		}
}




