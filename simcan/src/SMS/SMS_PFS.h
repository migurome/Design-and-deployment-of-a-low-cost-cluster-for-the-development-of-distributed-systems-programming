#ifndef __SMS_PFS_
#define __SMS_PFS_

#include <omnetpp.h>
#include "SplittingMessageSystem.h"
#include "SIMCAN_App_IO_Message.h"


/**
 * @class SMS_PFS SMS_PFS.h "SMS_PFS.h"
 *
 * Class that implements a SplittingMessageSystem abstract class.
 *
 * Specific features of this class:
 *
 * - Performs a linear searching and linear deleting.
 * - Inserts requests following a FIFO algorithm.
 * - Splitting method: Splits an I/O request into strideSize (KB) subRequests.
 * - Supported message type: SIMCAN_App_IO_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SMS_PFS : public SplittingMessageSystem {

	protected:

		/** Stride size (in bytes) */
		unsigned int strideSize;

		/** Number of servers */
		unsigned int numServers;

		int currentCommId;

		/** Meta-data in each getNode(in KB) */
	 	static const unsigned int meta_dataKB;
	 	
	 	/** List of subRequests */
		std::list <SIMCAN_App_IO_Message*> subRequests;

	public:

	   /**
		* Constructor.
		*
		* @param newStrideSizeKB Size (in KB) of each file slide.
		* @param newNumServers Number of servers.
		*/
		SMS_PFS (unsigned int newStrideSizeKB, unsigned int newNumServers);


	   /**
 		* Given a request size, this function split a request in <b>strideSize</b> KB subRequests.
 		* @param msg Request message.
 		*/
		void splitRequest (cMessage *msg);
		
		
		/**
		* Gets the first subRequest. This method does not remove the subRequest from list.
		* @return First subRequest if list is not empty or NULL if list is empty.
		*/
		SIMCAN_App_IO_Message* getFirstSubRequest();


	   /**
		* Pops the first subRequest. This method removes the subRequest from list.
		* @return First subRequest if list is not empty or NULL if list is empty.
		*/
		SIMCAN_App_IO_Message* popSubRequest();


	   /**
		* Parses a request (with the corresponding sub_request) to string.
		* IMPORTANT: This function must be called next to <b>splitRequest</b>.
		* If not, maybe some sub_requests not been linked to original request.
		* This function is for debugging purpose only.
		*
		* @param index Position ov the requested request.
		* @return A string with the corresponding request info.
		*/
		string requestToStringByIndex (unsigned int index);
		
		
	   /**
 		* Removes all requests
 		*/
		void clear ();
};

#endif /*__SMS_RAID_0_*/
