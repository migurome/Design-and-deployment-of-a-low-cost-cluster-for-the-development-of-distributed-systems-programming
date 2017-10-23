// Module that implements a Generic Volume Manager Module.
//
// This module contains:
//
// - A GenericBlockManager.
// - A GenericBlockScheduler.
// - A GenericMemory.
//
// This module calculates the location of the blocks involved in an IO operation,
// split the request messages in N subRequest messages to block Servers, and wait to
// all these subRequest responses to send a response to the corresponding File System.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30


package SIMCAN.OperatingSystems.VolumeManagers;

import SIMCAN.OperatingSystems.VolumeManagers.BlockManagers.IBlockManager;
import SIMCAN.OperatingSystems.VolumeManagers.BlockSchedulers.IBlockScheduler;
import SIMCAN.OperatingSystems.Memories.Caches.ICache;


module VolumeManagerModule

{
    parameters:
        string blockManagerType;									// Block Manager type		
        string schedulerType = default ("NullBlockScheduler");		// Block Scheduler Type		
        string cacheType = default ("NullCache");					// Cache Type
        double numFS;												// Number of File Systems
        double numBlockServers;										// Number of Block Servers
		

    gates:
        input fromFS[];			// Input gates from File Systems
        input fromBlockServer[];	// Input gates from Block Servers   						

        output toFS[];			// Output gates from File Systems
        output toBlockServer[];	// Output gates from Block Servers   


    submodules:

        blockManager: <blockManagerType> like IBlockManager {
            parameters:
                numBlockServers = numBlockServers;
                @display("i=block/layer;p=100,100");

            gates:
                fromBlockServer[numBlockServers];
                toBlockServer[numBlockServers];
        }
        scheduler: <schedulerType> like IBlockScheduler {
            parameters:
                @display("i=block/classifier;p=100,200");

        }
        cache: <cacheType> like ICache {
            parameters:
                numInputs = numFS;
                @display("i=device/memory;p=100,300");


            gates:
                fromInput[numFS];
                toInput[numFS];
        }
    connections allowunconnected:

        // Conections with FSs
        for i=0..numFS-1 {
            fromFS++ --> cache.fromInput++;
            toFS++ <-- cache.toInput++;
        }

        // Conections between Cache and Scheduler
        cache.fromOutput <-- scheduler.toInput;
        cache.toOutput --> scheduler.fromInput;

        // Conections between Scheduler and Block Manager
        scheduler.fromOutput <-- blockManager.toScheduler;
        scheduler.toOutput --> blockManager.fromScheduler;

        // Connections with Block Servers
        for i=0..numBlockServers-1 {
            fromBlockServer++ --> blockManager.fromBlockServer++;
            toBlockServer++ <-- blockManager.toBlockServer++;
        }

}
