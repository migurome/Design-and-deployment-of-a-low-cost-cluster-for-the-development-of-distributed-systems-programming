#ifndef __MAINMEMORY_BLOCKCACHE_H_
#define __MAINMEMORY_BLOCKCACHE_H_

#include <omnetpp.h>
#include "SMS_MainMemory.h"
#include "SIMCAN_MemoryBlock.h"
#include "cSIMCAN_Base.h"
using namespace std;


#define MAX_BLOCK_LATENCIES 16
#define BLOCKSIZE_LATENCY_INDEX 0
#define READ_LATENCY_INDEX 1
#define WRITE_LATENCY_INDEX 2


/**
 * @class MainMemoryBlockCache MainMemoryBlockCache.h "MainMemoryBlockCache.h"
 *
 * Class that represents a MainMemory_CacheBlockLatencies module with read-ahead and write-back funcionality.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-11
 */
class MainMemoryBlockCache :public cSIMCAN_Base{

	protected:

        /** Memory size (in GB)*/
        double sizeGB;

		/** Memory Block Size (in KB) */
        double blockSizeKB;
		
		/** Memory size for disk cache (in MB)*/
        double sizeCacheMB;
		
		/** Number of total blocks */
		unsigned int totalBlocks;
		
		/** Number of blocks for application space */
		unsigned int totalAppBlocks;
		
		/** Number of blocks used for application space */
		unsigned int freeAppBlocks;
		
		/** Number of blocks for cache space */
		unsigned int totalCacheBlocks;		
		
		/** readAheadBlocks blocks */
		unsigned int readAheadBlocks;

		/** Access time (in seconds) */
		simtime_t accessTime;
		
	    /** Read times to Lineal Interpolation */
		static const const_simtime_t latencyTimes [MAX_BLOCK_LATENCIES] [3];

	    /** Flush time (in seconds). */
		simtime_t flushTime;

		/** Number of inputs */
	    unsigned int numInputs;		

	    /** Gate IDs Vector. Output gates to Inputs gates. */
	    cGate** toInputGates;

	    /** Gate IDs Vector. Input gates from Input gates. */
	    cGate** fromInputGates;

	    /** Gate ID. Output gate to Output. */
	    cGate* toOutputGate;

	    /** Gate ID. Input gate from Output. */
	    cGate* fromOutputGate;

	    /** Flush timer */
	    cMessage *flushMessage;

	    /** Pending message */
	    SIMCAN_App_IO_Message *pendingMessage;

	    /** SplittingMessageSystem Object*/
	    SMS_MainMemory *SMS_memory;

	    /** Memory block list */
	    list <SIMCAN_MemoryBlock*> memoryBlockList;

	   	/** Requests Queue */
		cQueue flushQueue;


	   /**
		* Destructor
		*/
		~MainMemoryBlockCache();

	   /**
	 	*  Module initialization.
	 	*/
	    void initialize();

	   /**
	 	* Module ending.
	 	*/
	    void finish();

	   /**
		* Process current request message.
		*/
		void processCurrentRequestMessage ();


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

	   /*
		* Send a request to its destination!
		* @param sm Request message.
		*/
		void sendRequest (SIMCAN_Message *sm);

	   /*
		* Send a request to its destination without checking if there are any pending subRequest!
		* @param sm Request message.
		*/
		void sendRequestWithoutCheck (SIMCAN_Message *sm);

	   /**
		* Process the enqueued subrequests.
		*/
		void processSubRequests ();

	   /**
		* Check if all subRequest corresponding to a concrete Request has arrived.
		* If all subRequest has arrived, then the corresponding Request will vbe sent to App.
		*/
		void arrivesSubRequest (SIMCAN_App_IO_Message *subRequest);

	   /**
		* Flush all blocks in flushQueue!
		*/
		void flushMemory ();

	   /**
		* Insert a request on flushQueue.
		* If Memory Full, then force the flush!
		*/
		void insertRequestOnFlushQueue (SIMCAN_App_IO_Message *subRequest);

	   /**
		* Check if memory is full.
		* @return True if memory is full or False if there is any free block.
		*/
		bool isMemoryFull ();

  	   /**
		* Check if all cached blocks are pending.
		* @return True if all cached blocks are pending or false in another case.
		*/
		bool allBlocksPending ();

	   /**
		* Insert the block corresponding to current request (offset-size) at the
		* beginning of the memory.
		*
		* @param fileName File name.
		* @param offset Request offset
		* @param size Request size
		*/
		void insertBlock (string fileName, unsigned int offset, unsigned int size);

	   /**
		* Re-insert the current block at the beginning of the memory.
		*
		* @param block Block to be re-inserted.
		*/
		void reInsertBlock (SIMCAN_MemoryBlock *block);

	   /**
		* Search for a requested block in memory.
		*
		* @param fileName File name.
		* @param offset Offset
		* @return A pointer to requested block if it is in memory, or NULL in another case.
		*/
	 	SIMCAN_MemoryBlock * searchMemoryBlock (const char* fileName, unsigned int offset);

	private:

	   /**
		* Parses the memory contents to string!
		* @return Cache list on string format.
		*/
	 	string memoryListToString ();


	   /**
		* Shows the SMS contents. For debug purpose only!
		*/
		void showSMSContents (bool showContents);
		
		
		/**
		* Calculates the corresponding latency for the given requested block
		*/
		simtime_t calculateCurrentLatency (unsigned int totalRequestSize, bool isRead);
		
};


const const_simtime_t MainMemoryBlockCache::latencyTimes [MAX_BLOCK_LATENCIES] [3] = {
	{ 512, 		0.00000006, 0.00000107 }, // 512
	{ (1*KB), 	0.00000010, 0.00000118 }, // 1 KB
	{ (2*KB), 	0.00000020, 0.00000135 }, // 2 KB
	{ (4*KB), 	0.00000040, 0.00000164 }, // 4 KB
	{ (8*KB), 	0.00000100, 0.00000187 }, // 8 KB
	{ (16*KB), 	0.00000120,	0.00000190 }, // 16 KB
	{ (32*KB), 	0.00000160, 0.00000253 }, // 32 KB
	{ (64*KB), 	0.00000240, 0.00000403 }, // 64 KB
	{ (128*KB), 0.00000500, 0.00000754 }, // 128 KB
	{ (256*KB), 0.00001130, 0.00001594 }, // 256 KB
	{ (512*KB), 0.00002350, 0.00003131 }, // 512 KB
	{ (1*MB), 	0.00004600, 0.00006074 },  // 1 MB
	{ (2*MB), 	0.00009500, 0.00013574 },  // 2 MB
	{ (4*MB), 	0.00019100, 0.00028453 },  // 4 MB
	{ (8*MB), 	0.00038700, 0.00058337 },  // 8 MB
	{ (16*MB), 	0.00076500, 0.00134453 }  // 16 MB
	};	
	
#endif
