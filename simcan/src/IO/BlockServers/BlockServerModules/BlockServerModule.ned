// Module that implements a Generic Block Server Module.
//
// This module contains:
//
// - A GenericDisk.
// - A GenericMemory.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.IO.BlockServers.BlockServerModules;

import SIMCAN.IO.Disks.IDisk;
import SIMCAN.OperatingSystems.Memories.Caches.ICache;

module BlockServerModule

{
    parameters:
        string diskType;		// Disk Type
        string cacheType;		// Cache Type

        @display("bgb=331,134");
    gates:
        input fromOS;		// Input gate from Operating System	
        output toOS;		// Output gate to Operating System	


    submodules:

        disk: <diskType> like IDisk {
            parameters:
                @display("i=device/disk;p=51,61");

        }
        cache: <cacheType> like ICache {
            parameters:
                numInputs = 1;
                @display("i=device/memory;p=244,61");

            gates:
                fromInput[1];
                toInput[1];
        }
    connections allowunconnected:

        // Conections with Operating System
        fromOS --> cache.fromInput[0];
        toOS <-- cache.toInput[0];

        // Conections between Cache and Disk
        cache.fromOutput <-- disk.out;
        cache.toOutput --> disk.in;

}
