#ifndef __SERVICE_REDIRECTOR_H_
#define __SERVICE_REDIRECTOR_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"


/**
 * @class ServiceRedirector ServiceRedirector.h "ServiceRedirector.h"
 *   
 * Service Redirector Module. This module is in charge of redirect incomming requests
 * from application to I/O, Network or CPU service.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class ServiceRedirector :public cSIMCAN_Base{


	protected:			

		/** Number of applications */
		int numApps;

		/** Input gates from Apps. */
		cGate** fromAppGates;
		
		/** Input gate from Memory. */
		cGate* fromMemoryGate;
		
		/** Input gate from Net. */
		cGate* fromNetGate;
		
		/** Input gate from CPU. */
		cGate* fromCPUGate;		

		/** Output gates to Apps. */
		cGate** toAppGates;
		
		/** Input gate to Memory. */
		cGate* toMemoryGate;
		
		/** Input gate to Net. */
		cGate* toNetGate;
		
		/** Input gate to CPU. */
		cGate* toCPUGate;
		
	        	
	        	
	   /**
	    * Destructor.
	    */    		
	    ~ServiceRedirector();			
	  	        			  	    	    
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
