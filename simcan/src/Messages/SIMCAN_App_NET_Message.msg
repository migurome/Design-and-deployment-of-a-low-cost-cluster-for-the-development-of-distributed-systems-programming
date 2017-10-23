cplusplus {{

	#include "SIMCAN_Message.h"
			
}}


class SIMCAN_Message;


///
/// SIMCAN Message for NET requests
///
message SIMCAN_App_NET_Message extends SIMCAN_Message
{
    @customize(true);

    string destinationIP = "";		// Destination IP
    string localIP = "";			// Local IP
    int destinationPort = 0;		// Destination port
    int localPort = 0;				// Local port   
    unsigned int size = 0;          // Request size
    string data = "";          		// Data sended
}
