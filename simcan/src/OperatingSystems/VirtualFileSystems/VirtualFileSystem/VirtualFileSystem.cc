#include "VirtualFileSystem.h"

Define_Module(VirtualFileSystem);



VirtualFileSystem::~VirtualFileSystem(){
	
	pathTable.clear();
}


void VirtualFileSystem::initialize(){

	int i;
	std::ostringstream osStream;


		// Set the moduleIdName
		osStream << "VirtualFileSystem." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		cSIMCAN_Base::initialize();

		// Get module parameters	    
		numFS = par ("numFS");
		nfsClientIndex = par ("nfsClientIndex");

	    // Get gate Ids
		fromMemoryGate = gate ("fromMemory");
		toMemoryGate = gate ("toMemory");

	    // Init the gate IDs to/from Input gates...
	    fromFSGates = new cGate* [numFS];
	    toFSGates = new cGate* [numFS];

		for (i=0; i<numFS; i++){
			fromFSGates[i] = gate ("fromFS", i);
			toFSGates[i] = gate ("toFS", i);
	    }

	    // Parse the config file
	    parseConfigFile();

		// Shows VirtualFileSystem table! (debug only)
		showStartedModule (" %s ", VFSTableToSring().c_str());
}


void VirtualFileSystem::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* VirtualFileSystem::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from cache
		if (msg->getArrivalGate()==fromMemoryGate){
			if (toMemoryGate->getNextGate()->isConnected()){
				return (toMemoryGate);
			}
		}

		// If msg arrive from FS's
		else if (msg->arrivedOn("fromFS")){
			for (i=0; i<numFS; i++)
				if (msg->arrivedOn ("fromFS", i))
					return (toFSGates[i]);
		}

	// If gate not found!
	return NULL;
}


void VirtualFileSystem::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void VirtualFileSystem::processRequestMessage (SIMCAN_Message *sm){

	string fileName;
	bool found;
	string::size_type searchResult;

	SIMCAN_App_IO_Message *sm_io;
	vector <pathEntry>::iterator vIterator;

	unsigned int index;	
	string moduleType;


		// Cast!
		sm_io = check_and_cast <SIMCAN_App_IO_Message*> (sm);

		// Init
		found = false;

		// Request came from FS. Remote operation! Send to memory...
		if (sm_io->getRemoteOperation()){
			sendRequestMessage (sm_io, toMemoryGate);
		}

		// Request came from memory!
		else{

			// Get the file name
	    	fileName = sm_io->getFileName();

	    	 // Search the path!
	    	 for (vIterator=pathTable.begin();((vIterator!=pathTable.end()) && (!found)); vIterator++){

				searchResult = fileName.find((*vIterator).path,0);

				 if (searchResult == 0){
					found=true;
					index = (*vIterator).index;
					moduleType = (*vIterator).type;
				 }
			}

			// Set the Module index, if found!
			if (found){
				
				// Is a FS destination or a remote Operation? Set Remote operation field and send message to corresponding module
				if (moduleType == VFS_LOCAL){
					
					if (DEBUG_VFS)
						showDebugMessage ("Redirecting request to FS[%u] %s", index, sm_io->contentsToString(DEBUG_MSG_VFS).c_str());
				
					sm_io->setNextModuleIndex (index);					
					sm_io->setRemoteOperation (false);
					sendRequestMessage (sm_io, toFSGates[index]);
				}				
				
				else{				
					
					if (DEBUG_VFS)
						showDebugMessage ("Redirecting request to APP[%u] - ServerID:%d %s", nfsClientIndex, index, sm_io->contentsToString(DEBUG_MSG_VFS).c_str());
								
					sm_io->setNextModuleIndex (nfsClientIndex);
					sm_io->setCommId(index);
					sm_io->setRemoteOperation (true);
					sendRequestMessage (sm_io, toMemoryGate);								
				}
			}

			// Path not found! :(
			else{
				showErrorMessage ("Path not found for file [%s]", fileName.c_str());
			}
		}
}


void VirtualFileSystem::processResponseMessage (SIMCAN_Message *sm){

	// Send back the message
	sendResponseMessage (sm);
}


void VirtualFileSystem::parseConfigFile (){
	
	int i;
	CfgVFS *iorCfg;
	FileConfigManager * fileCfgMng;
	cModule *fileConfigModule;
	pathEntry newEntry;
	

		// Node with local fileConfig?
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
		
		fileCfgMng = check_and_cast<FileConfigManager *>(fileConfigModule);
		iorCfg = fileCfgMng->getVfsCfg();
		
		if (iorCfg != NULL){

			// Store info in current class
			for (i=0; i<iorCfg->getNumEntries(); i++){
				
				newEntry.type = iorCfg->getType(i);
				newEntry.path = iorCfg->getPath(i);
				newEntry.index = iorCfg->getIndex(i);

				pathTable.push_back (newEntry);			
			}
		}
}


string VirtualFileSystem::VFSTableToSring (){

	std::ostringstream osStream;	
	int i;

		osStream << "Virtual File System table..." << endl;

		for (i=0; i<pathTable.size(); i++){		

			osStream << "  Entry[" << i << "] Prefix-Path:" << pathTable[i].path.c_str() <<
						"  Index:" << pathTable[i].index <<
						"  Type:" <<  pathTable[i].type.c_str() << endl;
		}

	return osStream.str();
}


