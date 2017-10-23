#include "BlockCache.h"

Define_Module (BlockCache);


BlockCache::~BlockCache(){
}


void BlockCache::initialize(){

	std::ostringstream osStream;
	int i;


	    // Set the moduleIdName
		osStream << "BlockCache." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();

		// Module parameters
	    numInputs = par ("numInputs");
		hitRatio = par ("hitRatio");
		
		// Check the Volume Manager stride size
		if ((hitRatio < 0) || (hitRatio > 1))
			showErrorMessage ("Invalid hitRatio value [%f]. Must be (0 <= hitRatio >= 1)", hitRatio);
		
	    
	   	// Init the gate IDs
	    toOutputGate = gate ("toOutput");
	    fromOutputGate = gate ("fromOutput");

	    toInputGates = new cGate* [numInputs];
        fromInputGates = new cGate* [numInputs];

        for (i=0; i<numInputs; i++){
            toInputGates[i] = gate ("toInput", i);
            fromInputGates[i] = gate ("fromInput", i);
        }
}


void BlockCache::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* BlockCache::getOutGate (cMessage *msg){

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
                    return (toInputGates[i]);
        }
		

	// If gate not found!
	return NULL;
}


void BlockCache::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void BlockCache::processRequestMessage (SIMCAN_Message *sm){	
	sendRequestMessage (sm, toOutputGate);	
}


void BlockCache::processResponseMessage (SIMCAN_Message *sm){
	sendResponseMessage (sm);
}

