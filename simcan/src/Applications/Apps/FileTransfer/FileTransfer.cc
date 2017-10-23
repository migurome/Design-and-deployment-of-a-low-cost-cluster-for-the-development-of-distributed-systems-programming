#include "FileTransfer.h"

#define MAX_FILE_SIZE 4000000000
#define MAX_BLOCK_SIZE (16*MB)
#define READ_OP "read"
#define WRITE_OP "write"

Define_Module(FileTransfer);

FileTransfer::~FileTransfer(){
}

void FileTransfer::initialize(){

	std::ostringstream osStream;
	cMessage *waitToExecuteMsg;

	    // Timer
	    runStartTime = time (NULL);

		// Set the moduleIdName
		osStream << "FileTransfer." << getId();
		moduleIdName = osStream.str();

		// Init the super-class
		SimcanAPI::initialize();

		// App Module parameters
		dataSize  = (int) par ("dataSize") * MB;
		blockSize  = (int) par ("blockSize") * KB;
		const char *newFileName = par ("fileName");
		fileName = newFileName;
		const char *newOperation = par ("operation");
		operation = newOperation;
		showLog = (bool) par ("showLog");


		// Check max data size
		if (dataSize>MAX_FILE_SIZE){
		    showErrorMessage ("Parameter dataSize (%d) exceeds %d bytes\n", dataSize, MAX_FILE_SIZE);
		}

		// Check block size
		if (blockSize > MAX_BLOCK_SIZE){
		    showErrorMessage ("Parameter blockSize (%d) exceeds %d bytes\n", blockSize, MAX_BLOCK_SIZE);
		}

		// Check operation
		if ((operation.compare(READ_OP) != 0) && (operation.compare(WRITE_OP) != 0)){
		    showErrorMessage ("Wrong operation. Expected [%s | %s] \n", READ_OP, WRITE_OP);
		}

		// Init
		offset = 0;
		remainingBytes = dataSize;

		if (showLog){
		    printf ("Initializing File transfer client...\n");
		    printf ("File:%s  -  Size:%d  -  Blocksize:%d  -  offset:%d  -  remainingBytes: %d  -  Operation:%s\n", fileName.c_str(), dataSize, blockSize, offset, remainingBytes, operation.c_str());
		}

		// Create SM_WAIT_TO_EXECUTE message for delaying the execution of this application
		waitToExecuteMsg = new cMessage (SM_WAIT_TO_EXECUTE.c_str());
	    scheduleAt (simTime().dbl()+startDelay, waitToExecuteMsg);
}

void FileTransfer::finish(){

	// Finish the super-class
    SimcanAPI::finish();
    printResults();
}

void FileTransfer::processSelfMessage (cMessage *msg){

    unsigned int currentDataRequest;

        if (!strcmp (msg->getName(), SM_WAIT_TO_EXECUTE.c_str())){

            // Delete msg!
            delete (msg);

            // Starting time...
            simStartTime = simTime();

            // Calculate current data request
            if (remainingBytes >= blockSize)
                currentDataRequest = blockSize;
            else
                currentDataRequest = remainingBytes;

            // Read
            if (operation.compare(READ_OP) == 0){

                if (showLog)
                    printf ("[%f - %s] Read(%s, %d, %d); -> remainingBytes:%d\n", simTime().dbl()-startDelay, moduleIdName.c_str(), fileName.c_str(), offset, currentDataRequest, remainingBytes);

                SIMCAN_request_read (fileName.c_str(), offset, currentDataRequest);
            }
            // Write
            else{

                if (showLog)
                    printf ("[%f - %s] Write(%s, %d, %d); -> remainingBytes:%d\n", simTime().dbl()-startDelay, moduleIdName.c_str(), fileName.c_str(), offset, currentDataRequest, remainingBytes);

                SIMCAN_request_write (fileName.c_str(), offset, currentDataRequest);
            }

            // Update offset
            offset+=currentDataRequest;
            remainingBytes-=currentDataRequest;
        }
        else
            showErrorMessage ("Unknown self message [%s]", msg->getName());
}

void FileTransfer::processRequestMessage (SIMCAN_Message *sm){
	showErrorMessage ("This module does not receive request messages!");
}

void FileTransfer::processResponseMessage (SIMCAN_Message *sm){

	SIMCAN_App_IO_Message *sm_io;
	bool isError;
	std::ostringstream osStream;
	unsigned int currentDataRequest;

		// Init...
		sm_io = dynamic_cast<SIMCAN_App_IO_Message *>(sm);

		// IO Message?
		if (sm_io != NULL){

		    // File not found!
		    if (sm_io->getResult() == SIMCAN_FILE_NOT_FOUND){
		        showErrorMessage ("File %s not found!", sm_io->getFileName());
            }

		    // Requested data is out of bounds
		    else if (sm_io->getResult() == SIMCAN_DATA_OUT_OF_BOUNDS){
		        showErrorMessage ("Requested data is out of bounds! File:%s  -  Offset:%d  -  RequestedData:%d!", sm_io->getFileName(), sm_io->getOffset(), sm_io->getSize());
		    }

		    else if (sm_io->getResult() == SIMCAN_DISK_FULL){
		        showErrorMessage ("Disk Full!");
		    }

		    // Response is OK!
		    else if (sm_io->getResult() == SIMCAN_OK){

		        // Read file
		        if (sm_io->getOperation() == SM_READ_FILE){

		            // Calculate current data request
                    if (remainingBytes >= blockSize)
                        currentDataRequest = blockSize;
                    else
                        currentDataRequest = remainingBytes;

                    if (currentDataRequest>0){

                        if (showLog)
                            printf ("[%f - %s] Read(%s, %d, %d); -> remainingBytes:%d\n", simTime().dbl()-startDelay, moduleIdName.c_str(), fileName.c_str(), offset, currentDataRequest, remainingBytes);

                        SIMCAN_request_read (fileName.c_str(), offset, currentDataRequest);

                        // Update offset
                        offset+=currentDataRequest;
                        remainingBytes-=currentDataRequest;
                    }
                }

		        // Write file
		        else if(sm_io->getOperation() == SM_WRITE_FILE){

		            // Calculate current data request
                    if (remainingBytes >= blockSize)
                        currentDataRequest = blockSize;
                    else
                        currentDataRequest = remainingBytes;

                    if (currentDataRequest>0){

                        if (showLog)
                            printf ("[%f - %s] Write(%s, %d, %d); -> remainingBytes:%d\n", simTime().dbl()-startDelay, moduleIdName.c_str(), fileName.c_str(), offset, currentDataRequest, remainingBytes);

                        SIMCAN_request_write (fileName.c_str(), offset, currentDataRequest);

                        // Update offset
                        offset+=currentDataRequest;
                        remainingBytes-=currentDataRequest;
                    }
		        }

		        // Wrong operation!
		        else{
		            showErrorMessage ("Wrong operation in response message: %s", sm_io->operationToString().c_str());
		        }
		    }

		    // Unknown code
		    else{
		        showErrorMessage ("Unknown result code:%d", sm_io->getResult());
		    }

			// Delete response message
			 delete (sm);
		}

		// Wrong response message!
		else{
		    showErrorMessage ("Unknown response message");
		}
} 


void FileTransfer::printResults (){

	// End time
    simEndTime = simTime();
    runEndTime = time (NULL);    
    
    printf ("[RESULT] Real run-time:%2f\n", difftime (runEndTime,runStartTime));
    printf ("[RESULT] Simulation time:%f\n", (simEndTime - simStartTime).dbl());
}


