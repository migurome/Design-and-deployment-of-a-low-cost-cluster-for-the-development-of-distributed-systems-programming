#include "SIMCAN_App_IO_Message.h"

Register_Class (SIMCAN_App_IO_Message);



SIMCAN_App_IO_Message::~SIMCAN_App_IO_Message(){
}


SIMCAN_App_IO_Message::SIMCAN_App_IO_Message(const char *name, int kind): SIMCAN_App_IO_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_IO_Message");
}


SIMCAN_App_IO_Message::SIMCAN_App_IO_Message(const SIMCAN_App_IO_Message& other): SIMCAN_App_IO_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_IO_Message");
}


SIMCAN_App_IO_Message& SIMCAN_App_IO_Message::operator=(const SIMCAN_App_IO_Message& other){

	SIMCAN_App_IO_Message_Base::operator=(other);
	return *this;
}


SIMCAN_App_IO_Message *SIMCAN_App_IO_Message::dup() const{

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

		// SIMCAN_App_IO_Message parameters...
		newMessage->setFileName (getFileName());
		newMessage->setOffset (getOffset());
		newMessage->setSize (getSize());

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


void SIMCAN_App_IO_Message::updateLength (){

	unsigned long int newSize;
	std::ostringstream osStream;


		// Request!
		if (!getIsResponse()){

			// READ
			if (getOperation() == SM_READ_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// WRITE
			else if (getOperation() == SM_WRITE_FILE)
				newSize = MSG_INITIAL_LENGTH + getSize();

			// OPEN
			else if (getOperation() == SM_OPEN_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// CLOSE
			else if (getOperation() == SM_CLOSE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// CREATE
			else if (getOperation() == SM_CREATE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// DELETE
			else if (getOperation() == SM_DELETE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// Unknown operation!
			else{
				osStream << "Error updating SIMCAM_App_IO_Message length (request). Wrong operation:"
						 << operationToString (getOperation());
				throw cRuntimeError(osStream.str().c_str());
			}
		}

		// Response!
		else{

			// READ
			if (getOperation() == SM_READ_FILE)
				newSize = MSG_INITIAL_LENGTH + getSize();

			// WRITE
			else if (getOperation() == SM_WRITE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// OPEN
			else if (getOperation() == SM_OPEN_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// CLOSE
			else if (getOperation() == SM_CLOSE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// CREATE
			else if (getOperation() == SM_CREATE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// DELETE
			else if (getOperation() == SM_DELETE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// Unknown operation!
			else{
				osStream << "Error updating SIMCAM_App_IO_Message length (request). Wrong operation:"
						 << operationToString (getOperation());
				throw cRuntimeError(osStream.str().c_str());
			}
		}


	// Set the new size!
	setByteLength (newSize);
}


string SIMCAN_App_IO_Message::getIsPending_string(){
	
	string result;
	
		if (getIsPending())
			result = "true";
		else
			result = "false";
	
	return result;
}


string SIMCAN_App_IO_Message::contentsToString (bool printContents){

	std::ostringstream osStream;
	
		if (printContents){

			osStream << SIMCAN_Message::contentsToString(printContents);
	
			osStream  << " - fileName:" <<  getFileName() << endl;
			osStream  << " - offset:" <<  getOffset() << endl;
			osStream  << " - size:" <<  getSize() << endl;
			osStream  << " - operation:" <<  operationToString(getOperation()) << endl;
			osStream  << " - isPending:" <<  getIsPending_string() << endl;
		}

	return osStream.str();
}


void SIMCAN_App_IO_Message::parsimPack(cCommBuffer *b){

	SIMCAN_App_IO_Message_Base::parsimPack(b);
}


void SIMCAN_App_IO_Message::parsimUnpack(cCommBuffer *b){

	SIMCAN_App_IO_Message_Base::parsimUnpack(b);
}
