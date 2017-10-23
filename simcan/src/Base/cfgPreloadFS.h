#ifndef __CFG_PRELOAD_FS_
#define __CFG_PRELOAD_FS_

#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SIMCAN_types.h"



/**
 * @class cfgPreloadFS cfgPreloadFS.h "cfgPreloadFS.h"
 *
 * Class that contains a list of preloaded files.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class CfgPreloadFS{

	struct cfgPreload_t{
			string fileName;			
			unsigned int sizeKB;
	};
	typedef struct cfgPreload_t preload_T;


	public:

		std::vector <preload_T> preloadVector;

		~CfgPreloadFS();
		CfgPreloadFS ();
		void parseFile (const char* fileName);
		int getNumFiles ();
		string getFileName (unsigned int index);
		unsigned int getSizeKB (unsigned int index);		
		string toString ();

};

#endif
