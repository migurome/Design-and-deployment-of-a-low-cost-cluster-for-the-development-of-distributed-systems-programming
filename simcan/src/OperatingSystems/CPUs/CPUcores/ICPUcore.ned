// Module that represents a Generic CPU core.
//
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.CPUs.CPUcores;

moduleinterface ICPUcore 

{
    parameters:
        double speed;			// CPU core speed (in MIPS)
        double tick @unit(s);	// Tick time (in seconds)

    gates:
        input in;			// Input gate
        output out;		// Output gate

}
