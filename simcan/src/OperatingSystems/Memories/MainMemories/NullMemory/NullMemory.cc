#include "NullMemory.h"

Define_Module (NullMemory);


NullMemory::~NullMemory(){
}


void NullMemory::initialize(){

	std::ostringstream osStream;
	int i;


	    // Set the moduleIdName
		osStream << "NullMemory." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();

		// Module parameters
		numInputs = par ("numInputs");

	    // Init the gate IDs to/from Input gates...
	    toInputGates = new cGate* [numInputs];
	    fromInputGates = new cGate* [numInputs];

	    for (i=0; i<numInputs; i++){
			toInputGates[i] = gate ("toInput", i);
			fromInputGates[i] = gate ("fromInput", i);
	    }

	    // Init the gate IDs to/from Output
	    toOutputGate = gate ("toOutput");
	    fromOutputGate = gate ("fromOutput");
}


void NullMemory::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* NullMemory::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromOutputGate){
			if (gate("toOutput")->getNextGate()->isConnected()){
				return (toOutputGate);
			}
		}

		// If msg arrive from Inputs
		else if (msg->arrivedOn("fromInput")){
			for (i=0; i<numInputs; i++)
				if (msg->arrivedOn ("fromInput", i))
					return (gate("toInput", i));
		}

	// If gate not found!
	return NULL;
}


void NullMemory::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void NullMemory::processRequestMessage (SIMCAN_Message *sm){


	// Send to destination! Probably a file system...
	if (!sm->getRemoteOperation())
		sendRequestMessage (sm, toOutputGate);

	// Request cames from VFS... send back to corresponding App!
	else
		sendRequestMessage (sm, toInputGates[sm->getNextModuleIndex()]);
}


void NullMemory::processResponseMessage (SIMCAN_Message *sm){
	sendResponseMessage (sm);
}

