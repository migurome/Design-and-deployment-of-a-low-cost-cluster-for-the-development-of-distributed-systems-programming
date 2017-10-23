#include "SIMCAN_Message.h"

Register_Class(SIMCAN_Message);


TraceComponent& TraceComponent::operator=(const TraceComponent& other){
	moduleID = other.moduleID;
	gateID = other.gateID;
	reqSequence = other.reqSequence;
}


TraceComponent::TraceComponent(int newModule, int newGate) :
  moduleID(newModule),
  gateID(newGate){
}


TraceComponent::TraceComponent(int newModule, int newGate, reqNum_t newReq) :
  moduleID(newModule),
  gateID(newGate){
	reqSequence.push_front (newReq);
}


SIMCAN_Message::~SIMCAN_Message(){

	int node, module;

		// For each node...
		for (node=0; node<trace.size(); node++){

			// For each module
			for (module=0; module<trace[node].second.size(); module++){

				// Removes all lists belonging to current module
				trace[node].second[module].reqSequence.clear();
			}

			// Removes current node vector
			trace[node].second.clear();
		}

		// Removes completlety the trace!
		trace.clear();
}


SIMCAN_Message::SIMCAN_Message(const char *name, int kind): SIMCAN_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_Message");
	parentRequest = NULL;
}


SIMCAN_Message::SIMCAN_Message(const SIMCAN_Message& other): SIMCAN_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_Message");
	parentRequest = NULL;
}


SIMCAN_Message& SIMCAN_Message::operator=(const SIMCAN_Message& other){

	SIMCAN_Message_Base::operator=(other);

	return *this;
}


SIMCAN_Message *SIMCAN_Message::dup() const{

	//vector <pair <string, vector<TraceComponent> > >::iterator trace_it;
	SIMCAN_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_Message();

		// Base parameters...
		newMessage->setOperation (getOperation());
		newMessage->setIsResponse (getIsResponse());
		newMessage->setRemoteOperation (getRemoteOperation());
		newMessage->setConnectionId (getConnectionId());
		newMessage->setCommId (getCommId());
		newMessage->setSourceId (getSourceId());
		newMessage->setNextModuleIndex (getNextModuleIndex());
		newMessage->setResult (getResult());

		newMessage->setByteLength (getByteLength());
		newMessage->setParentRequest (getParentRequest());

		// Copy the control info, if exists!
		if (getControlInfo() != NULL){
			controlOld = check_and_cast<TCPCommand *>(getControlInfo());
			controlNew = new TCPCommand();
			controlNew = controlOld->dup();
			newMessage->setControlInfo (controlNew);
		}

		// Reserve memory to trace!
		newMessage->setTraceArraySize (getTraceArraySize());

		// Copy trace!
		for (i=0; i<trace.size(); i++){
			newMessage->addNodeTrace (trace[i].first, trace[i].second);
		}

	return (newMessage);
}


void SIMCAN_Message::setTraceArraySize(unsigned int size){
	trace.reserve (size);
}


unsigned int SIMCAN_Message::getTraceArraySize() const{
	return trace.size();
}


TraceComponent& SIMCAN_Message::getTrace(unsigned int k){
}


void SIMCAN_Message::setTrace(unsigned int k, const TraceComponent& trace_var){
}


void SIMCAN_Message::addModuleToTrace (int module, int gate, reqNum_t currentRequest){

	struct TraceComponent currentModule(module,gate);

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");

		currentModule.reqSequence.push_back(currentRequest);

		// Update current module trace
		trace.back().second.push_back(currentModule);
}


void SIMCAN_Message::removeLastModuleFromTrace (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");

		// Removes last module from trace!
		trace.back().second.pop_back();
}


void SIMCAN_Message::addNodeToTrace (string node){

	vector<TraceComponent> currentModuleTrace;

		// Add a new node trace
		trace.push_back (make_pair(node, currentModuleTrace));
}


void SIMCAN_Message::removeLastNodeFromTrace (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");

		// Removes last node trace
		trace.pop_back();
}


void SIMCAN_Message::addRequestToTrace (int request){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");

		// Add a request number to last module trace
		trace.back().second.back().reqSequence.push_back (request);
}


void SIMCAN_Message::removeLastRequestFromTrace (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");

		// Add a request number to last module trace
		trace.back().second.back().reqSequence.pop_back ();
}


void removeLastRequestFromTrace ();


int SIMCAN_Message::getLastGateId (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");


	// Update current module trace
	return trace.back().second.back().gateID;
}


int SIMCAN_Message::getLastModuleId (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");


	// Update current module trace
	return trace.back().second.back().moduleID;
}


int SIMCAN_Message::getCurrentRequest (){

		// Check if trace is empty!
		if (trace.empty())
			opp_error ("Trace is empty!");


	// Update current module trace
	return trace.back().second.back().reqSequence.back();
}


cMessage* SIMCAN_Message::getParentRequest () const{
	return parentRequest;
}


void SIMCAN_Message::setParentRequest (cMessage* parent){
	parentRequest = parent;
}


string SIMCAN_Message::traceToString (){

	std::ostringstream traceLine;
	int currentNodeTrace;
	int currentModuleTrace;
	int currentReqSeq;
	std::list <reqNum_t>::iterator listIterator;


		// Clear the string buffer
		traceLine.str("");

		// Walk through node traces
		for (currentNodeTrace=0; currentNodeTrace<trace.size(); currentNodeTrace++){

			// If not first element, add a separator!
			if (currentNodeTrace!=0)
				traceLine << "|";

			// Get current hostName
			traceLine << trace[currentNodeTrace].first << "@";

			// Walk through module traces
			for (currentModuleTrace=0; currentModuleTrace<trace[currentNodeTrace].second.size(); currentModuleTrace++){

				// If not first element, add a separator!
				if (currentModuleTrace!=0)
					traceLine << "/";

				// Add module ID
				traceLine << trace[currentNodeTrace].second[currentModuleTrace].moduleID << ",";

				// Add gate ID
				traceLine << trace[currentNodeTrace].second[currentModuleTrace].gateID << ":";

					// Walk through request list
					for(listIterator = trace[currentNodeTrace].second[currentModuleTrace].reqSequence.begin();
						listIterator != trace[currentNodeTrace].second[currentModuleTrace].reqSequence.end();
						++listIterator){

						// If not the first element, add a separator!
						if (listIterator!=trace[currentNodeTrace].second[currentModuleTrace].reqSequence.begin())
							traceLine << ".";

						traceLine << *listIterator;
					}
			}
		}

	return traceLine.str();
}


string SIMCAN_Message::contentsToString (bool printContents){

	std::ostringstream osStream;	
	TCPCommand *ind;

	if (printContents){

		// Get control info
		if (getControlInfo() != NULL)
			ind = check_and_cast<TCPCommand *>(getControlInfo());
		else
			ind = NULL;

		osStream  << "SIMCAN Message contents" << endl;

		osStream  << " - name:" <<  getName() << endl;
		osStream  << " - operation:" <<  operationToString(getOperation()) << endl;
		osStream  << " - length:" <<  getByteLength() << " bytes" << endl;
		osStream  << " - isResponse:" <<  getIsResponse_string() << endl;
		osStream  << " - remoteOperation:" <<  getRemoteOperation_string() << endl;
		osStream  << " - connectionId:" <<  getConnectionId() << endl;
		osStream  << " - communicationId:" <<  getCommId() << endl;
		osStream  << " - sourceProcess ID:" <<  getSourceId() << endl;
		osStream  << " - result:" <<  getResult() << endl;
		
		if (ind != NULL)
			osStream  << " - ControlInfo->connId:" <<  ind->getConnId() << endl;
		else
			osStream  << " - ControlInfo->connId: NULL" << endl;

		if (PRINT_SM_TRACE)		
			osStream  << " - trace:" <<  traceToString() << endl;
	}

	return osStream.str();
}


string SIMCAN_Message::operationToString (unsigned int operation){

	string result;

		if (operation == NULL_OPERATION)
			result = "Operation not set!";

		else if (operation == SM_OPEN_FILE)
			result = "SM_OPEN_FILE";

		else if (operation == SM_CLOSE_FILE)
			result = "SM_CLOSE_FILE";

		else if (operation == SM_READ_FILE)
			result = "SM_READ_FILE";

		else if (operation == SM_WRITE_FILE)
			result = "SM_WRITE_FILE";

		else if (operation == SM_CREATE_FILE)
			result = "SM_CREATE_FILE";

		else if (operation == SM_DELETE_FILE)
			result = "SM_DELETE_FILE";		
		
		else if (operation == SM_CPU_EXEC)
			result = "SM_CPU_EXEC";
		
		else if (operation == SM_CREATE_CONNECTION)
			result = "SM_CREATE_CONNECTION";
		
		else if (operation == SM_LISTEN_CONNECTION)
			result = "SM_LISTEN_CONNECTION";
		
		else if (operation == SM_SEND_DATA_NET)
			result = "SM_SEND_DATA_NET";			
			
		else if (operation == SM_MEM_ALLOCATE)
			result = "SM_MEM_ALLOCATE";
			
		else if (operation == SM_MEM_RELEASE)
			result = "SM_MEM_RELEASE";
		
		else if (operation == MPI_SEND)
		    result = "MPI_SEND";

		else if (operation == MPI_RECV)
		    result = "MPI_RECV";
		
		
		else if (operation == MPI_NULL)
			result = "MPI_NULL";

		else if (operation == MPI_ERROR)
			result = "MPI_ERROR";

		else if (operation == MPI_BARRIER)
			result = "MPI_BARRIER";

		else if (operation == MPI_BARRIER_ACK)
			result = "MPI_BARRIER_ACK";

		else if (operation == MPI_PROCESS)
			result = "MPI_PROCESS";

		else if (operation == MPI_METADATA)
			result = "MPI_METADATA";

		else if (operation == MPI_METADATA_ACK)
			result = "MPI_METADATA_ACK";

		else if (operation == MPI_DATA)
			result = "MPI_DATA";

		else if (operation == MPI_DATA_ACK)
			result = "MPI_DATA_ACK";

		else if (operation == MPI_RESULT)
			result = "MPI_RESULT";

		else if (operation == MPI_RESULT_ACK)
			result = "MPI_RESULT_ACK";

		else if (operation == MPI_BARRIER_ACK_AND_READ_DATA)
			result = "MPI_BARRIER_ACK_AND_READ_DATA";

		else if (operation == MPI_PROCESS_AND_SEND_DATA_ACK)
			result = "MPI_PROCESS_AND_SEND_DATA_ACK";

		else if (operation == MPI_SEND_RESULT_AND_READ_DATA)
				result = "MPI_SEND_RESULT_AND_READ_DATA";

		else if (operation == MPI_SEND_RESULT_ACK_AND_READ_DATA)
				result = "MPI_SEND_RESULT_ACK_AND_READ_DATA";

		else
			result = "Unknown operation!";

	return result;
}


string SIMCAN_Message::operationToString (){

	return operationToString(getOperation());
}


string SIMCAN_Message::getIsResponse_string (){
	
	string result;
	
		if (getIsResponse())
			result = "true";
		else
			result = "false";
	
	return result;
}


string SIMCAN_Message::getRemoteOperation_string (){
	
	string result;
	
		if (getRemoteOperation())
			result = "true";
		else
			result = "false";
	
	return result;
}


bool SIMCAN_Message::isTraceEmpty (){
	return (trace.empty());
}


bool SIMCAN_Message::fitWithParent (){

	bool fit;
	size_t dot, twoDots;

	SIMCAN_Message *parent;
	string childTrace;
	string parentTrace;


		// Init...
		fit = false;
		parent = check_and_cast <SIMCAN_Message*> (getParentRequest());

		childTrace = traceToString();
		parentTrace = parent->traceToString();

		// Search for last dot!
		dot = childTrace.find_last_of (".");
		twoDots = childTrace.find_last_of (":");

		// if found... remove last sequence number!
		if ((dot != string::npos) && (twoDots != string::npos)){

			// last dot behind ':'
			if (dot > twoDots){

				childTrace = childTrace.substr (0, dot);

				if (childTrace.compare (parentTrace) == 0)
					fit = true;
			}
		}

	return fit;
}


bool SIMCAN_Message::fitWithParent (cMessage *parentMsg){

	bool fit;
	size_t dot, twoDots;

	SIMCAN_Message *parent;
	string childTrace;
	string parentTrace;


		// Init...
		fit = false;
		parent = check_and_cast <SIMCAN_Message*> (parentMsg);

		childTrace = traceToString();
		parentTrace = parent->traceToString();

		// Search for last dot!
		dot = childTrace.find_last_of (".");
		twoDots = childTrace.find_last_of (":");

		// if found... remove last sequence number!
		if ((dot != string::npos) && (twoDots != string::npos)){

			// last dot behind ':'
			if (dot > twoDots){

				childTrace = childTrace.substr (0, dot);

				if (childTrace.compare (parentTrace) == 0)
					fit = true;
			}
		}

	return fit;
}


void SIMCAN_Message::addNodeTrace (string host, vector<TraceComponent> nodeTrace){

	vector<TraceComponent>::iterator component_it;
	std::list <reqNum_t>::iterator sequence_it;
	struct TraceComponent currentComponent (0,0);
	vector<TraceComponent> currentNodeTrace;


		// Copy current node trace
		for (component_it=nodeTrace.begin(); component_it!=nodeTrace.end(); component_it++){

			currentComponent.moduleID = (*component_it).moduleID;
			currentComponent.gateID = (*component_it).gateID;
			currentComponent.reqSequence.clear();

			// Copy the sequence list
			for (sequence_it=(*component_it).reqSequence.begin();
				 sequence_it!=(*component_it).reqSequence.end();
				 sequence_it++){

					currentComponent.reqSequence.push_back (*sequence_it);
			}

			// Add current module to node trace
			currentNodeTrace.push_back (currentComponent);
		}

		// Add a new node trace
		trace.push_back (make_pair(host, currentNodeTrace));
}


string SIMCAN_Message::getHostName (int k){
	return trace[k].first;
}


vector<TraceComponent> SIMCAN_Message::getNodeTrace (int k){
	return trace[k].second;
}


void SIMCAN_Message::parsimPack(cCommBuffer *b){

	int numNodes;
	int numModules;
	int numRequests;
	char hostname [NAME_SIZE];
	int currentNode, currentModule;
	unsigned long parentPointer;
	std::list <reqNum_t>::iterator listIterator;


		// Pack SIMCAN_Message_Base
		SIMCAN_Message_Base::parsimPack(b);

		// Pack parent message
		memcpy (&parentPointer, &parentRequest, 4);
		b->pack (parentPointer);

		// Get and pack the number of nodes...
		numNodes = trace.size();
		b->pack (numNodes);

		if (DEBUG_PACKING) printf ("packing numNodes = %d\n", numNodes);

		// For each node...
		for (currentNode=0; currentNode<numNodes; currentNode++){

			// Pack the hostname!
			b->pack (trace[currentNode].first.c_str());

			if (DEBUG_PACKING) printf ("packing hostName [%d] = %s\n", currentNode, trace[currentNode].first.c_str());

			// Get and pack the number of modules in current node
			numModules = trace[currentNode].second.size();
			b->pack (numModules);

			if (DEBUG_PACKING) printf ("packing numModules = %d\n", numModules);

			// For each module...
			for (currentModule=0; currentModule<numModules; currentModule++){

				// Pack the moduleID and gateID
				b->pack (trace[currentNode].second[currentModule].moduleID);
				b->pack (trace[currentNode].second[currentModule].gateID);

				if (DEBUG_PACKING) printf ("unpacking moduleID = %d   gateID = %d\n",
											trace[currentNode].second[currentModule].moduleID,
											trace[currentNode].second[currentModule].gateID);

				// Get and pack the number of requests
				numRequests = trace[currentNode].second[currentModule].reqSequence.size();
				b->pack (numRequests);

				if (DEBUG_PACKING) printf ("packing numRequests = %d\n", numRequests);

				// For each request...
				for(listIterator = trace[currentNode].second[currentModule].reqSequence.begin();
					listIterator != trace[currentNode].second[currentModule].reqSequence.end();
					++listIterator){

						b->pack (*listIterator);
						if (DEBUG_PACKING) printf ("packing request = %d\n", *listIterator);
				}
			}
		}
}


void SIMCAN_Message::parsimUnpack(cCommBuffer *b){

	int numNodes;
	int numModules;
	int numRequests;
	char* hostname;
	int moduleID;
	int gateID;
	reqNum_t req;
	string nodeName;
	unsigned long parentPointer;

	int &numNodesRef = numNodes;
	int &numModulesRef = numModules;
	int &numRequestsRef = numRequests;
	char *&hostnameRef = hostname;
	int &moduleIDRef = moduleID;
	int &gateIDRef = gateID;
	reqNum_t &reqRef = req;
	unsigned long &parentPointerRef = parentPointer;

	int currentNode, currentModule, currentRequest;
	std::list <reqNum_t>::iterator listIterator;


		// Unpack SIMCAN_Message_Base
		SIMCAN_Message_Base::parsimUnpack(b);

		// Unpack parent message
		b->unpack (parentPointerRef);
		memcpy (&parentRequest, &parentPointer, 4);

		// Init...
		hostname = (char*) malloc (NAME_SIZE);

		// unpack the number of nodes...
		b->unpack (numNodesRef);

		if (DEBUG_PACKING) printf ("unpacking numNodes = %d\n", numNodes);

		// For each node...
		for (currentNode=0; currentNode<numNodes; currentNode++){

			// Unpack the hostname!
			bzero (hostname, NAME_SIZE);
			b->unpack (hostnameRef);

			// Add a new node!
			nodeName = hostname;
			addNodeToTrace (nodeName);

			if (DEBUG_PACKING) printf ("unpacking hostName [%d] = %s\n", currentNode, nodeName.c_str());

			// Gunpack the number of modules in current node
			b->unpack (numModulesRef);

			if (DEBUG_PACKING) printf ("unpacking numModules = %d\n", numModules);

			// For each module...
			for (currentModule=0; currentModule<numModules; currentModule++){

				// unpack the moduleID and gateID
				b->unpack (moduleIDRef);
				b->unpack (gateIDRef);

				if (DEBUG_PACKING) printf ("unpacking moduleID = %d   gateID = %d\n", moduleID, gateID);

				// Get and pack the number of requests
				b->unpack (numRequestsRef);

				if (DEBUG_PACKING) printf ("unpacking numRequests = %d\n", numRequests);

				// For each request...
				for (currentRequest=0; currentRequest<numRequests; currentRequest++){

					// unpack current request
					b->unpack (reqRef);

					if (DEBUG_PACKING) printf ("unpacking request = %d\n", req);

					// add a new module
					if (currentRequest == 0){
						addModuleToTrace (moduleID, gateID, req);
					}

					// add a new request
					else{
						addRequestToTrace (req);
					}
				}
			}
		}
}




