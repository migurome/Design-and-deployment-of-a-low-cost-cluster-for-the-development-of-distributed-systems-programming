// Module that implements a GenericApp.
//
// This application recieves NFS requests (TCP) from NFS clients.
//
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30
package SIMCAN.Applications.SystemApps.NFS_Server;

import SIMCAN.Applications.SimcanAPI.SimcanAPI;



simple NFS_Server extends SimcanAPI{
    
    parameters:
        int localPort = default (9000);		// Port number to listen on

        @display("i=block/app2");
        @class(NFS_Server);
}
