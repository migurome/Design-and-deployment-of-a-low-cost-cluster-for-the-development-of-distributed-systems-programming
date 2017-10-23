#include "ServiceRedirector.h"

Define_Module (ServiceRedirector);


ServiceRedirector::~ServiceRedirector(){
	
}


void ServiceRedirector::initialize(){

	std::ostringstream osStream;	
	int i;

	    // Set the moduleIdName
		osStream << "ServiceRedirector." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();
	    
	    // Get module params
	    numApps = par ("numApps");
	    
		    // Init the gate ids
		    toAppGates = new cGate* [numApps];
		    fromAppGates = new cGate* [numApps];
		    
		    for (i=0; i<numApps; i++){
				toAppGates[i] = gate ("toApps", i);
				fromAppGates[i] = gate ("fromApps", i);
		    }
		    
		    fromMemoryGate = gate ("fromMemory");
		    fromNetGate = gate ("fromNet");
		    fromCPUGate = gate ("fromCPU");
		    
		    toMemoryGate = gate ("toMemory");
		    toNetGate = gate ("toNet");
		    toCPUGate = gate ("toCPU");		    	    	
}


void ServiceRedirector::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* ServiceRedirector::getOutGate (cMessage *msg){

	int i;


		// If msg arrive from Applications
		if (msg->arrivedOn("fromApps")){
			for (i=0; i<numApps; i++)
				if (msg->arrivedOn ("fromApps", i))
					return (gate("toApps", i));
		}

		// If msg arrive from Memory
		else if (msg->getArrivalGate()==fromMemoryGate){
			if (gate("toMemory")->getNextGate()->isConnected()){
				return (toMemoryGate);
			}
		}
		
		// If msg arrive from Net
		else if (msg->getArrivalGate()==fromNetGate){
			if (gate("toNet")->getNextGate()->isConnected()){
				return (toNetGate);
			}
		}
		
		// If msg arrive from CPU
		else if (msg->getArrivalGate()==fromCPUGate){
			if (gate("toCPU")->getNextGate()->isConnected()){
				return (toCPUGate);
			}
		}		
		
	// If gate not found!
	return NULL;
}


void ServiceRedirector::processSelfMessage (cMessage *msg){
}


void ServiceRedirector::processRequestMessage (SIMCAN_Message *sm){

	// Msg cames from Network
	if (sm->getArrivalGate() == fromNetGate){
		
		if (DEBUG_Service_Redirector)
			showDebugMessage ("Incomming request from NET... sending to App[%d]. %s", 
								sm->getNextModuleIndex(), 
								sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
			
		sendRequestMessage (sm, toAppGates[sm->getNextModuleIndex()]);		
	}
	
	
	// Msg cames from CPU
	else if (sm->getArrivalGate() == fromCPUGate){		
		showErrorMessage ("This module cannot receive request messages from CPU!!!");
	}
	
	
	// Msg cames from Memory
	else if (sm->getArrivalGate() == fromMemoryGate){
		
		if (DEBUG_Service_Redirector)
			showDebugMessage ("Incomming request from OS... sending to App[%d]. %s", 
								sm->getNextModuleIndex(), 
								sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
			
		sendRequestMessage (sm, toAppGates[sm->getNextModuleIndex()]);
	}	
	
	// Msg cames from applications
	else{
		
		// I/O operation?
		if ((sm->getOperation () == SM_OPEN_FILE)   ||
			(sm->getOperation () == SM_CLOSE_FILE)  ||
			(sm->getOperation () == SM_READ_FILE)   ||
			(sm->getOperation () == SM_WRITE_FILE)  ||
			(sm->getOperation () == SM_CREATE_FILE) ||
			(sm->getOperation () == SM_DELETE_FILE)){
				
				// Remote operation? to NET
				if (sm->getRemoteOperation()){
					
					if (DEBUG_Service_Redirector)
						showDebugMessage ("Local request redirected to NET. %s", 
											sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
						
					sendRequestMessage (sm, toNetGate);
				}
				
				// Local operation? to local FS
				else{
					
					if (DEBUG_Service_Redirector)
						showDebugMessage ("Local request redirected to I/O. %s", 
											sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
						
					sendRequestMessage (sm, toMemoryGate);				
				}
		}
		
		// MPI operation?
		else if ((sm->getOperation () == MPI_SEND) ||
				 (sm->getOperation () == MPI_RECV) ||				 
				 (sm->getOperation () == MPI_BARRIER_UP)   ||
				 (sm->getOperation () == MPI_BARRIER_DOWN) ||
				 (sm->getOperation () == MPI_BCAST)   ||
				 (sm->getOperation () == MPI_SCATTER) ||
				 (sm->getOperation () == MPI_GATHER)){
			
			sendRequestMessage (sm, toNetGate);
		}
		
		
		// CPU operation?
		else if (sm->getOperation () == SM_CPU_EXEC){
			
			if (DEBUG_Service_Redirector)
				showDebugMessage ("Local request redirected to CPU. %s", sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
								
			sendRequestMessage (sm, toCPUGate);			
		}

		// MEM operation?
		else if ((sm->getOperation () == SM_MEM_ALLOCATE) ||
				 (sm->getOperation () == SM_MEM_RELEASE)){
			
			if (DEBUG_Service_Redirector)
				showDebugMessage ("Local request redirected to MEM. %s", sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
								
			sendRequestMessage (sm, toMemoryGate);			
		}

		
		// Net operation?		
		else if ((sm->getOperation () == SM_CREATE_CONNECTION) ||
				(sm->getOperation () == SM_LISTEN_CONNECTION) ||
				(sm->getOperation () == SM_SEND_DATA_NET)){
			
			if (DEBUG_Service_Redirector)
				showDebugMessage ("Local request redirected to NET. %s", sm->contentsToString(DEBUG_MSG_Service_Redirector).c_str());
											
			sendRequestMessage (sm, toNetGate);			
		}			
		
		// Unknown operation! -> Error!!!
		else
			showErrorMessage ("Unknown operation %s", sm->operationToString().c_str());
	}
}


void ServiceRedirector::processResponseMessage (SIMCAN_Message *sm){
		
	// Send back the message
	sendResponseMessage (sm);
}

