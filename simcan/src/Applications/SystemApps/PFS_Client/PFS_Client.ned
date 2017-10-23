// Module that implements a GenericApp.
//
// This application acts as the client side of the parallel file system.
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
package SIMCAN.Applications.SystemApps.PFS_Client;

import SIMCAN.Applications.SimcanAPI.SimcanAPI;



simple PFS_Client extends SimcanAPI{
    
    parameters:
        string connectAddress = default("");				// Server address
        int connectPort = default (0);						// Port number to connect to
        bool useServersFile = default (true);				// Use a servers file?       
        int requestSizeKB @unit (KiB) = default (128KiB);	// Request size (in KB)

        @display("i=block/app2");
        @class(PFS_Client);
}

