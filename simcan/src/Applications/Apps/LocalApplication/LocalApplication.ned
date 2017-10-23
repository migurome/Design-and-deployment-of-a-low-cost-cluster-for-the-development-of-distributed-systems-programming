// Module that implements a GenericApp.
//
// Example of a simple application that performs a read-cpu-write loop.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.Applications.Apps.LocalApplication;

import SIMCAN.Applications.SimcanAPI.SimcanAPI;


simple LocalApplication extends SimcanAPI{
    
    parameters:
        int inputDataSize @unit(MiB) = default (10MiB);		// Size of the data to be read in each iteration
        int outputDataSize @unit(MiB) = default (5MiB);		// Size of the data to be written in each iteration
        string inputFile = default ("/inputFile_0.dat");	// Input file
        string outputFile = default ("/outputFile_0.dat");	// Output file        
        int MIs = default (100000);							// Number of MIs to execute in each iteration
        int iterations = default (5);						// Number of iterations to be performed      
         
        @display("i=old/telnet");							//a
        @class(LocalApplication);							//b
}


