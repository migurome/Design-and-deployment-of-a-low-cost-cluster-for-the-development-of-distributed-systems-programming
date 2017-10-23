#ifndef _SIMCAN_APP_IO_MESSAGE_H_
#define _SIMCAN_APP_IO_MESSAGE_H_

#include "SIMCAN_App_IO_Message_m.h"


/**
 * @class SIMCAN_App_IO_Message SIMCAN_App_IO_Message.h "SIMCAN_App_IO_Message.h"
 *
 * Class that represents a SIMCAN_App_IO_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_App_IO_Message: public SIMCAN_App_IO_Message_Base{


	public:

	   /**
		* Destructor.
		*/
		virtual ~SIMCAN_App_IO_Message();

	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind
		*/
		SIMCAN_App_IO_Message (const char *name=NULL, int kind=0);


	   /**
		* Constructor of SIMCAN_Message
		* @param other Message
		*/
		SIMCAN_App_IO_Message(const SIMCAN_App_IO_Message& other);


	   /**
		* = operator
		* @param other Message
		*/
		SIMCAN_App_IO_Message& operator=(const SIMCAN_App_IO_Message& other);


	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_App_IO_Message *dup() const;


	   /**
		* Update the message length
		*/
		void updateLength ();


	   /**
		* Return if this message contains a pending operation in string format.
		* @return "true" if this message contains a pending operation or "false" in other case.
		*/
		string getIsPending_string ();


	   /**
		* Parse all parameters of current message to string.
		* @return String with the corresponding contents.
		*/
		virtual string contentsToString (bool printContents);


	   /**
		* Serializes a SIMCAN_Message.
		* @param b Communication buffer
		*/
		virtual void parsimPack(cCommBuffer *b);


	   /**
		* Deserializes a SIMCAN_Message.
		* @param b Communication buffer
		*/
		virtual void parsimUnpack(cCommBuffer *b);
};

#endif
