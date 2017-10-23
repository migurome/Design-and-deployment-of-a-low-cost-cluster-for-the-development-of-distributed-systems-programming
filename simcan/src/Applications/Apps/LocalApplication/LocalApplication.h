#ifndef __LOCAL_APPLICATION_H_
#define __LOCAL_APPLICATION_H_

#include <omnetpp.h>
#include "SimcanAPI.h"


/**
 * @class LocalApplication LocalApplication.h "LocalApplication.h"
 *
 * Example of a sequential application without traces.
 * This application alternates I/O operations with CPU.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-13
 */
class LocalApplication: public SimcanAPI{

	
	protected:

        /** Size of the data to be read (in bytes) */
		int inputDataSize;

		/** Size of the data to be written (in bytes) */
		int outputDataSize;

		/** Number of MIs to be executed */
		int MIs;

		/** Number of iterations to be performed */
		int iterations;

		/** Current iteration */
		unsigned int currentIteration;

		/** Input file name */
		string inputFile;

		/** Output file name */
		string outputFile;

		/** Simulation Starting timestamp */
		simtime_t simStartTime;

		/** Simulation Ending timestamp */
		simtime_t simEndTime;
		
		/** Running starting timestamp */
		time_t runStartTime;

		/** Running ending timestamp */
		time_t runEndTime;		
				
		/** Call Starting timestamp (IO) */
		simtime_t startServiceIO;
		
		/** Call Ending timestamp (IO) */
		simtime_t endServiceIO;
		
		/** Call Starting timestamp (CPU) */
		simtime_t startServiceCPU;
		
		/** Call Ending timestamp (CPU) */
		simtime_t endServiceCPU;
		
		/** Spent time in CPU system */
		simtime_t total_service_CPU;
		
		/** Spent time in IO system */
		simtime_t total_service_IO;
				
		/** Execute CPU */
		bool executeCPU;
		
		/** Execute read operation */
		bool executeRead;
				
		/** Execute write operation */
		bool executeWrite;
		
		/** Read offset */
		int readOffset;

		/** Write offset */
		int writeOffset;
				
								
		
	   /**
		* Destructor
		*/
		~LocalApplication();

	   /**
 		*  Module initialization.
 		*/
	    virtual void initialize();

	   /**
 		* Module ending.
 		*/
	    virtual void finish();

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


	private:			    

	   /**
		* Method that creates and sends a new I/O request.
		* @param executeRead Executes a read operation
		* @param executeWrite Executes a write operation
		*/
		void executeIOrequest(bool executeRead, bool executeWrite);		

	   /**
		* Method that creates and sends a CPU request.
		*/
		void executeCPUrequest();

	   /**
		* Print results.
		*/
		void printResults();

};

#endif
