// Module that represents a Generic Main Memory
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.Memories.MainMemories;

moduleinterface IMainMemory

{
    parameters:
        double numInputs;	// Number of inputs				

    gates:
        input fromInput[];	// Input gates from imputs
        input fromOutput;		// Input gate from output

        output toInput[];		// Output gates to inputs
        output toOutput;		// Output gate to output

}

