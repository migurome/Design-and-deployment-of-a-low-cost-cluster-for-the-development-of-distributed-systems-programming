// Module that implements a Generic Network Service.
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.NetworkServices.NullNetworkService;

import SIMCAN.OperatingSystems.NetworkServices.INetworkService;

simple NullNetworkService like INetworkService{

    gates:
        input fromServiceRedirector;		// Input gate from Service Redirector
        input fromNetTCP;					// Input gate from Network (TCP)

        output toServiceRedirector;			// Output gate to Service Redirector
        output toNetTCP;					// Output gate to Network (TCP)

}
