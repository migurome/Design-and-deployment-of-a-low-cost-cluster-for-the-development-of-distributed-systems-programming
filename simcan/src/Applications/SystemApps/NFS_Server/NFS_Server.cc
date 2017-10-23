#include "NFS_Server.h"


Define_Module(NFS_Server);


NFS_Server::~NFS_Server(){	
}


void NFS_Server::initialize(){

	std::ostringstream osStream;

		// Set the moduleIdName
		osStream << "NFS_Server." << getId();
		moduleIdName = osStream.str();

		// Super-init
		SimcanAPI::initialize();

		// Get module params		
		localPort = par("localPort");

	    // Listen...
	    createConnetion();
}


void NFS_Server::finish(){

	// Finish the super-class
    SimcanAPI::finish();
}


void NFS_Server::createConnetion (){	

	if (DEBUG_NFS_Server)
		showDebugMessage ("Creating a listening connection on port %d", localPort);
	
	SIMCAN_request_createListenConnection (localPort);
}


void NFS_Server::processSelfMessage (cMessage *msg){
	showErrorMessage ("NFS Server cannot receive self messages!");
}


void NFS_Server::processRequestMessage (SIMCAN_Message *sm){

	if (DEBUG_NFS_Server)
			showDebugMessage ("Receiving a request message: %s", sm->contentsToString(DEBUG_MSG_NFS_Server).c_str());
	
	sm->setRemoteOperation (false);
	sendRequestMessage (sm, toOSGate);
}


void NFS_Server::processResponseMessage (SIMCAN_Message *sm){

	if (DEBUG_NFS_Server)
	    showDebugMessage ("Receiving a response message: %s", sm->contentsToString(DEBUG_MSG_NFS_Server).c_str());
		
	sendResponseMessage (sm);
}


