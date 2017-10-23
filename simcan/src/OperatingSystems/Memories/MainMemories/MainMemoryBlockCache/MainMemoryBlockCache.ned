// Module that implements a Generic Main Memory
//
// Memory module that implements the read-ahead and write-back functionality.
// Receives as input a SIMCAN_App_IO_Message
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.Memories.MainMemories.MainMemoryBlockCache;

import SIMCAN.OperatingSystems.Memories.MainMemories.IMainMemory;

simple MainMemoryBlockCache like IMainMemory{
    
    parameters:                
        int accessTime @unit(us);			// Latency time (in seconds)       
        double flushTime @unit(s);			// Timer to flush cache (in seconds)	
        double size @unit(GiB);				// Memory size (in GB)
        double sizeCache @unit(MiB);		// Amount of memory dedicated for disk cache space (in MB)
        double blockSize @unit(KiB);		// Block Size (in KB)        
        double readAheadBlocks; 			// Window Size to read-ahead operations (in number of blocks)			
        double numInputs;					// Number of inputs
        @display("i=device/memory");
        
    gates:
        input fromInput[];		// Input gates from imputs
        input fromOutput;		// Input gate from output

        output toInput[];		// Output gates to inputs
        output toOutput;		// Output gate to output

}

