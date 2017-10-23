// Module that implements a Generic Block Scheduler
//
// This module schedules I/O requests.
// Basically, split all request on branch subRequests and inserts them on a queue.
//
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.VolumeManagers.BlockSchedulers.DataBlockScheduler;

import SIMCAN.OperatingSystems.VolumeManagers.BlockSchedulers.IBlockScheduler;

simple DataBlockScheduler like IBlockScheduler

{
    @display("i=block/classifier");
    gates:
        input fromInput;		// Input gate from input
        input fromOutput;		// Input gate from output		

        output toInput;		// Output gate to input
        output toOutput;		// Output gate to output

}

