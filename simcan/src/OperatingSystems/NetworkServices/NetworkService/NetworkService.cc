#include "NetworkService.h"

Define_Module (NetworkService);



NetworkService::~NetworkService(){
}


void NetworkService::initialize(){

	std::ostringstream osStream;

    	// Set the moduleIdName
		osStream << "NetworkService." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();	    
	    
	    // This IP must be left empty
	    localIP = "";
	    
	    // Module gates
	    fromServiceRedirectorGate = gate ("fromServiceRedirector");	    
	    fromNetTCPGate = gate ("fromNetTCP");
	    toServiceRedirectorGate = gate ("toServiceRedirector");    
	    toNetTCPGate = gate ("toNetTCP");	    
	    
	    // Service objects
	    clientTCP_Services = new TCP_ClientSideService (localIP, toNetTCPGate, this);
	    serverTCP_Services = new TCP_ServerSideService (localIP, toNetTCPGate, toServiceRedirectorGate, this);
}


void NetworkService::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


void NetworkService::handleMessage(cMessage *msg){

	SIMCAN_Message *sm;			
	cPacket *packet;

		// If msg is a Self Message...
		if (msg->isSelfMessage())
			processSelfMessage (msg);

		// Not a self message...
		else{			
			
			// Established connection message...
			if (!strcmp (msg->getName(), "ESTABLISHED")){
				receivedEstablishedConnection (msg);
			}
			
			// Not an ESTABLISHED message message...
			else{
										
				// Cast!
				sm = check_and_cast<SIMCAN_Message *>(msg);

				// Request!
				if (!sm->getIsResponse()){

					// Update message trace
					updateMessageTrace (sm);

					// Insert into queue
					queue.insert (sm);

					// If not processing any request...
					processCurrentRequestMessage ();
				}

				// Response message!
				else
					processResponseMessage (sm);
			}
		}	
}


cGate* NetworkService::getOutGate (cMessage *msg){
	
	
		// If msg arrive from Service Redirector
		if (msg->getArrivalGate()==fromServiceRedirectorGate){
			if (gate("toServiceRedirector")->getNextGate()->isConnected()){
				return (toServiceRedirectorGate);
			}
		}
		
		// If msg arrive from Service Redirector
		else if (msg->getArrivalGate()==fromNetTCPGate){
			if (gate("toNetTCP")->getNextGate()->isConnected()){
				return (toNetTCPGate);
			}
		}	
		

	// If gate not found!
	return NULL;
}


void NetworkService::processSelfMessage (cMessage *msg){
}


void NetworkService::processRequestMessage (SIMCAN_Message *sm){
	
	TCPSocket *socket;
	
	
		// Msg cames from Service Redirector...
		if (sm->getArrivalGate() == fromServiceRedirectorGate){
			
			if (DEBUG_Network_Service)
				showDebugMessage ("[processRequestMessage] from Service Redirector. %s",sm->contentsToString(DEBUG_MSG_Network_Service).c_str());
			
			
			// Create a new connection... client-side
			if (sm->getOperation() == SM_CREATE_CONNECTION){				
				clientTCP_Services->createConnection (sm);			
			}		
			
			// Create a listen connection... server-side
			else if (sm->getOperation() == SM_LISTEN_CONNECTION){
				serverTCP_Services->newListenConnection (sm);
			}
			
			// Send data...
			else if ((sm->getOperation () == SM_OPEN_FILE)   ||
					 (sm->getOperation () == SM_CLOSE_FILE)  ||
					 (sm->getOperation () == SM_READ_FILE)   ||
					 (sm->getOperation () == SM_WRITE_FILE)  ||
					 (sm->getOperation () == SM_CREATE_FILE) ||
					 (sm->getOperation () == SM_DELETE_FILE) ||
					 (sm->getOperation () == SM_SEND_DATA_NET)){				
				
				clientTCP_Services->sendPacketToServer (sm);
			}
			
			// MPI Calls			
			else if ((sm->getOperation () == MPI_SEND) ||
					 (sm->getOperation () == MPI_RECV) ||				 
					 (sm->getOperation () == MPI_BARRIER_UP)   ||
					 (sm->getOperation () == MPI_BARRIER_DOWN) ||
					 (sm->getOperation () == MPI_BCAST)   ||
					 (sm->getOperation () == MPI_SCATTER) ||
					 (sm->getOperation () == MPI_GATHER)){
				
				clientTCP_Services->sendPacketToServer (sm);
			}
			
			// Wrong operation...
			else{
				showErrorMessage ("Wrong request operation... %s", sm->contentsToString(true).c_str());
			}			
		}		
		
		// Msg cames from TCP Network
		else if (sm->getArrivalGate() == fromNetTCPGate){
			
			if (DEBUG_Network_Service)
				showDebugMessage ("[processRequestMessage] from TCP Network. %s", sm->contentsToString(DEBUG_MSG_Network_Service).c_str());

			// Seach the involved socket... server...
			socket = serverTCP_Services->getInvolvedSocket (sm);
			
			// Receiving data...
			if (socket != NULL){
				socket->processMessage(sm);				
			}

			// No socket found!
			else				
				showErrorMessage ("[processRequestMessage] No socket found!. %s",sm->contentsToString(true).c_str());							
		}
		
}


void NetworkService::processResponseMessage (SIMCAN_Message *sm){

	TCPSocket *socket;
		
	
		// Msg cames from Service Redirector...
		if (sm->getArrivalGate() == fromServiceRedirectorGate){
			
			if (DEBUG_Network_Service)
				showDebugMessage ("[processResponseMessage] from Service Redirector. %s",sm->contentsToString(DEBUG_MSG_Network_Service).c_str());						
			
			socket = serverTCP_Services->getInvolvedSocket (sm);
			
			// Sending data to corresponding client...
			if (socket != NULL){
				serverTCP_Services->sendPacketToClient(sm);
			}

			// Not socket found!
			else{				
				showErrorMessage ("[processResponseMessage] Socket not found... %s", sm->contentsToString(true).c_str());
			}		
		}

		// Msg cames from TCP Network
		else if (sm->getArrivalGate() == fromNetTCPGate){
			
			if (DEBUG_Network_Service)
				showDebugMessage ("[processResponseMessage] from TCP Network. %s",sm->contentsToString(DEBUG_MSG_Network_Service).c_str());
			
			socket = clientTCP_Services->getInvolvedSocket (sm);
						
			// Sending data to corresponding application...
			if (socket != NULL){
				socket->processMessage(sm);
			}

			// Not socket found!
			else{				
				showErrorMessage ("[processResponseMessage] Socket not found... %s", sm->contentsToString(true).c_str());
			}		
		}	
}


void NetworkService::receivedEstablishedConnection (cMessage *msg){
	
	TCPSocket *socket;
	
		socket = clientTCP_Services->getInvolvedSocket (msg);
			
		// Establishing connection... (client)
		if (socket != NULL)
			socket->processMessage(msg);
			
		// Establishing connection... (server)	
		else		
			serverTCP_Services->arrivesIncommingConnection(msg);		
}


