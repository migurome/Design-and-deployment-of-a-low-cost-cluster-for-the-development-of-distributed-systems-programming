#ifndef __CFG_SERVERS_
#define __CFG_SERVERS_

#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SIMCAN_types.h"



/**
 * @class cfgServers cfgServers.h "cfgServers.h"
 *
 * Class that contains a list of servers.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class CfgServers{

	struct cfgServer_t{
			string hostName;
			unsigned int port;
			unsigned int id;
	};
	typedef struct cfgServer_t server_T;


	public:

		std::vector <server_T> serversVector;

		~CfgServers();
		CfgServers ();
		void parseFile (const char* fileName);
		int getNumServers ();
		string getHostName (unsigned int index);
		unsigned int getPort (unsigned int index);
		unsigned int getId (unsigned int index);
		string toString ();

};

#endif
