#ifndef __VIRTUAL_FILE_SYSTEM_H_
#define __VIRTUAL_FILE_SYSTEM_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_App_IO_Message.h"
#include "FileConfigManager.h"
using std::vector;


/**
 * @class VirtualFileSystem VirtualFileSystem.h "VirtualFileSystem.h"
 *
 * Class that represents a I/O Redirector
 *
 * This VFS contains information about the redirections to the
 * corresponding FS or to a corresponding APP module. (See FileConfigManager module for a more
 * detailed information about the config file).
 *
 * This module contains a table with paths. Each one of these paths is associated
 * to an index and a module type. This index correspond to a FS in the node's FS 
 * array or to an APP in node's APP array.
 *
 * Local operations are sent to local File Systems: Parameter remoteOperation=false
 * Remote operations are sent to corresponding client application: Parameter remoteOperation=true
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class VirtualFileSystem :public cSIMCAN_Base{

	/**
	 * Structure that represents a path table entry.
	 */
	struct PathEntry{
		string type;				/**< Destination module type (LOCAL or REMOTE) */
		string path;				/**< Path */
		unsigned int index;			/**< Connection ID */		
	};
	typedef struct PathEntry pathEntry;


	protected:
	  	
	    /** Number of File Systems. */
	    int numFS;
	    
	    /** NFS client index */
	    int nfsClientIndex;

	    /** Just the table. */
	    vector <pathEntry> pathTable;

		/** Input Memory from Cache. */
		cGate* fromMemoryGate;

		/** Output Memory to Cache. */
		cGate* toMemoryGate;

		/** Input gatess from File Systems. */
		cGate** fromFSGates;

		/** Output gates to File Systems. */
		cGate** toFSGates;		
		


	   /**
		* Destructor
		*/
		~VirtualFileSystem();


	   /**
	 	* Module initialization.
	 	*/
	    void initialize();

	   /**
	 	* Module ending.
	 	*/
	    void finish();


	private:

	   /**
		* Get the outGate to the module that sent <b>msg</b>
		* @param msg Arrived message.
		* @return. Gate Id (out) to module that sent <b>msg</b> or NOT_FOUND if gate not found.
		*/
		cGate* getOutGate (cMessage *msg);

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
		* Reads the config file and fills the path table.
		*/
		void parseConfigFile ();

	   /**
		* Parse the IOR Table contents to a string
		* @return IOR Table in string format.
		*/
		string VFSTableToSring ();

};

#endif


