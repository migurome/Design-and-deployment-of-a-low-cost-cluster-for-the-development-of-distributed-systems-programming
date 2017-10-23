// Module that implements a Generic Main Memory
//
// Null memory. Receives as input a SIMCAN_App_IO_Message
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.Memories.MainMemories.NullMemory;

import SIMCAN.OperatingSystems.Memories.MainMemories.IMainMemory;

simple NullMemory like IMainMemory

{
    parameters:
        double numInputs;	// Number of inputs		

        @display("i=device/memory");
    gates:
        input fromInput[];	// Input gates from imputs
        input fromOutput;		// Input gate from output

        output toInput[];		// Output gates to inputs
        output toOutput;		// Output gate to output

}

