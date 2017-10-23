#include "NullBlockManager.h"

Define_Module(NullBlockManager);


NullBlockManager::~NullBlockManager(){
}


void NullBlockManager::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "NullBlockManager." << getId();
		moduleIdName = osStream.str();
		
		numBlockServers = par ("numBlockServers");
		
		if (numBlockServers > 1)
			showErrorMessage ("NullBlockManager cannot be used with more than one disk!!!");

		// Init the super-class
		cSIMCAN_Base::initialize();	

		// Init the gate IDs to/from Scheduler
	    toSchedulerGate = gate ("toScheduler");
	    fromSchedulerGate = gate ("fromScheduler");	  
	    
	    // Init the gates IDs to/from BlockServers
	    toBlockServerGates = new cGate* [numBlockServers];
	    fromBlockServerGates = new cGate* [numBlockServers];  

	    for (i=0; i<numBlockServers; i++){
			toBlockServerGates[i] = gate ("toBlockServer", i);
			fromBlockServerGates[i] = gate ("fromBlockServer", i);
	    }
}


void NullBlockManager::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* NullBlockManager::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from scheduler
		if (msg->getArrivalGate()==fromSchedulerGate){
			if (toSchedulerGate->getNextGate()->isConnected()){
				return (toSchedulerGate);
			}
		}

		// If msg arrive from blockServers
		else if (msg->arrivedOn("fromBlockServer")){
			for (i=0; i<numBlockServers; i++)
				if (msg->arrivedOn ("fromBlockServer", i))
					return (toBlockServerGates[i]);
		}

	// If gate not found!
	return NULL;
}


void NullBlockManager::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void NullBlockManager::processRequestMessage (SIMCAN_Message *sm){
	sendRequestMessage (sm, toBlockServerGates[0]);
}


void NullBlockManager::processResponseMessage (SIMCAN_Message *sm){
	sendResponseMessage (sm);	
}

