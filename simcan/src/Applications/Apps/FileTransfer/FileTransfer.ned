// Application that models a file transmission between a client and a server
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.Applications.Apps.FileTransfer;

import SIMCAN.Applications.SimcanAPI.SimcanAPI;


simple FileTransfer extends SimcanAPI{
    
    parameters:
        int dataSize @unit(MiB) = default (10MiB);			// Size of the data to be transferred (in MB)
        int blockSize @unit(Kib) = default (64Kib);			// Block size (in KB)
        string fileName = default ("/inputFile_0.dat");		// Input file name
        string operation = default ("read");				// Operation: read (copy file from server to the client) | write (copy file from client to the server)
        bool showLog = default (true);						// Show log messages
        
        @display("i=old/telnet");							// Display (graphic mode only)
        @class(FileTransfer);								// Class type
}


