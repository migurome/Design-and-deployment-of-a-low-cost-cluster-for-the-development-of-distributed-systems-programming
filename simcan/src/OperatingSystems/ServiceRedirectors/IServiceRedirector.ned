// Module that represents a Generic Service Redirector.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.ServiceRedirectors;

moduleinterface IServiceRedirector

{
    parameters:
        double numApps;			// Number of applications

    gates:
        input fromApps[];		// Input gates from Applications
        input fromMemory;		// Input gate from Memory
        input fromNet;			// Input gate from Network
        input fromCPU;			// Input gate from CPU

        output toApps[];		// Output gates to Applications
        output toMemory;		// Output gate to Memory
        output toNet;			// Output gate to Network
        output toCPU;			// Output gate to CPU

}
