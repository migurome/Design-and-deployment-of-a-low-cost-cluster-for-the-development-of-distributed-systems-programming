#include "SIMCAN_App_CPU_Message.h"

Register_Class (SIMCAN_App_CPU_Message);



SIMCAN_App_CPU_Message::~SIMCAN_App_CPU_Message(){
}


SIMCAN_App_CPU_Message::SIMCAN_App_CPU_Message(const char *name, int kind): SIMCAN_App_CPU_Message_Base(name,kind){

	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_CPU_Message");
}


SIMCAN_App_CPU_Message::SIMCAN_App_CPU_Message(const SIMCAN_App_CPU_Message& other): SIMCAN_App_CPU_Message_Base(other.getName()){

	operator=(other);
	setByteLength (MSG_INITIAL_LENGTH);
	setName ("SIMCAN_App_CPU_Message");
}


SIMCAN_App_CPU_Message& SIMCAN_App_CPU_Message::operator=(const SIMCAN_App_CPU_Message& other){

	SIMCAN_App_CPU_Message_Base::operator=(other);

	return *this;
}


SIMCAN_App_CPU_Message *SIMCAN_App_CPU_Message::dup() const{

	SIMCAN_App_CPU_Message *newMessage;
	TCPCommand *controlNew;
	TCPCommand *controlOld;
	int i;

		// Create a new message!
		newMessage = new SIMCAN_App_CPU_Message();

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

		// SIMCAN_App_CPU_Message parameters...
		newMessage->setCpuTime (getCpuTime());
		newMessage->setTotalMIs (getTotalMIs());
		newMessage->setRemainingMIs (getRemainingMIs());
		newMessage->setCpuPriority (getCpuPriority());
		newMessage->setQuantum (getQuantum());

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


void SIMCAN_App_CPU_Message::updateLength (){
	
		// Set the new size!
		setByteLength (MSG_INITIAL_LENGTH);
}


void SIMCAN_App_CPU_Message::executeMIs (unsigned long numberMIs){
	
	if ((getRemainingMIs() - numberMIs) >= 0)
		setRemainingMIs (getRemainingMIs() - numberMIs);
	else 
		setRemainingMIs (0);
}

void SIMCAN_App_CPU_Message::executeTime (simtime_t executedTime){
	
	simtime_t remainingTime;
	
	remainingTime = getCpuTime();	
	
	if ((remainingTime - executedTime) > 0.0)
		setCpuTime (remainingTime - executedTime);
	else 
		setCpuTime (0);
}



string SIMCAN_App_CPU_Message::contentsToString (bool printContents){

	std::ostringstream osStream;

		if (printContents){

			osStream << SIMCAN_Message::contentsToString(printContents);
			osStream  << " - cpuTime:" <<  getCpuTime() << endl;
			osStream  << " - totalMIs:" <<  getTotalMIs() << endl;
			osStream  << " - remainingMIs:" <<  getRemainingMIs() << endl;
			osStream  << " - CPU priority:" <<  getCpuPriority() << endl;
			osStream  << " - quantum:" <<  getQuantum() << endl;			
		}

	return osStream.str();
}


void SIMCAN_App_CPU_Message::parsimPack(cCommBuffer *b){

	SIMCAN_App_CPU_Message_Base::parsimPack(b);
}


void SIMCAN_App_CPU_Message::parsimUnpack(cCommBuffer *b){

	SIMCAN_App_CPU_Message_Base::parsimUnpack(b);
}
