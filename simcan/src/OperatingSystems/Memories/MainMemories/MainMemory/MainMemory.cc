#include "MainMemory.h"

Define_Module (MainMemory);


MainMemory::~MainMemory(){
	
	cMessage *message;	
	
		// Cancel the flush message
		cancelAndDelete (flushMessage);
		
		// Removes all messages placed on SMS
		SMS_memory->clear();
		
		// Removes the SMS object
		delete (SMS_memory);
		
		while (!memoryBlockList.empty()){
			delete (memoryBlockList.front());
			memoryBlockList.pop_front();
		}
		
		// Renoves all messages in memory list
		memoryBlockList.clear();
		
		// Removes all messages in flushQueue
		while (!flushQueue.empty()){			
			message = (cMessage *) flushQueue.pop();
			delete (message);			
		}
		
		flushQueue.clear();	
}


void MainMemory::initialize(){

	std::ostringstream osStream;
	int i;

	    // Set the moduleIdName
		osStream << "MainMemory." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();

		// Module parameters		
		numInputs = par ("numInputs");
		readAheadBlocks = par ("readAheadBlocks");
		sizeGB = par ("size");
		sizeCacheMB = par ("sizeCache");
		blockSizeKB = par ("blockSize");
		readLatencyTime = ((double) par ("readLatencyTime")) / (double) 1000000 ;
		writeLatencyTime = ((double) par ("writeLatencyTime")) / (double) 1000000 ;
		flushTime = par ("flushTime");

		// Check sizes...
		if ((blockSizeKB<=0) || (sizeCacheMB<=0) || (sizeGB<=0) || (readAheadBlocks<0))
			throw cRuntimeError("MainMemory, wrong memory size or blocks sizes!!!");		

		if (sizeCacheMB > (sizeGB*1024))
			throw cRuntimeError("Cache size cannot be larger than the total memory size!!!");	

		// Init variables
		totalBlocks = (sizeGB*1024*1024) / blockSizeKB;
		totalCacheBlocks = (sizeCacheMB*1024) / blockSizeKB;
		totalAppBlocks = totalBlocks - totalCacheBlocks;
		freeAppBlocks = totalAppBlocks;

		//printf ("Memory Size:%lf(GB) - Cache:%lf(MB) - BlockSize:%lf(KB)\n", sizeGB, sizeCacheMB, blockSizeKB);
		//printf ("totalBlocks:%d - totalCacheBlocks:%d - totalAppBlocks:%d - freeAppBlocks:%d\n", totalBlocks, totalCacheBlocks, totalAppBlocks, freeAppBlocks);

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

	    // Create the latency message and flush message
	    latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());
	    flushMessage = new cMessage ("flush-message");
	    pendingMessage = NULL;

	    // Creates the SMS object
	    SMS_memory = new SMS_MainMemory (sizeGB*KB, readAheadBlocks, blockSizeKB*KB);

	    // Flush Queue
	    flushQueue.clear();
	    
	    showStartedModule ("Size:%f(GB) - Cache:%f(MB) - BlockSize:%f(KB) - totalBlocks:%d - cacheBlocks:%d - Read-ahead:%d",
			    			sizeGB,
			    			sizeCacheMB,
			    			blockSizeKB,
			    			totalBlocks,
			    			totalCacheBlocks,
			    			readAheadBlocks);
}


void MainMemory::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* MainMemory::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromOutputGate){
			if (toOutputGate->getNextGate()->isConnected()){
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


void MainMemory::processCurrentRequestMessage (){

	SIMCAN_Message *sm;
	cMessage *unqueuedMessage;

		// If exists enqueued requests
		if (!queue.empty()){

			// Pop!
			unqueuedMessage = (cMessage *) queue.pop();

			// Dynamic cast!
			sm = check_and_cast<SIMCAN_Message *>(unqueuedMessage);
						
			// Process
			processRequestMessage (sm);
		}
		else
			processSubRequests();
}


void MainMemory::processSelfMessage (cMessage *msg){


		// Is a pending message?
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

			// There is a pending message...
			if (pendingMessage!=NULL){

				if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
					showDebugMessage ("End of memory processing! Pendign request:%s ", pendingMessage->contentsToString(DEBUG_MSG_Main_Memory).c_str());

				cancelEvent (msg);

				sendRequest (pendingMessage);
			}

			// No pending message waiting to be sent...
			else{

				if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
					showDebugMessage ("End of memory processing! No pending request.");
				
				
				cancelEvent (msg);

				// Process next subRequest...
				processCurrentRequestMessage ();
				//processSubRequests ();
			}
		}

		// Is a flush message?
		else if (!strcmp (msg->getName(), "flush-message")){

			// There is no blocks on flush queue... stop the timer!
			if (!flushQueue.empty())
				flushMemory ();
		}

		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void MainMemory::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_App_MEM_Message *sm_mem;
	unsigned int requiredBlocks;

	
	// Allocating memory for application space!
	if (sm->getOperation() == SM_MEM_ALLOCATE){
				
		// Cast!
		sm_mem = check_and_cast <SIMCAN_App_MEM_Message*> (sm);
		
		// Memory account
		requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / (blockSizeKB*KB));
				
		if (DEBUG_Main_Memory)
			showDebugMessage ("Memory Request. Free memory blocks: %d - Requested blocks: %d", freeAppBlocks, requiredBlocks);
		
		if (requiredBlocks <= freeAppBlocks)
			freeAppBlocks -= requiredBlocks;
		else{
			
			showDebugMessage ("Not enough memory!. Free memory blocks: %d - Requested blocks: %d", freeAppBlocks, requiredBlocks);			
			sm_mem->setResult (SM_NOT_ENOUGH_MEMORY);			
		}		
		
		// Response message
		sm_mem->setIsResponse(true);						

		// Send message back!
		sendResponseMessage (sm_mem);
	}
	
	
	// Releasing memory for application space!
	else if (sm->getOperation() == SM_MEM_RELEASE){
				
		// Cast!
		sm_mem = check_and_cast <SIMCAN_App_MEM_Message*> (sm);
		
		// Memory account
		requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / (blockSizeKB*KB));
		
		if (DEBUG_Main_Memory)
			showDebugMessage ("Memory Request. Free memory blocks: %d - Released blocks: %d", freeAppBlocks, requiredBlocks);
		
		
		// Update number of free blocks
		freeAppBlocks += requiredBlocks;				
		
		// Response message
		sm_mem->setIsResponse(true);

		// Send message back!
		sendResponseMessage (sm_mem);
	}
	
	
	// Disk cache space!
	else{

		// Cast!
		sm_io = check_and_cast <SIMCAN_App_IO_Message*> (sm);

		// Read or write operation?
		if ((sm_io->getOperation() == SM_READ_FILE) ||
			(sm_io->getOperation() == SM_WRITE_FILE)){

			// Request cames from Service Redirector... Split it and process subRequests!
			if (!sm_io->getRemoteOperation()){
								
				// Split and process current request!
				SMS_memory->splitRequest(sm_io);

				// Verbose mode? Show detailed request
				if (DEBUG_Main_Memory)
					showDebugMessage ("Processing original request:%s from message:%s", 
									SMS_memory->requestToString(sm_io, DEBUG_SMS_Main_Memory).c_str(),
									sm_io->contentsToString(DEBUG_MSG_Main_Memory).c_str());												


				showSMSContents(DEBUG_ALL_SMS_Main_Memory);

				// Process next request!
				processSubRequests();
			}

			// Request cames from VFS... send to corresponding App (NFS)!
			else
				sendRequest (sm_io);
		}

		// Control operation...
		else{
			sendRequest (sm_io);
		}
	}
}


void MainMemory::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_MemoryBlock* currentBlockCached;


		// Response cames from remote node...
		if (sm->arrivedOn("fromInput")){
			sendResponseMessage (sm);
		}

		// Response cames from I/O Redirector... (local)
		else{

			// Read or Write operation?
			if ((sm->getOperation() == SM_READ_FILE) ||
				(sm->getOperation() == SM_WRITE_FILE)){

				// Cast!
				sm_io = check_and_cast <SIMCAN_App_IO_Message*> (sm);

				// Update memory...
				currentBlockCached = searchMemoryBlock (sm_io->getFileName(), sm_io->getOffset());

				// If block in memory... set current block as NOT pending and re-intert on memory!
				if (currentBlockCached!=NULL){
					currentBlockCached->setIsPending (false);
					reInsertBlock (currentBlockCached);
				}

				// Arrives subRequest!
				arrivesSubRequest (sm_io);

				// Show memory contents?
				if ((DEBUG_SHOW_CONTENTS_Main_Memory) && (DEBUG_Main_Memory))
					showDebugMessage ("Arrives block response. Memory contents: %s ", memoryListToString().c_str());
			}

			// Control operation?
			else
				sendResponseMessage (sm);
		}
}


void MainMemory::sendRequest (SIMCAN_Message *sm){

	// Send to destination! I/O Redirector...
	if (!sm->getRemoteOperation())
		sendRequestMessage (sm, toOutputGate);

	// Request cames from I/O Redirector... send to corresponding App!
	else{	
		sendRequestMessage (sm, toInputGates[0]);
	}
}


void MainMemory::sendRequestWithoutCheck (SIMCAN_Message *sm){

	// If trace is empty, add current hostName, module and request number
	if (sm->isTraceEmpty()){
		sm->addNodeToTrace (getHostName());
		updateMessageTrace (sm);
	}

	// Send to destination! Probably a file system...
	if (!sm->getRemoteOperation()){
		send (sm, toOutputGate);
	}

	// Request cames from VFS... send to corresponding App!
	else{	
		send (sm, toInputGates[sm->getNextModuleIndex()]);
	}
}


void MainMemory::processSubRequests (){

	SIMCAN_App_IO_Message *sm_io;				// Request message!
	SIMCAN_App_IO_Message *sm_io_copy;			// Request message! (copy)

	bool is_memoryFull;							// Is the memory full?
	bool all_blocksPending;						// Are all cached blocks pending?
	SIMCAN_MemoryBlock* currentBlockCached;		// Is current block in memory?
	string isCached;							// Is block cached? in string format
	bool isMemorySaturated;						// Is cache saturated?
	bool currentSubReqIsPending;
	
		// Currently processing memory?
		if (latencyMessage->isScheduled()){

			if (DEBUG_Main_Memory)
				showDebugMessage ("Currently processing memory... current subRequest must wait!");

			return;
		}

		pendingMessage=NULL;

		// Get the first subrequest!
		sm_io = SMS_memory->getFirstSubRequest();

		// There is, at least, one subRequest...
		if (sm_io != NULL){

			// Check if current block is in memory!
			currentBlockCached = searchMemoryBlock (sm_io->getFileName(),
												   sm_io->getOffset());

			// Is the block cached? parse to string...
			if (currentBlockCached==NULL)
				isCached="false";
			else
				isCached="true";

			// Calculates is memory is full!
			is_memoryFull = isMemoryFull();

			// All blocks are pending?
			all_blocksPending = allBlocksPending();

			// Conditions to saturate memory...
			isMemorySaturated = ((is_memoryFull) && (all_blocksPending) && (currentBlockCached==NULL));

			
			if (DEBUG_Main_Memory)
				showDebugMessage ("Processing subRequest: Op:%s File:%s Offset:%u Size:%u",
								sm_io->operationToString().c_str(),
								sm_io->getFileName(),
								sm_io->getOffset(),
								sm_io->getSize());
			
			if (DEBUG_Main_Memory)
				showDebugMessage ("Status: blockCached?:%s memoryFull?:%s allPending?:%s -> saturated?:%s",
								isCached.c_str(),
								boolToString(is_memoryFull).c_str(),
								boolToString(all_blocksPending).c_str(),
								boolToString(isMemorySaturated).c_str());


			// Memory is saturated!!! Send directly the request!!!
			if (isMemorySaturated){
					
				if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
					showDebugMessage ("Memory is saturated!!! Sending subRequest directly to destination...");

				// Pop subrequest!
				sm_io = SMS_memory->popSubRequest();

				// Send..
				sendRequest (sm_io);
			}

			// Memory not saturated... using memory!!!
			else{

				 	// Pop subrequest!
					sm_io = SMS_memory->popSubRequest();
								
					// Is a READ operation?
					if (sm_io->getOperation() == SM_READ_FILE){

						// Current block is cached!
						if (currentBlockCached!=NULL){

							// If not pending, re-insert the block!
							if (!currentBlockCached->getIsPending())
								reInsertBlock (currentBlockCached);

							if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
								showDebugMessage ("Processing memory... block cached! (latencyTime_s). Request:%s",
													sm_io->contentsToString(DEBUG_MSG_Main_Memory).c_str());

							// Latency time!
							scheduleAt (readLatencyTime+simTime(), latencyMessage);

							// Send back as solved request!
							sm_io->setIsResponse(true);
							arrivesSubRequest (sm_io);
						}

						// Current block is not cached!
						else{

							// If memory full, remove a block!
							if (is_memoryFull)
								memoryBlockList.pop_back();

							// Insert current block in memory!
							insertBlock (sm_io->getFileName(), sm_io->getOffset(), sm_io->getSize());

							// Send request...
							pendingMessage = sm_io;

							if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
								showDebugMessage ("Processing memory... block NOT cached! (latencyTime_s). Request:%s",
													sm_io->contentsToString(DEBUG_MSG_Main_Memory).c_str());

							scheduleAt (readLatencyTime+simTime(), latencyMessage);

						}
					}

					// Is a WRITE operation?
					else{
												
						// Current block is cached!
						if (currentBlockCached!=NULL){

							// Current block is NOT pending!
							if (!currentBlockCached->getIsPending()){
								
								if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
									showDebugMessage ("Processing memory... block cached! (latencyTime_s). Request:%s",
														sm_io->contentsToString(DEBUG_MSG_Main_Memory).c_str());

								// Mark current block as pending!
								currentBlockCached->setIsPending(true);

								// Re insert current block...
								reInsertBlock (currentBlockCached);

								// enQueue on Flush Queue!
								insertRequestOnFlushQueue (sm_io);
								
								currentSubReqIsPending = false;
							}
							
							// Current block is pending...
							else{
								currentSubReqIsPending = true;								
							}
						}

						// not in memory!
						else{
							
							if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
								showDebugMessage ("Processing memory... block NOT cached:%s",
													sm_io->contentsToString(DEBUG_MSG_Main_Memory).c_str());
							
							currentSubReqIsPending = false;

							// If memory full, remove a block!
							if (is_memoryFull)
								memoryBlockList.pop_back();

							// Insert on memory
							insertBlock (sm_io->getFileName(), sm_io->getOffset(), sm_io->getSize());

							// enQueue on Flush Queue!
							insertRequestOnFlushQueue (sm_io);
						}

							// Copy request to send response to SMS!
							sm_io_copy = (SIMCAN_App_IO_Message*) sm_io->dup();
							sm_io_copy->setIsResponse(true);
							
							scheduleAt (writeLatencyTime+simTime(), latencyMessage);
							arrivesSubRequest (sm_io_copy);
							
							if (currentSubReqIsPending)
								delete (sm_io);
								
								
							// Flush!
							if (flushTime == 0)
								flushMemory ();
								
					}

					// Show memory contents?
					if ((DEBUG_SHOW_CONTENTS_Main_Memory) && (DEBUG_Main_Memory))
						showDebugMessage (" %s ", memoryListToString().c_str());
			}
		}
}


void MainMemory::arrivesSubRequest (SIMCAN_App_IO_Message *subRequest){

	cMessage *auxRequest;
	SIMCAN_App_IO_Message *parentRequest;
	bool isRequestHere;

		// Parent request		
		auxRequest = subRequest->getParentRequest();		

		// Search for the request on request vector!
		isRequestHere = (SMS_memory->searchRequest (auxRequest) != NOT_FOUND);

		// If request is not here... delete current subRequest!
		if (!isRequestHere){

			if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
				showDebugMessage ("Arrived subRequest has no parent.. deleting! %s", subRequest->contentsToString(DEBUG_MSG_Main_Memory).c_str());

			delete (subRequest);
		}

		else{

			// Casting!
			parentRequest = check_and_cast <SIMCAN_App_IO_Message*> (auxRequest);

			// SubRequest arrives...
			SMS_memory->arrivesSubRequest (subRequest, parentRequest);
			
			// Check for errors...
			if (subRequest->getResult() != SIMCAN_OK){
				parentRequest->setResult (subRequest->getResult());
			}			

			// Verbose mode? Show detailed request
			if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
				showDebugMessage ("Processing request response:%s from message:%s", 
									SMS_memory->requestToString(parentRequest, DEBUG_SMS_Main_Memory).c_str(),
									subRequest->contentsToString(DEBUG_MSG_Main_Memory).c_str());		
			
			
			// If all request have arrived...
			if ((SMS_memory->arrivesAllSubRequest(parentRequest)) ||
				(SMS_memory->arrivesRequiredBlocks(parentRequest, readAheadBlocks))){
							
				if ((DEBUG_DETAILED_Main_Memory) && (DEBUG_Main_Memory))
					showDebugMessage ("Arrives all subRequest!");

				// Removes the request object!
				SMS_memory->removeRequest (parentRequest);

				// Show complete SMS
				showSMSContents (DEBUG_SHOW_CONTENTS_Main_Memory);
				
				// Now is a Response Message
				parentRequest->setIsResponse (true);

				// Update the mesage length!
				parentRequest->updateLength();

				// Send response
				sendResponseMessage (parentRequest);
			}
		}
}


void MainMemory::flushMemory (){

 cMessage *unqueuedMessage;

	if (simTime() >= 2999)
		endSimulation();

	// If there is elements inside the queue
	while (flushQueue.length()>0){

		// pop next element...
		unqueuedMessage = (cMessage *) flushQueue.pop();
		SIMCAN_App_IO_Message *sm_io = check_and_cast<SIMCAN_App_IO_Message *>(unqueuedMessage);
		sendRequestWithoutCheck (sm_io);
	}
}


void MainMemory::insertRequestOnFlushQueue (SIMCAN_App_IO_Message *subRequest){

	// Insert a block on flush queue
	flushQueue.insert (subRequest);

	// If memory Full!! Must force the flush...
	if (isMemoryFull()){

		// If there was activated a previous timer, then cancel it!
		if (flushMessage->isScheduled())
			cancelEvent (flushMessage);

		// Forced flush!
		flushMemory();
	}

	// If timer is not active... activate it!
	else if (!flushMessage->isScheduled()){
		scheduleAt (flushTime+simTime(), flushMessage);
	}
}


bool MainMemory::isMemoryFull (){

	if (memoryBlockList.size() >= totalCacheBlocks)
		return true;
	else
		return false;
}


bool MainMemory::allBlocksPending (){

	list <SIMCAN_MemoryBlock*>::iterator listIterator;
	bool allPending;

		// Init
		allPending = true;

		// Is empty?
		if (memoryBlockList.empty())
			allPending = false;

		// Memory not empty!
		else{

			// Walk through the list searching the requested block!
			for (listIterator=memoryBlockList.begin(); (listIterator!=memoryBlockList.end() && (allPending)); ++listIterator){

				if (!(*listIterator)->getIsPending())
					allPending = false;
			}
		}

	return (allPending);
}


void MainMemory::insertBlock (string fileName, unsigned int offset, unsigned int size){

	SIMCAN_MemoryBlock *block;
	int blockOffset;
	int blockNumber;


		// Bigger than memory block size...
		if (size > (blockSizeKB*KB))
			throw cRuntimeError("[searchMemoryBlock] memory Block size too big!!!");

		// Block number and Block offset
		blockNumber = offset/(blockSizeKB*KB);
		blockOffset = blockNumber*(blockSizeKB*KB);

		if ((offset+size)>((blockNumber+1)*(blockSizeKB*KB))){
			throw cRuntimeError("[searchMemoryBlock] Wrong memory Block size (out of bounds)!!!");
		}

		block = new SIMCAN_MemoryBlock();

		// Set corresponding values
		block->setFileName (fileName);
		block->setOffset (blockOffset);
		block->setBlockSize (blockSizeKB*KB);
		block->setIsPending (true);

		// Insert block on memory list!
		memoryBlockList.push_front (block);
}


void MainMemory::reInsertBlock (SIMCAN_MemoryBlock *block){

	list <SIMCAN_MemoryBlock*>::iterator listIterator;
	SIMCAN_MemoryBlock* memoryBlock;
	bool found;


		// Init
		memoryBlock = NULL;
		found = false;

		// Walk through the list searching the requested block!
		for (listIterator=memoryBlockList.begin();
			(listIterator!=memoryBlockList.end() && (!found));
			++listIterator){

			// Found?
			if ((block->getFileName() == (*listIterator)->getFileName()) &&
				(block->getOffset() == (*listIterator)->getOffset() )){
				found = true;
				memoryBlock = *listIterator;
				memoryBlockList.erase (listIterator);
				memoryBlockList.push_front (memoryBlock);
			}
		}
}


SIMCAN_MemoryBlock* MainMemory::searchMemoryBlock (const char* fileName, unsigned int offset){

	list <SIMCAN_MemoryBlock*>::iterator listIterator;
	SIMCAN_MemoryBlock* memoryBlock;
	bool found;
	int currentBlock, requestedBlock;


		// Init
		memoryBlock = NULL;
		found = false;
		requestedBlock = offset/(blockSizeKB*KB);

		// Walk through the list searching the requested block!
		for (listIterator=memoryBlockList.begin(); ((listIterator!=memoryBlockList.end()) && (!found)); listIterator++){

			currentBlock = 	((*listIterator)->getOffset())/(blockSizeKB*KB);

			if ((!strcmp (fileName, (*listIterator)->getFileName().c_str()) ) &&
				(requestedBlock==currentBlock)){
				found = true;
				memoryBlock = *listIterator;
			}
		}


	return (memoryBlock);
}


string MainMemory::memoryListToString (){

	list <SIMCAN_MemoryBlock*>::iterator listIterator;
	std::ostringstream info;
	int currentBlock;


		// Init
		currentBlock = 0;
		info << "Memory list..." << endl;

			// Walk through the list searching the requested block!
			for (listIterator=memoryBlockList.begin();
				listIterator!=memoryBlockList.end();
				listIterator++){

					info << "Block[" << currentBlock << "]: " << (*listIterator)->memoryBlockToString() << endl;
					currentBlock++;
			}

	return info.str();
}


void MainMemory::showSMSContents (bool showContents){

	int i;
	int numRequests;
	
		if (showContents){

			// Get the number of requests
			numRequests = SMS_memory->getNumberOfRequests();
			
			showDebugMessage ("Showing the complete SMS vector... (%d requests)", numRequests);
	
			// Get all requests
			for (i=0; i<numRequests; i++)			
				showDebugMessage (" Request[%d]:%s ", i, SMS_memory->requestToStringByIndex(i).c_str());
		}
}


