#include "BasicFileSystem.h"

Define_Module (BasicFileSystem);

//const double Basic_FileSystem::OPEN_TIME = 0.00018173;
//const double Basic_FileSystem::CLOSE_TIME = 0.00001554;
//const double Basic_FileSystem::CREATE_TIME = 0.00193551;

const const_simtime_t BasicFileSystem::OPEN_TIME = 0.00011;
const const_simtime_t BasicFileSystem::CLOSE_TIME = 0.000013;
const const_simtime_t BasicFileSystem::CREATE_TIME = 0.000016;


BasicFileSystem::~BasicFileSystem(){
	
	fileList.clear();	
}


void BasicFileSystem::initialize(){

	std::ostringstream osStream;
	int i;

		// Set the moduleIdName
		osStream << "Basic_FileSystem." << getId();
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

		// Pre load files parameters...
		preLoadFiles = par ("preLoadFiles");

	    // Load files on FileSystem
	    if (preLoadFiles){

            if (preloadFSFiles () == SIMCAN_OK){
                if (DEBUG_Basic_FS){
                    showDebugMessage ("preLoad files succesful!");
                }
            }
            else
                showDebugMessage ("Error loading files...");
	    }
	    else
	    	showDebugMessage ("Empty File system. No files to load");

	    // Show file info?
	    if (DEBUG_FS_Basic_Files)
	    	showDebugMessage (" %s ", FSFilesToString().c_str());
}


void BasicFileSystem::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* BasicFileSystem::getOutGate (cMessage *msg){

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


void BasicFileSystem::processSelfMessage (cMessage *msg){

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


void BasicFileSystem::processRequestMessage (SIMCAN_Message *sm){
	processIORequest (sm);
}


void BasicFileSystem::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	SIMCAN_BlockList_Message *sm_bl;


		sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(sm);
		sm_io = sm_bl->transformToApp_IO_Message();

		delete (sm_bl);

		if (DEBUG_Basic_FS)
			showDebugMessage ("Processing Response. %s", sm_io->contentsToString(DEBUG_MSG_Basic_FS).c_str());

	sendResponseMessage (sm_io);
}


void BasicFileSystem::processIORequest (SIMCAN_Message *sm){

	string fileName;				// File Name
	int result;						// IO result

	SIMCAN_App_IO_Message *sm_io;			// IO Request message
	SIMCAN_BlockList_Message *sm_bl;		// Block List message
	vector <SIMCAN_File>::iterator list_it;	// File list iterator

    	// Cast!
    	sm_io = check_and_cast<SIMCAN_App_IO_Message *>(sm);

    	// Get file name!
    	fileName = sm_io->getFileName();    	
    	
    	if (DEBUG_Basic_FS)
    		showDebugMessage ("Processing IO Request. %s", sm_io->contentsToString(DEBUG_MSG_Basic_FS).c_str());
    		
    	// Show file info?
	    if (DEBUG_PARANOIC_Basic_FS){
	    	showDebugMessage ("------ Start FS contents ------");
	    	showStartedModule (" %s ", FSFilesToString().c_str());
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
			if ((DEBUG_FS_Basic_Files) && (DEBUG_Basic_FS))
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
			if ((DEBUG_FS_Basic_Files) && (DEBUG_Basic_FS))
				showDebugMessage (" %s ", FSFilesToString().c_str());

			scheduleAt (CLOSE_TIME+simTime(), latencyMessage);
		}

		// Read/Write File
		else if ((sm_io->getOperation() == SM_READ_FILE) ||
				 (sm_io->getOperation() == SM_WRITE_FILE)){


			// If numBytes == 0, IO request ends here, send back the message!
			if (sm_io->getSize() == 0){
				
				if (DEBUG_Basic_FS)
		    		showDebugMessage ("Returning request (numBytes = 0). %s", sm_io->contentsToString(DEBUG_MSG_Basic_FS).c_str());
				
				throw cRuntimeError("Error! Requesting 0 bytes!!!");
								
				sm_io->setResult (SIMCAN_OK);
				sm_io->setIsResponse (true);
				sendResponseMessage (sm_io);
			}

			// At least there is a byte to make the request! Calculate the involved blocks...
			else{

				// Creates a new block list message
				sm_bl = new SIMCAN_BlockList_Message (sm_io);

				// Creates a a message that contains one branch
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


int BasicFileSystem::preloadFSFiles(){
	
	int i, result;
	CfgPreloadFS *preloadCfg;
	FileConfigManager *fileCfgMng;
	cModule *fileConfigModule;
	
		// Init...
		result = SIMCAN_OK;	
			
		// Storage node with local fileConfig?
		fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
		
		// Not storage node!
		if (fileConfigModule == NULL){
			
			// Single node using a global file config
			fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
			// Not a global file Config
			if (fileConfigModule == NULL){
				
				// Racked compute node. Get file config from Rack!		
				fileConfigModule = getParentModule()->getParentModule()->getParentModule()->getParentModule()->getParentModule()->getSubmodule("fileConfig");
			
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
					insertNewFile (preloadCfg->getSizeKB(i)*KB, preloadCfg->getFileName(i).c_str());
				}
			}
		}
		else
			result = SIMCAN_ERROR;
			

    return result;
}


int BasicFileSystem::insertNewFile (unsigned int fileSize, string fileName){

	int fileBlocks;	
	SIMCAN_File newFile;
	SIMCAN_File& ref_newFile = newFile;		
		
		// Calculate info for new file
		newFile.setFileName (fileName);
		newFile.setFileSize (fileSize);

		// Insert new file
		fileList.push_back (newFile);

	return SIMCAN_OK;
}


vector <SIMCAN_File>::iterator BasicFileSystem::searchFile (string fileName){

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


void BasicFileSystem::deleteFile (string fileName){

	vector <SIMCAN_File>::iterator list_it;


		// Search the file!
		list_it = searchFile (fileName);

		// File found! remove it!
		if (list_it != fileList.end())
			fileList.erase (list_it);

}


void BasicFileSystem::translateIORequets (SIMCAN_BlockList_Message* sm_fsTranslated){

	unsigned int numSectors;
	string fileName;
	unsigned int offset;
	unsigned int numBytes;
	vector <SIMCAN_File>::iterator list_it;	// List iterator

		// Init
		fileName = sm_fsTranslated->getFileName ();
		offset = sm_fsTranslated->getOffset ();
		numBytes = sm_fsTranslated->getSize ();
		numSectors = 0;

		// Search for the requested file
		list_it = searchFile (fileName);

		// If file not found!!!
		if (list_it == fileList.end()){
				
			sm_fsTranslated->setIsResponse (true);
			sm_fsTranslated->setResult (SIMCAN_FILE_NOT_FOUND);		
			throw cRuntimeError("Error! Requesting 0 bytes!!!");	
		}
		
		else{
			
			numSectors = (unsigned int) ceil (numBytes/ BYTES_PER_SECTOR);
			
			// Add the branch!
			sm_fsTranslated->getFile().addBranch (0, numSectors);
			
			// Set the request size!
			sm_fsTranslated->getFile().setFileSize (sm_fsTranslated->getFile().getTotalSectors() * BYTES_PER_SECTOR);
		}

	if (DEBUG_DETAILED_Basic_FS)
		showDebugMessage ("Translating request -> %s",
				sm_fsTranslated->getFile().contentsToString(DEBUG_BRANCHES_Basic_FS, DEBUG_MSG_Basic_FS).c_str());
}


string BasicFileSystem::FSFilesToString (){

	vector <SIMCAN_File>::iterator list_it;		// List iterator
	std::ostringstream osStream;				// Stream
	unsigned int fileNumber;					// File number


		// Init...
		fileNumber = 0;
		osStream << "File System contents..." << endl;

		// Walk through the list searching the requested block!
		for (list_it=fileList.begin(); list_it!=fileList.end(); ++list_it){
			osStream << "file[" << fileNumber << "]: "<< list_it->contentsToString(DEBUG_BRANCHES_Basic_FS, DEBUG_FS_Basic_Files);
			fileNumber++;
		}

	return osStream.str();
}


