// Module that implements a Generic Main Memory
//
// Memory module that implements the basic functionality of memory system
// This module does not use cache blocks.
//
// Receives as input a SIMCAN_App_IO_Message
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.Memories.MainMemories.BasicMainMemory;

import SIMCAN.OperatingSystems.Memories.MainMemories.IMainMemory;

simple BasicMainMemory like IMainMemory{
    
    parameters:
        int readLatencyTime @unit(us);		// Latency time for read operations (in seconds)
        int writeLatencyTime @unit(us);		// Latency time for write operations (in seconds)		
        double size @unit(GiB);				// Memory size (in GB)        
        double blockSize @unit(KiB);		// Block Size (in KB)	
        double numInputs;					// Number of inputs
        @display("i=device/memory");
        
    gates:
        input fromInput[];	// Input gates from imputs
        input fromOutput;		// Input gate from output

        output toInput[];		// Output gates to inputs
        output toOutput;		// Output gate to output

}

