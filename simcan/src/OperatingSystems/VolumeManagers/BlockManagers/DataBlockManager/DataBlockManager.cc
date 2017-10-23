#include "DataBlockManager.h"

Define_Module(DataBlockManager);


DataBlockManager::~DataBlockManager(){
	
	int i;
	
		SMS_raid_0->clear();
		delete (SMS_raid_0);
		
		// Clear all queues...
		for (i=0; i<numBlockServers; i++){
			requestsQueue[i].clear();
		}
}


void DataBlockManager::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "BlockManager." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Get the number of block servers
		numBlockServers = par ("numBlockServers");
		strideSize = par ("strideSize");

		// State of blockServers
		isBlockServerIdle = new bool [numBlockServers];

		// Init state to idle!
		for (i=0; i<numBlockServers; i++)
			isBlockServerIdle[i] = true;

		// Create requestQueues
		requestsQueue = new cQueue [numBlockServers];

		// Init the queues
		for (i=0; i<numBlockServers; i++)
			requestsQueue[i].clear();				
		
		// Check the Volume Manager stride size
		if ((strideSize < BYTES_PER_SECTOR) || ((strideSize%BYTES_PER_SECTOR) != 0)){
			showErrorMessage ("Wrong Volume Manager strideSize %d. Must be multiple of %d",
								strideSize,
								BYTES_PER_SECTOR);
		}	

		// Creates the SMS object
		SMS_raid_0 = new SMS_RAID_0 (strideSize, numBlockServers);

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


void DataBlockManager::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* DataBlockManager::getOutGate (cMessage *msg){

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


void DataBlockManager::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void DataBlockManager::processRequestMessage (SIMCAN_Message *sm){

	int i;
	unsigned int diskIndex;
	SIMCAN_BlockList_Message *strippedRequest;
	SIMCAN_BlockList_Message *sm_bl;


		// Casting to debug!
		sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(sm);

		// Splits incomming request!
		SMS_raid_0->splitRequest (sm);

		// Debug
		if (DEBUG_Block_Manager)
			showDebugMessage ("Processing original request:%s from message:%s", 
							SMS_raid_0->requestToString(sm, DEBUG_SMS_Block_Manager).c_str(),
							sm_bl->contentsToString(DEBUG_BRANCHES_Block_Manager, DEBUG_MSG_Block_Manager).c_str());						
					
					
		// Show SMS contents?
		showSMSContents(DEBUG_ALL_SMS_Block_Manager);

		// Enqueue stripped requests!
		for (i=0; i<SMS_raid_0->getNumberOfSubRequest(sm); i++){

			// Get subRequest i
			strippedRequest = check_and_cast<SIMCAN_BlockList_Message *>(SMS_raid_0->popSubRequest (sm, i)); 
									
			// Get the blockServer index!
			diskIndex = strippedRequest->getNextModuleIndex();			

			// Check the disk index. If correct then enqueue current request!
			if ((diskIndex >= numBlockServers) || (diskIndex < 0))
			
				showErrorMessage ("(Request) Disk index error (%d). There are %d disks attached!\n",
									diskIndex,
									numBlockServers);
			else
				requestsQueue[diskIndex].insert (strippedRequest);
			
			
			if (DEBUG_Block_Manager)
				showDebugMessage ("Queuing stripped request to queue[%d] %s", 
									diskIndex,
									strippedRequest->contentsToString(DEBUG_BRANCHES_Block_Manager, DEBUG_MSG_Block_Manager).c_str());			
		}

		// Check for idle buses...
		checkBuses ();
}


void DataBlockManager::processResponseMessage (SIMCAN_Message *sm){

	unsigned int diskIndex;
	SIMCAN_BlockList_Message *requestMsg;
	SIMCAN_Message *nextSubRequest;
	cMessage *parentRequest;
	cMessage* unqueuedMessage;


		// Update bus state!
		diskIndex = sm->getNextModuleIndex();

		if ((diskIndex >= numBlockServers) || (diskIndex < 0))
			showErrorMessage ("(Response) Disk index error (%d). There are %d disks attached!\n",
								diskIndex,
								numBlockServers);
		else
			isBlockServerIdle[diskIndex] = true;

		// Parent request
		parentRequest = sm->getParentRequest();

		// SubRequest arrives...
		SMS_raid_0->arrivesSubRequest (sm, parentRequest);

		// Casting...
		requestMsg = check_and_cast <SIMCAN_BlockList_Message*> (parentRequest);

		// Debug
		if (DEBUG_Block_Manager)
			showDebugMessage ("Processing request response:%s from message:%s", 
							SMS_raid_0->requestToString(requestMsg, DEBUG_SMS_Block_Manager).c_str(),
							requestMsg->contentsToString(DEBUG_BRANCHES_Block_Manager, DEBUG_MSG_Block_Manager).c_str());
								
		
		// If all request have arrived... erase request and send back the response!
		if (SMS_raid_0->arrivesAllSubRequest (parentRequest)){
					
			if (DEBUG_Block_Manager)
				showDebugMessage ("Arrives all stripped request. %s", SMS_raid_0->requestToString(parentRequest, DEBUG_SMS_Block_Manager).c_str());

			// Removes the request object!
			SMS_raid_0->removeRequest(parentRequest);

			// Show SMS contents?
			showSMSContents(DEBUG_ALL_SMS_Block_Manager);

			// Now is a Response Message
			requestMsg->setIsResponse (true);

			// Update the mesage length!
			requestMsg->updateLength();

			// Sends the message
			sendResponseMessage (requestMsg);
		}

		// Check if there is pending request on queue i
		if (!requestsQueue[diskIndex].empty()){

			// Pop!
			unqueuedMessage = (cMessage *) requestsQueue[diskIndex].pop();

			// Dynamic cast!
			nextSubRequest = check_and_cast<SIMCAN_Message *>(unqueuedMessage);

			// Update state!
			isBlockServerIdle[diskIndex] = false;

			// Send!
			sendRequestMessage (nextSubRequest, toBlockServerGates[diskIndex]);
		}
}


void DataBlockManager::checkBuses (){

	int i;
	SIMCAN_Message* sm;
	cMessage* unqueuedMessage;

		// Check for idle buses
		for (i=0; i<numBlockServers; i++){

			// If bus[i] is idle, send next subRequest!
			if (isBlockServerIdle[i]){

				// Check if there is pending request on queue i
				if (!requestsQueue[i].empty()){

					// Pop!
					unqueuedMessage = (cMessage *) requestsQueue[i].pop();

					// Dynamic cast!
					sm = check_and_cast<SIMCAN_Message *>(unqueuedMessage);

					// Update state!
					isBlockServerIdle[i]=false;										

					// Send!
					sendRequestMessage (sm, toBlockServerGates[i]);
				}
			}
		}
}


void DataBlockManager::showSMSContents (bool showContents){

	int i;
	int numRequests;

		// Get the number of requests
		numRequests = SMS_raid_0->getNumberOfRequests();
				
		if (DEBUG_Block_Manager)
			showDebugMessage ("Showing the complete SMS vector... (%d requests)", numRequests);

		// Get all requests
		for (i=0; i<numRequests; i++)			
				if (DEBUG_Block_Manager)
					showDebugMessage (" Request[%d]:%s ", i, SMS_raid_0->requestToStringByIndex(i).c_str());
}


