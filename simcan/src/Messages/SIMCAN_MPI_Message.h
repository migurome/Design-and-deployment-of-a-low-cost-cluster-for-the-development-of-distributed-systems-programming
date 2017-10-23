#ifndef _SIMCAN_MPI_MESSAGE_H_
#define _SIMCAN_MPI_MESSAGE_H_

#include "SIMCAN_MPI_Message_m.h"
using std::ostringstream;


/**
 * @class SIMCAN_MPI_Message SIMCAN_MPI_Message.h "SIMCAN_MPI_Message.h"
 *   
 * Class that represents a SIMCAN_MPI_Message.
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_MPI_Message: public SIMCAN_MPI_Message_Base{
	
			
	public:			
		
	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind		* 
		*/
		SIMCAN_MPI_Message (const char *name=NULL, int kind=0);
		
		
	   /**
		* Constructor of SIMCAN_Message
		* @param other Message		 
		*/
		SIMCAN_MPI_Message(const SIMCAN_MPI_Message& other);
		
		
	   /**
		* = operator
		* @param other Message		 
		*/
		SIMCAN_MPI_Message& operator=(const SIMCAN_MPI_Message& other);
		
		
	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_MPI_Message *dup() const;

	   /**
        * Parse all parameters of current message to string.
        * @param printContents Print message contents.
        * @return String with the corresponding contents.
        */
        virtual string contentsToString (bool printContents);
						
};

#endif
