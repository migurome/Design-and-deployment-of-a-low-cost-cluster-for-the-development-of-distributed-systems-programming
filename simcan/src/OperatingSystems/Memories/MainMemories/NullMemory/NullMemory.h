#ifndef __NULL_MEMORY_H_
#define __NULL_MEMORY_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"


/**
 * @class NullMemory NullMemory.h "NullMemory.h"
 *   
 * Null Memory.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class NullMemory :public cSIMCAN_Base{


	protected:		
		
		/** Number of inputs */
	    int numInputs;    		
	  
	    /** Gate IDs Vector. Output gates to Inputs gates. */
	    cGate** toInputGates;
	    
	    /** Gate IDs Vector. Input gates from Input gates. */
	    cGate** fromInputGates;
	    
	    /** Gate ID. Output gate to Output. */
	    cGate* toOutputGate;
	    
	    /** Gate ID. Input gate from Output. */
	    cGate* fromOutputGate;	    	        	        
	        	
	        	
	   /**
	    * Destructor.
	    */    		
	    ~NullMemory();			
	  	        			  	    	    
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
