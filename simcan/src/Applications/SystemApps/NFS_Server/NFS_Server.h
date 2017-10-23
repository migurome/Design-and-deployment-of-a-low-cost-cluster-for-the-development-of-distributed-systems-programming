#ifndef __NFS_SERVER_H_
#define __NFS_SERVER_H_

#include "SIMCAN_App_IO_Message.h"
#include "SimcanAPI.h"

/**
 * @class NFS_Server NFS_Server.h "NFS_Server.h"
 *
 * This application recieves NFS requests (TCP) from NFS clients.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-10
 */
class NFS_Server : public SimcanAPI{


	protected:	    

	    /** Port to listen on */
	    int localPort;		
		

	   /**
		* Destructor
		*/
		~NFS_Server();

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
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);



	private:
	   

	   /**
 		* Create a connection to receive client requests.
 		*/
		void createConnetion ();
};

#endif
