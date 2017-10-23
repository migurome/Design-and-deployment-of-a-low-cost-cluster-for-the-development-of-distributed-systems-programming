#ifndef __SIMPLE_DISK_H_
#define __SIMPLE_DISK_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_BlockList_Message.h"


/**
 * @class BasicDisk BasicDisk.h "BasicDisk.h"
 *   
 * Class that simulates a disk with a user defined bandwidth (for reads and writes)
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class BasicDisk: public cSIMCAN_Base{


	protected:
				  	
	  	/** Read bandwidth */
	  	double readBandwidth;
	  	
	  	/** Write bandwidth */
	  	double writeBandwidth;
	
		/** Gate ID. Input gate. */
		cGate* inGate;	
		
		/** Gate ID. Output gate. */
		cGate* outGate;	
	    	    
	    
	private: 

		/** Request time. */
		simtime_t requestTime;
	
		/** Pending message */
	    cMessage *pendingMessage; 
  
  
  	   /**
  		* Destructor. 
  		*/
  		~BasicDisk();


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
 		* Method that calculates the spent time to process the current request.
 		* @param sm Message that contains the current I/O request.
 		* @return Spent time to process the current request. 
 		*/
		simtime_t service (SIMCAN_Message *sm);
		
		
	   /**
		* Method that implements the Disk simulation equation.
		* 
		* @param numBlocks Number of blocks to read/write.
		* @param operation Operation type.
		* @return Spent time to perform the corresponding I/O operation.
		*/
		simtime_t diskTime (size_blockList_t numBlocks, char operation);
};

#endif

