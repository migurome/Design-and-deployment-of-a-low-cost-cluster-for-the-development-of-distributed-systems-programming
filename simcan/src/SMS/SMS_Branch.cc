#include "SMS_Branch.h"


SMS_Branch::SMS_Branch (){
}


void SMS_Branch::splitRequest (cMessage *msg){

	int currentSubRequest;
	int numberOfBranches;

	off_blockList_t branchOffset;
	size_blockList_t branchSize;

	SIMCAN_BlockList_Message *sm_bl;			// Message (request) to make the casting!
	SIMCAN_BlockList_Message *subRequestMsg;	// SubRequest message


		// Cast the original message
		sm_bl = check_and_cast<SIMCAN_BlockList_Message *>(msg);

		// Add the new Request!
		numberOfBranches = sm_bl->getFile().getNumberOfBranches();
		addRequest (sm_bl, numberOfBranches);


		// Create all corresponding subRequests...
		for (currentSubRequest=0; currentSubRequest<numberOfBranches; currentSubRequest++){

    		// Creates a new subRequest message with the corresponding parameters
			subRequestMsg = (SIMCAN_BlockList_Message *) sm_bl->dup();
			subRequestMsg->setParentRequest (msg);

			// Set the corresponding attributes
			branchOffset = sm_bl->getFile().getBranchOffset(currentSubRequest);
			branchSize = sm_bl->getFile().getBranchSize(currentSubRequest);

			subRequestMsg->getFile().addBranch (branchOffset, branchSize);
			subRequestMsg->getFile().setFileName (sm_bl->getFile().getFileName());
			subRequestMsg->getFile().setFileSize (branchSize);

			// Update message length
			subRequestMsg->updateLength();		


    		// Insert current subRequest!
    		subRequests.push_back (subRequestMsg);
		}
}


SIMCAN_BlockList_Message* SMS_Branch::getFirstSubRequest(){

	if (subRequests.empty())
		return NULL;
	else
		return (subRequests.front());
}


SIMCAN_BlockList_Message* SMS_Branch::popSubRequest(){

	SIMCAN_BlockList_Message *msg;

		if (subRequests.empty())
			msg = NULL;
		else{
			msg = subRequests.front();
			subRequests.pop_front();
		}

	return msg;
}


string SMS_Branch::requestToStringByIndex (unsigned int index){

}


void SMS_Branch::clear (){
	
	int i;
	
		for (i=0; i<requestVector.size(); i++){
			delete (requestVector[i]);
			requestVector[i] = NULL;
		}	
		
	requestVector.clear();	
}
