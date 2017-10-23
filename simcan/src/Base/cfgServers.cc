#include "cfgServers.h"


CfgServers::~CfgServers (){
		
	if (serversVector.size() > 0)
		serversVector.clear();
}


CfgServers::CfgServers (){
}


void CfgServers::parseFile(const char* fileName){

	FILE *serversFD;
	char line [LINE_SIZE];
	int i;

	int readParameters;
	unsigned int numConnections;
	char hostName [LINE_SIZE];
	unsigned int port;
	unsigned int id;
	server_T currentServer;
	std::ostringstream info;


		// Init...
		bzero (line, LINE_SIZE);

		// Open the servers file
		serversFD = fopen (fileName, "r");

		// There is no servers file
		if (serversFD == NULL){
			info << "Error opening servers file:" << fileName;
			perror (info.str().c_str());
			opp_error (info.str().c_str());
			throw cRuntimeError(info.str().c_str());
		}


		// File not exists!
		else{

			// Read the number of entries
			fgets (line, LINE_SIZE, serversFD);
			numConnections = atoi (line);

			// Read all lines!
			for (i=0; i<numConnections; i++){

				// Load the current path entry
				if (fgets (line, LINE_SIZE, serversFD)==NULL){
					info << "Error reading file " << fileName << ". Line: " << line;
					throw cRuntimeError(info.str().c_str());
				}

				// Parse the data
				else{

					// Clear buffers
					bzero (hostName, LINE_SIZE);

					// Init
					readParameters = 0;

					// Parses the line!
					readParameters = sscanf (line, "%[^':\n']:%u:%u", hostName, &port, &id);

					// Well parsed!
					if (readParameters==3){
						currentServer.hostName = hostName;
						currentServer.port = port;
						currentServer.id = id;
						serversVector.push_back (currentServer);
					}

					// Error!
					else{
						info << "Error parsing file " << fileName << ". Line: " << line;
						throw cRuntimeError(info.str().c_str());
					}
				}
			}
		}
}


int CfgServers::getNumServers(){

	return (serversVector.size());
}


string CfgServers::getHostName (unsigned int index){

	if (index>=serversVector.size())
		throw cRuntimeError("Index out of bounds in getHostName (CfgServers)");
	else
		return serversVector[index].hostName;
}


unsigned int CfgServers::getPort (unsigned int index){

	if (index>=serversVector.size())
		throw cRuntimeError("Index out of bounds in getPort (CfgServers)");
	else
		return serversVector[index].port;
}


unsigned int CfgServers::getId (unsigned int index){

	if (index>=serversVector.size())
		throw cRuntimeError("Index out of bounds in getId (CfgServers)");
	else
		return serversVector[index].id;
}


string CfgServers::toString (){
	
	int i;
	std::ostringstream info;

		if (serversVector.size() == 0)
			info << "Server list is empty!" << endl;

		else{
			info << "Printing server list from cfgServer class!..." << endl;			

			for (i=0; i<serversVector.size(); i++){

				info << "  Server ["  << i << "]" 
					 << "  ID:"  << serversVector[i].id 
					 << "  HostName:"  << serversVector[i].hostName 
					 << "  Port:"  << serversVector[i].port 
					 << endl;
			}
		}
	return info.str();
}


