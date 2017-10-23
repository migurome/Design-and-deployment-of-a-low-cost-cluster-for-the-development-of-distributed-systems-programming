#ifndef __BASIC_MAIN_MEMORY_H_
#define __BASIC_MAIN_MEMORY_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
using namespace std;


/**
 * @class BasicMainMemory BasicMainMemory.h "BasicMainMemory.h"
 *
 * Memory module that implements the basic functionality of memory system
 * This module does not use cache blocks.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class BasicMainMemory :public cSIMCAN_Base{

	protected:

		/** Memory Block Size (in KB) */
		double blockSizeKB;
		
		/** Memory size (in GB)*/
		double sizeGB;

		/** Number of total blocks */
		unsigned int totalBlocks;

		/** Number of blocks used for application space */
		unsigned int freeAppBlocks;

		/** Number of blocks for application space */
		unsigned int totalAppBlocks;

	    /** Latency time for read operations (in seconds). */
		simtime_t readLatencyTime;
	    
	    /** Latency time for write operations (in seconds). */
		simtime_t writeLatencyTime;
		
		/** Number of inputs */
	    unsigned int numInputs;				

	    /** Gate IDs Vector. Output gates to Inputs gates. */
	    cGate** toInputGates;

	    /** Gate IDs Vector. Input gates from Input gates. */
	    cGate** fromInputGates;

	    /** Gate ID. Output gate to Output. */
	    cGate* toOutputGate;

	    /** Gate ID. Input gate from Output. */
	    cGate* fromOutputGate;

	    /** Pending message */
	    SIMCAN_Message *pendingMessage;


	   /**
		* Destructor
		*/
		~BasicMainMemory();

	   /**
	 	*  Module initialization.
	 	*/
	    void initialize();

	   /**
	 	* Module ending.
	 	*/
	    void finish();

	   /**
		* Process current request message.
		*/
		void processCurrentRequestMessage ();


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

	   /*
		* Send a request to its destination!
		* @param sm Request message.
		*/
		void sendRequest (SIMCAN_Message *sm);
};

#endif
