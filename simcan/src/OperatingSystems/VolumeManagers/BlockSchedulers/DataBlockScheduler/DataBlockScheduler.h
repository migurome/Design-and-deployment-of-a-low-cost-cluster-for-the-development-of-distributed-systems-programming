#ifndef __DATA_BLOCK_SCHEDULER_H_
#define __DATA_BLOCK_SCHEDULER_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SMS_Branch.h"
#include "SIMCAN_BlockList_Message.h"


/**
 * @class DataBlockScheduler DataBlockScheduler.h "DataBlockScheduler.h"
 * 
 * This module schedules I/O requests.
 * Derived classes must specify the schedulling policies.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-12
 */
class DataBlockScheduler: public cSIMCAN_Base{
		
	protected:  	  	
				 
	    /** Output gate to Input gate. */
	    cGate* toInputGate;
	    
	    /** Input gate from Input gate. */
	    cGate* fromInputGate;
	    
	    /** Output gate. */
	    cGate* toOutputGate;
	    
	    /** Input gate. */
	    cGate* fromOutputGate;	    	        
	    
	    /** SplittingMessageSystem Object*/
	    SMS_Branch *SMS_branch;	
		
	   /**
		* Destructor 
		*/
		~DataBlockScheduler();
	
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
		
	   /**
		* Process all pending branches	
		*/
		void processBranches ();		
		
	   /**
		* Send a request message to its destination!
		* @param sm Request message.
		* @param gate Gate used to send the message.
		*/		
		void sendRequestMessage (SIMCAN_Message *sm, cGate* gate);
};

#endif
