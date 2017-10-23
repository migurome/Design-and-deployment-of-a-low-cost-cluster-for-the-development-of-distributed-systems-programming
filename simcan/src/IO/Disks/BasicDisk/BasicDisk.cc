#include "BasicDisk.h"

Define_Module (BasicDisk);


BasicDisk::~BasicDisk(){
}


void BasicDisk::initialize(){

	int i;
	double readBandwidth_Mbps;
	double writeBandwidth_Mbps;
	std::ostringstream osStream;

		// Set the moduleIdName
		osStream << "SimpleDisk." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Assign ID to module's gates
		inGate = gate ("in");
		outGate = gate ("out");

		// Get the blockSize
		readBandwidth_Mbps = par ("readBandwidth");
		writeBandwidth_Mbps = par ("writeBandwidth");

		// Transform values into bytes
		readBandwidth = readBandwidth_Mbps / 8;
		writeBandwidth = writeBandwidth_Mbps / 8;

		showStartedModule ("Read:%f(Mbps) - Read:%f(MBps) - Write:%f(Mbps) - Write:%f(MBps)",
                            readBandwidth_Mbps,
                            readBandwidth,
                            writeBandwidth_Mbps,
                            writeBandwidth);

		// Init variables
		requestTime = 0;

		// Latency message
		latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());
		pendingMessage = NULL;
}


void BasicDisk::finish(){   

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* BasicDisk::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from scheduler
		if (msg->getArrivalGate()==inGate){
			if (outGate->getNextGate()->isConnected()){
				return (outGate);
			}
		}

	// If gate not found!
	return NULL;
}


void BasicDisk::processSelfMessage (cMessage *msg){

	SIMCAN_BlockList_Message *sm_bl;


		// Latency message...
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

			// Cast!
			sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(pendingMessage);

			// Init pending message...
			pendingMessage=NULL;

			// Send message back!
			sendResponseMessage (sm_bl);
		}

		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void BasicDisk::processRequestMessage (SIMCAN_Message *sm){

	// Link pending message
	pendingMessage = sm;

	// Process the current IO request
	requestTime = service (sm);

	// Schedule a selft message to wait the request time...
	scheduleAt (simTime()+requestTime, latencyMessage);
}


void BasicDisk::processResponseMessage (SIMCAN_Message *sm){
	showErrorMessage ("There is no response messages in Disk Modules!!!");
}


simtime_t BasicDisk::service(SIMCAN_Message *sm){

	int i;
	simtime_t totalDelay;			// Time to perform the IO operation
	char operation;				// Operation type

	size_blockList_t totalBlocks;		// Total Number of blocks
	SIMCAN_BlockList_Message *sm_bl;	// Block list message


    	// Init
    	totalDelay = 0;

    	// Cast to Standard message
	    sm_bl = dynamic_cast<SIMCAN_BlockList_Message *>(sm);

	    // Get the number of blocks
    	totalBlocks =  sm_bl->getFile().getTotalSectors ();

    	// Get the operation type
	    if (sm_bl->getOperation() == SM_READ_FILE)
	    	operation = 'r';
	    else
	    	operation = 'w';

	    // Calculate total time!
	    totalDelay = diskTime (totalBlocks, operation);

	    // Transfer data
		sm_bl->setIsResponse(true);

		// Update message length
        sm_bl->updateLength();

	return totalDelay;
}


simtime_t BasicDisk::diskTime (size_blockList_t numBlocks, char operation){

	simtime_t transferTime;					// Transfer time
	unsigned long long int totalBytes;		// Total Number of bytes


	    // Init...
		transferTime = 0.0;
		totalBytes = numBlocks*BYTES_PER_SECTOR;

		if (operation == 'r')
			transferTime = ((double)totalBytes/(double)MB)/readBandwidth;
		else
			transferTime = ((double)totalBytes/(double)MB)/writeBandwidth;


		 if (DEBUG_Disk)
            showDebugMessage ("Processing request. Time:%f - Requesting %d blocks - %d bytes",
                                transferTime.dbl(),
                                numBlocks,
                                totalBytes);

	return (transferTime);
}

