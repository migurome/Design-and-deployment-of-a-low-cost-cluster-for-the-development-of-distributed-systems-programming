#include "cfgPreloadFS.h"


CfgPreloadFS::~CfgPreloadFS (){
	
	if (preloadVector.size() > 0)
		preloadVector.clear();
}


CfgPreloadFS::CfgPreloadFS (){

	preloadVector.clear();
}


void CfgPreloadFS::parseFile(const char* fileName){

	FILE *preloadFD;
	char line [LINE_SIZE];
	int parsedParameters, i;
	
	unsigned int numPreloadFiles;
	unsigned int sizeValue;
	char fileNameValue [LINE_SIZE];
	
	preload_T preloadFile;
	std::ostringstream info;
	
	
		// Init...
		bzero (line, LINE_SIZE);
	
		// Open the machines file
		preloadFD = fopen (fileName, "r");

		// There is no servers file
		if (preloadFD == NULL){
			info << "Error opening preload FS file:" << fileName;
			perror (info.str().c_str());
			opp_error (info.str().c_str());
			throw cRuntimeError(info.str().c_str());
		}

		// start to load files...
		else{

			// Read the number of files to load
			fgets (line, LINE_SIZE, preloadFD);
			numPreloadFiles = atoi (line);

			// Preload numPreloadFiles files!!!
			for (i=0; i<numPreloadFiles; i++){
				
				// Load the current path entry
				if ((fgets (line, LINE_SIZE, preloadFD)) == NULL){
					info << "Error reading file " << fileName << ". Line: " << line;
					throw cRuntimeError(info.str().c_str());
				}

				else{
				
					// Parse current line
					bzero (fileNameValue, LINE_SIZE);
					parsedParameters = sscanf (line, "%[^':\n']:%u", fileNameValue, &sizeValue);
	
					// Well parsed!
					if (parsedParameters == 2){
						preloadFile.fileName = fileNameValue;
						preloadFile.sizeKB = sizeValue;
						preloadVector.push_back (preloadFile);
						
					}
					
					else{
						info << "Error parsing file " << fileName << ". Line: " << line;
						throw cRuntimeError(info.str().c_str());
					}
				}
			}
		}
}
		

int CfgPreloadFS::getNumFiles(){

	return (preloadVector.size());
}


string CfgPreloadFS::getFileName (unsigned int index){

	if (index>=preloadVector.size())
		throw cRuntimeError("Index out of bounds in getFileName (CfgPreloadFS)");
	else
		return preloadVector[index].fileName;
}


unsigned int CfgPreloadFS::getSizeKB (unsigned int index){

	if (index>=preloadVector.size())
		throw cRuntimeError("Index out of bounds in getSizeKB (CfgPreloadFS)");
	else
		return preloadVector[index].sizeKB;
}


string CfgPreloadFS::toString (){
	
	int i;
	std::ostringstream info;

		if (preloadVector.size() == 0)		
			info << "Preload list is empty!" << endl;

		else{
			info << "Printing preload file list from cfgPreloadFS class..." << endl;
			
			for (i=0; i<preloadVector.size(); i++){

				info << "  File [" << i << "]"  
					 << "  Name:" << preloadVector[i].fileName 
					 << "  Size:" << preloadVector[i].sizeKB 
					 << " KB" << endl;
			}
		}
		
	return info.str();
}

