// Module that represents a Generic CPU Module.
//
// This module contains the following subModules:
//
// - N GenericProcessor
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.CPUs;

moduleinterface ICPU_Module

{
    parameters:
        double numCPUs;			// Number of CPU cores        
        string cpuCoreType;		// Type of each CPU core

    gates:
        input fromOS[];		// Input gates from Operating System
        output toOS[];		// Output gates to Operating System

}


