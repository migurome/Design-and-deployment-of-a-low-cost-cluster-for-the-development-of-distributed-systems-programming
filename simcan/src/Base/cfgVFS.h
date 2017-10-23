#ifndef __CFG_VFS_
#define __CFG_VFS_

#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SIMCAN_types.h"



/**
 * @class cfgVFS cfgVFS.h "cfgVFS.h"
 *
 * Class that contains the VFS configuration file.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class CfgVFS{

	struct cfgVFS_t{
			string type;
			string path;		
			unsigned int index;
	};
	typedef struct cfgVFS_t vfs_T;


	public:

		std::vector <vfs_T> vfsVector;

		~CfgVFS ();
		CfgVFS ();
		void parseFile (const char* fileName);
		int getNumEntries ();
		string getType (unsigned int index);
		string getPath (unsigned int index);
		unsigned int getIndex (unsigned int index);
		string toString ();
};

#endif
