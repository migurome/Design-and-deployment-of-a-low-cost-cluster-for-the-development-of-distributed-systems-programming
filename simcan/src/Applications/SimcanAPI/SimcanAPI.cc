#include "SimcanAPI.h"


SimcanAPI::~SimcanAPI(){
	
	connections.clear();
}


void SimcanAPI::initialize(){
	
	// Init the super-class
	cSIMCAN_Base::initialize();

        // Is a distributed app?
        isDistributedApp = par ("isDistributedApp");

        // Test ID
        const char *newTestID = par ("testID");
        testID = newTestID;

        // Start and connection delays
        startDelay = par ("startDelay");
        connectionDelay = par ("connectionDelay");

        // App instance name
        const char *newAppInstance = par ("appInstance");
        appInstance = newAppInstance;


        // Get the OS gates
        fromOSGate = gate ("fromOS");
        toOSGate = gate ("toOS");
}


void SimcanAPI::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* SimcanAPI::getOutGate (cMessage *msg){

	// If msg arrive from IOR
	if (msg->getArrivalGate()==fromOSGate){
		if (toOSGate->getNextGate()->isConnected()){
			return (toOSGate);
		}
	}

	// If gate not found!
	return NULL;
}



// ---------------------------------- I/O ---------------------------------- //
						
void SimcanAPI::SIMCAN_request_open (const char* fileName){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!	
	
		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();		
		sm_io->setOperation (SM_OPEN_FILE);			
		
		// Set the corresponding parameters
		sm_io->setFileName(fileName);		
		
		// Update message length
		sm_io->updateLength ();		
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Open file %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);		
}
		
		
void SimcanAPI::SIMCAN_request_close (const char* fileName){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!
	
	
		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();
		sm_io->setOperation (SM_CLOSE_FILE);
				
		// Set the corresponding parameters
		sm_io->setFileName(fileName);		
		
		// Update message length
		sm_io->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Close file %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);
}
		
		
void SimcanAPI::SIMCAN_request_read (const char* fileName, unsigned int offset, unsigned int size){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!

		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();
		sm_io->setOperation (SM_READ_FILE);		
		
		// Set the corresponding parameters
		sm_io->setFileName(fileName);
		sm_io->setOffset (offset);
		sm_io->setSize (size);
		
		// Update message length
		sm_io->updateLength ();
		

		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Read data %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);
}
		
		
void SimcanAPI::SIMCAN_request_write (const char* fileName, unsigned int offset, unsigned int size){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!
	
	
		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();
		sm_io->setOperation (SM_WRITE_FILE);
		
		// Set the corresponding parameters
		sm_io->setFileName(fileName);
		sm_io->setOffset (offset);
		sm_io->setSize (size);

		// Update message length
		sm_io->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Write data %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());

		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);
}
	
		
void SimcanAPI::SIMCAN_request_create (const char* fileName){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!
	
	
		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();
		sm_io->setOperation (SM_CREATE_FILE);		
		
		// Set the corresponding parameters
		sm_io->setFileName(fileName);		
		
		// Update message length
		sm_io->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Create file %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);
}
		
		
void SimcanAPI::SIMCAN_request_delete (const char* fileName){
	
	SIMCAN_App_IO_Message *sm_io;		// Request message!
	
	
		// Creates the message
		sm_io = new SIMCAN_App_IO_Message ();
		sm_io->setOperation (SM_DELETE_FILE);		
		
		// Set the corresponding parameters
		sm_io->setFileName(fileName);
		
		// Update message length
		sm_io->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Delete file %s", sm_io->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_io, toOSGate);
}



// ----------------- CPU calls ----------------- //

void SimcanAPI::SIMCAN_request_cpu (unsigned int MIs){
	
	SIMCAN_App_CPU_Message *sm_cpu;		// Request message!
	
	
		// Creates the message
		sm_cpu = new SIMCAN_App_CPU_Message ();
		sm_cpu->setOperation (SM_CPU_EXEC);
		
		// Set the corresponding parameters
		sm_cpu->setTotalMIs(MIs);
		sm_cpu->setRemainingMIs(MIs);
		sm_cpu->setCpuTime(0.000000);

		// Update message length
		sm_cpu->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Computing (CPU) %s", sm_cpu->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_cpu, toOSGate);
}

void SimcanAPI::SIMCAN_request_cpuTime (simtime_t cpuTime){
	
	SIMCAN_App_CPU_Message *sm_cpu;		// Request message!
	
	
		// Creates the message
		sm_cpu = new SIMCAN_App_CPU_Message ();
		sm_cpu->setOperation (SM_CPU_EXEC);
		
		// Set the corresponding parameters
		sm_cpu->setTotalMIs(0);
		sm_cpu->setRemainingMIs(0);
		sm_cpu->setCpuTime (cpuTime);
		
		// Update message length
		sm_cpu->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Computing (CPU) %s", sm_cpu->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_cpu, toOSGate);
}


// ----------------- Memory calls ----------------- //


void SimcanAPI::SIMCAN_request_allocMemory (unsigned int memorySize, unsigned int region){
	
	SIMCAN_App_MEM_Message *sm_mem;		// Request message!
	
	
		// Creates the message
		sm_mem = new SIMCAN_App_MEM_Message ();
		sm_mem->setOperation (SM_MEM_ALLOCATE);
		
		// Set the corresponding parameters
		sm_mem->setMemSize(memorySize);
		sm_mem->setRegion(region);				
		
		// Update message length
		sm_mem->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Allocating memory (MEM) %s", sm_mem->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_mem, toOSGate);	
}

void SimcanAPI::SIMCAN_request_freeMemory (unsigned int memorySize, unsigned int region){
	
	SIMCAN_App_MEM_Message *sm_mem;		// Request message!
	
	
		// Creates the message
		sm_mem = new SIMCAN_App_MEM_Message ();
		sm_mem->setOperation (SM_MEM_RELEASE);
		
		// Set the corresponding parameters
		sm_mem->setMemSize(memorySize);
		sm_mem->setRegion(region);				
		
		// Update message length
		sm_mem->updateLength ();
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Allocating memory (MEM) %s", sm_mem->contentsToString(DEBUG_MSG_AppSystem).c_str());
		
		// Send the request to the Operating System
		sendRequestMessage (sm_mem, toOSGate);	
}




// ----------------- Network calls ----------------- // 

void SimcanAPI::SIMCAN_request_createConnection (string destAddress,
                                                  int destPort,
                                                  int id){


	struct SIMCAN_App_Connector newConnection;		// Connection struct
	SIMCAN_App_NET_Message *sm_net;					// Request message!
		
		// Configure the new connection		
		newConnection.destAddress = destAddress;		
		newConnection.destPort = destPort;				
		newConnection.localAddress = "Address not set yet!";		
		newConnection.localPort = 0;				
		newConnection.id = id;
		newConnection.connectionId = EMPTY;
		
		// Creates the message
		sm_net = new SIMCAN_App_NET_Message ();
		sm_net->setOperation (SM_CREATE_CONNECTION);

		// Set parameters
		sm_net->setDestinationIP (destAddress.c_str());
		sm_net->setDestinationPort (destPort);
		sm_net->setCommId (id);

		// Add the connection to the connection vector
		connections.push_back (newConnection);	
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Creating connection %s", sm_net->contentsToString(DEBUG_MSG_AppSystem).c_str());
						 			
		
	// Send the request message to Network Service module
	sendRequestMessage (sm_net, toOSGate);		
}


void SimcanAPI::SIMCAN_request_createListenConnection (int localPort){

	struct SIMCAN_App_Connector newConnection;		// Connection struct
	SIMCAN_App_NET_Message *sm_net;					// Request message!


		// Configure the new connection
		newConnection.destAddress = "Address not set yet!";
		newConnection.destPort = 0;
		newConnection.localAddress = "Address not set yet!";
		newConnection.localPort = localPort;
		newConnection.connectionId = EMPTY;

		// Creates the message
		sm_net = new SIMCAN_App_NET_Message ();
		sm_net->setOperation (SM_LISTEN_CONNECTION);				
		
		// Set parameters		
		sm_net->setLocalPort (localPort);
		sm_net->setNextModuleIndex (getParentModule()->getIndex());		

		// Add the connection to the connection vector
		connections.push_back (newConnection);
		
		if (DEBUG_AppSystem)
			showDebugMessage ("[SimcanAPI] Listen connection %s", sm_net->contentsToString(DEBUG_MSG_AppSystem).c_str());
								 							 			
		
	// Send the request message to Network Service module
	sendRequestMessage (sm_net, toOSGate);		
}


void SimcanAPI::SIMCAN_request_sendDataToNetwork (SIMCAN_Message *sm, int id){
	
	int index;
	int connectionId;
	TCPSendCommand *cmd;
	
		
		// Search connection...
		index = searchConnectionById (id);
		
		// Error?
		if (index == NOT_FOUND)
			showErrorMessage ("Connection with ID=%d Not found", id);
		
		else{
			sm->setConnectionId (connections[index].connectionId);

			if (DEBUG_AppSystem)
				showDebugMessage ("[SimcanAPI] Send data to server %s", sm->contentsToString(DEBUG_MSG_AppSystem).c_str());
						
			// Send to OS
			sendRequestMessage (sm, toOSGate);
		}
}


void SimcanAPI::setEstablishedConnection (SIMCAN_Message *sm){
	
	int index;
	int connectionId;
	SIMCAN_App_NET_Message *sm_net;
	
	
		// Cast
		sm_net = check_and_cast<SIMCAN_App_NET_Message *>(sm);
	
		// Get result!
		connectionId = sm_net->getConnectionId();
		
		// Error?
		//if (connectionId < 0)
		//	showErrorMessage ("Error creating connection: %s", sm_net->contentsToString(true).c_str());
		//else{
									
			index = searchConnectionById (sm_net->getCommId());
			
			// Error?
			if (index == NOT_FOUND)
				showErrorMessage ("Server with ID=%d Not found", sm_net->getConnectionId());
			else{
				connections[index].connectionId = connectionId;
				connections[index].localAddress = sm_net->getLocalIP();
				connections[index].localPort = sm_net->getLocalPort();			
			}				
				
			if (DEBUG_AppSystem)
				showDebugMessage ("[SimcanAPI] Conn. established: local IP:%s - dest IP:%s - local Port:%d - dest. Port:%d - Id:%d - connId:%d",
								connections[index].localAddress.c_str(),
								connections[index].destAddress.c_str(),
							  	connections[index].localPort,
							 	connections[index].destPort,
							 	connections[index].id, 
							 	connections[index].connectionId);			
		//}
}
	

string SimcanAPI::getLocalIP (){
	return appLocalIP;
}
		
		
void SimcanAPI::setLocalIP (string newIP){
	appLocalIP = newIP;
}


int SimcanAPI::getLocalPort (){
	return appLocalPort;
}


void SimcanAPI::setLocalPort (int newPort){
	appLocalPort = newPort;
}






int SimcanAPI::searchConnectionById (int id){

	int index;
	bool found;

		// Init...
		found = false;
		index = 0;

		// Searching...
		while ((!found) && (index<connections.size())){
			if (id == connections[index].id)
				found = true;
			else
				index++;
		}


	// Return the result...
	if (found)
		return index;
	else
		return NOT_FOUND;
}
	

string SimcanAPI::connectionsToString (bool printConnections){

	std::ostringstream osStream;
	int i;

		osStream.str("");

		// Connection list enable?
		if (printConnections){

			osStream << "Communication list..." << endl;

			for (i=0; i<connections.size(); i++){
				osStream << "  Connection[" << i << "] localAddress:" << connections[i].localAddress <<
							"  destinationAddress:" << connections[i].destAddress << endl <<
							"  localPort:" << connections[i].localPort <<
							"  destinationPort:" << connections[i].destPort << endl <<
							"  serverID:" << connections[i].id << 
							"  connecDescriptor:" << connections[i].connectionId << endl << endl;
			}
		}

	return osStream.str();
}
