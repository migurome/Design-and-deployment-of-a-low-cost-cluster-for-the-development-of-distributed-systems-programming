// Module that implements a Generic CPU Module.
//
// This module contains the following subModules:
//
// - N GenericProcessor
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.CPUs.CPU_Modules;

import SIMCAN.OperatingSystems.IOperatingSystemModule;
import SIMCAN.OperatingSystems.CPUs.CPUcores.ICPUcore;
import SIMCAN.OperatingSystems.CPUs.ICPU_Module;

module CPU_Module like ICPU_Module

{
    parameters:
        double numCPUs;								// Number of CPU cores        
        string cpuCoreType = default("CPUcore");	// Type of each CPU core

        @display("bgb=280,141");
    gates:
        input fromOS[];		// Input gates from Operating System
        output toOS[];		// Output gates to Operating System


    submodules:
        CPUcore[numCPUs]: <cpuCoreType> like ICPUcore {

            @display("p=136,59;i=block/network");
        }
    connections allowunconnected:

        // Connections between all CPU cores and outside
        for i=0..numCPUs-1 {
            CPUcore[i].in <-- fromOS[i];
            CPUcore[i].out --> toOS[i];
        }

}
