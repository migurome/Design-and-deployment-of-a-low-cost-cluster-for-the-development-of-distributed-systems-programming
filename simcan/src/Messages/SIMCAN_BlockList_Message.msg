cplusplus {{
	#include "SIMCAN_App_IO_Message.h"
	#include "SIMCAN_File.h"					
}}


//struct fileBranch;
class SIMCAN_App_IO_Message;

class noncobject SIMCAN_File;



///
/// SIMCAN Message for disk requests (branch list)
///
message SIMCAN_BlockList_Message extends SIMCAN_App_IO_Message
{
    @customize(true);

    SIMCAN_File file;		// Branch list
}

