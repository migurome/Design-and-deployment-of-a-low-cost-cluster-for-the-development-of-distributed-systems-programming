#include "cfgMPI.h"


CfgMPI::~CfgMPI(){
	if (processVector.size() > 0)
		processVector.clear();	

	if (rootVector.size() > 0)
	    rootVector.clear();
}

CfgMPI::CfgMPI (){
	processVector.clear();
	rootVector.clear();
}


void CfgMPI::parseFile(const char* fileName){

	FILE* machineFile_fd;
	char line [LINE_SIZE];
	int parsedParameters, parsedParametersRoot;
	int i, lineNumber;
	std::ostringstream info;
	cfgMPIProcess newProcess;
	cfgMPIRootProcess newProcessRoot;

	unsigned int numProcess;
	char hostNameValue [LINE_SIZE];
	char appInstanceValue [LINE_SIZE];
	char rootValue [LINE_SIZE];
	unsigned int portValue;
	unsigned int rankValue;
	

		// Init...
		bzero (line, LINE_SIZE);
		lineNumber = 1;
		parsedParameters = parsedParametersRoot = 0;

		// Open the servers file
		machineFile_fd = fopen (fileName, "r");

		// There is no servers file
		if (machineFile_fd == NULL){
			info << "Error opening MPI Trace Player config file:" << fileName;
			perror (info.str().c_str());
			opp_error (info.str().c_str());
			throw cRuntimeError(info.str().c_str());
		}

		// Read the number of entries
		fgets (line, LINE_SIZE, machineFile_fd);
	    numProcess = atoi (line);

		// Read all lines!
		for (i=0; i<numProcess; i++){

			// Load the current path entry
			if (fgets (line, LINE_SIZE, machineFile_fd)==NULL){
				info << "Error reading file " << fileName << ". Line: " << line;
				throw cRuntimeError(info.str().c_str());
			}

			// Parse the data
			else{

				// Clear buffers
				bzero (hostNameValue, LINE_SIZE);
				bzero (appInstanceValue, LINE_SIZE);
				bzero (rootValue, LINE_SIZE);

				// Parses the line!
				parsedParameters = sscanf (line, "%[^':\n']:%[^':\n']:%u:%u",
				                            appInstanceValue,
											hostNameValue,
											&portValue,
											&rankValue);

				// Parses the line!
                parsedParametersRoot = sscanf (line, "%[^':\n']:%[^':\n']:%u:%u:%[^':\n']",
                                            appInstanceValue,
                                            hostNameValue,
                                            &portValue,
                                            &rankValue,
                                            rootValue);

                // Current process is Root
                if ((parsedParametersRoot == 5) && (strcmp(rootValue,"ROOT") == 0)){

                    // Add current process
                    newProcess.appInstance = appInstanceValue;
                    newProcess.hostName = hostNameValue;
                    newProcess.port = portValue;
                    newProcess.rank = rankValue;
                    processVector.push_back (newProcess);

                    // Add root info!
                    newProcessRoot.appInstance = appInstanceValue;
                    newProcessRoot.rootIndex = i;
                    rootVector.push_back (newProcessRoot);
                }

                // Current process is not Root
                else if (parsedParameters == 4){
                    newProcess.appInstance = appInstanceValue;
                    newProcess.hostName = hostNameValue;
                    newProcess.port = portValue;
                    newProcess.rank = rankValue;
                    processVector.push_back (newProcess);
                }

				// Error!
				else{
					info << "Error parsing file " << fileName << ". Line: " << line;
					throw cRuntimeError(info.str().c_str());
				}
	    	}
		}

		//printf (" %s\n", toString().c_str());
}

string CfgMPI::getAppInstanceByIndex (int index){

    if (index>=processVector.size())
        throw cRuntimeError("Index out of bounds in getId (getAppInstance)");
    else
        return processVector[index].appInstance;
}

string CfgMPI::getHostNameByIndex (int index){

	if (index>=processVector.size())
		throw cRuntimeError("Index out of bounds in getId (getHostName)");
	else
		return processVector[index].hostName;
}

unsigned int CfgMPI::getPortByIndex (int index){

	if (index>=processVector.size())
		throw cRuntimeError("Index out of bounds in getId (getPort)");
	else
		return processVector[index].port;
}

unsigned int CfgMPI::getRankByIndex (int index){

	if (index>=processVector.size())
		throw cRuntimeError("Index out of bounds in getRank (getRank)");
	else
		return processVector[index].rank;
}

unsigned int CfgMPI::getTotalNumProcesses (){
	
	return (processVector.size());	
}

unsigned int CfgMPI::getRootProcessByApp (string appInstance){

    return processVector[getIndexOfRootProcess(appInstance)].rank;
}

unsigned int CfgMPI::getNumProcessesByApp (string appInstance){

    int i, numProcesses;

        // Init...
        numProcesses = 0;

        for (i=0; i<processVector.size(); i++){

            if (appInstance == processVector[i].appInstance)
                numProcesses++;
        }

    return numProcesses;
}

string CfgMPI::toString(){
	
	int i;
	std::ostringstream info;

		if (processVector.size() == 0)		
			info << "Machine file empty!" << endl;

		else{
			info << "Printing MPI environment (CfgMPI class): Number of processes:" << processVector.size() << endl;
			
			for (i=0; i<processVector.size(); i++){

				info << "  App instance:" << processVector[i].appInstance
					 << "  HostName:" << processVector[i].hostName 
					 << "  Port:" << processVector[i].port 
					 << "  Rank:" << processVector[i].rank
					 << "  Root: " << getRootProcessByApp(processVector[i].appInstance)
					 << endl;
			}
		}
		
	return info.str();
	
}

unsigned int CfgMPI::getIndexOfRootProcess (string appInstance){

    bool found;
    int i;

        // Init...
        found = false;
        i = 0;

        // Search in the root vector
        while ((!found) && (i<rootVector.size())){

            // Match?
            if (appInstance == rootVector.at(i).appInstance){
                found = true;
            }
            else
                i++;
        }

        // If not found... throw an exception!
        if (!found)
            throw cRuntimeError("Index out of bounds in getRank (getRank)");

        return rootVector.at(i).rootIndex;
}
