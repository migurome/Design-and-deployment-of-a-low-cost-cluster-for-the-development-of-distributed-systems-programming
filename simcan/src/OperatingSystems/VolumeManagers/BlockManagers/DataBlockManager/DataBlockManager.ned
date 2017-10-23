// Module that implements a Generic Block Manager
//
// This module calculates the location of the blocks involved in an I/O operation,
// and send the corresponding requests to the corresponding Block Server.
//
// If parameter <b>numBlockServers</b> is greater than 1, this volume manager
// will act like a RAID controller. Else, if <b>numBlockServers</b> is equal to
// 1, then this volume manager will act like a single disk controller.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.VolumeManagers.BlockManagers.DataBlockManager;

import SIMCAN.OperatingSystems.VolumeManagers.BlockManagers.IBlockManager;

simple DataBlockManager like IBlockManager

{
    parameters:
        double numBlockServers;								// Number of Block Servers
        double strideSize @unit(B) = default (128KiB);		// Stride size (in bytes)

        @display("i=block/layer");
    gates:
        input fromScheduler;		// Input gate from Scheduler
        input fromBlockServer[];	// Input gates from Block Servers

        output toScheduler;		// Output gate to Scheduler
        output toBlockServer[];	// Output gates to Block Servers

}

