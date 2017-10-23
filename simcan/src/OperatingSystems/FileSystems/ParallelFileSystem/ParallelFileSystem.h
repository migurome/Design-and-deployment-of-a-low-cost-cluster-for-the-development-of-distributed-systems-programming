#ifndef __PARALLEL_FILESYSTEM_H_
#define __PARALLEL_FILESYSTEM_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_App_IO_Message.h"
#include "SMS_PFS.h"
#include "FileConfigManager.h"



/**
 * @class ParallelFileSystem ParallelFileSystem.h "ParallelFileSystem.h"
 *
 * This module splits an I/O file request expressed as (fileName, offset, requestSize) to several
 * subRequests, each one associated to a corresponding server.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class ParallelFileSystem : public cSIMCAN_Base{

	protected:

	   /**
		* Structure that represents a connection with a server.
		*/
		struct SIMCAN_Server_Connector{
			string address;			/**< Destination Adress. */
			unsigned int port;		/**< Listen Port */
			unsigned int id;		/**< Connection ID */
		};
		typedef struct SIMCAN_Server_Connector connector;



	    /** Stride size (in KB) */
	    unsigned int strideSizeKB;

	    /** Number of servers */
	    unsigned int numberOfServers;

	    /** NFS client index */
	    unsigned int clientAppIndex;
	    
	    /** Connector vector that contains the corresponding data to establish connection with servers. */
    	vector <connector> connections;

    	/** Gate ID. Input gate from VM. */
		cGate* fromVMGate;

		/** Gate ID. Input gate from IOR. */
		cGate* fromIORGate;

		/** Gate ID. Output gate to VM. */
		cGate* toVMGate;

		/** Gate ID. Output gate to IOR. */
		cGate* toIORGate;

	    /** SplittingMessageSystem Object*/
	    SMS_PFS *SMS_pfs;
	    
	     /** Pending message */
	    SIMCAN_App_IO_Message *pendingMessage;


	   /**
		* Destructor
		*/
		~ParallelFileSystem();


	   /**
	 	*  Module initialization.
	 	*/
	    void initialize();


	   /**
	 	* Module ending.
	 	*/
	    void finish();
	    
	    
	   /**
		* Process current request message.
		*/
		void processCurrentRequestMessage ();


	private:

	   /**
		* Get the outGate to the module that sent <b>msg</b>
		* @param msg Arrived message.
		* @return. Gate Id (out) to module that sent <b>msg</b> or NOT_FOUND if gate not found.
		*/
		cGate* getOutGate (cMessage *msg);

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
		* Process the enqueued subrequests.
		*/
		void processSubRequests ();
		

	   /**
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);

	   /**
		* Parses the servers file.
		*/
		void parseServersFile ();

	   /**
		* Parse all communication information to string. For debug purpose only!
		* @return A string that contains all communication information.
		*/
		string serverListToString ();


	   /**
		* Shows the SMS contents. For debug purpose only!
		*/
		void showSMSContents (bool showContents);
};

#endif
