#include "MainMemoryBlockCache.h"

Define_Module (MainMemoryBlockCache);


MainMemoryBlockCache::~MainMemoryBlockCache(){
	
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


void MainMemoryBlockCache::initialize(){

	std::ostringstream osStream;
	int i;

	    // Set the moduleIdName
		osStream << "MainMemory_CacheBlockLatencies." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();

		// Module parameters	
		numInputs = par ("numInputs");
		flushTime = par ("flushTime");
		accessTime = ((double) par ("accessTime")) / (double) 1000000;
		sizeGB = par ("size");
		sizeCacheMB = par ("sizeCache");
		blockSizeKB = par ("blockSize");
		readAheadBlocks = par ("readAheadBlocks");

		// Check sizes...
		if ((blockSizeKB<=0) || (sizeGB<=0) || (readAheadBlocks<0))
			throw cRuntimeError("MainMemory_CacheBlockLatencies, wrong memory size or blocks sizes!!!");

		if (sizeCacheMB > (sizeGB*1024))
			throw cRuntimeError("Cache size cannot be larger than the total memory size!!!");
			
		// Init variables
		totalBlocks = (sizeGB*1024*1024) / blockSizeKB;
		totalCacheBlocks = (sizeCacheMB*1024) / blockSizeKB;
		totalAppBlocks = totalBlocks - totalCacheBlocks;
		freeAppBlocks = totalAppBlocks;
		
		printf ("Memory Size:%lf(GB) - Cache:%lf(MB) - BlockSize:%lf(KB)\n", sizeGB, sizeCacheMB, blockSizeKB);
		printf ("totalBlocks:%d - totalCacheBlocks:%d - totalAppBlocks:%d - freeAppBlocks:%d\n", totalBlocks, totalCacheBlocks, totalAppBlocks, freeAppBlocks);

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
	    SMS_memory = new SMS_MainMemory (sizeGB*GB, readAheadBlocks, blockSizeKB*KB);

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


void MainMemoryBlockCache::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* MainMemoryBlockCache::getOutGate (cMessage *msg){

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


void MainMemoryBlockCache::processCurrentRequestMessage (){

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


void MainMemoryBlockCache::processSelfMessage (cMessage *msg){


	// Is a pending message?
	if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

		// There is a pending message...
		if (pendingMessage!=NULL){

			if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
				showDebugMessage ("End of memory processing! Pending request:%s ", pendingMessage->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());
				
			cancelEvent (msg);
			
			// If response message... arrive current request!
			if (pendingMessage->getIsResponse()){
				arrivesSubRequest (pendingMessage);
				processSubRequests ();					
			}
			
			// Request message... sent it!
			else{
				sendRequest (pendingMessage);
			}
		}

		// No pending message waiting to be sent...
		else{

			if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
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


void MainMemoryBlockCache::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_App_MEM_Message *sm_mem;
	unsigned int requiredBlocks;	
	
	
		// Allocating memory for application space!
		if (sm->getOperation() == SM_MEM_ALLOCATE){

			// Cast!
			sm_mem = check_and_cast <SIMCAN_App_MEM_Message*> (sm);

			// Memory account
			requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / blockSizeKB*KB);
					
			if (DEBUG_Main_Memory)
				showDebugMessage ("Memory Request. Free memory blocks: %d - Requested blocks: %d", freeAppBlocks, requiredBlocks);
			
			if (requiredBlocks <= freeAppBlocks){
				freeAppBlocks -= requiredBlocks;
				SMS_memory->splitRequest(sm_mem);
				processSubRequests();
			}
			else{
				
				showDebugMessage ("Not enough memory!. Free memory blocks: %d - Requested blocks: %d", freeAppBlocks, requiredBlocks);			
				sm_mem->setResult (SM_NOT_ENOUGH_MEMORY);

				// Response message
				sm_mem->setIsResponse(true);

				// Send message back!
				sendResponseMessage (sm_mem);
			}
		}
		
		
		// Releasing memory for application space!
		else if (sm->getOperation() == SM_MEM_RELEASE){
					
			// Cast!
			sm_mem = check_and_cast <SIMCAN_App_MEM_Message*> (sm);
			
			// Memory account
			requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / blockSizeKB*KB);
			
			if (DEBUG_Main_Memory)
				showDebugMessage ("Memory Request. Free memory blocks: %d - Released blocks: %d", freeAppBlocks, requiredBlocks);
			
			
			// Update number of free blocks
			freeAppBlocks += requiredBlocks;

			SMS_memory->splitRequest(sm_mem);
			processSubRequests();
			
			// Response message
			//sm_mem->setIsResponse(true);
	
			// Send message back!
			//sendResponseMessage (sm_mem);
		}
	
	
		// Disk cache
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
					if (DEBUG_CacheBlock_Latencies_Memory)
						showDebugMessage ("Processing original request:%s from message:%s", 
										SMS_memory->requestToString(sm_io, DEBUG_SMS_CacheBlock_Latencies_Memory).c_str(),
										sm_io->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());
	
	
					showSMSContents(DEBUG_ALL_SMS_CacheBlock_Latencies_Memory);
	
					// Process next request!
					processSubRequests();
				}
	
				// Request cames from VFS ... send to corresponding App (NFS)!
				else
					sendRequest (sm_io);
			}
	
			// Control operation...
			else{
				sendRequest (sm_io);
			}
		}
}


void MainMemoryBlockCache::processResponseMessage (SIMCAN_Message *sm){

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
				if ((DEBUG_SHOW_CONTENTS_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
					showDebugMessage ("Arrives block response. Memory contents: %s ", memoryListToString().c_str());
			}

			// Control operation?
			else
				sendResponseMessage (sm);
		}
}


void MainMemoryBlockCache::sendRequest (SIMCAN_Message *sm){

	// Send to destination! I/O Redirector...
	if (!sm->getRemoteOperation())
		sendRequestMessage (sm, toOutputGate);

	// Request cames from I/O Redirector... send to corresponding App!
	else{	
		sendRequestMessage (sm, toInputGates[0]);
	}
}


void MainMemoryBlockCache::sendRequestWithoutCheck (SIMCAN_Message *sm){

	// If trace is empty, add current hostName, module and request number
	if (sm->isTraceEmpty()){
		sm->addNodeToTrace (getHostName());
		updateMessageTrace (sm);
	}

	// Send to destination! Probably a file system...
	if (!sm->getRemoteOperation()){
		send (sm, toOutputGate);
	}

	// Request cames from IOR... send to corresponding App!
	else{	
		send (sm, toInputGates[sm->getNextModuleIndex()]);
	}
}


void MainMemoryBlockCache::processSubRequests (){

	SIMCAN_App_IO_Message *sm_io;				// Request message!
	SIMCAN_App_IO_Message *sm_io_copy;			// Request message! (copy)
	SIMCAN_App_IO_Message *sm_io_req;			// Request message!	
	cMessage *auxRequest;							
	bool isRequestHere;

	bool is_memoryFull;							// Is the memory full?
	bool all_blocksPending;						// Are all cached blocks pending?
	SIMCAN_MemoryBlock* currentBlockCached;		// Is current block in memory?
	string isCached;							// Is block cached? in string format
	bool isMemorySaturated;						// Is cache saturated?
	bool currentSubReqIsPending;
	simtime_t currentLatency;
	

		// Currently processing memory?
		if (latencyMessage->isScheduled()){

			if (DEBUG_CacheBlock_Latencies_Memory)
				showDebugMessage ("Currently processing memory... current subRequest must wait!");

			return;
		}

		// Init...
		pendingMessage=NULL;
		currentLatency = 0.0;

		// Get the first subrequest!
		sm_io = SMS_memory->getFirstSubRequest();

		// There is, at least, one subRequest...
		if (sm_io != NULL){

			// ALLOCATE memory
			if (sm_io->getOperation() == SM_MEM_ALLOCATE){

				sm_io = SMS_memory->popSubRequest();

				// Send back as solved request!
				sm_io->setIsResponse(true);

				// Set pending message!
				pendingMessage = sm_io;

				// Set latency!
				scheduleAt (accessTime+simTime(), latencyMessage);
			}


			// RELEASE MEMORY
			else if (sm_io->getOperation() == SM_MEM_RELEASE){

				sm_io = SMS_memory->popSubRequest();

				// Send back as solved request!
				sm_io->setIsResponse(true);

				// Set pending message!
				pendingMessage = sm_io;

				// Set latency!
				scheduleAt (accessTime+simTime(), latencyMessage);
			}


			// READ or WRITE operation
			else {

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

				if (DEBUG_CacheBlock_Latencies_Memory)
					showDebugMessage ("Processing subRequest: Op:%s File:%s Offset:%u size:%u",
									sm_io->operationToString().c_str(),
									sm_io->getFileName(),
									sm_io->getOffset(),
									sm_io->getSize());

				if (DEBUG_CacheBlock_Latencies_Memory)
					showDebugMessage ("Status: blockCached?:%s memoryFull?:%s allPending?:%s -> saturated?:%s",
									isCached.c_str(),
									boolToString(is_memoryFull).c_str(),
									boolToString(all_blocksPending).c_str(),
									boolToString(isMemorySaturated).c_str());


				// Memory is saturated!!! Send directly the request!!!
				if (isMemorySaturated){

					if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
						showDebugMessage ("Memory is saturated!!! Sending subRequest directly to destination...");

					// Pop subrequest!
					sm_io = SMS_memory->popSubRequest();

					// Send..
					sendRequest (sm_io);
				}

				// Memory not saturated... using memory !!!
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

							// Get parent request
							auxRequest = sm_io->getParentRequest();

							// Search for the request on request vector!
							isRequestHere = (SMS_memory->searchRequest (auxRequest) != NOT_FOUND);
					
							// If request is not here... current block is a read-ahead block!
							if (isRequestHere){
								sm_io_req = check_and_cast<SIMCAN_App_IO_Message *>(sm_io->getParentRequest());
								currentLatency = calculateCurrentLatency (sm_io_req->getSize(), true);
							}
							else
								currentLatency = calculateCurrentLatency (blockSizeKB*KB, true);

							if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
								showDebugMessage ("Processing memory... block cached! (latencyTime_s:%f). Request:%s",
													currentLatency.dbl(),
													sm_io->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());

							// Send back as solved request!
							sm_io->setIsResponse(true);

							// Set pending message!
							pendingMessage = sm_io;

							// Set latency!
							scheduleAt (currentLatency+simTime(), latencyMessage);
						}

						// Current block is not cached!
						else{

							// If memory full, remove a block!
							if (is_memoryFull)
								memoryBlockList.pop_back();

							// Insert current block in memory!
							insertBlock (sm_io->getFileName(), sm_io->getOffset(), sm_io->getSize());

							// Send pending request...
							pendingMessage = sm_io;

							// Latency time
							currentLatency = accessTime;

							if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
								showDebugMessage ("Processing memory... block NOT cached! (latencyTime_s:%f). Request:%s",
													currentLatency.dbl(),
													sm_io->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());

							scheduleAt (currentLatency+simTime(), latencyMessage);

						}
					}

					// Is a WRITE operation?
					else{
						
						// Get parent request		
						auxRequest = sm_io->getParentRequest();
				
						// Search for the request on request vector!
						isRequestHere = (SMS_memory->searchRequest (auxRequest) != NOT_FOUND);
				
						// If request is not here... current block is a read-ahead block!
						if (isRequestHere){
							sm_io_req = check_and_cast<SIMCAN_App_IO_Message *>(sm_io->getParentRequest());
							currentLatency = calculateCurrentLatency (sm_io_req->getSize(), false);
						}							
						else							
							currentLatency = calculateCurrentLatency (blockSizeKB*KB, false);

						// Current block is cached!
						if (currentBlockCached!=NULL){

							// Current block is NOT pending!
							if (!currentBlockCached->getIsPending()){

								if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
									showDebugMessage ("Processing memory... block cached! (latencyTime_s:%f). Request:%s",
														currentLatency.dbl(),
														sm_io->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());

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
							
							if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
								showDebugMessage ("Processing memory...(latencyTime_s:%f) block NOT cached:%s",
													currentLatency.dbl(),
													sm_io->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());
							
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

							// Send pending request...
							pendingMessage = sm_io_copy;

							scheduleAt (currentLatency+simTime(), latencyMessage);

							if (currentSubReqIsPending)
								delete (sm_io);

							// Flush!
							if (flushTime == 0)
								flushMemory ();
					}

					// Show memory contents?
					if ((DEBUG_SHOW_CONTENTS_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
						showDebugMessage (" %s ", memoryListToString().c_str());
				}
			}
		}
}


void MainMemoryBlockCache::arrivesSubRequest (SIMCAN_App_IO_Message *subRequest){

	cMessage *auxRequest;
	SIMCAN_App_IO_Message *parentRequest;
	SIMCAN_App_MEM_Message *parentRequestMem;
	bool isRequestHere;

		// Parent request		
		auxRequest = subRequest->getParentRequest();		

		// Search for the request on request vector!
		isRequestHere = (SMS_memory->searchRequest (auxRequest) != NOT_FOUND);

		// If request is not here... delete current subRequest!
		if (!isRequestHere){

			if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
				showDebugMessage ("Arrived subRequest has no parent.. deleting! %s", subRequest->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());

			delete (subRequest);						
		}

		// Request is here!
		else{
			
			// Verbose mode? Show detailed request
			if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
				showDebugMessage ("Processing subRequest response from message:%s",									
								  subRequest->contentsToString(DEBUG_MSG_CacheBlock_Latencies_Memory).c_str());

			
			// Casting!
			parentRequestMem = dynamic_cast <SIMCAN_App_MEM_Message*> (auxRequest);

			if (parentRequestMem != NULL){

				// Check for errors...
				if (subRequest->getResult() != SIMCAN_OK){
					parentRequestMem->setResult (subRequest->getResult());
				}

				// SubRequest arrives...
				SMS_memory->arrivesSubRequest (subRequest, parentRequestMem);

				if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
					showDebugMessage ("Arrives ALL requests of ALLOC | RELEASE operation");


				if (SMS_memory->arrivesAllSubRequest(parentRequestMem)){

					// Removes the request object!
					SMS_memory->removeRequest (parentRequestMem);

					// Now is a Response Message
					parentRequestMem->setIsResponse (true);

					// Send response
					sendResponseMessage (parentRequestMem);
				}
			}


			
			// IO MEssage
			else{
			
				// Casting!
				parentRequest = check_and_cast <SIMCAN_App_IO_Message*> (auxRequest);

				// Check for errors...
				if (subRequest->getResult() != SIMCAN_OK){
					parentRequest->setResult (subRequest->getResult());
				}

				// SubRequest arrives...
				SMS_memory->arrivesSubRequest (subRequest, parentRequest);

				// Verbose mode? Show detailed request
				if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
					showDebugMessage ("Processing subRequest response. SMS contents:%s",
										SMS_memory->requestToString(parentRequest, DEBUG_SMS_CacheBlock_Latencies_Memory).c_str());

				
				// If all request have arrived...
				if ((SMS_memory->arrivesAllSubRequest(parentRequest)) ||
					(SMS_memory->arrivesRequiredBlocks(parentRequest, readAheadBlocks))){

					if ((DEBUG_DETAILED_CacheBlock_Latencies_Memory) && (DEBUG_CacheBlock_Latencies_Memory))
						showDebugMessage ("Arrives all subRequest!");

					// Removes the request object!
					SMS_memory->removeRequest (parentRequest);

					// Show complete SMS
					showSMSContents (DEBUG_SHOW_CONTENTS_CacheBlock_Latencies_Memory);

					// Now is a Response Message
					parentRequest->setIsResponse (true);

					// Update the mesage length!
					parentRequest->updateLength();

					// Send response
					sendResponseMessage (parentRequest);
				}
			}
		}
}


void MainMemoryBlockCache::flushMemory (){

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


void MainMemoryBlockCache::insertRequestOnFlushQueue (SIMCAN_App_IO_Message *subRequest){

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


bool MainMemoryBlockCache::isMemoryFull (){

	if (memoryBlockList.size() >= totalCacheBlocks)
		return true;
	else
		return false;
}


bool MainMemoryBlockCache::allBlocksPending (){

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


void MainMemoryBlockCache::insertBlock (string fileName, unsigned int offset, unsigned int size){

	SIMCAN_MemoryBlock *block;
	int blockOffset;
	int blockNumber;

		// Bigger than memory block size...
		if (size > (blockSizeKB*KB))
			throw cRuntimeError("[searchMemoryBlock] memory Block is too big!!!");

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


void MainMemoryBlockCache::reInsertBlock (SIMCAN_MemoryBlock *block){

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


SIMCAN_MemoryBlock* MainMemoryBlockCache::searchMemoryBlock (const char* fileName, unsigned int offset){

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


string MainMemoryBlockCache::memoryListToString (){

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


void MainMemoryBlockCache::showSMSContents (bool showContents){

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


simtime_t MainMemoryBlockCache::calculateCurrentLatency (unsigned int totalRequestSize, bool isRead){

	int index;
	bool found;
	unsigned int blockSize_bytes;
	simtime_t fixedLatency;
	simtime_t currentLatency;
	
		// Init...
		index=0;
		found=false;
		blockSize_bytes = blockSizeKB*KB;
		
		
			// Search the corresponding latency
			while ((index<MAX_BLOCK_LATENCIES) && (!found)){				
				
				//printf ("---> Comparing[%d] - req:%u - block:%lf\n", index, totalRequestSize, latencyTimes[index][BLOCKSIZE_LATENCY_INDEX]);
								
				if (((simtime_t)totalRequestSize) <= latencyTimes[index][BLOCKSIZE_LATENCY_INDEX])
					found = true;
				else
					index++;
			}
						
			
			if (found){
			
				// Read operation
				if (isRead)
					currentLatency = latencyTimes[index][READ_LATENCY_INDEX] / ((double)totalRequestSize / blockSize_bytes);     
				
				// Write operation
				else
					currentLatency = latencyTimes[index][WRITE_LATENCY_INDEX] / ((double)totalRequestSize / blockSize_bytes);   	
			}			
			
			
			// blockSizeKB not found! Adjust!
			else{
				
				if (isRead){
					fixedLatency = ((double)totalRequestSize * (latencyTimes[MAX_BLOCK_LATENCIES-1][READ_LATENCY_INDEX])) / latencyTimes[MAX_BLOCK_LATENCIES-1][BLOCKSIZE_LATENCY_INDEX];					
					currentLatency = fixedLatency / ((double)totalRequestSize / latencyTimes[MAX_BLOCK_LATENCIES-1][BLOCKSIZE_LATENCY_INDEX]);
				}

				else{
					fixedLatency = ((double)totalRequestSize * (latencyTimes[MAX_BLOCK_LATENCIES-1][WRITE_LATENCY_INDEX])) / latencyTimes[MAX_BLOCK_LATENCIES-1][BLOCKSIZE_LATENCY_INDEX];					
					currentLatency = fixedLatency / ((double)totalRequestSize / latencyTimes[MAX_BLOCK_LATENCIES-1][BLOCKSIZE_LATENCY_INDEX]);
				}					
			}
			
//			printf ("--->TotalRequest:%u - blockSizeKB:%u - isRead:%d - Found:%d -  currentLatency:%lf - index:%d\n",
//					totalRequestSize, 
//					blockSize_bytes,
//					isRead,
//					found,					
//					currentLatency,
//					index
//					);

//			printf ("------------> %lf\n",(double)totalRequestSize / latencyTimes[index][BLOCKSIZE_LATENCY_INDEX]); 
				
							
	return currentLatency;
}



