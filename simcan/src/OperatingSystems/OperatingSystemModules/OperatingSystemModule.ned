// Module that implements a Generic Operating System Module.
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

package SIMCAN.OperatingSystems.OperatingSystemModules;

import SIMCAN.OperatingSystems.ServiceRedirectors.IServiceRedirector;
import SIMCAN.OperatingSystems.IOperatingSystemModule;
import SIMCAN.OperatingSystems.NetworkServices.INetworkService;
import SIMCAN.OperatingSystems.CPU_Schedulers.ICPU_Scheduler;
import SIMCAN.OperatingSystems.Memories.MainMemories.IMainMemory;
import SIMCAN.OperatingSystems.VirtualFileSystems.IVirtualFileSystem;
import SIMCAN.OperatingSystems.FileSystems.FSModule;
import SIMCAN.OperatingSystems.VolumeManagers.VolumeManagerModule;


module OperatingSystemModule like IOperatingSystemModule
{
    parameters:
        double numApps;													// Number of applications
        double numCPUs;													// Number of CPUs
        double numFS;													// Number of File Systems
        double numBlockServers;											// Number of Block Servers

        string serviceRedirectorType = default("ServiceRedirector");	// Service Redirector type
        string vfsType = default("VirtualFileSystem");					// Virtual File System type		
        string fsModuleType = default("FSModule");						// File System module type
        string vmModuleType = default("VolumeManagerModule");			// Volume Manager module type
        string networkServiceType;										// Network Service type
        string cpuSchedulerType;										// CPU Service type
        string memoryType;												// Memory type     
        @display("bgb=746,290");


    gates:
        input fromApps[];			// Input gates from Applications
        input fromBlockServers[];	// Input gates from Block Servers
        input fromNet_TCP;			// Input gate from Network (Ethernet TCP)
        input fromCPU[];			// Input gates from CPU

        output toApps[];			// Output gates to Applications
        output toBlockServers[];	// Output gates to Block Servers
        output toNet_TCP;			// Output gate to Network (Ethernet TCP)
        output toCPU[];				// Output gates to CPU


    submodules:

        serviceRedirector: <serviceRedirectorType> like IServiceRedirector {
            parameters:
                numApps = numApps;
                @display("p=499,108;i=old/bwgen");
            gates:
                fromApps[numApps];
                toApps[numApps];

        }
        networkService: <networkServiceType> like INetworkService {
            @display("p=677,108;i=device/card");
        }
        cpuScheduler: <cpuSchedulerType> like ICPU_Scheduler {
            parameters:
                numCPUs = numCPUs;
                @display("p=499,195;i=block/classifier");
            gates:
                fromCPU[numCPUs];
                toCPU[numCPUs];

        }
        memory: <memoryType> like IMainMemory {
            parameters:
                numInputs = 1;
                @display("p=368,108;i=device/memory");
            gates:
                fromInput[1];
                toInput[1];

        }
        vfs: <vfsType> like IVirtualFileSystem {
            parameters:
                numFS = numFS;
                @display("p=280,108;i=block/dispatch");
            gates:
                fromFS[numFS];
                toFS[numFS];

        }

        fsModule[numFS]: FSModule {

            @display("p=177,108;i=block/table2");
        }
        vmModule: VolumeManagerModule {
            parameters:
                numBlockServers = numBlockServers;
                numFS = numFS;
                @display("p=59,108;i=device/drive");
            gates:
                fromFS[numFS];
                toFS[numFS];
                fromBlockServer[numBlockServers];
                toBlockServer[numBlockServers];


        }
    connections allowunconnected:


        // Connections between Service Redirector and Applications
        for i=0..numApps-1 {
            serviceRedirector.fromApps[i] <-- fromApps[i];
            serviceRedirector.toApps[i] --> toApps[i];
        }


        // Connections between Service Redirector and Network Service
        serviceRedirector.fromNet <-- networkService.toServiceRedirector;
        serviceRedirector.toNet --> networkService.fromServiceRedirector;


        // Connections between Service Redirector and CPU Service
        serviceRedirector.fromCPU <-- cpuScheduler.toServiceRedirector;
        serviceRedirector.toCPU --> cpuScheduler.fromServiceRedirector;


        // Connections between Service Redirector and Memory	
        serviceRedirector.fromMemory <-- memory.toInput[0];
        serviceRedirector.toMemory --> memory.fromInput[0];


        // Connections between Memory and I/O Redirector		
        memory.fromOutput <-- vfs.toMemory;
        memory.toOutput --> vfs.fromMemory;


        // Connections between Virtual File System and File Systems
        for i=0..numFS-1 {
            vfs.fromFS[i] <-- fsModule[i].toIOR;
            vfs.toFS[i] --> fsModule[i].fromIOR;
        }


        // Connections between File Systems and Volume Manager
        for i=0..numFS-1 {
            vmModule.fromFS[i] <-- fsModule[i].toVM;
            vmModule.toFS[i] --> fsModule[i].fromVM;
        }


        // Connections between Volume Manager and Block Server
        for i=0..numBlockServers-1 {
            vmModule.fromBlockServer[i] <-- fromBlockServers[i];
            vmModule.toBlockServer[i] --> toBlockServers[i];
        }


        // Connections between Network Service and Networks
        networkService.fromNetTCP <-- fromNet_TCP;
        networkService.toNetTCP --> toNet_TCP;


        // Connections between CPU Scheduler and CPU
        for i=0..numCPUs-1 {
            cpuScheduler.fromCPU[i] <-- fromCPU[i];
            cpuScheduler.toCPU[i] --> toCPU[i];
        }


}
