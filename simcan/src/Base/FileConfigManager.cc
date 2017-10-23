#include "FileConfigManager.h"

Define_Module (FileConfigManager);


FileConfigManager::~FileConfigManager(){
	
	if (serverNfsCfg != NULL)
		delete (serverNfsCfg);
		
	if (serverPfsCfg != NULL)
		delete (serverPfsCfg);
		
	if (mpiEnv != NULL)
		delete (mpiEnv);
	
	if (preloadFS != NULL)
		delete (preloadFS);
	
	if (vfsCfg != NULL)
		delete (vfsCfg);
}


void FileConfigManager::initialize (int stage){

    cSimpleModule::initialize(stage);

    if (stage == 0){

		// Super-init!
		cSIMCAN_Base::initialize();

		// Parse the NFS servers file
		const char *serversFileNFS = par ("serversNfs");

		// Create the cfgServer object!
		if (strlen(serversFileNFS) > 0){
			serverNfsCfg = new CfgServers();		
			serverNfsCfg->parseFile (serversFileNFS);
			
			// Debug?
			if (DEBUG_FILE_CONFIG_printServers)
			    showStartedModule ("NFS Servers: %s ", serverNfsCfg->toString().c_str());
		}
		else
			serverNfsCfg = NULL;
			
			
		// Parse the PFS servers file
		const char *serversFilePFS = par ("serversPfs");

		// Create the cfgServer object!
		if (strlen(serversFilePFS) > 0){
			serverPfsCfg = new CfgServers();		
			serverPfsCfg->parseFile (serversFilePFS);
			
			// Debug?
			if (DEBUG_FILE_CONFIG_printServers)
			    showStartedModule ("Parallel File System Servers: %s ", serverPfsCfg->toString().c_str());
		}
		else
			serverPfsCfg = NULL;			
		
		
		
		// Parse the preLoaded files (File System)
		const char *preloadFileList = par ("preloadFiles");

		// Create the preLoad object!
		if (strlen(preloadFileList) > 0){
			preloadFS = new CfgPreloadFS();
			preloadFS->parseFile (preloadFileList);
			
			if (DEBUG_FILE_CONFIG_printPreload)
			    showStartedModule (" %s ", preloadFS->toString().c_str());
		}
		else
			preloadFS = NULL;
		
		// Parse the IOR file
		const char *vfsConfigFile = par ("vfsConfigFile");

		// Create the cfgIOR object!
		if (strlen(vfsConfigFile) > 0){
			vfsCfg = new CfgVFS();
			vfsCfg->parseFile (vfsConfigFile);
			
			if (DEBUG_FILE_CONFIG_printIORCfg)
			    showStartedModule (" %s ", vfsCfg->toString().c_str());
		}
		else
			vfsCfg = NULL;
			
		
		// Parse the MPI trace player machine file
		const char *mpiEnv_char = par ("mpiEnv");		

		// Create the cfgMPI object!
		if (strlen(mpiEnv_char) > 0){
			mpiEnv = new CfgMPI();
			mpiEnv->parseFile (mpiEnv_char);
			
			if (DEBUG_FILE_CONFIG_printMPIEnvCfg)
			    showStartedModule (" %s ", mpiEnv->toString().c_str());
		}
		else
			mpiEnv = NULL;		
    }
}



void FileConfigManager::handleMessage (cMessage *msg){
}


void FileConfigManager::finish(){
}


void FileConfigManager::processSelfMessage (cMessage *msg){
}


void FileConfigManager::processRequestMessage (SIMCAN_Message *sm){
}


void FileConfigManager::processResponseMessage (SIMCAN_Message *sm){
}


cGate* FileConfigManager::getOutGate (cMessage *msg){
}


CfgServers* FileConfigManager::getServerNfsCfg(){
	return serverNfsCfg;
}


CfgServers* FileConfigManager::getServerPfsCfg(){
	return serverPfsCfg;
}


CfgPreloadFS* FileConfigManager::getPreloadFS(){
	return preloadFS;
}


CfgVFS* FileConfigManager::getVfsCfg(){
	return vfsCfg;
}


CfgMPI* FileConfigManager::getMPIEnv(){
	return mpiEnv;	
}


