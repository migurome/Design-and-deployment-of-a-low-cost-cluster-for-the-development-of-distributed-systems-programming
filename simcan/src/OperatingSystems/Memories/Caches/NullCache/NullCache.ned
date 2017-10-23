// Module that implements a Generic Cache
//
// Null memory.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.Memories.Caches.NullCache;

import SIMCAN.OperatingSystems.Memories.Caches.ICache;

simple NullCache like ICache

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

