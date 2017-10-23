cplusplus {{

	#include "SIMCAN_Message.h"
			
}}


class SIMCAN_Message;


///
/// SIMCAN Message for MEM requests
///
message SIMCAN_App_MEM_Message extends SIMCAN_Message
{
    @customize(true);

    unsigned int memSize = 0;		// Amount of memory
    unsigned int region = 0;		// Region of memory
}
