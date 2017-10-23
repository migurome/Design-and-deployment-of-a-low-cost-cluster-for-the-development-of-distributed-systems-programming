package SIMCAN.simulations.cliServExample; 

import ned.DatarateChannel;
import SIMCAN.Nodes.Racks.*;
import SIMCAN.Nodes.Nodes.*;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.ethernet.EtherSwitch;

// ---------------------------------------------------------------
//   Definition of the scenario
// ---------------------------------------------------------------
network cliServExample{

   // -------------------------------------------------------
   //   Definition of main modules
   // -------------------------------------------------------
   submodules:

	// ---------------------------------------------------------------
	//   Network configurator
	// ---------------------------------------------------------------
	configurator: IPv4NetworkConfigurator;

	// ---------------------------------------------------------------
	//   Definition of switches
	// ---------------------------------------------------------------
	// Switch (GeneralSwitch)[2]
	switch_0:EtherSwitch;

	// ---------------------------------------------------------------
	//   Definition of Computing Nodes
	// ---------------------------------------------------------------
	// Computing (computingNodeExample)[0]
	nCmp0_computingNodeExample:singleNode;

	// ---------------------------------------------------------------
	//   Definition of Storage Nodes
	// ---------------------------------------------------------------
	// Storage (storageNodeExample)[1]
	nSto0_storageNodeExample:singleNode;

	// ---------------------------------------------------------------
	//   Connections section
	// ---------------------------------------------------------------
	connections allowunconnected:

	   nCmp0_computingNodeExample.ethg++ <--> Eth100M_channel <--> switch_0.ethg++;
	   nSto0_storageNodeExample.ethg++ <--> Eth100M_channel <--> switch_0.ethg++;

}