//
// Rack that contains "numBoards" NodeBoards and a FileConfigManager.
//
// This FileConfigManager is shared by all nodes contained in this rack. 
//
package SIMCAN.Nodes.Racks;

import SIMCAN.Nodes.Racks.NodeBoard;
import SIMCAN.Base.FileConfigManager;

module Rack
{
    parameters:
        int numBoards;		// Number of Node Boards.
        int nodesPerBoard;	// Number of nodes per Board.

        @display("bgb=317,205");
    gates:
        inout ethg[numBoards];


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
       
    connections:

        // Connections between Nodeboards and the out connection of the rack
        for i=0..numBoards-1 {
            nodeBoard[i].ethg++ <--> ethg++;
        }
}


