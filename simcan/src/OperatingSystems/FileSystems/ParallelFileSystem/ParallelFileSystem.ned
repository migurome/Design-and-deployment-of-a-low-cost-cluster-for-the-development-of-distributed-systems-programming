// Module that implements a GenericFS.
//
// This module splits an I/O file request expressed as (fileName, offset, requestSize) to several
// subRequests, each one associated to a corresponding server.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.FileSystems.ParallelFileSystem;

import SIMCAN.OperatingSystems.FileSystems.IFS;

simple ParallelFileSystem like IFS

{
    parameters:
        int clientAppIndex;									// Index of the app vector that contains the client-side of this File system
        int strideSizeKB @unit (KiB) = default (64KiB);	// Stride Size (in KB)

        @display("i=block/table2");
    gates:
        input fromVM;		// Input gate from VM (Volume Manager)
        input fromIOR;		// Input gate from IOR (I/O Redirector)

        output toVM;		// Output gate to VM (Volume Manager)
        output toIOR;		// Output gate to IOR (I/O Redirector)

}

