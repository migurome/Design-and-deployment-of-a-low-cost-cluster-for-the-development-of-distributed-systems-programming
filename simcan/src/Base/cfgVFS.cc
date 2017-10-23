#include "cfgVFS.h"


CfgVFS::~CfgVFS (){	
	
	if (vfsVector.size() > 0)
		vfsVector.clear();
}


CfgVFS::CfgVFS (){
	vfsVector.clear();
}


void CfgVFS::parseFile(const char* fileName){

	FILE *vfsFD;
	char line [LINE_SIZE];
	int parsedParameters, i;
	
	unsigned int numEntries;
	char pathValue_LOCAL [LINE_SIZE];
	int readParameters_LOCAL;
	int indexValue_LOCAL;
	char pathValue_REMOTE [LINE_SIZE];
	int readParameters_REMOTE;
	int indexValue_REMOTE;
	
	vfs_T iorEntry;
	std::ostringstream info;
	
	
		// Init...
		bzero (line, LINE_SIZE);
	
		// Open the machines file
		vfsFD = fopen (fileName, "r");

		// There is no servers file
		if (vfsFD == NULL){
			info << "Error opening IOR config file:" << fileName;
			perror (info.str().c_str());
			opp_error (info.str().c_str());
			throw cRuntimeError(info.str().c_str());
		}

		// start to load files...
		else{

			// Read the number of files to load
			fgets (line, LINE_SIZE, vfsFD);
			numEntries = atoi (line);

			// Preload numEntries files!!!
			for (i=0; i<numEntries; i++){
				
				// Load the current path entry
				if (fgets (line, LINE_SIZE, vfsFD) == NULL){
					info << "Error reading file " << fileName << ". Line: " << line;
					throw cRuntimeError(info.str().c_str());
				}

				else{
					
					// Clear buffers
					bzero (pathValue_LOCAL, LINE_SIZE);
					bzero (pathValue_REMOTE, LINE_SIZE);
	
					// Init
					readParameters_LOCAL = readParameters_REMOTE = 0;
	
					// Parses the line!
					readParameters_LOCAL = sscanf (line, "LOCAL:%[^':\n']:%d", pathValue_LOCAL, &indexValue_LOCAL);
					readParameters_REMOTE = sscanf (line, "REMOTE:%[^':\n']:%d", pathValue_REMOTE, &indexValue_REMOTE);
	
					// FS entry (local)
					if (readParameters_LOCAL>readParameters_REMOTE){
						iorEntry.type = VFS_LOCAL;
						iorEntry.path = pathValue_LOCAL;
						iorEntry.index = indexValue_LOCAL;
						vfsVector.push_back (iorEntry);						
					}
	
					// APP entry (remote)
					else if (readParameters_LOCAL<readParameters_REMOTE){
						iorEntry.type = VFS_REMOTE;
						iorEntry.path = pathValue_REMOTE;
						iorEntry.index = indexValue_REMOTE;	
						vfsVector.push_back (iorEntry);					
					}
					
					// Error parsing!
					else{
						info << "Error parsing file " << fileName << ". Line: " << line;
						throw cRuntimeError(info.str().c_str());
					}					
				}
			}
		}
}			
		

int CfgVFS::getNumEntries(){

	return (vfsVector.size());
}


string CfgVFS::getType (unsigned int index){

	if (index>=vfsVector.size())
		throw cRuntimeError("Index out of bounds in getType (CfgVFS)");
	else
		return vfsVector[index].type;
}


string CfgVFS::getPath (unsigned int index){

	if (index>=vfsVector.size())
		throw cRuntimeError("Index out of bounds in getPath (CfgVFS)");
	else
		return vfsVector[index].path;
}


unsigned int CfgVFS::getIndex (unsigned int index){

	if (index>=vfsVector.size())
		throw cRuntimeError("Index out of bounds in getIndex (CfgVFS)");
	else
		return vfsVector[index].index;
}


string CfgVFS::toString (){
	
	int i;
	std::ostringstream info;

		if (vfsVector.size() == 0)
			info << "VFS config is empty!" << endl;

		else{
			info << "Printing VFS entries from cfgVFS class..." << endl;

			for (i=0; i<vfsVector.size(); i++){

				info << "  Entry[" << i << "]" 
				     << "  Type:" << vfsVector[i].type 
				     << "  Path:" << vfsVector[i].path 
				     << "  Index:" << vfsVector[i].index
				     << endl;
			}
		}
		
	return info.str();
}

