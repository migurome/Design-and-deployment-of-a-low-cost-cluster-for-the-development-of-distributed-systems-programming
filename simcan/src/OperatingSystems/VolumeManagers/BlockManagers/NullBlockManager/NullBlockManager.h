#ifndef __NULL_BLOCK_MANAGER_H_
#define __NULL_BLOCK_MANAGER_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_BlockList_Message.h"
#include "SMS_RAID_0.h"


/**
 * @class NullBlockManager NullBlockManager.h "NullBlockManager.h"
 *
 * This module calculates the location of the blocks involved in an I/O operation,
 * and send the corresponding requests to the corresponding Block Server.
 *
 * If parameter <b>numBlockServers</b> is greater than 1, this volume manager
 * will act like a RAID controller.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-12
 */
class NullBlockManager :public cSIMCAN_Base{

	protected:

	    /** Number of block servers */
	    unsigned int numBlockServers;


	    /** Output gate to Scheduler. */
	    cGate* toSchedulerGate;

	    /** Input gate from Scheduler. */
	    cGate* fromSchedulerGate;

	    /** Output gates to Block Server. */
	    cGate** toBlockServerGates;

	    /** Input gates from Block Server. */
	    cGate** fromBlockServerGates;

		

	   /**
		* Destructor
		*/
		~NullBlockManager();

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
