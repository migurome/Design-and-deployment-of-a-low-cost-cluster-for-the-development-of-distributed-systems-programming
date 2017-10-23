// Module that implements an Generic I/O Redirector.
//
// This VFS contains information about the redirections to the 
// corresponding FS (LOCAL) or to a corresponding APP module (REMOTE).
//
// This module contains a table with paths. Each one of these paths is associated
// to an index and a module type. This index correspond to a FS in the node's FS 
// array or to an APP in node's APP array.
//
// Receives as input a SIMCAN_App_IO_Message
//
// Local operations are sent to local File Systems: Parameter remoteOperation=false
// Remote operations are sent to corresponding client application: Parameter remoteOperation=true
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.VirtualFileSystems.VirtualFileSystem;

import SIMCAN.OperatingSystems.VirtualFileSystems.IVirtualFileSystem;

simple VirtualFileSystem like IVirtualFileSystem
{
    parameters:
        double nfsClientIndex = default(0);		// NFS client index in App vector
        double numFS;						    // Number of File Systems

        @display("i=block/dispatch");
    gates:
        input fromMemory;		// Input gate from Memory
        input fromFS[];			// Input gates from File Systems

        output toMemory;		// Output gate to Memory
        output toFS[];			// Output gates to File Systems
}

