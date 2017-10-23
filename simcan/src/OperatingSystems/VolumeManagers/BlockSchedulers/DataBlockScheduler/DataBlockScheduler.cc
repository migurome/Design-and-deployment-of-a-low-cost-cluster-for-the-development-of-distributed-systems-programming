#include "DataBlockScheduler.h"

Define_Module(DataBlockScheduler);


DataBlockScheduler::~DataBlockScheduler(){
	
	SMS_branch->clear();
	delete (SMS_branch);	
}


void DataBlockScheduler::initialize(){

	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "BranchScheduler." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Init the gates
	    toInputGate = gate ("toInput");
	    fromInputGate = gate ("fromInput");
	    toOutputGate = gate ("toOutput");
	    fromOutputGate = gate ("fromOutput");

	    // New SMS object!
	    SMS_branch = new SMS_Branch();
}


void DataBlockScheduler::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* DataBlockScheduler::getOutGate (cMessage *msg){

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


void DataBlockScheduler::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void DataBlockScheduler::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_BlockList_Message *sm_bl;

		// Cast!
		sm_bl = check_and_cast <SIMCAN_BlockList_Message*> (sm);		
		
			// Split and process current request!
			SMS_branch->splitRequest(sm_bl);			
			
			// Debug
			if (DEBUG_Block_Scheduler)
				showDebugMessage ("Processing original request:%s from message:%s", 
							SMS_branch->requestToString(sm, DEBUG_SMS_Block_Scheduler).c_str(),
							sm_bl->contentsToString(DEBUG_BRANCHES_Block_Scheduler, DEBUG_MSG_Block_Scheduler).c_str());
			
			processBranches();
}


void DataBlockScheduler::processResponseMessage (SIMCAN_Message *sm){
	sendResponseMessage (sm);
}


void DataBlockScheduler::processBranches (){

	SIMCAN_Message *subRequest;

		// Send all enqueued subRequest!
		do{

			subRequest = (SIMCAN_Message*) SMS_branch->popSubRequest();

			// There is a subRequest!
			if (subRequest != NULL){
				sendRequestMessage (subRequest, toOutputGate);
			}

		}while (subRequest!=NULL);
}


void DataBlockScheduler::sendRequestMessage (SIMCAN_Message *sm, cGate* gate){

	// If trace is empty, add current hostName, module and request number
	if (sm->isTraceEmpty()){
		sm->addNodeToTrace (getHostName());
		updateMessageTrace (sm);
	}

	// Send the message!
	send (sm, gate);
}

