#ifndef _SIMCAN_BLOCKLIST_MESSAGE_H_
#define _SIMCAN_BLOCKLIST_MESSAGE_H_

#include "SIMCAN_BlockList_Message_m.h"

/**
 * @class SIMCAN_BlockList_Message SIMCAN_BlockList_Message.h "SIMCAN_BlockList_Message.h"
 *
 * Class that represents a SIMCAN_BlockList_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_BlockList_Message: public SIMCAN_BlockList_Message_Base{

	public:

	   /**
		* Destructor.
		*/
		virtual ~SIMCAN_BlockList_Message();

	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind
		*/
		SIMCAN_BlockList_Message (const char *name=NULL, int kind=0);


	   /**
		* Constructor of SIMCAN_Message
		* @param other Message
		*/
		SIMCAN_BlockList_Message (const SIMCAN_BlockList_Message& other);


	   /**
		* Constructor of SIMCAN_Message
		* @param sm_io Message
		*/
		SIMCAN_BlockList_Message (SIMCAN_App_IO_Message *sm_io);


	   /**
		* = operator
		* @param other Message
		*/
		SIMCAN_BlockList_Message& operator=(const SIMCAN_BlockList_Message& other);

	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_BlockList_Message *dup() const;


	   /**
		* Creates a copy of current message returning a SIMCAN_App_IO_Message object.
		* @return SIMCAN_App_IO_Message object
		*/

		SIMCAN_App_IO_Message* transformToApp_IO_Message ();


	   /**
		* Update the message length
		*/
		void updateLength ();


	   /**
		* Parse all parameters of current message to string.
		* @param withBranches Parse branch information?
		* @param printContents Print contents?
		* @return String with the corresponding contents.
		*/
		virtual string contentsToString (bool withBranches, bool printContents);
};

#endif
