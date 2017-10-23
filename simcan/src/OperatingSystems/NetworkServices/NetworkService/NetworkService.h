#ifndef __NETWORK_SERVICE_H_
#define __NETWORK_SERVICE_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_App_NET_Message.h"
#include "TCP_ClientSideService.h"
#include "TCP_ServerSideService.h"



class TCP_ClientSideService;
class TCP_ServerSideService;

/**
 * @class NetworkService NetworkService.h "NetworkService.h"
 *   
 * Network Service Module
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class NetworkService : public cSIMCAN_Base{


	protected:	
		
		/** Local IP address */
		string localIP;
										
		/** TCP Client-side Services */
		TCP_ClientSideService *clientTCP_Services;
		
		/** TCP Server-side Services */
		TCP_ServerSideService *serverTCP_Services;
		
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
	    ~NetworkService();		
	  	        			  	    	    
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
		* This method classifies an incoming message and invokes the corresponding method
		* to process it.
		*
		* For self-messages, it invokes processSelfMessage(sm);
		* For request-messages, it invokes processRequestMessage(sm);
		* For response-messages, it invokes processResponseMessage(sm);
		*/
		void handleMessage (cMessage *msg);
	
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
		
		/**
		 * Receiving an established connection message.
		 */
		void receivedEstablishedConnection (cMessage *msg);
};

#endif
