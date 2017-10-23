// Module that represents a Generic I/O Redirector
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.VirtualFileSystems;

moduleinterface IVirtualFileSystem

{
    parameters:
        double numFS;			// Number of File Systems

    gates:
        input fromMemory;		// Input gate from Memory
        input fromFS[];			// Input gates from File Systems

        output toMemory;		// Output gate to Memory
        output toFS[];			// Output gates to File Systems

}

