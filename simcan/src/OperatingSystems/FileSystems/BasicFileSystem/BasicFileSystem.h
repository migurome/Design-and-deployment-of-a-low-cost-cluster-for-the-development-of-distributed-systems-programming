#ifndef __BASIC_FILE_SYSTEM_H_
#define __BASIC_FILE_SYSTEM_H_

#include <omnetpp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cSIMCAN_Base.h"
#include "SIMCAN_File.h"
#include "SIMCAN_BlockList_Message.h"
#include "SIMCAN_App_IO_Message.h"
#include "FileConfigManager.h"

/**
 * @class BasicFileSystem BasicFileSystem.h "BasicFileSystem.h"
 *
 * This module translates an I/O file request expressed as (fileName, offset, requestSize) to a list of disk blocks.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class BasicFileSystem: public cSIMCAN_Base{

	protected:		

		/** PreLoad File System Files? */
		bool preLoadFiles;
		
		/** Gate ID. Input gate from VM. */
		cGate* fromVMGate;

		/** Gate ID. Input gate from IOR. */
		cGate* fromIORGate;

		/** Gate ID. Output gate to VM. */
		cGate* toVMGate;

		/** Gate ID. Output gate to IOR. */
		cGate* toIORGate;

	    /** Pending message */
	    cMessage *pendingMessage;

		/** File list*/
		vector <SIMCAN_File> fileList;

		/** Open Time */
		static const const_simtime_t OPEN_TIME;

		/** Close Time */
		static const const_simtime_t CLOSE_TIME;
		
		/** Create Time */
		static const const_simtime_t CREATE_TIME;



	   /**
		* Destructor. 
		*/
		~BasicFileSystem();

	   /**
	 	* Module initialization.
	 	*/
	    virtual void initialize();

	   /**
	 	* Module ending.
	 	*/
	    virtual void finish();


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
		* Calculates the involved blocks (list of branches) of an I/O request and sends the request to Block Server.
		*
		* @param sm StandardMessage that contains the I/O request operation.
		*/
		void processIORequest (SIMCAN_Message *sm);

	   /**
		* PreLoad files in File System.
		* @return SIMCAN_OK if preLoad files is successful of SIMCAN_ERROR in another case.
		*/
		int preloadFSFiles();

	   /**
		* Inserts a new file inside the File System's table.
		*
		* @param fileSize File's size.
		* @param fileName File's name.
		* @return SIMCAN_OK if the file was been inserted correctly, or SIMCAN_ERROR if something was wrong.
		*/
		int insertNewFile (unsigned int fileSize, string fileName);

	   /**
		* Search a file in the file system.
		*
		* @param fileName File name.
		* @return Iterator to the corresponding entry of <b>fileName</b>
		*/
		vector <SIMCAN_File>::iterator searchFile (string fileName);

	   /**
		* Romoves a file from the File System's table.
		*
		* @param fileName File's name.
		*/
		void deleteFile (string fileName);


	   /**
		* Translate an user I/O request into a branch list.
		* If somethings goes wrong (file not exist, request out of file size,... ) the returned
		* string will be empty.
		*
		* @param sm_bl Block list Message.
		*/
		void translateIORequets (SIMCAN_BlockList_Message *sm_bl);

	   /**
		* Parse all files information to string. Debug purpose only!
		* @return All files stored on FS in string format.
		*/
		string FSFilesToString ();
};

#endif
