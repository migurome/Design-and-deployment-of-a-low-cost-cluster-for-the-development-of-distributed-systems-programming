#ifndef __NULL_BLOCK_SCHEDULER_H_
#define __NULL_BLOCK_SCHEDULER_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_BlockList_Message.h"


/**
 * @class NullBlockScheduler NullBlockScheduler.h "NullBlockScheduler.h"
 * 
 * Null Scheduler
 *  
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-12
 */
class NullBlockScheduler: public cSIMCAN_Base{
		
	protected:  	  	
				 
	    /** Output gate to Input gate. */
	    cGate* toInputGate;
	    
	    /** Input gate from Input gate. */
	    cGate* fromInputGate;
	    
	    /** Output gate. */
	    cGate* toOutputGate;
	    
	    /** Input gate. */
	    cGate* fromOutputGate;
	    
	   
	  /**
	   * Destructor 
	   */ 
	   ~NullBlockScheduler();	    	    		
	
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
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);	   
};

#endif
