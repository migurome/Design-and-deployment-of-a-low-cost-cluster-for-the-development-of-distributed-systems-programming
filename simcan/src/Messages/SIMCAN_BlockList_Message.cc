#include "SIMCAN_BlockList_Message.h"

Register_Class(SIMCAN_BlockList_Message);


SIMCAN_BlockList_Message::~SIMCAN_BlockList_Message(){
}


SIMCAN_BlockList_Message::SIMCAN_BlockList_Message(const char *name, int kind): SIMCAN_BlockList_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_BlockList_Message");
}


SIMCAN_BlockList_Message::SIMCAN_BlockList_Message(const SIMCAN_BlockList_Message& other): SIMCAN_BlockList_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_BlockList_Message");
}


SIMCAN_BlockList_Message::SIMCAN_BlockList_Message (SIMCAN_App_IO_Message *sm_io){

	int i;

		setByteLength (MSG_INITIAL_LENGTH);
		setName ("SIMCAN_BlockList_Message");

		// Base parameters...
		setOperation (sm_io->getOperation());
		setIsResponse (sm_io->getIsResponse());
		setRemoteOperation (sm_io->getRemoteOperation());
		setConnectionId (sm_io->getConnectionId());
		setCommId (sm_io->getCommId());
		setSourceId (sm_io->getSourceId());
		setNextModuleIndex (sm_io->getNextModuleIndex());
		setResult (sm_io->getResult());

		setByteLength (sm_io->getByteLength());
		setParentRequest (sm_io->getParentRequest());

		// SIMCAN_App_IO_Message parameters...
		setFileName (sm_io->getFileName());
		setOffset (sm_io->getOffset());
		setSize (sm_io->getSize());

		// File info...
		getFile().setFileName (sm_io->getFileName());
		getFile().setFileSize (0);

		// Copy the control info, if exists!
		if (sm_io->getControlInfo() != NULL)
			setControlInfo (sm_io->removeControlInfo());

		// Reserve memory to trace!
		setTraceArraySize (sm_io->getTraceArraySize());

		// Copy trace!
		for (i=0; i<sm_io->getTraceArraySize(); i++){
			addNodeTrace (sm_io->getHostName(i), sm_io->getNodeTrace(i));
		}
}


SIMCAN_BlockList_Message& SIMCAN_BlockList_Message::operator=(const SIMCAN_BlockList_Message& other){

	SIMCAN_BlockList_Message_Base::operator=(other);
	return *this;
}


SIMCAN_BlockList_Message *SIMCAN_BlockList_Message::dup() const{

	SIMCAN_BlockList_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_BlockList_Message();

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


SIMCAN_App_IO_Message* SIMCAN_BlockList_Message::transformToApp_IO_Message (){

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


void SIMCAN_BlockList_Message::updateLength (){

	unsigned long int newSize;
	std::ostringstream osStream;

		// Request!
		if (!getIsResponse()){

			// READ
			if (getOperation() == SM_READ_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// WRITE
			else if (getOperation() == SM_WRITE_FILE)
				newSize = MSG_INITIAL_LENGTH + getFile().getFileSize();

			// Unknown operation!
			else{
				osStream << "Error updating SIMCAM_BlockList_Message length (request). Wrong operation:"
						 << operationToString (getOperation());
				throw cRuntimeError(osStream.str().c_str());
			}
		}

		// Response!
		else{

			// READ
			if (getOperation() == SM_READ_FILE)
				newSize = MSG_INITIAL_LENGTH + getFile().getFileSize();

			// WRITE
			else if (getOperation() == SM_WRITE_FILE)
				newSize = MSG_INITIAL_LENGTH;

			// Unknown operation!
			else{
				osStream << "Error updating SIMCAM_BlockList_Message length (response). Wrong operation:"
						 << operationToString (getOperation());
				throw cRuntimeError(osStream.str().c_str());
			}
		}


	// Set the new size!
	setByteLength (newSize);
}


string SIMCAN_BlockList_Message::contentsToString (bool withBranches, bool printContents){

	std::ostringstream osStream;
	
		if (printContents){
	
			osStream  << SIMCAN_App_IO_Message::contentsToString(printContents);
			osStream  << getFile().contentsToString(withBranches, printContents) << endl;
		}

	return osStream.str();
}

