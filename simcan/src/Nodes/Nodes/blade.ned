//
// This node is used on Racks and NodeBoards that have a global FileConfigManager.
//
package SIMCAN.Nodes.Nodes;

import inet.nodes.inet.NodeBase;
import inet.applications.ITCPApp;
import inet.transport.ITCP;
import SIMCAN.Applications.AppModule;
import SIMCAN.OperatingSystems.IOperatingSystemModule;
import SIMCAN.OperatingSystems.CPUs.ICPU_Module;
import SIMCAN.IO.BlockServers.BlockServerModules.BlockServerModule;


module blade extends NodeBase
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

        appModule[numApps]: AppModule {
            parameters:
                @display("i=old/telnet;p=490,102,row");
        }

        osModule: <osModuleType> like IOperatingSystemModule {
            parameters:
                numFS = numFS;
                numApps = numApps;
                numCPUs = numCPUs;
                numBlockServers = numBlockServers;
                @display("p=328,117;i=abstract/penguin");
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
                @display("p=216,51;i=device/cpu");
            gates:
                fromOS[numCPUs];
                toOS[numCPUs];
        }

        bsModule[numBlockServers]: BlockServerModule {
            parameters:
                @display("i=device/drive;p=329,49,row");
        }

        tcp: <tcpType> like ITCP {
            parameters:
                @display("p=329,194");
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

