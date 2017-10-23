// Module that implements a GenericAppModule.
//
// This module contains the following subModules:
//
// - A GenericApp.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30
package SIMCAN.Applications;

import SIMCAN.Applications.Apps.IApp;

module AppModule
{
    parameters:
        string appType;			// Application type

    gates:
        input fromOS;			// Input gate from OS (Operating System)
        output toOS;			// Output gate to OS (Operating System)

    submodules:
        app: <appType> like IApp {
            parameters:
                @display("i=block/network;p=60,100");


        }
    connections allowunconnected:

        // Connections with Operating System
        fromOS --> app.fromOS;
        toOS <-- app.toOS;
}
