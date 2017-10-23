#ifndef _cSIMCAN_Base_H_
#define _cSIMCAN_Base_H_

#include <omnetpp.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdexcept>
#include <math.h>
//#include "stdio.h"
//#include "stdlib.h"
#include "SIMCAN_types.h"
#include "SIMCAN_App_IO_Message.h"
#include "SIMCAN_App_CPU_Message.h"
#include "SIMCAN_App_MEM_Message.h"
#include "SIMCAN_App_NET_Message.h"
#include "SIMCAN_MPI_Message.h"
using std::vector;


/** Macro to perform output loggin. */
//#define EV ev.isDisabled()?std::cout:ev ==> EV is now part of <omnetpp.h>


/**
 * @class cSIMCAN_Base cSIMCAN_Base.h "cSIMCAN_Base.h"
 *
 * Base class that contains common functionality for SIMCAN classes.
 * This functionality includes:
 *
 * - Manage the path between modules for the received/sent messages.
 * - Send response/request messages to the corresponding destination module.
 * - Recognize response/request messages.
 *
 */
class cSIMCAN_Base: public cSimpleModule{

	private:

		/** File that contains Started module messages */
	   	int fdStartedModules;

	   	/** Started modules file path */
	   	char startedModulesPath [NAME_SIZE];

	   	/** File that contains log messages */
	   	int fdLog;

	   	/** Lof file path */
	   	char logPath [NAME_SIZE];

	   	/** File that contains debug messages */
	   	int fdDebug;

	   	/** Debug file path */
	   	char debugPath [NAME_SIZE];

	   	/** File that contains error messages */
	   	int fdError;

	   	/** Error file path */
	   	char errorPath [NAME_SIZE];

	   	/** File that contains results */
	   	int fdResults;

	   	/** Results file path */
	   	char resultPath [NAME_SIZE];

	   	/** File corresponding to <b>moduleIdName</b> of all modules at start time */
		static const string STARTED_MODULES_FILE;

		/** File corresponding to log messages */
		static const string LOG_MESSAGES_FILE;

		/** File corresponding to debug messages */
		static const string DEBUG_MESSAGES_FILE;

		/** File corresponding to error messages */
		static const string ERROR_MESSAGES_FILE;

		/** File corresponding to results */
		static const string RESULTS_MESSAGES_FILE;

		/** Output File directory */
		static const string OUTPUT_DIRECTORY;


	protected:

		/** Incoming messages Queue */
	    cQueue queue;

	    /** Module's Id Name. (must be initializated in derived classes) */
	  	string moduleIdName;

	   	/** Latency message */
	    cMessage *latencyMessage;

	    /** Current request */
		reqNum_t currentRequest;


	  protected:
	  
	   /**
		* Destructor
		*/
		virtual ~cSIMCAN_Base();
	  
	  
	   /**
	 	* Module initialization.
	 	*/
	    virtual void initialize ();

	   /**
		* This method classifies an incoming message and invokes the corresponding method
		* to process it.
		*
		* For self-messages, it invokes processSelfMessage(sm);
		* For request-messages, it invokes processRequestMessage(sm);
		* For response-messages, it invokes processResponseMessage(sm);
		*/
		virtual void handleMessage (cMessage *msg);

	   /**
	 	* Module ending.
	 	*/
	    virtual void finish ();

	public:

	   /**
		* Send a request message to its destination!
		* @param sm Request message.
		* @param gate Gate used to send the message.
		*/
		virtual void sendRequestMessage (SIMCAN_Message *sm, cGate* gate);

	   /**
		* Send a response message to its destination!
		* @param sm Response message.
		*/
		virtual void sendResponseMessage (SIMCAN_Message *sm);
	

	   /**
		* Shows the started module's <b>moduleIdName</b>.
		* 
		* First parameter must be a string containing the type of each parameter:
		*
		* - d: int
		* - c: char
		* - f: double
		* - s: string
		* - u: unsigned int
		* - U: unsigned long int
		* - w: unsigned long long int
		*/
		void showStartedModule (const char *args, ...);

	   /**
		* Prints a log message.
		*
		* @param logMsg Message to print.
		*/
		void showLogMessage (string logMsg);

	   /**
		* Prints a debug message.
		*
		* First parameter must be a string containing the type of each parameter:
		*
		* - d: int
		* - c: char
		* - f: double
		* - s: string
		* - u: unsigned int
		* - U: unsigned long int
		* - w: unsigned long long int
		*
		* @param args Arguments.
		*/
		void showDebugMessage (const char *args, ...);

	   /**
		* Shows a BIG message.
		* This method splits a big message in several parts and invoke showDebugMessage method
		* with each one of those parts.
		* @param message BIG message.
		*/
		void showBigDebugMessage (string message);

	   /**
		* Shows an error message when somethings goes wrong.
		*
		* First parameter must be a string containing the type of each parameter:
		*
		* - d: int
		* - c: char
		* - f: double
		* - s: string
		* - u: unsigned int
		* - U: unsigned long int
		* - w: unsigned long long int
		*
		* @param args Arguments.
		*/
		void showErrorMessage (const char *args, ...);

	   /**
		* Shows results messages.
		*
		* First parameter must be a string containing the type of each parameter:
		*
		* - d: int
		* - c: char
		* - f: double
		* - s: string
		* - u: unsigned int
		* - U: unsigned long int
		* - w: unsigned long long int
		*
		* @param args Arguments.
		*/
		void showResultMessage (const char *args, ...);


	protected:

	   /**
		* Update the current message trace with current module ID and current request number.
		* @param sm Received message to be updated.
		*/
		void updateMessageTrace (SIMCAN_Message *sm);

	   /**
		* Get the hostname of the node that contains current module.
		* @return Node's hostname that contains the module which invoked this method.
		*/
		string getHostName ();

	   /**
		* Get the out Gate to the module that sent <b>msg</b>.
		* @param msg Arrived message.
		* @return Gate (out) to module that sent <b>msg</b> or NULL if gate not found.
		*/
		virtual cGate* getOutGate (cMessage *msg) = 0;

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
		* Process a response message.
		* @param sm Request message.
		*/
		virtual void processResponseMessage (SIMCAN_Message *sm) = 0;


	    template <typename C, typename T>
	   /**
		* Parses a T type into a string.
		*
		* @param x Value to parse.
		* @return Parsed value into a string.
		*/
		C to_string (T x){

			typedef typename C::value_type value_type;
			typedef typename C::traits_type traits_type;
			typedef typename C::allocator_type allocator_type;

			std::basic_ostringstream<value_type, traits_type, allocator_type> f;
			f << x;

			return f.str();
		}

	   /**
		* Process current request message.
		*/
		virtual void processCurrentRequestMessage ();

	   /**
		* This method calculates if there is any pending request.
		* @return True if there is no pending request or false if there is a pending request.
		*/
		virtual bool isPendingRequest ();

		/**
		 * Parses a bool value to string
		 * @return Bool value in string format.
		 */
		string boolToString (bool value);
		
				
	private:

	   /**
		* Check if a directory exists.
		*
		* If <b>path</b> does not exists, then will be created.
		* @param path Directory path.
		*/
		void checkDir (const char *path);


	   /**
		* Check the current Run directory.
		* @return Current directory run.
		*/
		string checkCurrentRunDir ();
};

#endif /*cSIMCAN_Base_H_*/
