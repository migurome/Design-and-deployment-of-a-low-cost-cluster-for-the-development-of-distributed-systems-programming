// Module that represents a Generic FS (File System)
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.FileSystems;

moduleinterface IFS

{
    gates:
        input fromVM;		// Input gate from VM (Volume Manager)
        input fromIOR;	// Input gate from IOR (I/O Redirector)

        output toVM;		// Output gate to VM (Volume Manager)
        output toIOR;		// Output gate to IOR (I/O Redirector)

}

