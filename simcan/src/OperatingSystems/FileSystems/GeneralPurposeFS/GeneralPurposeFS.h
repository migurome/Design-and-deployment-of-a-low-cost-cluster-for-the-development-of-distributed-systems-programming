#ifndef __GENERAL_PURPOSE_FS_H_
#define __GENERAL_PURPOSE_FS_H_

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
 * @class GeneralPurposeFS GeneralPurposeFS.h "GeneralPurposeFS.h"
 *
 * This module translates an I/O file request expressed as (fileName, offset, requestSize) to a list of disk blocks.
 *
 *
 * To calculate the branch list of each file, this module uses distribution functions like:<br>
 *
 *	<b>Weibull</b>
 *
 *		To use the Weibull distribution is neccessary to load the next files:<br>
 *
 *			- weibull/bunchSize_ext2_Alpha.txt<br>
 *			- weibull/bunchSize_ext2_Beta.txt<br>
 *			- weibull/bunchSize_reiserFS_Alpha.txt<br>
 *			- weibull/bunchSize_reiserFS_Beta.txt<br>
 *			- weibull/contiguous_ext2_Alpha.txt<br>
 *			- weibull/contiguous_ext2_Beta.txt<br>
 *			- weibull/contiguous_reiserFS_Alpha.txt<br>
 *			- weibull/contiguous_reiserFS_Beta.txt<br>
 *			- weibull/holeSize_ext2_Alpha.txt<br>
 *			- weibull/holeSize_ext2_Beta.txt<br>
 *			- weibull/holeSize_reiserFS_Alpha.txt<br>
 *			- weibull/holeSize_reiserFS_Beta.txt<br><br>
 *
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class GeneralPurposeFS: public cSIMCAN_Base{

	protected:		

		/** Maximum number of blocks */
		unsigned long int maxBlocks;

		/** Number of used blocks */
		unsigned long int usedBlocks;
		
		/** File System block Size (in bytes) */
		unsigned int fsBlockSize_b;

		/** Initial Filled disk ratio. */
		double diskRatio;

		/** File System Type */
		string fsType;

		/** Path that contains the weibull distributions */
		string simcanHomePath;

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


	    /** Weibull Alpha parameter table to calculate the bunchSize (to Ext2 File System) */
	    float bunchSize_ext2_Alpha_table [10][5];

	    /** Weibull Beta parameter table to calculate the bunchSize (to Ext2 File System) */
		float bunchSize_ext2_Beta_table [10][5];

		/** Weibull Alpha parameter table to calculate the bunchSize (to reiser File System) */
		float bunchSize_reiserFS_Alpha_table [10][5];

		/** Weibull Beta parameter table to calculate the bunchSize (to reiser File System) */
		float bunchSize_reiserFS_Beta_table [10][5];


		/** Weibull Alpha parameter table to calculate the contiguous probability (to Ext2 File System) */
		float contiguous_ext2_Alpha_table [10][5];

		/** Weibull Beta parameter table to calculate the contiguous probability (to Ext2 File System) */
		float contiguous_ext2_Beta_table [10][5];

		/** Weibull Alpha parameter table to calculate the contiguous probability (to reiser File System) */
		float contiguous_reiserFS_Alpha_table [10][5];

		/** Weibull Beta parameter table to calculate the contiguous probability (to reiser File System) */
		float contiguous_reiserFS_Beta_table [10][5];


		/** Weibull Alpha parameter table to calculate the holeSize (to Ext2 File System) */
		float holeSize_ext2_Alpha_table [10][5];

		/** Weibull Beta parameter table to calculate the holeSize (to Ext2 File System) */
		float holeSize_ext2_Beta_table [10][5];

		/** Weibull Alpha parameter table to calculate the holeSize (to reiser File System) */
		float holeSize_reiserFS_Alpha_table [10][5];

		/** Weibull Beta parameter table to calculate the holeSize (to reiser File System) */
		float holeSize_reiserFS_Beta_table [10][5];



		/** Open Time */
		static const const_simtime_t OPEN_TIME;

		/** Close Time */
		static const const_simtime_t CLOSE_TIME;
		
		/** Create Time */
		static const const_simtime_t CREATE_TIME;

		/** Weibull Alpha parameter to calculate the bunchSize (to Ext2 File System) */
		static const string bunchSize_ext2_Alpha_file;

		/** Weibull Beta parameter to calculate the bunchSize (to Ext2 File System) */
		static const string bunchSize_ext2_Beta_file;

		/** Weibull Alpha parameter to calculate the bunchSize (to reiser File System) */
		static const string bunchSize_reiserFS_Alpha_file;

		/** Weibull Beta parameter to calculate the bunchSize (to reiser File System) */
		static const string bunchSize_reiserFS_Beta_file;

		/** Weibull Alpha parameter to calculate the contiguous probability (to Ext2 File System) */
		static const string contiguous_ext2_Alpha_file;

		/** Weibull Beta parameter to calculate the contiguous probability (to Ext2 File System) */
		static const string contiguous_ext2_Beta_file;

		/** Weibull Alpha parameter to calculate the contiguous probability (to reiser File System) */
		static const string contiguous_reiserFS_Alpha_file;

		/** Weibull Beta parameter to calculate the contiguous probability (to reiser File System) */
		static const string contiguous_reiserFS_Beta_file;

		/** Weibull Alpha parameter to calculate the holeSize (to Ext2 File System) */
		static const string holeSize_ext2_Alpha_file;

		/** Weibull Beta parameter to calculate the holeSize (to Ext2 File System) */
		static const string holeSize_ext2_Beta_file;

		/** Weibull Alpha parameter to calculate the holeSize (to reiser File System) */
		static const string holeSize_reiserFS_Alpha_file;

		/** Weibull Beta parameter to calculate the holeSize (to reiser File System) */
		static const string holeSize_reiserFS_Beta_file;

		/** Relative path where the Weibull table files are stored */
		static const string relative_Weibull_Files_Path;



	   /**
		* Destructor. 
		*/
		~GeneralPurposeFS();

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
		* Calculate the list of branches that composes a file of size <b>fileSize</b> (Ext2 FS).
		*
		* @param fileSize Size of the file to calculate its list of branches.
		* @param file SIMCAN File object
		*/
		void calculateExt2BranchList(unsigned int fileSize, SIMCAN_File& file);


	   /**
		* Calculate the list of branches that composes a file of size <b>fileSize</b> (Reiser FS).
		*
		* @param fileSize Size of the file to calculate its list of branches.
		* @param file SIMCAN File object
		*/
		void calculateReiserFSBranchList(unsigned int fileSize, SIMCAN_File& file);


	   /**
		* Load the Weibull (alpha & beta) parameters (Ext2 FS).
		*/
		void loadExt2WeibullParameters();


	   /**
		* Load the Weibull (alpha & beta) parameters (Reiser FS).
		*/
		void loadReiserFSWeibullParameters();


	   /**
		* Parse the contents of all weibull parameters tables (Ext2 FS) to string.
		* @return Tables in string format.
		*/
		string ext2WeibullTablesToString();


	   /**
		* Parse the contents of all weibull parameters tables (Reiser FS) to string.
		* @return Tables in string format.
		*/
		string reiserFSWeibullTablesToString();


	   /**
		* Parse all files information to string. Debug purpose only!
		* @return All files stored on FS in string format.
		*/
		string FSFilesToString ();


	   /**
		* Gets a corresponding file size category (maximum) to a given file size.
		* @param fileSize Current file size.
		* @return Maximum file size.
		*/
		unsigned int getCategoryMaxSize (unsigned int fileSize);


	   /**
		* Gets a corresponding diskRatio category. This is the row (x axis) in the weibull tables.
		* @return Disk ratio category (row index)
		*/
		int getDiskRatio_x_axis ();


	   /**
		* Gets a corresponding file size category. This is the column (y axis) in the weibull tables.
		* @param fileSize Current file size.
		* @return File size category (column index)
		*/
		int getFileSizeCategory_y_axis (unsigned int fileSize);
};

#endif
