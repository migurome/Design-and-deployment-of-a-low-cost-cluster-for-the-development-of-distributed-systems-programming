#ifndef __SIMCAN_REQUEST__
#define __SIMCAN_REQUEST__

#include "SIMCAN_Message.h"
#include "SIMCAN_App_IO_Message.h"

/**
 * Class that represents a Request.
 */
class SIMCAN_Request{

	cMessage *parentRequest;			/**< Request request */
	vector <cMessage*> subRequests;		/**< Vector with pointers to subRequests */
	unsigned int arrivedSubRequests;	/**< Number of arrived subRequests */

	
	public:
	   
	   /**
		* Constructor.
		*/
		SIMCAN_Request ();
		
	   /**
		* Constructor.
		* @param newParent Request message
		*/
		SIMCAN_Request (cMessage *newParent);
	
		/**
		* Constructor.
		* @param newParent Request message
		* @param numSubReq Number of subRequest
		*/
		SIMCAN_Request (cMessage *newParent, unsigned int numSubReq);
		
	   /**
		* Destructor.
		*/
		~SIMCAN_Request ();	
	
		
		cMessage* getParentRequest ();
		void setParentRequest (cMessage* newParent);
		unsigned int getNumSubRequest ();
		unsigned int getNumArrivedSubRequest ();
		void setSubRequest (cMessage* subRequest, unsigned int index);
		void addSubRequest (cMessage* subRequest);
		SIMCAN_Message* getSubRequest (unsigned int index);	
		SIMCAN_Message* popSubRequest (unsigned int index);	
		SIMCAN_Message* popNextSubRequest ();
		bool arrivesAllSubRequest ();
		void arrivesSubRequest (cMessage* subRequest, unsigned int index);
		void clear ();
};


#endif /*__SIMCAN_REQUEST__*/
