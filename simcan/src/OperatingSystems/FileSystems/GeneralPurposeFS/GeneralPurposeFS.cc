#include "GeneralPurposeFS.h"

Define_Module (GeneralPurposeFS);

//const double GeneralPurposeFS::OPEN_TIME = 0.00018173;
//const double GeneralPurposeFS::CLOSE_TIME = 0.00001554;
//const double GeneralPurposeFS::CREATE_TIME = 0.00193551;

const const_simtime_t GeneralPurposeFS::OPEN_TIME = 0.00011;
const const_simtime_t GeneralPurposeFS::CLOSE_TIME = 0.000013;
const const_simtime_t GeneralPurposeFS::CREATE_TIME = 0.000016;


const string GeneralPurposeFS::bunchSize_ext2_Alpha_file = "/bunchSize_ext2_Alpha.txt";
const string GeneralPurposeFS::bunchSize_ext2_Beta_file = "/bunchSize_ext2_Beta.txt";
const string GeneralPurposeFS::bunchSize_reiserFS_Alpha_file = "/bunchSize_reiserFS_Alpha.txt";
const string GeneralPurposeFS::bunchSize_reiserFS_Beta_file = "/bunchSize_reiserFS_Beta.txt";
const string GeneralPurposeFS::contiguous_ext2_Alpha_file = "/contiguous_ext2_Alpha.txt";
const string GeneralPurposeFS::contiguous_ext2_Beta_file = "/contiguous_ext2_Beta.txt";
const string GeneralPurposeFS::contiguous_reiserFS_Alpha_file = "/contiguous_reiserFS_Alpha.txt";
const string GeneralPurposeFS::contiguous_reiserFS_Beta_file = "/contiguous_reiserFS_Beta.txt";
const string GeneralPurposeFS::holeSize_ext2_Alpha_file = "/holeSize_ext2_Alpha.txt";
const string GeneralPurposeFS::holeSize_ext2_Beta_file = "/holeSize_ext2_Beta.txt";
const string GeneralPurposeFS::holeSize_reiserFS_Alpha_file = "/holeSize_reiserFS_Alpha.txt";
const string GeneralPurposeFS::holeSize_reiserFS_Beta_file = "/holeSize_reiserFS_Beta.txt";
const string GeneralPurposeFS::relative_Weibull_Files_Path = "/src/OperatingSystems/FileSystems/GeneralPurposeFS/weibull";



GeneralPurposeFS::~GeneralPurposeFS(){
	
	fileList.clear();	
}


void GeneralPurposeFS::initialize(){

	std::ostringstream osStream;
	int i;

		// Set the moduleIdName
		osStream << "GeneralPurposeFS." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Init gate IDs
		fromVMGate = gate ("fromVM");
		toVMGate = gate ("toVM");
		fromIORGate = gate ("fromIOR");
		toIORGate = gate ("toIOR");

		// Create the latency message
	    latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());
	    pendingMessage = NULL;

		// Module's parameters		
		maxBlocks = par ("maxBlocks");
		diskRatio = par ("diskRatio");
		fsBlockSize_b = par ("fsBlockSize_b");
		usedBlocks = (unsigned long int) ((double)maxBlocks*diskRatio);
		
		// Check the FS blockSize
		if ((fsBlockSize_b < BYTES_PER_SECTOR) || ((fsBlockSize_b%BYTES_PER_SECTOR) != 0)){
			showErrorMessage ("Wrong File System BlockSize %d. Must be multiple of %d",
								fsBlockSize_b,
								BYTES_PER_SECTOR);
		}		

		// File system type
		const char *newFsType = par ("fsType");
		fsType = newFsType;

		// Set SIMCAN's home path
		const char *simcanPath = par ("simcanHome");
		if (!simcanPath[0])
		    simcanHomePath = getenv("SIMCAN_HOME");
		else
		    simcanHomePath = simcanPath;


		// Pre load files parameters...
		preLoadFiles = par ("preLoadFiles");		

		// Adjust the ratio
		if (diskRatio > 1)
			diskRatio = 1;
		else if (diskRatio < 0)
			diskRatio = 0;

	    // Load Weibull parameters
	    if (!fsType.compare ("ext2")){

	    	loadExt2WeibullParameters ();

	    	// Print tables...
	    	if (DEBUG_TABLES_FS)
	    		showStartedModule (" %s ", ext2WeibullTablesToString().c_str());
	    }
	    else if (!fsType.compare ("reiserFS")){

	    	loadReiserFSWeibullParameters ();

	    	// Print tables...
			if (DEBUG_TABLES_FS)
				showStartedModule (" %s ", reiserFSWeibullTablesToString().c_str());
	    }
	    else
	       	showStartedModule ("Unknown File System Type [%s]", fsType.c_str());

	    // Load files on FileSystem
	    if (preLoadFiles){

			if (preloadFSFiles () == SIMCAN_OK)
				showStartedModule ("preLoad files succesful!");

			else
				showStartedModule ("Not preLoad files!");
	    }
	    

	    // Show file info?
	    if (DEBUG_FS_Files)
	    	showStartedModule (" %s ", FSFilesToString().c_str());
}


void GeneralPurposeFS::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* GeneralPurposeFS::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromVMGate){
			if (toVMGate->getNextGate()->isConnected()){
				return (toVMGate);
			}
		}

		// If msg arrive from Inputs
		else if (msg->getArrivalGate()==fromIORGate){
			if (toIORGate->getNextGate()->isConnected()){
				return (toIORGate);
			}
		}

	// If gate not found!
	return NULL;
}


void GeneralPurposeFS::processSelfMessage (cMessage *msg){

	SIMCAN_Message *sm;

		// Operation performed in FS...
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){

			// Cast!
			sm = check_and_cast<SIMCAN_Message *>(pendingMessage);

			// Init pending message...
			pendingMessage=NULL;

			// Send message back!
			sendResponseMessage (sm);
		}

		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void GeneralPurposeFS::processRequestMessage (SIMCAN_Message *sm){
	processIORequest (sm);
}


void GeneralPurposeFS::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_BlockList_Message *sm_bl;


		sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(sm);
		sm_io = sm_bl->transformToApp_IO_Message();
		delete (sm_bl);

	sendResponseMessage (sm_io);
}


void GeneralPurposeFS::processIORequest (SIMCAN_Message *sm){

	string fileName;				// File Name
	int result;						// IO result

	SIMCAN_App_IO_Message *sm_io;			// IO Request message
	SIMCAN_BlockList_Message *sm_bl;		// Block List message
	vector <SIMCAN_File>::iterator list_it;	// File list iterator

    	// Cast!
    	sm_io = check_and_cast<SIMCAN_App_IO_Message *>(sm);

    	// Get file name!
    	fileName = sm_io->getFileName();    	
    	
    	if (DEBUG_FS)
    		showDebugMessage ("Processing IO Request. %s", sm_io->contentsToString(DEBUG_MSG_FS).c_str());
    		
    		
    	// Show file info?
	    if (DEBUG_PARANOIC_FS){
	    	showDebugMessage ("------ Start FS contents (before executing this call) ------ diskRatio:%f usedBlocks:%U", diskRatio, usedBlocks);
	    	showDebugMessage (" %s ", FSFilesToString().c_str());
	    	showDebugMessage ("------ End FS contents ------");
	    }    		

		// Open File
		if (sm_io->getOperation() == SM_OPEN_FILE){

			pendingMessage = sm_io;

			// Answer message is sent back...
			sm_io->setIsResponse (true);

			// Search for the file
			list_it = searchFile (fileName);

			// Add the return value...
			if (list_it == fileList.end()){
				sm_io->setResult (SIMCAN_FILE_NOT_FOUND);	
				throw cRuntimeError("Error! File does not exists!!!");							
			}
			else
				sm_io->setResult (SIMCAN_OK);

			scheduleAt (OPEN_TIME+simTime(), latencyMessage);
		}

		// Close File
		else if (sm_io->getOperation() == SM_CLOSE_FILE){	

			pendingMessage = sm_io;

			// Answer message is sent back...
			sm_io->setResult (SIMCAN_OK);
			sm_io->setIsResponse (true);

			scheduleAt (CLOSE_TIME+simTime(), latencyMessage);
		}

		// Create File
		else if (sm_io->getOperation() == SM_CREATE_FILE){			
		
			pendingMessage = sm_io;

			// Answer message is sent back...
			sm_io->setIsResponse(true);

			// If fileName already exists, trunc it!
			list_it = searchFile (fileName);

			if (list_it != fileList.end())
				deleteFile (fileName);

			// Creates the file!
			if ((insertNewFile (0, fileName)) == SIMCAN_OK)
				sm_io->setResult (SIMCAN_OK);
			else
				sm_io->setResult (SIMCAN_DISK_FULL);

			 // Show file info?
			if ((DEBUG_FS_Files) && (DEBUG_FS))
				showDebugMessage (" %s ", FSFilesToString().c_str());

			scheduleAt (CREATE_TIME+simTime(), latencyMessage);
		}

		// Delete File
		else if (sm_io->getOperation() == SM_DELETE_FILE){
			
			pendingMessage = sm_io;

			// Answer message is sent back...
			sm_io->setResult (SIMCAN_OK);
			sm_io->setIsResponse(true);

			// Removes the file!
			deleteFile (fileName);

			 // Show file info?
			if ((DEBUG_FS_Files) && (DEBUG_FS))
				showDebugMessage (" %s ", FSFilesToString().c_str());

			scheduleAt (CLOSE_TIME+simTime(), latencyMessage);
		}

		// Read/Write File
		else if ((sm_io->getOperation() == SM_READ_FILE) ||
				 (sm_io->getOperation() == SM_WRITE_FILE)){


			// If numBytes == 0, IO request ends here, send back the message!
			if (sm_io->getSize() == 0){
				
				if (DEBUG_FS)
		    		showDebugMessage ("Returning request (numBytes = 0). %s", sm_io->contentsToString(DEBUG_MSG_FS).c_str());
				
				throw cRuntimeError("Error! Requesting 0 bytes!!!");
								
				sm_io->setResult (SIMCAN_OK);
				sm_io->setIsResponse (true);
				sendResponseMessage (sm_io);
			}

			// At least there is a byte to make the request! Calculate the involved blocks...
			else{

				// Creates a new block list message
				sm_bl = new SIMCAN_BlockList_Message (sm_io);
				translateIORequets (sm_bl);				

				// Error?
				if (sm_bl->getIsResponse()){	
					
					//showErrorMessage ("Error in File System [%d]", sm_bl->getResult ());
					//throw cRuntimeError("Error! Translating Error!!!");
									
					sm_io->setIsResponse (true);
					sm_io->setResult (sm_bl->getResult());
					delete (sm_bl);					
					sendResponseMessage (sm_io);
				}
				else{
					// Erase the request message!
					delete (sm_io);
					sendRequestMessage (sm_bl, toVMGate);
				}
			}
		}

		// Unknown IO operation
		else
			showErrorMessage ("Unknown operation [%d]", sm_io->getOperation());			
		
}


int GeneralPurposeFS::preloadFSFiles(){
	
	int i, result;
	CfgPreloadFS *preloadCfg;
	FileConfigManager *fileCfgMng;
	cModule *fileConfigModule;
	
		// Init...
		result = SIMCAN_OK;	
			
		// Storage node with local fileConfig?
		fileConfigModule = getParentModule()->getParentModule()->getSubmodule("fileConfig");
		
		// Not storage node!
		if (fileConfigModule == NULL){
			
			// Single node using a global file config
			fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
			// Not a global file Config
			if (fileConfigModule == NULL){
				
				// Racked compute node. Get file config from Rack!		
				fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
				// Wrong packaging configuration!
				if (fileConfigModule == NULL)
					showErrorMessage ("Wrong packaging configuration. Can not find FileConfigManager");
			}
		}	
	
		// Get the corresponding servers...		
		fileCfgMng = check_and_cast<FileConfigManager *>(fileConfigModule);
		preloadCfg = fileCfgMng->getPreloadFS();
		
		if (preloadCfg != NULL){
			
			// No servers file!
			if (preloadCfg->getNumFiles() == 0)
				result = SIMCAN_ERROR;
			
			else{
	
				// Store info in current class
				for (i=0; i<preloadCfg->getNumFiles(); i++){
					printf ("Inserting file %d of %d\n", i, preloadCfg->getNumFiles());
					insertNewFile (preloadCfg->getSizeKB(i)*KB, preloadCfg->getFileName(i).c_str());
				}
			}
		}
		else
			result = SIMCAN_ERROR;
			

    return result;
}


int GeneralPurposeFS::insertNewFile (unsigned int fileSize, string fileName){

	unsigned int categoryMaxSize;
	int fileBlocks;	
	SIMCAN_File newFile;
	SIMCAN_File& ref_newFile = newFile;		


		// Maximum filesize to each category
		categoryMaxSize = getCategoryMaxSize (fileSize);		
		
		// Calculate info for new file
		newFile.setFileName (fileName);
		newFile.setFileSize (fileSize);

		// Calculate the file's brachList
		if (!fsType.compare ("ext2"))
			calculateExt2BranchList (categoryMaxSize, ref_newFile);
		else if (!fsType.compare ("reiserFS"))
			calculateReiserFSBranchList (categoryMaxSize, ref_newFile);

		fileList.push_back (newFile);

		// Update usebBlocks and current disk ratio
		fileBlocks = newFile.getTotalSectors();
		
		// Exceed maimum number of blocks...
		if ((usedBlocks+fileBlocks) > maxBlocks){
			showErrorMessage ("Maximum number of blocks exceeded! There is no free space! maxBlocks:%d usedBlocks:%d fileblocks:%d fileName:%s",
												maxBlocks, usedBlocks, fileBlocks, fileName.c_str());
			return SIMCAN_ERROR;
		}
		
		else{			
			usedBlocks+= fileBlocks;
			diskRatio = ((double)usedBlocks/(double)maxBlocks);		
		}		

	return SIMCAN_OK;
}


vector <SIMCAN_File>::iterator GeneralPurposeFS::searchFile (string fileName){

	bool fileLocated;							// Is the file in the File System
	vector <SIMCAN_File>::iterator list_it;		// List iterator

		// Init...
		fileLocated = false;
		list_it=fileList.begin();

		// Walk through the list searching the requested block!
		while (list_it!=fileList.end() && (!fileLocated)){

			if (!list_it->getFileName().compare(fileName))
				fileLocated = true;
			else
				list_it++;
		}

		return list_it;
}


void GeneralPurposeFS::deleteFile (string fileName){

	unsigned int fileSize;
	int fileBlocks;
	vector <SIMCAN_File>::iterator list_it;		// List iterator


		// Search the file!
		list_it = searchFile (fileName);

		// File found!
		if (list_it != fileList.end()){

			// Get file size
			fileSize = list_it->getFileSize();

			// Get file size (in blocks)
			fileBlocks = ((fileSize%BYTES_PER_SECTOR) == 0)?
						fileSize/BYTES_PER_SECTOR:
						(fileSize/BYTES_PER_SECTOR)+1;

			// Update disk Ratio
			usedBlocks -= fileBlocks;
			diskRatio = ((double)usedBlocks/(double)maxBlocks);

			// Remove!!!
			fileList.erase (list_it);
		}
}


void GeneralPurposeFS::translateIORequets (SIMCAN_BlockList_Message* sm_fsTranslated){

	unsigned int fileBranch;
	off_blockList_t fileSector;

	string fileName;
	unsigned int offset;
	unsigned int numBytes;
	
	SIMCAN_BlockList_Message* sm_bl;
	size_blockList_t restBlocks;		// Rest of blocks to perform the request.
	size_blockList_t firstBlock;		// First block.
	off_blockList_t toOffset;			// Blocks to locate the first block.
	unsigned int limitedNumBytes;		// Requested bytes are out of bounds.

	unsigned int fileIndex;				// File's index.
	unsigned int numBranches;			// Number of branches of this request.

	unsigned int fileBlocks;			// File Blocks used.
	off_blockList_t addedBytes;			// Added bytes to file.
	size_blockList_t previousBlocks;	// Number of blocks used by the file

	bool locateFirstBlock;					// Is located the first block (offset).

	unsigned int currentBranch;				// Current branch.
	off_blockList_t currentBranchOffset;	// Current branch's offset.
	size_blockList_t currentBranchSize;		// Current branch's size.

	int previousCategory;					// Previous file size category
	int newCategory;						// New File Size category

	vector <SIMCAN_File>::iterator list_it;	// List iterator


		// Init...
		if (fsBlockSize_b != BYTES_PER_SECTOR)
			sm_bl = (SIMCAN_BlockList_Message*) sm_fsTranslated->dup();
		else
			sm_bl = sm_fsTranslated;
		
		restBlocks = firstBlock = toOffset = fileBranch = 0;
		limitedNumBytes = numBranches = currentBranchSize = previousCategory = newCategory = 0;
		currentBranchOffset = 0;
		locateFirstBlock = false;
		currentBranch = 0;
		fileName = sm_bl->getFileName ();
		offset = sm_bl->getOffset ();
		numBytes = sm_bl->getSize ();


		// Search for the requested file
		list_it = searchFile (fileName);

		// If file not found!!!
		if (list_it == fileList.end()){
			
			if (fsBlockSize_b != BYTES_PER_SECTOR)
				delete (sm_bl);
				
			sm_fsTranslated->setIsResponse (true);
			sm_fsTranslated->setResult (SIMCAN_FILE_NOT_FOUND);
			throw cRuntimeError("Error! File does not exists!!!");
			return;
		}

		// READ?
		if (sm_bl->getOperation() == SM_READ_FILE){

			// No data to read or offset out of bounds...
			if ((list_it->getFileSize() == 0) || (offset >= list_it->getFileSize())){
				
				if (fsBlockSize_b != BYTES_PER_SECTOR)
					delete (sm_bl);
				
				sm_fsTranslated->setIsResponse (true);
				sm_fsTranslated->setResult (SIMCAN_DATA_OUT_OF_BOUNDS);				
				return;
			}

			// No all bytes are inside the file
			else if ((offset+numBytes) > list_it->getFileSize()){
				limitedNumBytes = list_it->getFileSize() - offset;
				restBlocks = ((limitedNumBytes%BYTES_PER_SECTOR) == 0)?
							limitedNumBytes/BYTES_PER_SECTOR:
							(limitedNumBytes/BYTES_PER_SECTOR)+1;
			}

			// All bytes are inside the file...
			else
				restBlocks = ((numBytes%BYTES_PER_SECTOR) == 0)?
								numBytes/BYTES_PER_SECTOR:
								(numBytes/BYTES_PER_SECTOR)+1;
		}

		// WRITE?
		else
			restBlocks = ((numBytes%BYTES_PER_SECTOR) == 0)?
						numBytes/BYTES_PER_SECTOR:
						(numBytes/BYTES_PER_SECTOR)+1;		


		// Increase the file size???
		if ((sm_bl->getOperation() == SM_WRITE_FILE) && ((offset+numBytes) > list_it->getFileSize())){

			// Get current category
			previousCategory = getCategoryMaxSize (list_it->getFileSize());
			
			//	Get the number of blocks
			previousBlocks = list_it->getTotalSectors();

			// Update file size			
			list_it->setFileSize(offset+numBytes);
			
			// Get new category
			newCategory = getCategoryMaxSize (list_it->getFileSize());


			// Calculate new brach list...
			if (previousCategory != newCategory){

				// Calculate the file's brachList
				if (!fsType.compare ("ext2"))
					calculateExt2BranchList (newCategory, *list_it);
				else if (!fsType.compare ("reiserFS"))
					calculateReiserFSBranchList (newCategory, *list_it);					
					
				// Update used blocks...
				usedBlocks += (list_it->getTotalSectors() - previousBlocks);
				diskRatio = ((double)usedBlocks/(double)maxBlocks);
			}
		}
		
		// First block
		firstBlock = offset/BYTES_PER_SECTOR;

		// Search the first requested block...
		while (!locateFirstBlock){

			currentBranchOffset = list_it->getBranchOffset (currentBranch);
			currentBranchSize = list_it->getBranchSize (currentBranch);						

			if ((toOffset + currentBranchSize) > firstBlock)
				locateFirstBlock = true;
			else{
				toOffset+=currentBranchSize;
				currentBranch++;
			}
		}		

		// Here we have the first block...
		// Only one brach
		if (currentBranchSize >= (firstBlock-toOffset)+restBlocks){
									
			sm_bl->getFile().addBranch (currentBranchOffset+(firstBlock-toOffset), restBlocks);
			restBlocks = 0;
			numBranches++;
		}

		// More than one branch...
		else{						

			// Add the first branch!
			sm_bl->getFile().addBranch (currentBranchOffset+(firstBlock-toOffset),
									    currentBranchSize-(firstBlock-toOffset));

			restBlocks-= currentBranchSize-(firstBlock-toOffset);
			currentBranch++;
			numBranches++;

			// Calculate the rest of the brachList
			while (restBlocks > 0){

				currentBranchOffset = list_it->getBranchOffset (currentBranch);
				currentBranchSize = list_it->getBranchSize (currentBranch);

				// Last branch...
				if (currentBranchSize >= restBlocks){
					sm_bl->getFile().addBranch (currentBranchOffset, restBlocks);
					restBlocks = 0;
				}

				// Intermediate branch...
				else{
					sm_bl->getFile().addBranch (currentBranchOffset, currentBranchSize);
					restBlocks-= currentBranchSize;
					currentBranch++;
				}

				// Update to next branch!
				numBranches++;
			}
		}	
			

	// Set the request size!
	sm_bl->getFile().setFileSize (sm_bl->getFile().getTotalSectors() * BYTES_PER_SECTOR);				
			
			
	// Translate the request using the corresponding FS blockSize!
	if (fsBlockSize_b != BYTES_PER_SECTOR){
		
		// Only one block!
		if (sm_bl->getFile().getTotalSectors() >= (fsBlockSize_b / BYTES_PER_SECTOR)){

			currentBranchOffset = sm_bl->getFile().getBranchOffset(fileBranch);
			currentBranchSize = sm_bl->getFile().getBranchSize(fileBranch);
			sm_fsTranslated->getFile().addBranch (currentBranchOffset, currentBranchSize);			
		}
		
		
		// More than one block!
		else{
		
			// For each Branch...
			for (fileBranch=0; fileBranch<sm_bl->getFile().getNumberOfBranches(); fileBranch++){
	
				currentBranchOffset = sm_bl->getFile().getBranchOffset(fileBranch);
				currentBranchSize = sm_bl->getFile().getBranchSize(fileBranch);
	
				// For each sector...
				for (fileSector=currentBranchOffset;
					 fileSector<(currentBranchOffset+currentBranchSize);
					 fileSector++){
	
					if (!sm_fsTranslated->getFile().containThisSector(fileSector))
						sm_fsTranslated->getFile().addBlock (fileSector, fsBlockSize_b);			
				}
			}
		}
		
		sm_fsTranslated->getFile().setFileSize (sm_fsTranslated->getFile().getTotalSectors() * BYTES_PER_SECTOR);
		delete (sm_bl);
	}

	if (DEBUG_DETAILED_FS)
		showDebugMessage ("Translating request. diskRatio:%f usedBlocks:%U -> %s",
						diskRatio,
						usedBlocks,
						sm_fsTranslated->getFile().contentsToString(DEBUG_BRANCHES_FS, DEBUG_MSG_FS).c_str());
}


void GeneralPurposeFS::calculateExt2BranchList (unsigned int fileSize, SIMCAN_File& file){

	int i;
	bool isContiguous;				// Is a contiguous file?
	bool allValue_1;				// All branchSizes == 1?
	float contiguousProbWeibull;	// Contiguous_ext2_table value
	float contiguousProbBernoulli;	// Bernoulli value (Contiguous)

	float bunchSizeWeibull;			// bunchSize_ext2_table value
	float holeSizeWeibull;			// holeSize_ext2_table value

	int x;								// Disk Ratio Index
	int y;								// File Size category (index)
	size_blockList_t restFileBlocks;	// Rest of the file! (in blocks)
	size_blockList_t fileTotalBlocks;	// Total blocks of the corresponding file
	int numBranches;					// Number of total branches

	off_blockList_t currentOffset;		// Current file's data offset

	 	// Init
	 	file.clearBranchList();
	 	restFileBlocks = ((fileSize%BYTES_PER_SECTOR) == 0)?
	 					fileSize/BYTES_PER_SECTOR:
	 					(fileSize/BYTES_PER_SECTOR) + 1;
	 	numBranches = currentOffset = 0;
	 	contiguousProbWeibull = contiguousProbBernoulli = 0;
	 	allValue_1 = false;
	 	fileTotalBlocks = restFileBlocks;
	 	x = y = 0;

		// Disk Ratio (table index)
		x = getDiskRatio_x_axis();

		// File Size (table index)
		y = getFileSizeCategory_y_axis (fileSize);

		// All-Non contiguous
		if ((contiguous_ext2_Alpha_table[x][y] == 1) && (contiguous_ext2_Beta_table[x][y] == 1)){
			contiguousProbWeibull = 0;
		}

		// All contiguous
		else if ((contiguous_ext2_Alpha_table[x][y] == 0) && (contiguous_ext2_Beta_table[x][y] == 0)){
			contiguousProbWeibull = 1;
		}

		// Calculate the contiguous probability
		else{
			contiguousProbWeibull = weibull (contiguous_ext2_Alpha_table[x][y], contiguous_ext2_Beta_table[x][y], 0);
			contiguousProbBernoulli = bernoulli (contiguousProbWeibull, 0);
		}

		// Calculate the file's first block
		currentOffset = random () % (maxBlocks);

		// Is a contiguous file (1 branch)
		if ((contiguousProbWeibull>0) || (contiguousProbBernoulli > 0)){

			// If the file leaves the disk limits...
			if ((currentOffset+restFileBlocks) > maxBlocks){

				// Recalculate the file's initial block!
				currentOffset =  random () % (maxBlocks-fileTotalBlocks);
			}

			numBranches = 1;
			file.addBranch (currentOffset, restFileBlocks);
		}

		// Not a contiguous file, more than 1 branch.
		else{

			if ((bunchSize_ext2_Alpha_table[x][y]==1) && (bunchSize_ext2_Beta_table[x][y]==1)){
				allValue_1=true;
			}

			while (restFileBlocks > 0){

				numBranches++;

				// Calculate the bunch size
				if (allValue_1)
					bunchSizeWeibull=1;
				else
					bunchSizeWeibull = weibull (bunchSize_ext2_Alpha_table[x][y], bunchSize_ext2_Beta_table[x][y], 0);

				// Calculate hole size
				holeSizeWeibull = weibull (holeSize_ext2_Alpha_table[x][y], holeSize_ext2_Beta_table[x][y], 0);

				// Avoid zero values (bunch size)
				if (0 == (int)bunchSizeWeibull)
					bunchSizeWeibull = bunchSizeWeibull+1;

				// Avoid zero values (hole size)
				if (0 == (int)holeSizeWeibull)
					holeSizeWeibull = holeSizeWeibull+1;

				// If the file leaves the disk limits...
				if ((currentOffset+(int)bunchSizeWeibull) > maxBlocks){

					// Recalculate the file's initial block!
					currentOffset =  random () % (fileTotalBlocks);
				}

				// Not the last brach!
				if (restFileBlocks > bunchSizeWeibull){

					// Append the current brach
					file.addBranch (currentOffset, (size_blockList_t)bunchSizeWeibull);

					// Update the rest of the file to be branched!
					restFileBlocks-=(int)bunchSizeWeibull;
				}

				// last Branch!
				else{

					// Append the last Branch!
					file.addBranch (currentOffset, restFileBlocks);

					// All file is branched!
					restFileBlocks=0;
				}

				// Update the offset (hole to the next branch)
				currentOffset +=  (int) holeSizeWeibull;
			}
		}
}


void GeneralPurposeFS::calculateReiserFSBranchList(unsigned int fileSize, SIMCAN_File& file){

	int i;
	bool isContiguous;				// Is a contiguous file?
	bool allValue_1;				// All branchSizes == 1?
	float contiguousProbWeibull;	// Contiguous_reiserFS_table value
	float contiguousProbBernoulli;	// Bernoulli value (Contiguous)

	float bunchSizeWeibull;			// bunchSize_reiserFS_table value
	float holeSizeWeibull;			// holeSize_reiserFS_table value

	int x;								// Disk Ratio Index
	int y;								// File Size category (index)
	size_blockList_t restFileBlocks;	// Rest of the file! (in blocks)
	size_blockList_t fileTotalBlocks;	// Total blocks of the corresponding file
	int numBranches;					// Number of total branches
	off_blockList_t currentOffset;		// Current file's data offset


	 	// Init
	 	restFileBlocks = ((fileSize%BYTES_PER_SECTOR) == 0)?
	 					fileSize/BYTES_PER_SECTOR:
	 					(fileSize/BYTES_PER_SECTOR) + 1;
	 	numBranches = currentOffset = 0;
	 	contiguousProbWeibull = contiguousProbBernoulli = 0;
	 	allValue_1 = false;
	 	fileTotalBlocks = restFileBlocks;
	 	x = y = 0;

		// Disk Ratio (table index)
		x = getDiskRatio_x_axis();

		// File Size (table index)
		y = getFileSizeCategory_y_axis (fileSize);

		// All-Non contiguous
		if ((contiguous_reiserFS_Alpha_table[x][y] == 1) && (contiguous_reiserFS_Beta_table[x][y] == 1)){
			contiguousProbWeibull = 0;
		}

		else if ((contiguous_reiserFS_Alpha_table[x][y] == 0) && (contiguous_reiserFS_Beta_table[x][y] == 0)){
			contiguousProbWeibull = 1;
		}

		// Calculate the contiguous probability
		else{
			contiguousProbWeibull = weibull (contiguous_reiserFS_Alpha_table[x][y], contiguous_reiserFS_Beta_table[x][y], 0);
			contiguousProbBernoulli = bernoulli (contiguousProbWeibull, 0);
		}

		// Calculate the file's first block
		currentOffset = random () % (maxBlocks);

		// Is a contiguous file (1 branch)
		if ((contiguousProbWeibull>0) || (contiguousProbBernoulli > 0)){

			// If the file leaves the disk limits...
			if ((currentOffset+restFileBlocks) > maxBlocks){

				// Recalculate the file's initial block!
				currentOffset =  random () % (maxBlocks-fileTotalBlocks);
			}

			numBranches = 1;
			file.addBranch (currentOffset, restFileBlocks);
		}

		// Not a contiguous file, more than 1 branch.
		else{

			if ((bunchSize_reiserFS_Alpha_table[x][y]==1) && (bunchSize_reiserFS_Beta_table[x][y]==1)){
				allValue_1=true;
			}

			while (restFileBlocks > 0){

				numBranches++;

				// Calculate the bunch size
				if (allValue_1)
					bunchSizeWeibull=1;
				else
					bunchSizeWeibull = weibull (bunchSize_reiserFS_Alpha_table[x][y], bunchSize_reiserFS_Beta_table[x][y], 0);

				// Calculate hole size
				holeSizeWeibull = weibull (holeSize_reiserFS_Alpha_table[x][y], holeSize_reiserFS_Beta_table[x][y], 0);

				// Avoid zero values (bunch size)
				if (0 == (int)bunchSizeWeibull)
					bunchSizeWeibull = bunchSizeWeibull+1;

				// Avoid zero values (hole size)
				if (0 == (int)holeSizeWeibull)
					holeSizeWeibull = holeSizeWeibull+1;

				// If the file leaves the disk limits...
				if ((currentOffset+(int)bunchSizeWeibull) > maxBlocks){

					// Recalculate the file's initial block!
					currentOffset =  random () % (fileTotalBlocks);
				}

				// Not the last brach!
				if (restFileBlocks > bunchSizeWeibull){

					// Append the current brach
					file.addBranch (currentOffset, (int)bunchSizeWeibull);

					// Update the rest of the file to be branched!
					restFileBlocks-=(int)bunchSizeWeibull;
				}

				// last Branch!
				else{

					// Append the last Branch!
					file.addBranch (currentOffset, restFileBlocks);

					// All file is branched!
					restFileBlocks=0;
				}

				// Update the offset (hole to the next branch)
				currentOffset +=  (int) holeSizeWeibull;
			}
		}
}


void GeneralPurposeFS::loadExt2WeibullParameters (){

	int i, num;
	char line [LINE_SIZE];
	char fileName [NAME_SIZE];
	char* res;

	FILE* bunchSize_ext2_Alpha_fd;
	FILE* bunchSize_ext2_Beta_fd;

	FILE* contiguous_ext2_Alpha_fd;
	FILE* contiguous_ext2_Beta_fd;

	FILE* holeSize_ext2_Alpha_fd;
	FILE* holeSize_ext2_Beta_fd;


		// Load bunchSize_ext2_Alpha_table!
		//sprintf (fileName, "%s/src/%s%s\0", getenv("SIMCAN_HOME"), relative_Weibull_Files_Path.c_str(), bunchSize_ext2_Alpha_file.c_str());
	    sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), bunchSize_ext2_Alpha_file.c_str());
		bunchSize_ext2_Alpha_fd = fopen (fileName, "r");


		// Error opening the file?
		if (bunchSize_ext2_Alpha_fd == NULL)
			showErrorMessage ("Error: %s. Error opening the file bunchSize_ext2_Alpha!", fileName);

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, bunchSize_ext2_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file bunchSize_ext2_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &bunchSize_ext2_Alpha_table[i][0], &bunchSize_ext2_Alpha_table[i][1], &bunchSize_ext2_Alpha_table[i][2], &bunchSize_ext2_Alpha_table[i][3], &bunchSize_ext2_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file bunchSize_ext2_Alpha!");
			}
		}

		// Load bunchSize_ext2_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), bunchSize_ext2_Beta_file.c_str());
		bunchSize_ext2_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (bunchSize_ext2_Beta_fd == NULL)
			showErrorMessage ("Error opening the file bunchSize_ext2_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, bunchSize_ext2_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file bunchSize_ext2_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &bunchSize_ext2_Beta_table[i][0], &bunchSize_ext2_Beta_table[i][1], &bunchSize_ext2_Beta_table[i][2], &bunchSize_ext2_Beta_table[i][3], &bunchSize_ext2_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file bunchSize_ext2_Beta!");
				}
		}

		// Load contiguous_ext2_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), contiguous_ext2_Alpha_file.c_str());
		contiguous_ext2_Alpha_fd = fopen (fileName, "r");

		// Error opening the file?
		if (contiguous_ext2_Alpha_fd == NULL)
			showErrorMessage ("Error opening the file contiguous_ext2_Alpha!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, contiguous_ext2_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file contiguous_ext2_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &contiguous_ext2_Alpha_table[i][0], &contiguous_ext2_Alpha_table[i][1], &contiguous_ext2_Alpha_table[i][2], &contiguous_ext2_Alpha_table[i][3], &contiguous_ext2_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file contiguous_ext2_Alpha!");
			}
		}

		// Load contiguous_ext2_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), contiguous_ext2_Beta_file.c_str());
		contiguous_ext2_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (contiguous_ext2_Beta_fd == NULL)
			showErrorMessage ("Error opening the file contiguous_ext2_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, contiguous_ext2_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file contiguous_ext2_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &contiguous_ext2_Beta_table[i][0], &contiguous_ext2_Beta_table[i][1], &contiguous_ext2_Beta_table[i][2], &contiguous_ext2_Beta_table[i][3], &contiguous_ext2_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file contiguous_ext2_Beta!");
			}
		}

		// Load holeSize_ext2_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), holeSize_ext2_Alpha_file.c_str());
		holeSize_ext2_Alpha_fd = fopen (fileName, "r");

		// Error opening the file?
		if (holeSize_ext2_Alpha_fd == NULL)
			showErrorMessage ("Error opening the file holeSize_ext2_Alpha!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, holeSize_ext2_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file holeSize_ext2_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &holeSize_ext2_Alpha_table[i][0], &holeSize_ext2_Alpha_table[i][1], &holeSize_ext2_Alpha_table[i][2], &holeSize_ext2_Alpha_table[i][3], &holeSize_ext2_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file holeSize_ext2_Alpha!");
			}
		}

		// Load holeSize_ext2_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), holeSize_ext2_Beta_file.c_str());
		holeSize_ext2_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (holeSize_ext2_Beta_fd == NULL)
			showErrorMessage ("Error opening the file holeSize_ext2_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, holeSize_ext2_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file holeSize_ext2_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &holeSize_ext2_Beta_table[i][0], &holeSize_ext2_Beta_table[i][1], &holeSize_ext2_Beta_table[i][2], &holeSize_ext2_Beta_table[i][3], &holeSize_ext2_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file holeSize_ext2_Beta!");
			}
		}
}


void GeneralPurposeFS::loadReiserFSWeibullParameters (){

	int i, num;
	char line[LINE_SIZE];
	char fileName[NAME_SIZE];
	char* res;

	FILE* bunchSize_reiserFS_Alpha_fd;
	FILE* bunchSize_reiserFS_Beta_fd;

	FILE* contiguous_reiserFS_Alpha_fd;
	FILE* contiguous_reiserFS_Beta_fd;

	FILE* holeSize_reiserFS_Alpha_fd;
	FILE* holeSize_reiserFS_Beta_fd;


		// Load bunchSize_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), bunchSize_reiserFS_Alpha_file.c_str());
		bunchSize_reiserFS_Alpha_fd = fopen (fileName, "r");

		// Error opening the file?
		if (bunchSize_reiserFS_Alpha_fd == NULL)
			showErrorMessage ("Error opening the file bunchSize_reiserFS_Alpha!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, bunchSize_reiserFS_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file bunchSize_reiserFS_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &bunchSize_reiserFS_Alpha_table[i][0], &bunchSize_reiserFS_Alpha_table[i][1], &bunchSize_reiserFS_Alpha_table[i][2], &bunchSize_reiserFS_Alpha_table[i][3], &bunchSize_reiserFS_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file bunchSize_reiserFS_Alpha!");
			}
		}

		// Load bunchSize_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), bunchSize_reiserFS_Beta_file.c_str());
		bunchSize_reiserFS_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (bunchSize_reiserFS_Beta_fd == NULL)
			showErrorMessage ("Error opening the file bunchSize_reiserFS_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, bunchSize_reiserFS_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file bunchSize_reiserFS_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &bunchSize_reiserFS_Beta_table[i][0], &bunchSize_reiserFS_Beta_table[i][1], &bunchSize_reiserFS_Beta_table[i][2], &bunchSize_reiserFS_Beta_table[i][3], &bunchSize_reiserFS_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file bunchSize_reiserFS_Beta!");
			}
		}

		// Load contiguous_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), contiguous_reiserFS_Alpha_file.c_str());
		contiguous_reiserFS_Alpha_fd = fopen (fileName, "r");

		// Error opening the file?
		if (contiguous_reiserFS_Alpha_fd == NULL)
			showErrorMessage ("Error opening the file contiguous_reiserFS_Alpha!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, contiguous_reiserFS_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file contiguous_reiserFS_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &contiguous_reiserFS_Alpha_table[i][0], &contiguous_reiserFS_Alpha_table[i][1], &contiguous_reiserFS_Alpha_table[i][2], &contiguous_reiserFS_Alpha_table[i][3], &contiguous_reiserFS_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file contiguous_reiserFS_Alpha!");
			}
		}

		// Load contiguous_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), contiguous_reiserFS_Beta_file.c_str());
		contiguous_reiserFS_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (contiguous_reiserFS_Beta_fd == NULL)
			showErrorMessage ("Error opening the file contiguous_reiserFS_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, contiguous_reiserFS_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file contiguous_reiserFS_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &contiguous_reiserFS_Beta_table[i][0], &contiguous_reiserFS_Beta_table[i][1], &contiguous_reiserFS_Beta_table[i][2], &contiguous_reiserFS_Beta_table[i][3], &contiguous_reiserFS_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file contiguous_reiserFS_Beta!");
			}
		}

		// Load holeSize_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), holeSize_reiserFS_Alpha_file.c_str());
		holeSize_reiserFS_Alpha_fd = fopen (fileName, "r");

		// Error opening the file?
		if (holeSize_reiserFS_Alpha_fd == NULL)
			showErrorMessage ("Error opening the file holeSize_reiserFS_Alpha!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, holeSize_reiserFS_Alpha_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file holeSize_reiserFS_Alpha!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &holeSize_reiserFS_Alpha_table[i][0], &holeSize_reiserFS_Alpha_table[i][1], &holeSize_reiserFS_Alpha_table[i][2], &holeSize_reiserFS_Alpha_table[i][3], &holeSize_reiserFS_Alpha_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file holeSize_reiserFS_Alpha!");
			}
		}

		// Load holeSize_reiserFS_Alpha_table!
		sprintf (fileName, "%s%s%s\0", simcanHomePath.c_str(), relative_Weibull_Files_Path.c_str(), holeSize_reiserFS_Beta_file.c_str());
		holeSize_reiserFS_Beta_fd = fopen (fileName, "r");

		// Error opening the file?
		if (holeSize_reiserFS_Beta_fd == NULL)
			showErrorMessage ("Error opening the file holeSize_reiserFS_Beta!");

		else{

			// Load all values!
			for (i=0; i<10; i++){

				res = fgets (line, LINE_SIZE, holeSize_reiserFS_Beta_fd);

				// Error reading the file!
				if (res == NULL)
					showErrorMessage ("Error reading the file holeSize_reiserFS_Beta!");

				num = sscanf (line, "%f\t%f\t%f\t%f\t%f", &holeSize_reiserFS_Beta_table[i][0], &holeSize_reiserFS_Beta_table[i][1], &holeSize_reiserFS_Beta_table[i][2], &holeSize_reiserFS_Beta_table[i][3], &holeSize_reiserFS_Beta_table[i][4]);

				// Error parsing the file!
				if (num != 5)
					showErrorMessage ("Error parsing the file holeSize_reiserFS_Beta!");
			}
		}
}


string GeneralPurposeFS::ext2WeibullTablesToString (){

	int i;
	char line [LINE_SIZE];
	std::ostringstream osStream;

		// Init...
		bzero (line, LINE_SIZE);
		sprintf (line, "--- Module %d. Contents of Ext2 FS Weibull tables ---\0", getId());
		osStream << line << endl;

		sprintf (line, "Contents of bunchSize_ext2_Alpha_table\0");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", bunchSize_ext2_Alpha_table[i][0], bunchSize_ext2_Alpha_table[i][1], bunchSize_ext2_Alpha_table[i][2], bunchSize_ext2_Alpha_table[i][3], bunchSize_ext2_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of bunchSize_ext2_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", bunchSize_ext2_Beta_table[i][0], bunchSize_ext2_Beta_table[i][1], bunchSize_ext2_Beta_table[i][2], bunchSize_ext2_Beta_table[i][3], bunchSize_ext2_Beta_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of contiguous_ext2_Alpha_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", contiguous_ext2_Alpha_table[i][0], contiguous_ext2_Alpha_table[i][1], contiguous_ext2_Alpha_table[i][2], contiguous_ext2_Alpha_table[i][3], contiguous_ext2_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of contiguous_ext2_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", contiguous_ext2_Beta_table[i][0], contiguous_ext2_Beta_table[i][1], contiguous_ext2_Beta_table[i][2], contiguous_ext2_Beta_table[i][3], contiguous_ext2_Beta_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of holeSize_ext2_Alpha_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", holeSize_ext2_Alpha_table[i][0], holeSize_ext2_Alpha_table[i][1], holeSize_ext2_Alpha_table[i][2], holeSize_ext2_Alpha_table[i][3], holeSize_ext2_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of holeSize_ext2_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", holeSize_ext2_Beta_table[i][0], holeSize_ext2_Beta_table[i][1], holeSize_ext2_Beta_table[i][2], holeSize_ext2_Beta_table[i][3], holeSize_ext2_Beta_table[i][4]);
			osStream << line << endl;
		}

	return osStream.str().c_str();
}


string GeneralPurposeFS::reiserFSWeibullTablesToString (){

	int i;
	char line [LINE_SIZE];
	std::ostringstream osStream;

		// Init...
		bzero (line, LINE_SIZE);
		sprintf (line, "--- Module %d. Contents of Reiser FS Weibull tables ---\0", getId());
		osStream << line << endl;

		sprintf (line, "Contents of bunchSize_reiserFS_Alpha_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", bunchSize_reiserFS_Alpha_table[i][0], bunchSize_reiserFS_Alpha_table[i][1], bunchSize_reiserFS_Alpha_table[i][2], bunchSize_reiserFS_Alpha_table[i][3], bunchSize_reiserFS_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of bunchSize_reiserFS_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", bunchSize_reiserFS_Beta_table[i][0], bunchSize_reiserFS_Beta_table[i][1], bunchSize_reiserFS_Beta_table[i][2], bunchSize_reiserFS_Beta_table[i][3], bunchSize_reiserFS_Beta_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of contiguous_reiserFS_Alpha_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", contiguous_reiserFS_Alpha_table[i][0], contiguous_reiserFS_Alpha_table[i][1], contiguous_reiserFS_Alpha_table[i][2], contiguous_reiserFS_Alpha_table[i][3], contiguous_reiserFS_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of contiguous_reiserFS_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", contiguous_reiserFS_Beta_table[i][0], contiguous_reiserFS_Beta_table[i][1], contiguous_reiserFS_Beta_table[i][2], contiguous_reiserFS_Beta_table[i][3], contiguous_reiserFS_Beta_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of holeSize_reiserFS_Alpha_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", holeSize_reiserFS_Alpha_table[i][0], holeSize_reiserFS_Alpha_table[i][1], holeSize_reiserFS_Alpha_table[i][2], holeSize_reiserFS_Alpha_table[i][3], holeSize_reiserFS_Alpha_table[i][4]);
			osStream << line << endl;
		}

		sprintf (line, "Contents of holeSize_reiserFS_Beta_table");
		osStream << line << endl;

		for (i=0; i<10; i++){
			sprintf (line, "%f\t%f\t%f\t%f\t%f", holeSize_reiserFS_Beta_table[i][0], holeSize_reiserFS_Beta_table[i][1], holeSize_reiserFS_Beta_table[i][2], holeSize_reiserFS_Beta_table[i][3], holeSize_reiserFS_Beta_table[i][4]);
			osStream << line << endl;
		}

	return osStream.str();
}


string GeneralPurposeFS::FSFilesToString (){

	vector <SIMCAN_File>::iterator list_it;		// List iterator
	std::ostringstream osStream;				// Stream
	unsigned int fileNumber;					// File number


		// Init...
		fileNumber = 0;
		osStream << "File System contents..." << endl;

		// Walk through the list searching the requested block!
		for (list_it=fileList.begin(); list_it!=fileList.end(); ++list_it){
			osStream << "file[" << fileNumber << "]: "<< list_it->contentsToString(DEBUG_BRANCHES_FS, DEBUG_MSG_FS);
			fileNumber++;
		}

	return osStream.str();
}


unsigned int GeneralPurposeFS::getCategoryMaxSize (unsigned int fileSize){

	unsigned int category;

		// Init...
		category = 0;

		// Calculate the category...
		if ((fileSize >= 0) && (fileSize <= (10*KB)))
			category=10*KB;
		else if ((fileSize > (10*KB)) && (fileSize <= (100*KB)))
			category=100*KB;
		else if ((fileSize > (100*KB)) && (fileSize <= MB))
			category=MB;
		else if ((fileSize > MB) && (fileSize <= (10*MB)))
			category=10*MB;
		else if ((fileSize > 10*MB) && (fileSize <= (100*MB)))
			category=100*MB;
		else if ((fileSize > 100*MB) && (fileSize <= (2*GB)))
			category=2*GB;

	return category;
}


int GeneralPurposeFS::getDiskRatio_x_axis (){

	int x;

		// Init...
		x=0;

		if ((diskRatio >= 0) && (diskRatio < 0.1))
			x = 0;
		else if ((diskRatio >= 0.1) && (diskRatio < 0.2))
			x = 1;
		else if ((diskRatio >= 0.2) && (diskRatio < 0.3))
			x = 2;
		else if ((diskRatio >= 0.3) && (diskRatio < 0.4))
			x = 3;
		else if ((diskRatio >= 0.4) && (diskRatio < 0.5))
			x = 4;
		else if ((diskRatio >= 0.5) && (diskRatio < 0.6))
			x = 5;
		else if ((diskRatio >= 0.6) && (diskRatio < 0.7))
			x = 6;
		else if ((diskRatio >= 0.7) && (diskRatio < 0.8))
			x = 7;
		else if ((diskRatio >= 0.8) && (diskRatio < 0.9))
			x = 8;
		else if (diskRatio >= 0.9)
			x = 9;


	return x;
}


int GeneralPurposeFS::getFileSizeCategory_y_axis (unsigned int fileSize){

	int y;

		// Init...
		y=0;

		if ((fileSize >= 0) && (fileSize <= (10*KB)))
			y=0;
		else if ((fileSize > (10*KB)) && (fileSize <= (100*KB)))
			y=1;
		else if ((fileSize > (100*KB)) && (fileSize <= MB))
			y=2;
		else if ((fileSize > MB) && (fileSize <= (10*MB)))
			y=3;
		else if (fileSize > 10*MB)
			y=4;

	return y;
}


