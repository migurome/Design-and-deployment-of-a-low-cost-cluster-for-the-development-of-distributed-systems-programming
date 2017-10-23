//
// Rack that contains "numBoards" NodeBoards and a FileConfigManager.
//
// This FileConfigManager is shared by all nodes contained in this rack. 
//
package SIMCAN.Nodes.Racks;

import SIMCAN.Nodes.Racks.NodeBoard;
import SIMCAN.Base.FileConfigManager;
import inet.nodes.ethernet.EtherSwitch;
import SIMCAN.Nodes.Nodes.*;

module SingleRack{
    
    parameters:
        int numBoards;		// Number of Node Boards.
        int nodesPerBoard;	// Number of nodes per Board.

        @display("bgb=317,205");

    submodules:

        // File configuration
        fileConfig: FileConfigManager {
            parameters:
                @display("p=50,50");
        }

        // Node array
        nodeBoard[numBoards]: NodeBoard {
            parameters:
                numNodes = nodesPerBoard;
                @display("i=device/lan-bus;p=164,97,row,70");
        }
        
         // Switch
        switch: EtherSwitch {
            parameters:
                @display("i=device/modem;p=184,124");
            gates:
                ethg[numBoards];
        }
       
    connections:

        // Connections between Nodeboards and the out connection of the rack
        for i=0..numBoards-1 {
            nodeBoard[i].ethg++ <--> Eth1G_channel <--> switch.ethg++;
        }
}


