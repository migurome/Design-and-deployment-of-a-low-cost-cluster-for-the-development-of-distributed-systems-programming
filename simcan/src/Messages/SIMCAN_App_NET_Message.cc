#include "SIMCAN_App_NET_Message.h"

Register_Class (SIMCAN_App_NET_Message);



SIMCAN_App_NET_Message::~SIMCAN_App_NET_Message(){
}


SIMCAN_App_NET_Message::SIMCAN_App_NET_Message(const char *name, int kind): SIMCAN_App_NET_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_NET_Message");
}


SIMCAN_App_NET_Message::SIMCAN_App_NET_Message(const SIMCAN_App_NET_Message& other): SIMCAN_App_NET_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_NET_Message");
}


SIMCAN_App_NET_Message& SIMCAN_App_NET_Message::operator=(const SIMCAN_App_NET_Message& other){

	SIMCAN_App_NET_Message_Base::operator=(other);

	return *this;
}


SIMCAN_App_NET_Message *SIMCAN_App_NET_Message::dup() const{

	SIMCAN_App_NET_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_App_NET_Message();

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

		// SIMCAN_App_NET_Message parameters...
		newMessage->setDestinationIP (getDestinationIP());
		newMessage->setDestinationPort (getDestinationPort());		
		newMessage->setLocalIP (getLocalIP());
		newMessage->setLocalPort (getLocalPort());		
		newMessage->setSize (getSize());		
		newMessage->setData (getData());		

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


void SIMCAN_App_NET_Message::updateLength (){
	
	unsigned long int newSize;

	// Set the new size!
	newSize = MSG_INITIAL_LENGTH;
	if (!getIsResponse() ) {
		if (getOperation() == SM_SEND_DATA_NET) {
			newSize = MSG_INITIAL_LENGTH + getSize();
		} 
	}

	setByteLength (newSize);
}


string SIMCAN_App_NET_Message::contentsToString (bool printContents){

	std::ostringstream osStream;

		if (printContents){

			osStream << SIMCAN_Message::contentsToString(printContents);
	
			osStream  << " - Local IP:" <<  getLocalIP() << endl;
			osStream  << " - Destination IP:" <<  getDestinationIP() << endl;
			osStream  << " - Local Port:" <<  getLocalPort() << endl;
			osStream  << " - Destination Port:" <<  getDestinationPort() << endl;
			osStream  << " - Size:" <<  getSize() << endl;
			osStream  << " - Data:" <<  getData() << endl;
		
		}

	return osStream.str();
}


void SIMCAN_App_NET_Message::parsimPack(cCommBuffer *b){

	SIMCAN_App_NET_Message_Base::parsimPack(b);
}


void SIMCAN_App_NET_Message::parsimUnpack(cCommBuffer *b){

	SIMCAN_App_NET_Message_Base::parsimUnpack(b);
}
