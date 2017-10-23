//
// This node contains a FileConfigManager that have to be configured locally.
//
package SIMCAN.Nodes.Nodes;

import SIMCAN.Base.FileConfigManager;

import inet.nodes.inet.NodeBase;
import inet.applications.ITCPApp;
import inet.transport.ITCP;
import SIMCAN.Applications.AppModule;
import SIMCAN.OperatingSystems.IOperatingSystemModule;
import SIMCAN.OperatingSystems.CPUs.ICPU_Module;
import SIMCAN.IO.BlockServers.BlockServerModules.BlockServerModule;


module singleNode extends NodeBase
{
    parameters:
        @labels(node,ethernet-node);
        string hostName = default("");															// Node's hostname      
        string tcpType = default(firstAvailable("TCP", "TCP_lwIP", "TCP_NSC", "TCP_None"));     // TCP type   
        string cpuModuleType = default("CPU_Module");											// CPU Module type
        string osModuleType = default("OperatingSystemModule");									// Operating System Module type
        int numFS;													// Number of File Systems
        int numApps;												// Number of Applications
        int numCPUs;												// Number of CPUs
        int numBlockServers;										// Number of Block Servers
        IPForward = default(false);  								// disable routing by default
        networkLayer.proxyARP = default(false);


    submodules:

        fileConfig: FileConfigManager {
            parameters:
                @display("p=558,60");
        }

        appModule[numApps]: AppModule {
            parameters:
                @display("i=old/telnet;p=519,186,row");
        }

        osModule: <osModuleType> like IOperatingSystemModule {
            parameters:
                numFS = numFS;
                numApps = numApps;
                numCPUs = numCPUs;
                numBlockServers = numBlockServers;
                @display("p=314,186;i=abstract/penguin");
            gates:
                fromApps[numApps];
                toApps[numApps];
                fromCPU[numCPUs];
                toCPU[numCPUs];
                fromBlockServers[numBlockServers];
                toBlockServers[numBlockServers];
        }

        cpuModule: <cpuModuleType> like ICPU_Module {
            parameters:
                numCPUs = numCPUs;
                @display("p=255,80;i=device/cpu");
            gates:
                fromOS[numCPUs];
                toOS[numCPUs];
        }

        bsModule[numBlockServers]: BlockServerModule {
            parameters:
                @display("i=device/drive;p=407,76,row");
        }

        tcp: <tcpType> like ITCP {
            parameters:
                @display("p=194,187");
        }


    connections allowunconnected:

        // Connections between Apps and Operating System
        for i=0..numApps-1 {
            appModule[i].toOS --> osModule.fromApps[i];
            appModule[i].fromOS <-- osModule.toApps[i];
        }

        // Connections between CPU and Operating System
        for i=0..numCPUs-1 {
            cpuModule.toOS[i] --> osModule.fromCPU[i];
            cpuModule.fromOS[i] <-- osModule.toCPU[i];
        }

        // Connections between Block Servers and Operating System
        for i=0..numBlockServers-1 {
            bsModule[i].toOS --> osModule.fromBlockServers[i];
            bsModule[i].fromOS <-- osModule.toBlockServers[i];
        }

        // Connections between Operating System and Network
        osModule.toNet_TCP --> tcp.appIn++;
        osModule.fromNet_TCP <-- tcp.appOut++;

        // INET internals...
        tcp.ipOut --> networkLayer.transportIn++;
        tcp.ipIn <-- networkLayer.transportOut++;
}
