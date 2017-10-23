// Module that represents a Generic CPU Scheduler.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30


package SIMCAN.OperatingSystems.CPU_Schedulers;

moduleinterface ICPU_Scheduler

{
    parameters:
        double numCPUs;			// Number of CPUs

    gates:
        input fromServiceRedirector;		// Input gate from Service Redirector
        input fromCPU[];					// Input gates from CPU

        output toServiceRedirector;		// Output gate to Service Redirector
        output toCPU[];					// Output gates to CPU

}
