// Module that implements a Generic CPU Scheduler.
//
// CPU scheduler that implements a Round-Robin policy
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.CPU_Schedulers.CPU_Scheduler_RR;

import SIMCAN.OperatingSystems.IOperatingSystemModule;
import SIMCAN.OperatingSystems.CPU_Schedulers.ICPU_Scheduler;

simple CPU_Scheduler_RR like ICPU_Scheduler

{
    parameters:
        double numCPUs;						// Number of CPUs
        int quantum = default (10);			// Quantum (in ticks)

        @display("i=block/classifier");
    gates:
        input fromServiceRedirector;		// Input gate from Service Redirector
        input fromCPU[];					// Input gates from CPU

        output toServiceRedirector;		// Output gate to Service Redirector
        output toCPU[];					// Output gates to CPU

}
