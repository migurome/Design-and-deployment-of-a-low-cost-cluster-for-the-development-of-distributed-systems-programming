// Module that implements a Generic Service Redirector.
//
// This module redirects the requests to the corresponding module:
//	 - Network Service for remote requests.
//	 - CPU Service for local computing requests.
//	 - I/O Service for local data requests .
//
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.ServiceRedirectors.ServiceRedirector;

import SIMCAN.OperatingSystems.ServiceRedirectors.IServiceRedirector;

simple ServiceRedirector like IServiceRedirector

{
    parameters:
        double numApps;				// Number of applications

    gates:
        input fromApps[];				// Input gates from Applications
        input fromMemory;				// Input gate from Memory
        input fromNet;					// Input gate from Network
        input fromCPU;					// Input gate from CPU

        output toApps[];				// Output gates to Applications
        output toMemory;				// Output gate to Memory
        output toNet;					// Output gate to Network
        output toCPU;					// Output gate to CPU

}
