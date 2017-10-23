#ifndef _SIMCAN_MEMORY_BLOCK_H_
#define _SIMCAN_MEMORY_BLOCK_H_

//#include "stdio.h"
//#include "stdlib.h"
#include "SIMCAN_types.h"
#include <omnetpp.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

/**
 * @class SIMCAN_MemoryBlock SIMCAN_MemoryBlock.h "SIMCAN_MemoryBlock.h"
 *
 * Base class that contains a cache block.
 *  
 */
class SIMCAN_MemoryBlock{
	
	private:	
		   	
		/** File Name */
		string fileName;
				
		/** File Size (in bytes) */
		unsigned int offset;		
		
		/** BlockSize */
		unsigned int blockSize;	
		
		/** is Pending? */
		bool isPending;
	
	public:
	
	
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
		* Gets the offset
		* @return offset (in bytes)
		*/
		unsigned int getOffset ();
		
	   /**
		* Sets the offset
		* @param newOffset (in bytes)
		*/
		void setOffset (unsigned int newOffset);
		
		
	   /**
		* Gets the blockSize
		* @return blockSize (in bytes)
		*/
		unsigned int getBlockSize ();
		
	   /**
		* Sets the blockSize
		* @param newBlockSize (in bytes)
		*/
		void setBlockSize (unsigned int newBlockSize);
		
		
	   /**
		* Is a pending block?
		* @return true if current block is pending or falsein another case.
		*/
		bool getIsPending ();
		
		
	   /**
		* Sets the block state
		* @param newIsPending Block state
		*/
		void setIsPending (bool newIsPending);				
		
		
	   /**
		* Parses the cache block info to string. Debug purpose only!
		* 
		*/
		string memoryBlockToString ();  			 			
};

#endif /*_SIMCAN_MemoryBlock_H_*/
