#ifndef __APPLICATION_HPC_H_
#define __APPLICATION_HPC_H_

#include <omnetpp.h>
#include "AppMPI_Base.h"


/**
 * @class ApplicationHPC ApplicationHPC.h "ApplicationHPC.h"
 *
 * This Application models a generic behavior of a HPC application
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-13
 */
class ApplicationHPC : public AppMPI_Base{

	protected:
	
        unsigned int dataSet_MB;
		unsigned int sliceToWorkers_MB;
		unsigned int sliceToCoordinator_MB;
		unsigned int sliceCPU;
		bool workersRead;
        bool workersWrite;
		
		bool createdResultsFile;
		int totalResponses;

		char outFileName [NAME_SIZE];
		char inputFileName [NAME_SIZE];
		string inputFilePrefix;
        string outputFilePrefix;
		unsigned int currentInputFile;
        unsigned int currentOutputFile;

		/** Offsets in the current files (input and results) */
		unsigned int offsetRead;
		unsigned int offsetWrite;

		/** Global offset in the data-set (in MB)*/
		unsigned int dataSetOffset;
		unsigned int initDatasetOffset;
		unsigned int endDatasetOffset;
		unsigned int activeWorkers;

        bool showLog;

		simtime_t totalIO;
		simtime_t ioStart;
		simtime_t ioEnd;

		simtime_t totalNET;
		simtime_t netStart;
		simtime_t netEnd;

		simtime_t totalCPU;
		simtime_t cpuStart;
		simtime_t cpuEnd;
	
	
		/** Simulation Starting timestamp */
		simtime_t simStartTime;

		/** Simulation Ending timestamp */
		simtime_t simEndTime;
		
		/** Running starting timestamp */
		time_t runStartTime;

		/** Running ending timestamp */
		time_t runEndTime;
		
		/** State */
		unsigned int currentState;

		
	   /**
		* Destructor
		*/
		~ApplicationHPC();

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
		* Process a response message.
		* @param sm Request message.
		*/
		void processResponseMessage (SIMCAN_Message *sm);		
		
	   /**
		* This function calculate next state of a given process.
		* 
		*/
		void calculateNextState ();	
		
	   /**
		* Process next iteration
		* 
		*/
		void nextIteration ();		
		
	   /**
		* This function will be invoked when current blocked process can continue its execution.		
		* 
		*/
		void continueExecution ();	


	private:	
	
	   /**
		* Coordinator reads the corresponding data from input dataset.
		*/
		void readInputData();	
		
		/**
		* Workers read the corresponding data from input dataset.
		*/
		void readInputDataWorkers();	
	
	   /**
		* Coordinator process split data among worker processes.
		*/
		void deliverData();


		void allocating();
		
		/**
		* Coordinator process waits for results sent from worker processes
		*/
		void receivingResults();		
	
	   /**
		* Worker processes receive data from master process
		*/
		void receiveInputData();		
	
	   /**
		* Worker processes processing CPU.
		*/
		void processingCPU();	
		
	   /**
		* Coordinator process writes the data of current iteration
		*/
		void writtingData();	
		
	   /**
		* Workers processes write the data of current iteration
		*/
		void writtingDataWorkers();
		
	   /**
		* Worker processes send results to master process
		*/
		void sendResults();			
	
	   /**
		* Process a NET call response.
		* 
		* @param responseMsg Response message.
		*/
		void processNetCallResponse (SIMCAN_App_NET_Message *responseMsg);
		
		
	   /**
		* Process an IO call response.
		* 
		* @param responseMsg Response message.
		*/
		void processIOCallResponse (SIMCAN_App_IO_Message *responseMsg);
		
	   /**
		* Process a CPU call response.
		* 
		* @param responseMsg Response message.
		*/
		void processCPUCallResponse (SIMCAN_App_CPU_Message *responseMsg);

		void processMEMCallResponse (SIMCAN_App_MEM_Message *responseMsg);
	
		/**
		 * Parses a state to a string format.
		 * 
		 * @return State in string format.
		 */
		string stateToString(int state);
		
		/**
		 * Show results of the simulation
		 */
		void showResults ();
};

#endif
