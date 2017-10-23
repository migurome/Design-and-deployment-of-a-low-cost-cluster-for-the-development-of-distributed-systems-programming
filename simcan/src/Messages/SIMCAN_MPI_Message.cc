#include "SIMCAN_MPI_Message.h"

Register_Class(SIMCAN_MPI_Message);

SIMCAN_MPI_Message::SIMCAN_MPI_Message(const char *name, int kind): SIMCAN_MPI_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_MPI_Message");
}


SIMCAN_MPI_Message::SIMCAN_MPI_Message(const SIMCAN_MPI_Message& other): SIMCAN_MPI_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_MPI_Message");
}


SIMCAN_MPI_Message& SIMCAN_MPI_Message::operator=(const SIMCAN_MPI_Message& other){

	SIMCAN_MPI_Message_Base::operator=(other);

	return *this;
}


SIMCAN_MPI_Message *SIMCAN_MPI_Message::dup() const{

	SIMCAN_MPI_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_MPI_Message();

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

		// SIMCAN_MPI_Message parameters...
		newMessage->setSourceRank (getSourceRank());
		newMessage->setDestRank (getDestRank());
		newMessage->setFileName (getFileName());
		newMessage->setOffset (getOffset());
		newMessage->setSequence (getSequence());

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


string SIMCAN_MPI_Message::contentsToString (bool printContents){

    std::ostringstream osStream;

        if (printContents){

            osStream << SIMCAN_Message::contentsToString(printContents);
            osStream  << " - sourceRank:" << getSourceRank() << endl;
            osStream  << " - destRank:" << getDestRank() << endl;
        }

    return osStream.str();
}

