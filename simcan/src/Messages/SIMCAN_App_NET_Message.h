#ifndef _SIMCAN_APP_NET_MESSAGE_H_
#define _SIMCAN_APP_NET_MESSAGE_H_

#include "SIMCAN_App_NET_Message_m.h"


/**
 * @class SIMCAN_App_NET_Message SIMCAN_App_NET_Message.h "SIMCAN_App_NET_Message.h"
 *
 * Class that represents a SIMCAN_App_NET_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_App_NET_Message: public SIMCAN_App_NET_Message_Base{


	public:

	   /**
		* Destructor.
		*/
		virtual ~SIMCAN_App_NET_Message();

	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind
		*/
		SIMCAN_App_NET_Message (const char *name=NULL, int kind=0);


	   /**
		* Constructor of SIMCAN_Message
		* @param other Message
		*/
		SIMCAN_App_NET_Message(const SIMCAN_App_NET_Message& other);


	   /**
		* = operator
		* @param other Message
		*/
		SIMCAN_App_NET_Message& operator=(const SIMCAN_App_NET_Message& other);


	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_App_NET_Message *dup() const;


	   /**
		* Update the message length
		*/
		void updateLength ();
   

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
