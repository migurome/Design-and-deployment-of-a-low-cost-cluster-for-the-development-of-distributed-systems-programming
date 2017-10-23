#ifndef __CFG_MPI_
#define __CFG_MPI_

#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SIMCAN_types.h"



/**
 * @class CfgMPI cfgMPI.h "cfgMPI.h"
 *
 * Class that contains all parameters for MPI Apps
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class CfgMPI{
	

	struct cfgMPI_t{
	    string appInstance;
		string hostName;
		unsigned int port;
		unsigned int rank;
	};
	typedef struct cfgMPI_t cfgMPIProcess;

	struct cfgMPIRoot_t{
        string appInstance;
        unsigned int rootIndex;
    };
    typedef struct cfgMPIRoot_t cfgMPIRootProcess;

    std::vector <cfgMPIProcess> processVector;
    std::vector <cfgMPIRootProcess> rootVector;


	public:
		CfgMPI ();
		~CfgMPI();
		void parseFile (const char* fileName);		
		string getHostNameByIndex (int index);
		string getAppInstanceByIndex (int index);
		unsigned int getPortByIndex (int index);
		unsigned int getRankByIndex (int index);
		unsigned int getTotalNumProcesses ();

		unsigned int getRootProcessByApp (string appInstance);
		unsigned int getNumProcessesByApp (string appInstance);
		string toString ();


	private:
		unsigned int getIndexOfRootProcess (string appInstance);

};

#endif
