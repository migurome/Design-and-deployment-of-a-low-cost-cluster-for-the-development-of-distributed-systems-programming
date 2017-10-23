#ifndef __FILE_CONFIG_MANAGER_H_
#define __FILE_CONFIG_MANAGER_H_

#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include "cSIMCAN_Base.h"
#include "cfgServers.h"
#include "cfgMPI.h"
#include "cfgPreloadFS.h"
#include "cfgVFS.h"


/**
 * @class FileConfigManager FileConfigManager.h "FileConfigManager.h"
 *
 * Class that manages all configuration files.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class FileConfigManager :public cSIMCAN_Base{

	public:

		CfgMPI* mpiEnv;
		CfgServers* serverNfsCfg;		
		CfgServers* serverPfsCfg;
		CfgPreloadFS* preloadFS;
		CfgVFS* vfsCfg;
		
		
	  /**
	   * Destructor 
	   */
	   ~FileConfigManager();

	   /**
		* Module initialization.
		*
		* @param stage Stage fase.
		*/
	   virtual void initialize(int stage);

	   /**
		* Init the stage value.
		*/
	   virtual int numInitStages() const  { return 2; }

	   /**
		* Function that handles incoming messages.
		*
		* @param msg incoming message.
		*/
		virtual void handleMessage (cMessage *msg);
		
		 /**
		* Process a self message.
		* @param msg Self message.
		*/
		void processSelfMessage (cMessage *msg);

	   /**
		* Process a request message.
		* @param sm Request message.
		*/
		void processRequestMessage (SIMCAN_Message *sm);

	   /**
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);
		
	   /**
		* Get the outGate to the module that sent <b>msg</b>
		* @param msg Arrived message.
		* @return. Gate Id (out) to module that sent <b>msg</b> or NOT_FOUND if gate not found.
		*/
		cGate* getOutGate (cMessage *msg);


	   /**
	 	* Module ending.
	 	*/
	    void finish();


	    CfgServers* getServerNfsCfg();
	    CfgServers* getServerPfsCfg();
	    CfgPreloadFS* getPreloadFS();
	    CfgVFS* getVfsCfg();
	    CfgMPI* getMPIEnv();
};

#endif

