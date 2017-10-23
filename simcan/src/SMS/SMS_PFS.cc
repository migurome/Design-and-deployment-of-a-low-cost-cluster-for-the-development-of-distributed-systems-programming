#include "SMS_PFS.h"

const unsigned int SMS_PFS::meta_dataKB = 1;


SMS_PFS::SMS_PFS (unsigned int newStrideSizeKB, unsigned int newNumServers){
	strideSize = newStrideSizeKB * KB;
	numServers = newNumServers;
	currentCommId = 0;
}


void SMS_PFS::splitRequest (cMessage *msg){

	int i;
	SIMCAN_App_IO_Message *sm_io;			// IO Request message
	SIMCAN_App_IO_Message *subRequestMsg;	// SubRequests message!

	unsigned int numberOfsubRequest;		// Number of stripped request
	unsigned int requestSize;				// Request Size
	unsigned int subRequestOffset;			// SubRequest offset
	unsigned int subRequestSize;			// Subrequest Size


		// Cast!
    	sm_io = check_and_cast<SIMCAN_App_IO_Message *>(msg);

		// Open File
		if (sm_io->getOperation() == SM_OPEN_FILE){

			// Add the new Request!
			addRequest (msg, numServers);

			// One OPEN request per server...
			for (i=0; i<numServers; i++){

				// Copy the message and set new values!
				subRequestMsg = (SIMCAN_App_IO_Message *) sm_io->dup();
				subRequestMsg->setParentRequest (msg);

				// Read the metadata in each node!
				subRequestMsg->setOffset (0);
	    		subRequestMsg->setSize(meta_dataKB*KB);
	    		subRequestMsg->setOperation(SM_READ_FILE);

	    		// Set the corresponding server
	    		subRequestMsg->setCommId (i);

	    		// Link current subRequest with its parent request
	    		//setSubRequest (msg, subRequestMsg, i);

	    		// Set message length
	    		subRequestMsg->updateLength();

	    		// Update the current subRequest Message ID...
	    		subRequestMsg->addRequestToTrace (i);
	    		
	    		// Insert current subRequest!
	    		subRequests.push_back (subRequestMsg);
			}
		}

		// Create File (2 messages per server: create+write_metaData)
		else if (sm_io->getOperation() == SM_CREATE_FILE){

			// Add the new Request!
			addRequest (msg, 2*numServers);

			// Two requests per server...
			for (i=0; i<numServers; i++){

				// Copy the message and set new values! (Create message)
				subRequestMsg = (SIMCAN_App_IO_Message *) sm_io->dup();
				subRequestMsg->setParentRequest (msg);

	    		// Set the corresponding server
	    		subRequestMsg->setCommId (i);

	    		// Set message length
	    		subRequestMsg->updateLength();

	    		// Update the current subRequest Message ID...
	    		subRequestMsg->addRequestToTrace (i);

	    		// Link current subRequest with its parent request
	    		//setSubRequest (msg, subRequestMsg, i);
	    		
	    		// Insert current subRequest!
	    		subRequests.push_back (subRequestMsg);

	    		// Write message...
	    		subRequestMsg = (SIMCAN_App_IO_Message *) sm_io->dup();
				subRequestMsg->setParentRequest (msg);

				// Read the metadata in each node!
				subRequestMsg->setOffset (0);
	    		subRequestMsg->setSize(meta_dataKB*KB);
	    		subRequestMsg->setOperation(SM_WRITE_FILE);

	    		// Set the corresponding server
	    		subRequestMsg->setCommId (i);

	    		// Set message length
	    		subRequestMsg->updateLength();

	    		// Update the current subRequest Message ID...
	    		subRequestMsg->addRequestToTrace (i+numServers);

	    		// Link current subRequest with its parent request
	    		//setSubRequest (msg, subRequestMsg, i+numServers);
	    		
	    		// Insert current subRequest!
	    		subRequests.push_back (subRequestMsg);
			}
		}

		// Delete File
		else if (sm_io->getOperation() == SM_DELETE_FILE){

			// Add the new Request!
			addRequest (msg, numServers);

			// One OPEN request per server...
			for (i=0; i<numServers; i++){

				// Copy the message and set new values!
				subRequestMsg = (SIMCAN_App_IO_Message *) sm_io->dup();
				subRequestMsg->setParentRequest (msg);

	    		// Set the corresponding server
	    		subRequestMsg->setCommId (i);

	    		// Link current subRequest with its parent request
	    		//setSubRequest (msg, subRequestMsg, i);

	    		// Set message length
	    		subRequestMsg->updateLength();

	    		// Update the current subRequest Message ID...
	    		subRequestMsg->addRequestToTrace (i);
	    		
	    		// Insert current subRequest!
	    		subRequests.push_back (subRequestMsg);
			}
		}

		// Read/Write File
		else if ((sm_io->getOperation() == SM_READ_FILE) ||
				 (sm_io->getOperation() == SM_WRITE_FILE)){

			// Get request size...
			requestSize = sm_io->getSize();

			// Calculate the number of subRequests
			numberOfsubRequest = ((requestSize%strideSize)==0)?
								  (requestSize/strideSize):
								  (requestSize/strideSize)+1;


			// Add the new Request!
		    addRequest (msg, numberOfsubRequest);

			// First offset
			subRequestOffset = sm_io->getOffset();

			// create all subRequests
			for (i=0; i<numberOfsubRequest; i++){

				// Copy the message and set new values!
				subRequestMsg = (SIMCAN_App_IO_Message *) sm_io->dup();
				subRequestMsg->setParentRequest (msg);

				//  Calculates the current subRequest size
				subRequestSize = (requestSize>=strideSize)?
									strideSize:
									requestSize;

				// Read the metadata in each node!
				subRequestMsg->setOffset (subRequestOffset);
	    		subRequestMsg->setSize (subRequestSize);

	    		// Set the corresponding server
	    		//subRequestMsg->setCommId (i%numServers);

	    		subRequestMsg->setCommId (currentCommId);
	    		currentCommId = currentCommId+1;

	    		if (currentCommId >= numServers)
	    			currentCommId = 0;

	    		//printf ("Adding new request. Offset:%d - Size:%d - to server:%d\n", subRequestOffset, subRequestSize, i%numServers);

	    		// Link current subRequest with its parent request
	    		//setSubRequest (msg, subRequestMsg, i);

	    		// Set message length
	    		subRequestMsg->updateLength();

	    		// Update the current subRequest Message ID...
	    		subRequestMsg->addRequestToTrace (i);

	    		// update offset!
	    		subRequestOffset+=subRequestSize;
	    		requestSize-=subRequestSize;
	    		
	    		// Insert current subRequest!
	    		subRequests.push_back (subRequestMsg);
			}
		}
}


SIMCAN_App_IO_Message* SMS_PFS::getFirstSubRequest(){

	if (subRequests.empty())
		return NULL;
	else
		return (subRequests.front());
}


SIMCAN_App_IO_Message* SMS_PFS::popSubRequest(){

	SIMCAN_App_IO_Message *msg;

		if (subRequests.empty())
			msg = NULL;
		else{
			msg = subRequests.front();
			subRequests.pop_front();
		}

	return msg;
}


string SMS_PFS::requestToStringByIndex (unsigned int index){

	std::ostringstream info;
	int i;
	int numSubRequest;
	SIMCAN_App_IO_Message *sm_subReq;
	SIMCAN_App_IO_Message *sm_io;


		// Request not found...
		if ((index>=requestVector.size()) || (index<0)){
			info << "Request" << index << " Not Found!" << endl;
		}

		// Request found!
		else{

			sm_io = check_and_cast<SIMCAN_App_IO_Message *>((requestVector[index])->getParentRequest());

			// Get the number of subRequests
			numSubRequest = getNumberOfSubRequest (sm_io);

			// Original request info...
			info << " Op:" << sm_io->operationToString()
				 << " File:" << sm_io->getFileName()
				 << " Offset:" << sm_io->getOffset()
				 <<	" Size:" << sm_io->getSize()
				 <<	" subRequests:" << numSubRequest
				 << endl;

			// Get info of all subRequests...
			for (i=0; i<numSubRequest; i++){

				// Is NULL?
				if ((requestVector[index])->getSubRequest(i) == NULL)
					info << "  subRequest[" << i << "]: Not arrived yet!" << endl;
				else{

					// Cast!
					sm_subReq = check_and_cast<SIMCAN_App_IO_Message *>((requestVector[index])->getSubRequest(i));

					info << "  subRequest[" << i << "]:"
						 << " Op:" << sm_io->operationToString()
						 << " Offset:" << sm_subReq->getOffset()
						 << " Size:" << sm_subReq->getSize()
						 << " CommId:" << sm_subReq->getCommId()
						 << " appIndex:" << sm_subReq->getNextModuleIndex()
						 << endl;
				}
			}
		}

	return info.str();
}


void SMS_PFS::clear (){
	
	int i;
	
		for (i=0; i<requestVector.size(); i++){
			delete (requestVector[i]);
			requestVector[i] = NULL;
		}	
	
	requestVector.clear();
}

