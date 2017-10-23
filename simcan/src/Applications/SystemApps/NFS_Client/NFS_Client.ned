// Module that implements a GenericApp.
//
// This application recieves I/O requests (TCP) and generates NFS requests to
// send them to a NFS server application.
//
// If (useServersFile==true) then this application will use servers information
// from the servers file (FileConfigManager object). Else, this application will 
// use local parameter to establish a connection with one server. Note that to set up
// a connection with several servers a servers file have to be used.
//
// Connection types used:
//
// - Ethernet network= "Ethernet"
// - Simple network = "Simple"
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.Applications.SystemApps.NFS_Client;

import SIMCAN.Applications.SimcanAPI.SimcanAPI;



simple NFS_Client extends SimcanAPI{
    
    parameters:
        string connectAddress = default ("");				// Server address
        int connectPort = default (0);						// Port number to connect to
        bool useServersFile = default (true);				// Use a servers file?        
        int requestSizeKB @unit (KiB) = default (128KiB);	// Request size (in KB)

        @display("i=block/app2");
        @class(NFS_Client);
}
