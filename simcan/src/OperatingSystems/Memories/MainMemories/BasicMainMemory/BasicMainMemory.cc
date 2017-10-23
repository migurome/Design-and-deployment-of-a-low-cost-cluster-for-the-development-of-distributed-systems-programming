#include "BasicMainMemory.h"

Define_Module (BasicMainMemory);



BasicMainMemory::~BasicMainMemory(){
	
}


void BasicMainMemory::initialize(){

	std::ostringstream osStream;
	int i;

	    // Set the moduleIdName
		osStream << "BasicMainMemory." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();

		// Module parameters		
		numInputs = par ("numInputs");
		sizeGB = par ("size");
		blockSizeKB = par ("blockSize");
		readLatencyTime = ((double) par ("readLatencyTime")) / (double) 1000000 ;
		writeLatencyTime = ((double) par ("writeLatencyTime")) / (double) 1000000 ;

		// Check sizes...
		if ((blockSizeKB<=0) || (sizeGB<=0))
			throw cRuntimeError("BasicMainMemory, wrong memory size!!!");

		// Init variables
		totalBlocks = (sizeGB*1024*1024*1024) / blockSizeKB;
		totalAppBlocks = 0;
		freeAppBlocks = totalBlocks;

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
	    pendingMessage = NULL;
	    
	    showStartedModule ("Size:%f(GB) - BlockSize:%f(KB) - totalBlocks:%d",
                            sizeGB,
                            blockSizeKB,
                            totalBlocks);
}


void BasicMainMemory::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* BasicMainMemory::getOutGate (cMessage *msg){

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


void BasicMainMemory::processCurrentRequestMessage (){

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
}


void BasicMainMemory::processSelfMessage (cMessage *msg){


		// Is a pending message?
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

			// There is a pending message...
			if (pendingMessage!=NULL){

				if ((DEBUG_DETAILED_Basic_Main_Memory) && (DEBUG_Basic_Main_Memory))
					showDebugMessage ("End of memory processing! Pendign request:%s ", pendingMessage->contentsToString(DEBUG_MSG_Basic_Main_Memory).c_str());

				// Send current request
				cancelEvent (msg);
				sendRequest (pendingMessage);
			}

			// No pending message waiting to be sent...
			else{

				if ((DEBUG_DETAILED_Basic_Main_Memory) && (DEBUG_Basic_Main_Memory))
					showDebugMessage ("End of memory processing! No pending request.");
				
				// Process next request
				cancelEvent (msg);
				processCurrentRequestMessage ();
			}
		}

		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void BasicMainMemory::processRequestMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_App_MEM_Message *sm_mem;
	unsigned int requiredBlocks;

	
	// Allocating memory for application space!
	if (sm->getOperation() == SM_MEM_ALLOCATE){
				
		// Cast!
		sm_mem = check_and_cast <SIMCAN_App_MEM_Message*> (sm);
		
		// Memory account
		requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / blockSizeKB*KB);
				
		if (DEBUG_Basic_Main_Memory)
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
		requiredBlocks = (unsigned int) ceil (sm_mem->getMemSize() / blockSizeKB*KB);
		
		if (DEBUG_Basic_Main_Memory)
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

			// Request came from Service Redirector... Split it and process subRequests!
			if (!sm_io->getRemoteOperation()){

				// Verbose mode? Show detailed request
				if (DEBUG_Basic_Main_Memory)
					showDebugMessage ("Processing request:%s",
									  sm_io->contentsToString(DEBUG_MSG_Basic_Main_Memory).c_str());

				// Send request
				sendRequest (sm_io);
			}

			// Request cames from I/O Redirector... send to corresponding App (NFS)!
			else
				sendRequest (sm_io);
		}

		// Control operation...
		else{
			sendRequest (sm_io);
		}
	}
}


void BasicMainMemory::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;

	// Verbose mode? Show detailed request
	if (DEBUG_Basic_Main_Memory){

		sm_io = check_and_cast <SIMCAN_App_IO_Message*> (sm);

		showDebugMessage ("Sending response:%s",
					       sm_io->contentsToString(DEBUG_MSG_Basic_Main_Memory).c_str());
	}

	sendResponseMessage (sm);
}


void BasicMainMemory::sendRequest (SIMCAN_Message *sm){

	// Send to destination! Virtual File System
	if (!sm->getRemoteOperation())
		sendRequestMessage (sm, toOutputGate);

	// Request cames from VFS... send to corresponding App!
	else{	
		//sendRequestMessage (sm, toInputGates[0]);
		send (sm, toInputGates[sm->getNextModuleIndex()]);
	}
}




