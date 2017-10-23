cplusplus {{

	#include "SIMCAN_App_NET_Message.h"
			
}}


class SIMCAN_App_NET_Message;


///
/// SIMCAN Message for MPI calls
///
message SIMCAN_MPI_Message extends SIMCAN_App_NET_Message
{
    @customize(true);

    unsigned int sourceRank = 0;	// Rank of the source process
    unsigned int destRank = 0;		// Rank of the destination process
    string fileName = "";			// File name
    int offset = 0;					// Offset			
    unsigned int sequence = 0;		// Sequence number	
}
