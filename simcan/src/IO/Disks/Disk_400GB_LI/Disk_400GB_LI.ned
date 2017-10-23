// Module that implements a Generic Disk
//
// This module simulates a disk using Lineal Interpolation.
//
// This module receives messages with a branch list. To calculate the service time, 
// this model uses calculates each block position on disk.
//
// This module has been created using the tool HD_benchmark with sequential operations.
//
// Receives as input a SIMCAN_BlockList_Message
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
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.IO.Disks.Disk_400GB_LI;

import SIMCAN.IO.Disks.IDisk;

simple Disk_400GB_LI like IDisk

{
    @display("i=device/disk");
    gates:
        input in;		// Input gate
        output out;	// Output gate

}

