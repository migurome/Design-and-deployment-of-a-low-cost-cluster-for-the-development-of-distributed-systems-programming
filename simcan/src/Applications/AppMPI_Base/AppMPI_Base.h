#ifndef __APPMPI_BASE_H_
#define __APPMPI_BASE_H_

#include <omnetpp.h>
#include "FileConfigManager.h"
#include "SimcanAPI.h"
#include "MPI_Call.h"
using namespace std;

/**
 * @class AppMPI_Base AppMPI_Base.h "AppMPI_Base.h"
 *
 * Base class used by MPI applications.
 * 
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 04-02-2010
 */
class AppMPI_Base: public SimcanAPI{


	protected:

        /** Process ID (public myRank)*/
        unsigned int processID;

        /** Total number of MPI processes */
        int numProcesses;

        /** Number of worker processes per set */
        int workersSet;
		
        /** Root ID */
        unsigned int rootProcessID;

		/** Connection between all processes? */
		bool allToAllConnections;

		/** Simulation Starting timestamp */
	  	simtime_t simStartTime;

		/** Simulation Ending timestamp */
	  	simtime_t simEndTime;

		/** Running starting timestamp */
		time_t runStartTime;

		/** Running ending timestamp */
		time_t runEndTime;  
		

	   /**
		* Destructor
		*/
		~AppMPI_Base();

	   /**
 	 	*  Module initialization.
  		*/
	    void initialize();

	   /**
		* Module ending.
		*/
		void finish();

	   /**
		* Process a self message.
		* @param msg Self message.
		*/
		virtual void processSelfMessage (cMessage *msg) = 0;		
		
	   /**
		* Process a response message from a module in the local node. Empty method.
		* All response messages are computed by TCPSocket::CallbackInterface
		* @param sm Request message.
		*/
		virtual void processResponseMessage (SIMCAN_Message *sm) = 0;
		
	   /**
		* This function will be invoked when current blocked process can continue its execution.		
		* 
		*/
		virtual void continueExecution () = 0;

	   /**
		* Process a request message.
		* @param sm Request message.
		*/
		virtual void processRequestMessage (SIMCAN_Message *sm);





		// ------ Management of process ID's ------


		/**
		 *
		 * Checks whether a process is a master process
		 *
		 * @param procID ID of the process to be checked
		 * @return True if current process is a master process, or false in another case.
		 *
		 */
		bool isCoordinator (int procID);
		
		/**
		 * Gets the master process of a given process
		 *
		 * @param procID ID of the process
		 * @return Coordinator process of the process with rank processRank
		 *
		 */
		int getMyCoordinatorID (int procID);

		/**
         * Gets the number of previous workers
         *
         * @param procID ID of the process
         * @return Number of previous workers
         *
         */
		int getNumPreviousWorkers (int procID);

		/**
         * Gets the number of previous masters
         *
         * @param procID ID of the process
         * @return Number of previous masters
         *
         */
        int getNumPreviousCoordinators (int procID);

		/**
		 * Checks if a given process belongs to a master
		 *
		 * @param masterID Rank of the master process
		 * @param procID Rank of the worker process
		 *
		 * @return True if the worker process with rank processRank belong to master.
		 *
		 */
		bool isThisWorkerInMyRange (int masterID, int procID);



		// ------ MPI calls ------


 	   /**
 		* Executes a MPI_SEND call.
 		* @param procID ID of the process that receive data.
 		* @param bufferSize Number of sent bytes.
 		*/
 		virtual void mpi_send (unsigned int procID, int bufferSize);
 		
		/**
		* Executes a MPI_SEND call.
		* @param sm_mpi Message to be sent.
		*/
 		virtual void mpi_send (SIMCAN_MPI_Message *sm_mpi);

 	   /**
 		* Executes a MPI_RECV call.
 		* @param procID ID of the process that send data.
 		* @param bufferSize Number of sent bytes.
 		* @result Returns MPI message
 		*/
 		virtual SIMCAN_MPI_Message * mpi_recv (unsigned int procID, int bufferSize);

 	   /**
 		* Executes a MPI_BARRIER call.
 		*/
 		virtual void mpi_barrier ();

 	   /**
 		* Executes a MPI_BCAST call.
 		* @param rootID ID of the process that splits and sends data.
 		* @param bufferSize Number of sent bytes to each process.
 		*/
 		virtual void mpi_bcast (unsigned int rootID, int bufferSize);

 	   /**
 		* Executes a MPI_SCATTER call.
 		* @param rootID ID of the process that splits and sends data.
 		* @param bufferSize Number of sent bytes to each process.
 		*/
 		virtual void mpi_scatter (unsigned int rootID, int bufferSize);
 		
 	   /**
 		* Executes a MPI_GATHER call.
 		* @param rootID ID of the process that splits and sends data.
 		* @param bufferSize Number of sent bytes to each process.
 		*/
 		virtual void mpi_gather (unsigned int rootID, int bufferSize);

       /**
        * Establish all pre-configured connections with corresponding processes
        */
        void establishAllConnections ();

       /**
        * Process a MPI call request.
        */
        virtual void processMPICallRequest (SIMCAN_MPI_Message *sm_mpi);






	 private:

	        /**
	          * Structure that represents a connection between two TCP applications.
	          */
	          struct MPIApp_Connector{
	              string destAddress;             /**< Destination Address. */
	              int destPort;                   /**< Destination Port */
	              int idConnector;                /**< Local connection ID */
	          };
	          typedef struct MPIApp_Connector mpiConnector;

	          /** connector vector that contains the corresponding data to establish connection with another processes.*/
	          vector <mpiConnector> mpiConnections;

	          /** Vector that contains the ranks of each process that belongs to the same application's instance */
	          vector <int> ranksVector;

	          /** Pending request flag */
	          bool waitingForSend;

	          /** Computing delay message */
	          cMessage *computingDelayMessage;

	          /** Current MPI call */
	          MPI_Call *currentMPICall;

	          /** Current MPI call */
	          MPI_Call *lastSynchroMPICall;

	          /** Sync barrier to all processes */
	          int *barriers;

	          /** Cache list */
	          list <MPI_Call*> mpiCallList;

	          /** Process Rank */
	          unsigned int myRank;



	           /**
	            * Parse the machines File.
	            * This method set the attributes "myRank", "numProcess" and "numSockets".
	            *
	            */
	            void parseMachinesFile ();

	           /**
	            * Gets the process rank given its position in the ranks vector
	            *
	            * @param procID Process position.
	            * @return Process rank.
	            *
	            */
	            int getRank (unsigned int procID);

	           /**
	            * Gets the index of a process allocated in ranksVector given its rank
	            *
	            * @param processRank Rank of the process
	            * @return Index of the process in the ranksVector vector
	            */
	           int getProcessIndexByRank (unsigned int processRank);

	          /**
	           * Synchronize current MPI call with incoming MPI Call
	           * @return MPI message
	           */
	           virtual SIMCAN_MPI_Message * synchronize (int incomingCall);

	          /**
	           * Inserts a MPI call on list and sends a corresponding ACK message to sender process.
	           * @param sm_mpi Message that contains the MPI call
	           */
	           virtual void insertMPICall (SIMCAN_MPI_Message *sm_mpi);

	          /**
	           * Search a concrete MPI incoming call on list.
	           * @param call MPI call type
	           * @param sender Corresponding sender process
	           * @param size Number of bytes sent
	           * @return Incoming call to search. If not found, NULL value will be returned.
	           */
	           virtual MPI_Call* searchMPICall(unsigned int call, unsigned int sender, int size);

	          /**
	           * Deletes a MPI Call
	           * @param removedCall MPI Call to remove
	           */
	           virtual void deleteMPICall(MPI_Call* removedCall);

	          /**
	           * Parses all received MPI Calls to string.
	           * This functions has debug purposes only.
	           * @return MPI received calls in string format.
	           */
	           string incomingMPICallsToString ();

	          /**
	           * Parses a MPI call to string format.
	           * @param call MPI call.
	           * @return MPI call in string format.
	           */
	           string callToString (int call);

	          /**
	           * Add a new connection.
	           *
	           * @param hostName Hostname where is executing the process to establish connection with.
	           * @param port Destination port.
	           * @param rank Rank of the process to establish connection with.
	           *
	           */
	           void addNewConnection (string hostName, unsigned int port, unsigned int rank);

	           /**
	            * Calculate the number of processes to send a BCast message
	            */
	           unsigned int calculateBcastMax ();


	           /**
	            * Returns the last synchronized call
	            *
	            * @return Last synchronized call
	            */
	           MPI_Call* getLastSynchronizedCall ();

	          /**
	           * Parses all MPI commincations to a string.
	           *
	           * @return String that contains all MPI communications.
	           */
	           string mpiCommunicationsToString ();

	           /**
                * Calculate is process with rank rankProcess is executed in the same node that the process that invokes this method.
                *
                */
               bool isProcessInThisNode (unsigned int rankProcess);

               /**
                * Process a MPI_BARRIER_ACK Message.
                * This function enables to process that performed MPI_BARRIER to continue its execution.
                */
                virtual void mpi_barrier_up ();

               /**
                * Process a MPI_BARRIER_DOWN Message.
                * This function is executed only by MPI_MASTER_RANK. Its purpose is to syncronize all process.
                * @param sm_mpi BARRIER_DOWN message.
                */
                virtual void mpi_barrier_down (SIMCAN_MPI_Message *sm_mpi);

               /**
                * Init barriers.
                */
                virtual void initBarriers ();

               /**
                * Check barriers for all processes
                */
                virtual void checkBarriers ();

              /**
               * Process an incoming MPI_GATHER message.
               * @param sm Message that contains a MPI_GATHER message.
               */
               virtual void mpi_gather_arrives (SIMCAN_Message *sm);


};

#endif
