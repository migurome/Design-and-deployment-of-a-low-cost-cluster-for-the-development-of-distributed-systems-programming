#ifndef _SIMCAN_APP_MEM_MESSAGE_H_
#define _SIMCAN_APP_MEM_MESSAGE_H_

#include "SIMCAN_App_MEM_Message_m.h"


/**
 * @class SIMCAN_App_MEM_Message SIMCAN_App_MEM_Message.h "SIMCAN_App_MEM_Message.h"
 *
 * Class that represents a SIMCAN_App_MEM_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_App_MEM_Message: public SIMCAN_App_MEM_Message_Base{


	public:

	   /**
		* Destructor.
		*/
		virtual ~SIMCAN_App_MEM_Message();

	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind
		*/
		SIMCAN_App_MEM_Message (const char *name=NULL, int kind=0);


	   /**
		* Constructor of SIMCAN_Message
		* @param other Message
		*/
		SIMCAN_App_MEM_Message(const SIMCAN_App_MEM_Message& other);


	   /**
		* = operator
		* @param other Message
		*/
		SIMCAN_App_MEM_Message& operator=(const SIMCAN_App_MEM_Message& other);


	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_App_MEM_Message *dup() const;


		virtual SIMCAN_Message *dupGeneric() const;


	   /**
		* Update the message length
		*/
		void updateLength ();
   

	   /**
		* Parse all parameters of current message to string.
		* @param printContents Print message contents.
		* @return String with the corresponding contents.
		*/
		virtual string contentsToString (bool printContents);


	   /**
		* 
		* Parse a memory region to string format
		* 
		* @param region Memory region
		* @return Memory region in string format
		*  
		*/
		virtual string regionToString (int region);


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
