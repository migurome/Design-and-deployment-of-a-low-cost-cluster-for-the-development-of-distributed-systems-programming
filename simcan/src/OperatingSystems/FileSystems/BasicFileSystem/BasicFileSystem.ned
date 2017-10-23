// Module that implements a GenericFS.
//
// This module translates an I/O file request (SIMCAN_App_IO_Message) to a list of disk blocks (SIMCAN_BlockList_Message)
// This list contains only 1 branch with sequential blocks
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.FileSystems.BasicFileSystem;

import SIMCAN.OperatingSystems.FileSystems.IFS;

simple BasicFileSystem like IFS
{
    parameters:
        bool preLoadFiles;			// PreLoad File System Files?

        @display("i=block/table2");
    gates:
        input fromVM;		// Input gate from VM (Volume Manager)
        input fromIOR;		// Input gate from IOR (I/O Redirector)

        output toVM;		// Output gate to VM (Volume Manager)
        output toIOR;		// Output gate to IOR (I/O Redirector)

}

