// Module that implements a GenericBasicFSModule.
//
// This module contains:
//
// - A GenericFS.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.FileSystems;

import SIMCAN.OperatingSystems.FileSystems.IFS;

module FSModule

{
    parameters:
        string fsType;		// File System Type

    gates:
        input fromVM;			// Input gate from VM (Volume Manager)
        input fromIOR;		// Input gate from IOR (I/O Redirector)

        output toVM;			// Output gate to VM (Volume Manager)
        output toIOR;			// Output gate to IOR (I/O Redirector)


    submodules:
        fs: <fsType> like IFS {
            parameters:
                @display("i=block/network;p=60,100");

        }
    connections allowunconnected:

        // Conections with IOR (I/O Redirector)
        fromIOR --> fs.fromIOR;
        toIOR <-- fs.toIOR;

        // Conections with VM (Volume Manager)
        fromVM --> fs.fromVM;
        toVM <-- fs.toVM;

}
