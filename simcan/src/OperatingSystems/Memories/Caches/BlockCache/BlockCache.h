#ifndef __BLOCK_CACHE_H_
#define __BLOCK_CACHE_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"


/**
 * @class BlockCache BlockCache.h "BlockCache.h"
 *   
 * Null Cache.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class BlockCache :public cSIMCAN_Base{


	protected:		
		
		/** Number of inputs */
		unsigned int numInputs;
		
		/** Cache hit ratio */
	    double hitRatio;    		
	  
	    /** Gate ID. Output gate to Inputs gates. */
	    cGate** toInputGates;
	    
	    /** Gate ID. Input gate from Input gates. */
	    cGate** fromInputGates;
	    
	    /** Gate ID. Output gate to Output. */
	    cGate* toOutputGate;
	    
	    /** Gate ID. Input gate from Output. */
	    cGate* fromOutputGate;	    	        	        
	        	
	        	
	   /**
	    * Destructor.
	    */    		
	    ~BlockCache();			
	  	        			  	    	    
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
