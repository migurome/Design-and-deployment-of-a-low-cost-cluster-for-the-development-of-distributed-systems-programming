#ifndef _SIMCAN_FILE_H_
#define _SIMCAN_FILE_H_

//#include "stdio.h"
//#include "stdlib.h"
#include "SIMCAN_types.h"
#include <omnetpp.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

/**
 * @class SIMCAN_File SIMCAN_File.h "SIMCAN_File.h"
 *
 * Base class that contains a branch list.
 * The branchList contains a set of pairs offset:branchSize.
 * 
 * The branchSize unit is BYTES_PER_SECTOR bytes.  
 *
 */
class SIMCAN_File{

	private:

		/** File Size (in bytes) */
		unsigned int fileSize;

		/** File Name */
		string fileName;

		/** Branch list */
		vector <fileBranch> branchList;

	public:

	   /**
		* Destructor.
		*/
		~SIMCAN_File();

	   /**
		* Gets the file size
		* @return File size (in bytes)
		*/
		unsigned int getFileSize ();

	   /**
		* Sets the file size
		* @param newSize File size (in bytes)
		*/
		void setFileSize (unsigned int newSize);		
		
		/**
		 * Updates the file size.
		 */
		void updateFileSize ();

	   /**
		* Gets the file name
		* @return File name
		*/
		string getFileName ();

	   /**
		* Sets the file name
		* @param newName File name
		*/
		void setFileName (string newName);

	   /**
		* Gets the number of branches
		* @return Number of branches
		*/
		unsigned int getNumberOfBranches ();

	   /**
		* Gets the size of branch number <b>branchNumber</b>
		* @return Branch size
		*/
		size_blockList_t getBranchSize (unsigned int branchNumber);

	   /**
		* Gets the offset of branch number <b>branchNumber</b>
		* @return Branch offset
		*/
		off_blockList_t getBranchOffset (unsigned int branchNumber);

	   /**
		* Adds a new Branch to brach list.
		* @param offset Branch offset
		* @param size Branch size (in sectors)
		*/
		void addBranch (off_blockList_t offset, size_blockList_t size);
				
	   /**
		* Add a complete block that contains <b>sector</b> to the branch list.
		* @param sector Sector to add.
		* @param blockSize Block size (in bytes)
		*/
		void addBlock (off_blockList_t sector, unsigned int blockSize);

	   /**
		* Gets the number of sectors (on disk) of current file.
		* @return Number of sectors
		*/
		size_blockList_t getTotalSectors ();		
		
	   /**
		* Calculates if sector is contained on current request.
		* @param sector to search on current request.
		* @return True if this request cotains sector or false in another case.
		*/
		bool containThisSector (size_blockList_t sector);

	   /**
		* Removes current branch list.
		*/
		void clearBranchList();

	   /**
		* Parses the File information to string
		* @param withBranches Parses branches information!
		* @param printContents Print contents?
		* @return A string with file info.
		*/
		string contentsToString (bool withBranches, bool printContents);
};

#endif /*_SIMCAN_FILE_H_*/
