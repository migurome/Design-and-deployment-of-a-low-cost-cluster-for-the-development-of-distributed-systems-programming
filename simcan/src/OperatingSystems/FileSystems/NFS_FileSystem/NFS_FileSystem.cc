#include "NFS_FileSystem.h"

Define_Module (NFS_FileSystem);


NFS_FileSystem::~NFS_FileSystem(){

}


void NFS_FileSystem::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "NFS_FS." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();


		// Debug?
		//showStartedModule (" %s ", serverListToString().c_str());

		// Init gate IDs
		fromVMGate = gate ("fromVM");
		toVMGate = gate ("toVM");
		fromIORGate = gate ("fromIOR");
		toIORGate = gate ("toIOR");
		
		// Create the latency message and flush message
	    latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());	    
	    pendingMessage = NULL;
}


void NFS_FileSystem::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* NFS_FileSystem::getOutGate (cMessage *msg){

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


void NFS_FileSystem::processCurrentRequestMessage (){
	showErrorMessage ("This module cannot receive messages! To be implemented... ");
}


void NFS_FileSystem::processSelfMessage (cMessage *msg){
	showErrorMessage ("This module cannot receive messages! To be implemented... ");
}


void NFS_FileSystem::processRequestMessage (SIMCAN_Message *sm){
	showErrorMessage ("This module cannot receive messages! To be implemented... ");
}


void NFS_FileSystem::processResponseMessage (SIMCAN_Message *sm){
	showErrorMessage ("This module cannot receive messages! To be implemented... ");
}



