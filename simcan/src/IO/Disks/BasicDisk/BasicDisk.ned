// Module that implements a Generic Disk
//
// This module simulates a disk with a user defined bandwidth (for reads and writes)
//
// This module receives messages with a branch list. To calculate the service time, 
// this model only uses the total number of requested blocks, not each block position on disk.
//
// Operation list:
//
// - <b>Read a file</b><br>
//		- MessageName: SIMCAN_BlockList_Message <br>
// 		- Operation: SM_READ_FILE<br>
//
// - <b>Write a file</b><br>
//		- MessageName: SIMCAN_BlockList_Message <br>
// 		- Operation: SM_WRITE_FILE<br>
//
// Receives as input a SIMCAN_BlockList_Message
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.IO.Disks.BasicDisk;

import SIMCAN.IO.Disks.IDisk;

simple BasicDisk like IDisk

{
    parameters:
        double readBandwidth @unit(Mbps);		// Bandwidth (in Mbps) for reads
        double writeBandwidth @unit(Mbps);		// Bandwidth (in Mbps) for writes		

        @display("i=device/disk");
    gates:
        input in;		// Input gate
        output out;	// Output gate

}
