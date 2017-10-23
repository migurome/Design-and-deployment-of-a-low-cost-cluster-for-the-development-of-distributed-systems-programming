#include "NullNetworkService.h"

Define_Module (NullNetworkService);



NullNetworkService::~NullNetworkService(){
}


void NullNetworkService::initialize(){

	std::ostringstream osStream;

    	// Set the moduleIdName
		osStream << "NullNetworkService." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();	    
	    
	    // Module parameters
	    localIP = "";
	    
	    // Module gates
	    fromServiceRedirectorGate = gate ("fromServiceRedirector");	    
	    fromNetTCPGate = gate ("fromNetTCP");
	    toServiceRedirectorGate = gate ("toServiceRedirector");    
	    toNetTCPGate = gate ("toNetTCP");	    
}


void NullNetworkService::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* NullNetworkService::getOutGate (cMessage *msg){
	
	
		// If msg arrive from Service Redirector
		if (msg->getArrivalGate()==fromServiceRedirectorGate){
			if (gate("toServiceRedirector")->getNextGate()->isConnected()){
				return (toServiceRedirectorGate);
			}
		}
		
		// If msg arrive from Service Redirector
		else if (msg->getArrivalGate()==fromNetTCPGate){
			if (gate("toNetTCP")->getNextGate()->isConnected()){
				return (toNetTCPGate);
			}
		}	
		

	// If gate not found!
	return NULL;
}


void NullNetworkService::processSelfMessage (cMessage *msg){
	
	showErrorMessage ("[processRequestMessage] This module should not receive any SELF message!");
}


void NullNetworkService::processRequestMessage (SIMCAN_Message *sm){
	
	showErrorMessage ("[processRequestMessage] This module should not receive any REQUEST message!");
		
}


void NullNetworkService::processResponseMessage (SIMCAN_Message *sm){

	showErrorMessage ("[processRequestMessage] This module should not receive any RESPONSE message!");
}

