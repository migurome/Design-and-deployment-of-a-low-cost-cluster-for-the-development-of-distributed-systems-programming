#ifndef __NFS_CLIENT_H_
#define __NFS_CLIENT_H_

#include "SMS_NFS.h"
#include "FileConfigManager.h"
#include "SimcanAPI.h"


/**
 * @class NFS_Client NFS_Client.h "NFS_Client.h"
 *
 * This application recieves I/O requests (TCP) and generates NFS requests to
 * send them to a NFS server application.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-10
 */
class NFS_Client : public SimcanAPI{


	protected:			
	    
	    /** Destination address. */
	    string destAddress;	    		
	    
	    /** Local port. */
	    int destPort;
	    
	    /** Use servers file? */
	    bool useServersFile;

	  	/** Request size (in KB) */
		int requestSizeKB;

		/** Connection delay time (seconds) */
		simtime_t connectionDelay;

		/** SplittingMessageSystem Object*/
	    SMS_NFS *SMS_nfs;	    	    		
				


	   /**
		* Destructor
		*/
		~NFS_Client();

	   /**
 	 	*  Module initialization.
  		*/
	    void initialize();

	   /**
		* Module ending.
		*/
		void finish();

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
		* Process a response message from a module in the local node. Empty method.
		* All response messages are computed by TCPSocket::CallbackInterface
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);
		
		/**
		 * Process a NET call response
		 */
		void processNetCallResponse (SIMCAN_App_NET_Message *sm_net);
				
		/**
		 * Process an I/O call response
		 */
		void processIOCallResponse (SIMCAN_App_IO_Message *sm_io);


	private:

	   /**
		* Create corresponding connections with servers.
		* @return SIMCAN_OK if the parse process was performed without errors.
		* or SIMCAN_ERROR in other case.
		*/
		int createConnectionsWithServers ();


	   /**
		* Shows the SMS contents. For debug purpose only!
		* @param showContents
		*/
		void showSMSContents (bool showContents);
};

#endif
