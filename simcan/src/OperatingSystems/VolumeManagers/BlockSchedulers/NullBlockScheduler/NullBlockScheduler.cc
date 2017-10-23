#include "NullBlockScheduler.h"

Define_Module(NullBlockScheduler);

NullBlockScheduler::~NullBlockScheduler(){
}


void NullBlockScheduler::initialize(){

	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "NullBlockScheduler." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Init the gates
	    toInputGate = gate ("toInput");
	    fromInputGate = gate ("fromInput");
	    toOutputGate = gate ("toOutput");
	    fromOutputGate = gate ("fromOutput");
}


void NullBlockScheduler::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* NullBlockScheduler::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Input
		if (msg->getArrivalGate()==fromInputGate){
			if (toInputGate->getNextGate()->isConnected()){
				return (toInputGate);
			}
		}

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromOutputGate){
			if (toOutputGate->getNextGate()->isConnected()){
				return (toOutputGate);
			}
		}

	// If gate not found!
	return NULL;
}


void NullBlockScheduler::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void NullBlockScheduler::processRequestMessage (SIMCAN_Message *sm){
	sendRequestMessage (sm, toOutputGate);
}


void NullBlockScheduler::processResponseMessage (SIMCAN_Message *sm){
	sendResponseMessage (sm);
}

