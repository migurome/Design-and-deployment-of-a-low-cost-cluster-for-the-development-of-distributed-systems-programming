package SIMCAN.simulations.scenario; 

import ned.DatarateChannel;
import SIMCAN.Nodes.Racks.*;
import SIMCAN.Nodes.Nodes.*;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.ethernet.EtherSwitch;

// ---------------------------------------------------------------
//   Definition of the scenario
// ---------------------------------------------------------------
network scenario_100M{

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
	// Switch (GeneralSwitch)[1]
	switch_0:EtherSwitch;

	// ---------------------------------------------------------------
	//   Definition of Computing Nodes
	// ---------------------------------------------------------------
	// Computing (ClientNode)[0]
	nCmp0_ClientNode:singleNode;

	// ---------------------------------------------------------------
	//   Definition of Storage Nodes
	// ---------------------------------------------------------------
	// Storage (ServerNode)[2]
	nSto0_ServerNode:singleNode;

	// ---------------------------------------------------------------
	//   Connections section
	// ---------------------------------------------------------------
	connections allowunconnected:

	   nCmp0_ClientNode.ethg++ <--> Eth100M_channel <--> switch_0.ethg++;
	   nSto0_ServerNode.ethg++ <--> Eth100M_channel <--> switch_0.ethg++;

}