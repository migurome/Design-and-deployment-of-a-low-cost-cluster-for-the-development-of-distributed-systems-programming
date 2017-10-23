#ifndef __APP_SIMCAN_API_H_
#define __APP_SIMCAN_API_H_

#include <omnetpp.h>
#include "cSIMCAN_Base.h"
#include "TCPSocket.h"
#include "SIMCAN_App_IO_Message.h"
#include "SIMCAN_App_CPU_Message.h"
#include "SIMCAN_App_NET_Message.h"
#include "SIMCAN_App_MEM_Message.h"

using std::vector;


/**
 * @class SimcanAPI SimcanAPI.h "SimcanAPI.h"
 *
 * Base class used by applications. This class contains the Operating System calls interface.
 * 
 * 
 * This Base class manages the following parameters in the .ned file
 * 
 *  - application_netType
 * 
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 2009-03-10
 */
class SimcanAPI: public cSIMCAN_Base{


	protected:

	   /**
		* Structure that represents a connection between two TCP applications.
		*/
		struct SIMCAN_App_Connector{
			string localAddress;			/**< Local Address. */
			string destAddress;				/**< Destination Address. */
			int localPort;					/**< Local Port */
			int destPort;					/**< Destination Port */
			int id;							/**< Local connection ID */
			int connectionId;				/**< Connection Id (for sockets) */
		};
		typedef struct SIMCAN_App_Connector connector;

		/** Local IP */
		string appLocalIP;

		/** Local port */
		int appLocalPort;
		
		/** Input gate from OS. */
		cGate* fromOSGate;

		/** Output gate to OS. */
		cGate* toOSGate;

	   /** connector vector that contains the corresponding data to establish connection with servers.
    	* Note: Must be initialized on derived classes.
    	*/
    	vector <connector> connections;

        /** Is a distributed application? */
        bool isDistributedApp;

        /** Test ID */
        string testID;

        /** Starting time delay */
        double startDelay;

        /** Connection delay time */
        double connectionDelay;

        /** Name of the application's instance */
        string appInstance;






		
	   /**
		* Destructor
		*/
	    ~SimcanAPI();


	   /**
		* Module initialization.
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
		virtual void processSelfMessage (cMessage *msg) = 0;


	   /**
		* Process a request message.
		* @param sm Request message.
		*/
		virtual void processRequestMessage (SIMCAN_Message *sm) = 0;


	   /**
		* Process a response message from a module in the local node.
		* @param sm Response message.
		*/
		virtual void processResponseMessage (SIMCAN_Message *sm) = 0;
				
		
		
		// ----------------- I/O ----------------- //
		
		/**
		 * Open a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to open.
		 */
		void SIMCAN_request_open (const char* fileName);
		
		/**
		 * Close a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to close.
		 */
		void SIMCAN_request_close (const char* fileName);
		
		/**
		 * Read a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to be opened.
		 * @param offset Offset (in bytes)
		 * @param size Number of bytes to read.
		 */
		void SIMCAN_request_read (const char* fileName, unsigned int offset, unsigned int size);
		
		/**
		 * Close a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to be opened.
		 * @param offset Offset (in bytes)
		 * @param size Number of bytes to read.
		 */
		void SIMCAN_request_write (const char* fileName, unsigned int offset, unsigned int size);
		
		/**
		 * Create a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to create.
		 */
		void SIMCAN_request_create (const char* fileName);
		
		/**
		 * Delete a file.
		 * This function creates and sends the corresponding message to Operating System module.
		 * @param fileName Name of the corresponding file to delete.
		 */
		void SIMCAN_request_delete (const char* fileName);
		
		
		
		// ----------------- CPU ----------------- //
		
		/**
		 * Request for CPU execution
		 * This function creates and sends the corresponding message to CPU module.
		 * @param MIs Million instructions to be executed.		 
		 */
		void SIMCAN_request_cpu (unsigned int MIs);
		
		
		/**
		 * Request for CPU execution
		 * This function creates and sends the corresponding message to CPU module.
		 * @param cpuTime Time to execute the current CPU request.		 
		 */
		void SIMCAN_request_cpuTime (simtime_t cpuTime);
		
		
		
		// ----------------- Memory ----------------- //
		
		/**
		 * Request for allocating memory.
		 * This function allocates a corresponding amount of memory.
		 * @param memorySize Amount of memory (in bytes) to be allocated.
		 * @param region Region of the memory.		 
		 */
		void SIMCAN_request_allocMemory (unsigned int memorySize, unsigned int region);
		
		
		/**
		 * Request for releasing memory.
		 * This function releases a corresponding amount of memory.
		 * @param memorySize Amount of memory (in bytes) to be released.
		 * @param region Region of the memory.		 
		 */
		void SIMCAN_request_freeMemory (unsigned int memorySize, unsigned int region);



	   // ----------------- Network ----------------- //

	   /**
		* Create a connection with the corresponding server (for clients)
		* 
		* This function create a connection with destAddress:destPort.
		* Parameter setConnectionId contains the serverId.
		* Response message will contain the connectionDescriptor stored in parameter connectionId.
		* 
		* @param destAddress Server address.
		* @param destPort Server port.
		* @param id Local connection ID.
		*/
		void SIMCAN_request_createConnection (string destAddress,							   
											   int destPort,							  
											   int id);
		
		/**
		 * Create a listen connection (for servers)
		 * 
		 * @param localPort Port where communication will be listen		 
		 */
		void SIMCAN_request_createListenConnection (int localPort);
		
		/**
		 * Sends a packet through the network.
		 * @param sm Message to send
		 * @param id Local connection ID
		 */
		void SIMCAN_request_sendDataToNetwork (SIMCAN_Message *sm, int id);			
					
		
		
		/**
		 * This method set the established connection.
		 * 
		 * This method must be invoked when the response of a SM_CREATE_CONNECTION operation arrives.
		 * The response message will contain the corresponding connection descriptor stored in parameter communicationId.
		 * 
		 * @param sm Response message.
		 */
		void setEstablishedConnection (SIMCAN_Message *sm);		
				
		/**
		 * Search a connection by ID
		 * @param id Connection ID
		 * @return Index of requested connection if id exists or NOT_FOUND in other case.
		 * 
		 */
		int searchConnectionById (int id);		
		
	    /**
		 * Parses all connections info to string. 
		 * 
		 * @param printConnections If true, this functions parses communication to string, else, do nothing.		
		 * @return Connections info in string format.
		 */
		string connectionsToString (bool printConnections);		
		
		/**
		 * Get the local IP
		 * 	
		 * @return Local IP.
		 */
		string getLocalIP ();
		
		/**
		 * Set the local IP
		 * 	
		 * @param newIP Local IP.
		 */
		void setLocalIP (string newIP);
		
		/**
		 * Get the local port
		 * 	
		 * @return Local port
		 */
		int getLocalPort ();		
		
		/**
		 * Set the local port
		 * 	
		 * @param newPort Local port.
		 */
		void setLocalPort (int newPort);
		
		
	private:

	   /**
		* Get the out Gate to the module that sent <b>msg</b>.
		* @param msg Arrived message.
		* @return Gate (out) to module that sent <b>msg</b> or NULL if gate not found.
		*/
		cGate* getOutGate (cMessage *msg);		
		
};

#endif

