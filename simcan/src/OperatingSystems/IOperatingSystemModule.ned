// Module that represents a Generic Operating System Module.
//
// This module contains the following subModules:
//
// - A GenericServiceRedirector
// - A GenericNetworkService
// - A GenericCpuService
// - A GenericMemory
// - A GenericIORedirector
// - N GenericFSModule
// - A GenericVolumeManagerModule
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems;

moduleinterface IOperatingSystemModule

{
   parameters:        
        string serviceRedirectorType;	// Service Redirector type
        string networkServiceType;		// Network Service type
        string cpuSchedulerType;		// CPU Service type
        string memoryType;				// Memory type
        string vfsType;					// Virtual File System type		
        string fsModuleType;			// File System module type
        string vmModuleType;			// Volume Manager module type
        double numApps;					// Number of applications
        double numCPUs;					// Number of CPUs
        double numFS;					// Number of File Systems
        double numBlockServers;			// Number of Block Servers


    gates:
        input fromApps[];			// Input gates from Applications
        input fromBlockServers[];	// Input gates from Block Servers
        input fromNet_TCP;			// Input gate from Network (Ethernet TCP)
        input fromCPU[];			// Input gates from CPU

        output toApps[];			// Output gates to Applications
        output toBlockServers[];	// Output gates to Block Servers
        output toNet_TCP;			// Output gate to Network (Ethernet TCP)
        output toCPU[];				// Output gates to CPU

}
