#include "Disk_400GB_LI.h"

Define_Module (Disk_400GB_LI);


Disk_400GB_LI::~Disk_400GB_LI(){
}


void Disk_400GB_LI::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "Disk_400GB_LI." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Assign ID to module's gates
		inGate = gate ("in");
		outGate = gate ("out");

		// Calculate bandwidth avegare
		readBandwidth = (unsigned int) ((double) 1 / readTimes[MB_BLOCKSIZE_INDEX][KB32_JUMP_INDEX]);
		writeBandwidth = (unsigned int) ((double) 1 / writeTimes[MB_BLOCKSIZE_INDEX][KB32_JUMP_INDEX]);

		// Init variables
		requestTime = 0;
		lastBlock = 0;

		// Latency message
		latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());
		pendingMessage = NULL;
}


void Disk_400GB_LI::finish(){   

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* Disk_400GB_LI::getOutGate (cMessage *msg){

	// If msg arrive from scheduler
	if (msg->getArrivalGate()==inGate){
		if (outGate->getNextGate()->isConnected()){
			return (outGate);
		}
	}

	// If gate not found!
	return NULL;
}


void Disk_400GB_LI::processSelfMessage (cMessage *msg){

	SIMCAN_BlockList_Message *sm_bl;


		// Latency message...
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

			// Cast!
			sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(pendingMessage);

			// Init pending message...
			pendingMessage=NULL;

			if (DEBUG_Disk)
				showDebugMessage ("Sending response. %s",
					sm_bl->contentsToString(DEBUG_BRANCHES_Disk, DEBUG_MSG_Disk).c_str());

			// Send message back!
			sendResponseMessage (sm_bl);
		}

		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void Disk_400GB_LI::processRequestMessage (SIMCAN_Message *sm){

	// Link pending message
	pendingMessage = sm;

	// Process the current IO request
	requestTime = service (sm);

	// Schedule a selft message to wait the request time...
	scheduleAt (simTime()+requestTime, latencyMessage);
}


void Disk_400GB_LI::processResponseMessage (SIMCAN_Message *sm){
	showErrorMessage ("There is no response messages in Disk Modules!");
}


simtime_t Disk_400GB_LI::service (SIMCAN_Message *sm){

	unsigned int i;
	simtime_t totalDelay;					// Time to perform the IO operation
	char operation;							// Operation type

	size_blockList_t currentBranchSize;		// Size of current branch
	off_blockList_t currentBranchOffset;	// Offset of current branch
	unsigned int numBranches;				// NUmber of branches
	size_blockList_t totalBlocks;			// Total Number of blocks
	SIMCAN_BlockList_Message *sm_bl;		// Block list message


    	// Init
    	totalDelay = 0;

    	// Cast to Standard message
	    sm_bl = dynamic_cast<SIMCAN_BlockList_Message *>(sm);

	    // Get the number of blocks
    	numBranches = sm_bl->getFile().getNumberOfBranches();
    	totalBlocks =  sm_bl->getFile().getTotalSectors ();

    	// Get the operation type
	    if (sm_bl->getOperation() == SM_READ_FILE)
	    	operation = 'r';
	    else
	    	operation = 'w';

	    // Update message length
	    sm_bl->updateLength();

    	// For each brach, calculates
    	for (i=0; i<numBranches; i++){

    		currentBranchOffset = sm_bl->getFile().getBranchOffset (i);
    		currentBranchSize = sm_bl->getFile().getBranchSize (i);

    		// Calculate current branch service time
    		totalDelay += diskTime (currentBranchOffset, currentBranchSize, operation);
   		}
    	
    	if (DEBUG_Disk)
	    	showDebugMessage ("Processing request. Time:%f %s",
	    						totalDelay.dbl(),
								sm_bl->contentsToString(DEBUG_BRANCHES_Disk, DEBUG_MSG_Disk).c_str());							    	 
    	
	    // Transfer data
		sm_bl->setIsResponse(true);


	return totalDelay;
}


simtime_t Disk_400GB_LI::diskTime (off_blockList_t offsetBlock, size_blockList_t brachSize, char operation){

	simtime_t transferTime;	// Transfer time
	int64 jumpBytes;		// Total Number of bytes
	int64 totalBytes;		// Total Number of bytes

	int blockSizeIndex_1;	// Index to first blockSize
	int blockSizeIndex_2;	// Index to second blockSize
	int offsetIndex_1;		// Index to first jump
	int offsetIndex_2;		// Index to second jump

	simtime_t subResult_A;		// First Lineal interpolation subResult
	simtime_t subResult_B;		// Second lineal interpolation subResult


		// Init...
		transferTime = 0.0;
		jumpBytes = llabs (lastBlock - ((off64_T)offsetBlock));
		jumpBytes*= BYTES_PER_SECTOR;
		totalBytes = brachSize*BYTES_PER_SECTOR;

		// Update last accessed block
		lastBlock = offsetBlock+brachSize;

			// Calculate index to lineal interpolation
			calculateIndex (jumpBytes, totalBytes, &blockSizeIndex_1, &blockSizeIndex_2, &offsetIndex_1, &offsetIndex_2);

			// Calculate that coordinates do not exceed the limit!
			// Out of rage! Use bandWith!
			if ((blockSizeIndex_1<0) || (blockSizeIndex_2<0) || (offsetIndex_1<0) || (offsetIndex_2<0) ||
				(blockSizeIndex_1>=NUM_BLOCK_SIZES) || (blockSizeIndex_2>=NUM_BLOCK_SIZES) ||
				(offsetIndex_1>=NUM_JUMP_SIZES) || (offsetIndex_2>=NUM_JUMP_SIZES)){

					if (operation == 'r')
						transferTime = (double) ((double)totalBytes/(readBandwidth*MB));
					else
						transferTime = (double) ((double)totalBytes/(writeBandwidth*MB));
			}

			// In range! Use REAL ;) lineal interpolation!
			else{

				subResult_A = fabs (linealInterpolation ( jumpBytes,
													jumpsSizes[offsetIndex_1],
													jumpsSizes[offsetIndex_2],
													readTimes [blockSizeIndex_1][offsetIndex_1],
													readTimes [blockSizeIndex_1][offsetIndex_2]));

				subResult_B = fabs (linealInterpolation ( jumpBytes,
													jumpsSizes[offsetIndex_1],
													jumpsSizes[offsetIndex_2],
													readTimes [blockSizeIndex_2][offsetIndex_1],
													readTimes [blockSizeIndex_2][offsetIndex_2]));

				transferTime = fabs (linealInterpolation (totalBytes,
													blockSizes[blockSizeIndex_1],
													blockSizes[blockSizeIndex_2],
													subResult_A,
													subResult_B));
			}


		// Debug
		if (DEBUG_DETAILED_Disk)
			showDebugMessage ("Calculating time... op:%c  offsetBlock:%w  branchSize:%w",
								operation,
								offsetBlock,
								brachSize);

	return (transferTime);
}


void Disk_400GB_LI::calculateIndex (int64 jump, int64 numBytes, int *blockSizeIndex_1, int *blockSizeIndex_2, int *offsetIndex_1, int *offsetIndex_2){

	int currentOffset, currentBlockSize;
	bool found;

		// Init...
		found = false;
		currentOffset = 0;
		currentBlockSize = 0;

		// Search the Offset coordinates!
		while ((currentOffset<NUM_JUMP_SIZES) && (!found)){

			// Found!
			if (jumpsSizes[currentOffset] > jump){
				*offsetIndex_1 = currentOffset-1;
				*offsetIndex_2 = currentOffset;
				found = true;
			}

			// Not found!
			else
				currentOffset++;
		}

		// Out of range! Use the last Jump Size!
		if (!found){
			*offsetIndex_1 = NUM_JUMP_SIZES-1;
			*offsetIndex_2 = NUM_JUMP_SIZES;
		}

		// Re-init!
		found = false;

		// Search the blockSize coordinates!
		while ((currentBlockSize<NUM_BLOCK_SIZES) && (!found)){

			// Found!
			if (blockSizes[currentBlockSize] > numBytes){

				// Adjust!
				if (currentBlockSize==0){
					*blockSizeIndex_1 = 0;
					*blockSizeIndex_2 = 1;
				}

				else{
					*blockSizeIndex_1 = currentBlockSize-1;
					*blockSizeIndex_2 = currentBlockSize;
				}

				found = true;
			}

			// Not found!
			else
				currentBlockSize++;
		}

		// Out of range!
		if (!found){
			*blockSizeIndex_1 = -1;
			*blockSizeIndex_2 = -1;
		}
}


simtime_t Disk_400GB_LI::linealInterpolation (int64 x, int64 x0, int64 x1, simtime_t y0, simtime_t y1){

	simtime_t y;

		y = y0 + ( ((y1-y0) / (x1-x0)) * (x-x0));

	return y;
}


