#include "SIMCAN_App_MEM_Message.h"
#include "SIMCAN_App_IO_Message.h"


Register_Class (SIMCAN_App_MEM_Message);



SIMCAN_App_MEM_Message::~SIMCAN_App_MEM_Message(){
}


SIMCAN_App_MEM_Message::SIMCAN_App_MEM_Message(const char *name, int kind): SIMCAN_App_MEM_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_MEM_Message");
}


SIMCAN_App_MEM_Message::SIMCAN_App_MEM_Message(const SIMCAN_App_MEM_Message& other): SIMCAN_App_MEM_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_MEM_Message");
}


SIMCAN_App_MEM_Message& SIMCAN_App_MEM_Message::operator=(const SIMCAN_App_MEM_Message& other){

	SIMCAN_App_MEM_Message_Base::operator=(other);

	return *this;
}


SIMCAN_App_MEM_Message *SIMCAN_App_MEM_Message::dup() const{

	SIMCAN_App_MEM_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_App_MEM_Message();

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

		// SIMCAN_App_MEM_Message parameters...
		newMessage->setMemSize (getMemSize());
		newMessage->setRegion (getRegion());
		
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


SIMCAN_Message *SIMCAN_App_MEM_Message::dupGeneric() const{

	SIMCAN_App_IO_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_App_IO_Message();

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


void SIMCAN_App_MEM_Message::updateLength (){
	
		// Set the new size!
		setByteLength (MSG_INITIAL_LENGTH);
}


string SIMCAN_App_MEM_Message::contentsToString (bool printContents){

	std::ostringstream osStream;

		if (printContents){

			osStream << SIMCAN_Message::contentsToString(printContents);
			osStream  << " - Requested mem size:" <<  getMemSize() << endl;
			osStream  << " - Memory region:" <<  regionToString(getRegion()) << endl;
		}

	return osStream.str();
}


string SIMCAN_App_MEM_Message::regionToString (int region){
	
	string result;

		if (region == SM_MEMORY_REGION_CODE)
			result = "SM_MEMORY_REGION_CODE";

		else if (region == SM_MEMORY_REGION_LOCAL_VAR)
			result = "SM_MEMORY_REGION_LOCAL_VAR";

		else if (region == SM_MEMORY_REGION_GLOBAL_VAR)		
			result = "SM_MEMORY_REGION_GLOBAL_VAR";
						
		else if (region == SM_MEMORY_REGION_DYNAMIC_VAR)
			result = "SM_MEMORY_REGION_DYNAMIC_VAR";
						
		else 
			result = "REGION not found!";
			
	return result;
}


void SIMCAN_App_MEM_Message::parsimPack(cCommBuffer *b){

	SIMCAN_App_MEM_Message_Base::parsimPack(b);
}


void SIMCAN_App_MEM_Message::parsimUnpack(cCommBuffer *b){

	SIMCAN_App_MEM_Message_Base::parsimUnpack(b);
}



