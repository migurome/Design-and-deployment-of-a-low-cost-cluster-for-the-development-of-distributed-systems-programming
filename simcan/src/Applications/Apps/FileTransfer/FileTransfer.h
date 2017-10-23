#ifndef __FILE_TRANSFER_H_
#define __FILE_TRANSFER_H_

#include <omnetpp.h>
#include "SimcanAPI.h"


/**
 * @class FileTransfer FileTransfer.h "FileTransfer.h"
 *
 * Application that models a file transmission between a client and a server
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-13
 */
class FileTransfer: public SimcanAPI{

	
	protected:

        /** Data size to be sent to the server side (in bytes) */
		unsigned int dataSize;

		/** Block Size (in bytes) */
		unsigned int blockSize;

		/** File name */
		string fileName;

		/** Operation type */
        string operation;

		/** Show log messages? */
		bool showLog;


		/** Simulation Starting timestamp */
		simtime_t simStartTime;

		/** Simulation Ending timestamp */
		simtime_t simEndTime;
		
		/** Running starting timestamp */
		time_t runStartTime;

		/** Running ending timestamp */
		time_t runEndTime;		
		
		/** offset */
		unsigned int offset;

		/** Reaining bytes */
		unsigned int remainingBytes;
				
								
		
	   /**
		* Destructor
		*/
		~FileTransfer();

	   /**
 		*  Module initialization.
 		*/
	    virtual void initialize();

	   /**
 		* Module ending.
 		*/
	    virtual void finish();

	   /**
		* Process a self message.
		* @param msg Self message.
		*/
		void processSelfMessage (cMessage *msg);

	   /**
		* Process a request message.
		* @param sm Request message.
		*/
		void processRequestMessage (SIMCAN_Message *sm);

	   /**
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);

	private:			    

	   /**
		* Print results.
		*/
		void printResults();

};

#endif
