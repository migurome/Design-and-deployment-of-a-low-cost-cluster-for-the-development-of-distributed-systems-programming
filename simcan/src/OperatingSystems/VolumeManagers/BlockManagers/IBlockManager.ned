// Module that represents a Generic Block Manager.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.VolumeManagers.BlockManagers;

moduleinterface IBlockManager

{
    parameters:
        double numBlockServers;		// Number of Block Servers		

    gates:
        input fromScheduler;		// Input gate from Scheduler
        input fromBlockServer[];	// Input gates from Block Servers		

        output toScheduler;			// Output gate to Scheduler
        output toBlockServer[];		// Output gates to Block Servers	

}
