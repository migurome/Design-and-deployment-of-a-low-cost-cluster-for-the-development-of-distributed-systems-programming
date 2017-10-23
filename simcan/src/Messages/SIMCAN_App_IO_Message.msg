cplusplus {{

	#include "SIMCAN_Message.h"
			
}}


class SIMCAN_Message;


///
/// SIMCAN Message for IO requests
///
message SIMCAN_App_IO_Message extends SIMCAN_Message
{
    @customize(true);

    string fileName = "";		// File name
    unsigned int offset = 0;	// Offset		
    unsigned int size = 0;		// Request size
    bool isPending = false;		// Is current request pending?			
}
