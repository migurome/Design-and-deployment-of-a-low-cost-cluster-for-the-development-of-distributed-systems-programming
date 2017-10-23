//
// Node Board that contains "numNode" nodes and a switch
//
package SIMCAN.Nodes.Racks;

import inet.nodes.ethernet.EtherLink;
import SIMCAN.Nodes.Nodes.*;
import inet.nodes.ethernet.EtherSwitch;


module NodeBoard
{
    parameters:
        int numNodes;
        string channelType;

        @display("bgb=295,249");
    gates:
        inout ethg[] @labels(EtherFrame-conn);


    submodules:

        // Node array
        node[numNodes]: blade {
            parameters:
                @display("i=device/server;p=67,124,row,50");
        }

        // Switch
        switch: EtherSwitch {
            parameters:
                @display("i=device/modem;p=184,124");
            gates:
                ethg[numNodes+1];
        }

    connections:

        // Connections between Nodes and Switch
        for i=0..numNodes-1 {
            node[i].ethg++ <--> <channelType> like IEthChannel <--> switch.ethg++;
        }

        // Connection between switch and outside
        switch.ethg++ <--> ethg++;
}

