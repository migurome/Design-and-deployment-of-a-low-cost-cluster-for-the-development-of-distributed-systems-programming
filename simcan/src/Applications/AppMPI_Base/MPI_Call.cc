#include "MPI_Call.h"

MPI_Call::MPI_Call (){
	
	call = MPI_NO_VALUE;
	sender = MPI_NO_VALUE;
	receiver = MPI_NO_VALUE;
	root = MPI_NO_VALUE;
	fileName.clear();
	offset = -1;
	sequence = -1;
	bufferSize = -1;
	pendingACKs = -1;	
}

void MPI_Call::clear (){
	
	call = MPI_NO_VALUE;
	sender = MPI_NO_VALUE;
	receiver = MPI_NO_VALUE;
	root = MPI_NO_VALUE;
	fileName.clear();
	offset = -1;
	bufferSize = -1;
	pendingACKs = -1;	
}


void MPI_Call::copyMPICall (MPI_Call *dest){

	dest->setCall(call);
	dest->setSender(sender);
	dest->setReceiver(receiver);
	dest->setRoot(root);
	dest->setFileName(fileName);
	dest->setOffset(offset);
	dest->setSequence(sequence);
	dest->setBufferSize(bufferSize);
	dest->setPendingACKs(pendingACKs);
}


string MPI_Call::toString(){
	
	std::ostringstream osStream;

		osStream << "\n";

		if (call != MPI_NO_VALUE)	
			osStream << "  MPI_Call:" << callToString () << endl;
		
		if (sender != MPI_NO_VALUE)
			osStream << "  Sender:" << sender << endl;;
			
		if (receiver != MPI_NO_VALUE)
			osStream << "  Receiver:" << receiver << endl;
			
		if (root != MPI_NO_VALUE)
			osStream << "  Root:" << root << endl;		
		
		
		if (fileName.size() > 0)			
			osStream << "  fileName:" << fileName << endl;
		
		if (offset != -1)
			osStream << "  offset:" << offset << endl;
		
		if (sequence != -1)
			osStream << "  sequence:" << sequence << endl;

		if (bufferSize != -1)
			osStream << "  bufferSize:" << bufferSize << endl;			
		
			
		if (pendingACKs != -1)
			osStream << "  pending Acks:" << pendingACKs;				
			
			
	return osStream.str();
}

string MPI_Call::callToString(){
	
	string result;
			
		if (call == MPI_NO_VALUE)
			result =  "MPI_NO_VALUE";
		else if (call == MPI_BARRIER_UP)
			result =  "MPI_BARRIER_UP";
		else if (call == MPI_BARRIER_DOWN)
			result =  "MPI_BARRIER_DOWN";
		else if (call == MPI_SEND)
			result =  "MPI_SEND";
		else if (call == MPI_RECV)
			result =  "MPI_RECV";
		else if (call == MPI_BARRIER)
			result =  "MPI_BARRIER";
		else if (call == MPI_BCAST)
			result =  "MPI_BCAST";
		else if (call == MPI_SCATTER)
			result =  "MPI_SCATTER";
		else if (call == MPI_GATHER)
			result =  "MPI_GATHER";
		else if (call == MPI_FILE_OPEN)
			result =  "MPI_FILE_OPEN";
		else if (call == MPI_FILE_CLOSE)
			result =  "MPI_FILE_CLOSE";
		else if (call == MPI_FILE_DELETE)
			result =  "MPI_FILE_DELETE";
		else if (call == MPI_FILE_READ)
			result =  "MPI_FILE_READ";
		else if (call == MPI_FILE_WRITE)
			result =  "MPI_FILE_WRITE";
		else if (call == MPI_FILE_CREATE)
			result =  "MPI_FILE_CREATE";
		else
			result = "UNKNOWN_MPI_CALL";
		
	return result;	
}

unsigned int MPI_Call::getCall (){
	return call;
}

void MPI_Call::setCall (unsigned int newCall){
	call = newCall;
}

unsigned int MPI_Call::getSender (){
	return sender;
}

void MPI_Call::setSender (unsigned int newSender){
	sender = newSender;
}

unsigned int MPI_Call::getReceiver (){
	return receiver;
}

void MPI_Call::setReceiver (unsigned int newReceiver){
	receiver = newReceiver;
}

unsigned int MPI_Call::getRoot (){
	return root;
}

void MPI_Call::setRoot (unsigned int newRoot){
	root = newRoot;
}

string MPI_Call::getFileName (){
	return fileName;
}

void MPI_Call::setFileName (string newFileName){
	fileName = newFileName;
}

int MPI_Call::getOffset (){
	return offset;
}

void MPI_Call::setOffset (int newOffset){
	offset = newOffset;
}

int MPI_Call::getSequence (){
	return sequence;
}

void MPI_Call::setSequence (int newSequence){
	sequence = newSequence;
}

int MPI_Call::getResult (){
	return result;
}

void MPI_Call::setResult (int newResult){
	result = newResult;
}


int MPI_Call::getBufferSize (){
	return bufferSize;
}

void MPI_Call::setBufferSize (int newBufferSize){
	bufferSize = newBufferSize;
}

int MPI_Call::getPendingACKs (){
	return pendingACKs;
}

void MPI_Call::setPendingACKs (int newPendingAcks){
	pendingACKs = newPendingAcks;
}

void MPI_Call::arrivesACK (){
	pendingACKs--;
}


