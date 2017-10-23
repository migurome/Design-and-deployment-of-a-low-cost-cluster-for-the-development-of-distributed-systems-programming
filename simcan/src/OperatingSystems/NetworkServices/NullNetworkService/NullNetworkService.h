#ifndef __NULL_NETWORK_SERVICE_H_
#define __NULL_NETWORK_SERVICE_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_App_NET_Message.h"



/**
 * @class NetworkService NetworkService.h "NetworkService.h"
 *   
 * Network Service Module
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class NullNetworkService : public cSIMCAN_Base{


	protected:	
		
		/** Local IP address */
		string localIP;
	
		/** Input gate from Service Redirector */
	    cGate* fromServiceRedirectorGate;

	    /** Input gate from Network (TCP) */
	    cGate* fromNetTCPGate;

	    /** Output gate to Service Redirector */
	    cGate* toServiceRedirectorGate;

	    /** Output gate to Network (TCP) */
	    cGate* toNetTCPGate;	
	    
       
	    /**
	    * Destructor.
	    */    		
	    ~NullNetworkService();		
	  	        			  	    	    
	   /**
	 	*  Module initialization.
	 	*/
	    void initialize();
	    
	   /**
	 	* Module ending.
	 	*/ 
	    void finish();
	    
	    
	private:
		   
	
	   /**
		* Get the outGate ID to the module that sent <b>msg</b>
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
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);
};

#endif
