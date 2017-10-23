#include "SIMCAN_Request.h"


SIMCAN_Request::SIMCAN_Request (){
	arrivedSubRequests = 0;
	parentRequest = NULL;
}


SIMCAN_Request::SIMCAN_Request (cMessage *newParent){
	arrivedSubRequests = 0;
	parentRequest = newParent;
}


SIMCAN_Request::SIMCAN_Request (cMessage *newParent, unsigned int numSubReq){
	
	int i;
	
		arrivedSubRequests = 0;
		parentRequest = newParent;
		subRequests.reserve (numSubReq);
		
		for (i=0; i<numSubReq; i++)
			subRequests.push_back (NULL);
}


SIMCAN_Request::~SIMCAN_Request (){
	
	int i;		

		// Removes messages...
		for (i=0; i<subRequests.size(); i++){
			
			if (subRequests[i] != NULL)				
				delete (subRequests[i]);
				subRequests[i] = NULL;
		}
		
	subRequests.clear();		
}


cMessage* SIMCAN_Request::getParentRequest (){
	return parentRequest;	
}


void SIMCAN_Request::setParentRequest (cMessage* newParent){
	parentRequest = newParent;
}


unsigned int SIMCAN_Request::getNumSubRequest (){
	return subRequests.size();
}


unsigned int SIMCAN_Request::getNumArrivedSubRequest (){
	return arrivedSubRequests;
}


void SIMCAN_Request::setSubRequest (cMessage* subRequest, unsigned int index){
	
	if (index < subRequests.size())
		subRequests[index] = subRequest;
	else		
		throw cRuntimeError("[SIMCAN_Request.setSubRequest] Index out of bounds!");	
	
}


void SIMCAN_Request::addSubRequest (cMessage* subRequest){
	subRequests.push_back (subRequest);
}


SIMCAN_Message* SIMCAN_Request::getSubRequest (unsigned int index){	
	
	SIMCAN_Message* sm;
		
		sm = NULL;
	
		if (index < subRequests.size()){
			if (subRequests[index] != NULL)
			sm = check_and_cast<SIMCAN_Message *> (subRequests[index]);
		}
		else
			throw cRuntimeError("[SIMCAN_Request.getSubRequest] Index out of bounds!");
		
	return sm;
}


SIMCAN_Message* SIMCAN_Request::popSubRequest (unsigned int index){
	
	SIMCAN_Message* subRequest;
	
		subRequest = NULL;
		
		if (index < subRequests.size()){			
			if (subRequests[index] != NULL){
				subRequest = check_and_cast<SIMCAN_Message *> (subRequests[index]);
				subRequests[index] = NULL;			
			}		
		}
		else
			throw cRuntimeError("[SIMCAN_Request.getSubRequest] Index out of bounds!");
		
	return subRequest;
}


SIMCAN_Message* SIMCAN_Request::popNextSubRequest (){
	
	SIMCAN_Message* subRequest;
	int i;
	bool found;

		// Init...
		subRequest = NULL;
		found = false;
		i=0;

		// Search for the next subRequest
		while ((!found) && (i<subRequests.size())){

			if (subRequests[i] != NULL){

				// Casting
				subRequest = check_and_cast<SIMCAN_Message *>(subRequests[i]);

				// Found!
				if (!subRequest->getIsResponse()){
					subRequests[i] = NULL;
					found=true;
				}
			}
			
			// Try with next subRequest...
			if (!found)
				i++;			
		}

	return subRequest;
}


bool SIMCAN_Request::arrivesAllSubRequest (){
	
	return (arrivedSubRequests == subRequests.size());
}


void SIMCAN_Request::arrivesSubRequest (cMessage* subRequest, unsigned int index){

	if (index < subRequests.size()){
		
		if (subRequests[index] == NULL){
			subRequests[index] = subRequest;
			arrivedSubRequests++;
		}
		else
			delete (subRequest);				
	}
	else{
		//throw cRuntimeError("[SIMCAN_Request.arrivesSubRequest] Index out of bounds!");	
	}
}


void SIMCAN_Request::clear (){
	
	int i;	

		if (parentRequest != NULL){
			delete (parentRequest);
			parentRequest = NULL;
		}
	
		// Removes messages...
		for (i=0; i<subRequests.size(); i++){
			
			if (subRequests[i] != NULL)				
				delete (subRequests[i]);
				subRequests[i] = NULL;
		}	
}

