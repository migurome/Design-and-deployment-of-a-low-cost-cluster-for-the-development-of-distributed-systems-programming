#ifndef _SIMCAN_MESSAGE_H_
#define _SIMCAN_MESSAGE_H_

#include "SIMCAN_Message_m.h"
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <TCPSocketMap.h>
using std::string;
using std::pair;
using std::vector;

static const bool DEBUG_PACKING = false;


/** No value operation */
static const int SM_NO_VALUE_OPERATION = 0;

/** Null operation (init) */
static const int NULL_OPERATION = 0;

/** Infinite quantum */
static const int INFINITE_QUANTUM = -1;


// ---------- IO Operations ---------- //

/** SIMCAN Message Open File operation */
static const int SM_OPEN_FILE = 10;

/** SIMCAN Message Close File operation */
static const int SM_CLOSE_FILE = 11;

/** SIMCAN Message Read File operation */
static const int SM_READ_FILE  = 12;

/** SIMCAN Message Write File operation */
static const int SM_WRITE_FILE = 13;

/** SIMCAN Message Create File operation */
static const int SM_CREATE_FILE = 14;

/** SIMCAN Message Delete File operation */
static const int SM_DELETE_FILE = 15;


// ---------- MEM Operations ---------- //

/** SIMCAN Message Allocate memory */
static const int SM_MEM_ALLOCATE = 50;

/** SIMCAN Message Release memory */
static const int SM_MEM_RELEASE = 51;


// ---------- CPU Operations ---------- //

/** SIMCAN Message Execution (CPU) */
static const int SM_CPU_EXEC = 70;


// ---------- Net Operations ---------- //

/** SIMCAN Message Create connection */
static const int SM_CREATE_CONNECTION = 200;

/** SIMCAN Message Listen for connection */
static const int SM_LISTEN_CONNECTION = 201;

/** SIMCAN Message Send data trough network */
static const int SM_SEND_DATA_NET = 202;




// ---------- MPI Operations (Trace Method) ---------- //

/** MPY any sender process */
//#define MPI_ANY_SENDER 4294967294U
#define MPI_ANY_SENDER 99999999

/** MPI No value */
#define MPI_NO_VALUE 999999995

/** MPI Master process Rank */
//#define MPI_MASTER_RANK 0

/** MPI Send */
#define MPI_SEND 100

/** MPI Receive */
#define MPI_RECV 200

/** MPI Barrier */
#define MPI_BARRIER 300

/** Barrier Up */
#define MPI_BARRIER_UP 301

/** Barrier Down */
#define MPI_BARRIER_DOWN 302

/** MPI Broadcast */
#define MPI_BCAST 400

/** MPI Scatter */
#define MPI_SCATTER 500

/** MPI Gather */
#define MPI_GATHER 600

/** MPI File Open */
#define MPI_FILE_OPEN 700

/** MPI File Close */
#define MPI_FILE_CLOSE 800

/** MPI File Create */
#define MPI_FILE_CREATE 900

/** MPI File Delete */
#define MPI_FILE_DELETE 1000

/** MPI File Read */
#define MPI_FILE_READ 1100

/** MPI File Write */
#define MPI_FILE_WRITE 1200



// ---------- MPI Operations (Graph Method) ---------- //

/** MPI Null operation */
static const unsigned int MPI_NULL = 999;

/** MPI error */
static const unsigned int MPI_ERROR = 110;

/** MPI Barrier */
static const unsigned int MPI_BARRIER_GO = 120;

/** MPI Barrier Ack */
static const unsigned int MPI_BARRIER_ACK = 130;

/** MPI Processing (cpu) */
static const unsigned int MPI_PROCESS = 140;

/** MPI Sending Metadata */
static const unsigned int MPI_METADATA = 150;

/** MPI Metadata Ack */
static const unsigned int MPI_METADATA_ACK = 160;

/** MPI Sinding data */
static const unsigned int MPI_DATA = 170;

/** MPI Data Ack */
static const unsigned int MPI_DATA_ACK = 180;

/** MPI Sending results */
static const unsigned int MPI_RESULT = 190;

/** MPI Results Ack */
static const unsigned int MPI_RESULT_ACK = 200;

/** MPI Send Barrier Ack and Read data */
static const unsigned int MPI_BARRIER_ACK_AND_READ_DATA = 210;

/** Process data and send data ack */
static const unsigned int MPI_PROCESS_AND_SEND_DATA_ACK = 220;

/** Send results and read data */
static const unsigned int MPI_SEND_RESULT_AND_READ_DATA = 230;

/** Send results ack and read data */
static const unsigned int MPI_SEND_RESULT_ACK_AND_READ_DATA = 240;




// ---------- NFS2 Message sizes---------- //

/** NFS message (open Request operation) length */
static const int64 SM_NFS2_OPEN_REQUEST = 36;

/** NFS message (open Response operation) length */
static const int64 SM_NFS2_OPEN_RESPONSE = 104;

/** NFS message (close Request operation) length */
static const int64 SM_NFS2_CLOSE_REQUEST = 36;

/** NFS message (close Response operation) length */
static const int64 SM_NFS2_CLOSE_RESPONSE = 4;

/** NFS message (read Request operation) length */
static const int64 SM_NFS2_READ_REQUEST = 44;

/** NFS message (read Response operation) length */
static const int64 SM_NFS2_READ_RESPONSE = 80;

/** NFS message (write Request operation) length */
static const int64 SM_NFS2_WRITE_REQUEST = 52;

/** NFS message (write Response operation) length */
static const int64 SM_NFS2_WRITE_RESPONSE = 72;

/** NFS message (create Request operation) length */
static const int64 SM_NFS2_CREATE_REQUEST = 68;

/** NFS message (create Response operation) length */
static const int64 SM_NFS2_CREATE_RESPONSE = 104;

/** NFS message (delete Request operation) length */
static const int64 SM_NFS2_DELETE_REQUEST = 36;

/** NFS message (delete Response operation) length */
static const int64 SM_NFS2_DELETE_RESPONSE = 4;





// ---------- SIMCAN Message Memory regions ---------- //

/** Memory region for code */
static const int SM_MEMORY_REGION_CODE = 61;

/** Memory region for local variables */
static const int SM_MEMORY_REGION_LOCAL_VAR = 62;

/** Memory region for global variables */
static const int SM_MEMORY_REGION_GLOBAL_VAR = 63;

/** Memory region for dynamic variables */
static const int SM_MEMORY_REGION_DYNAMIC_VAR = 64;

/** Memory error! Not enough memory! */
static const int SM_NOT_ENOUGH_MEMORY = 65;




// ---------- SIMCAN Message types ---------- //

/** Message to wait for the connection step*/
static const string SM_WAIT_TO_CONNECT = "Wait_To_Connect";

/** Message to wait for the execution step*/
static const string SM_WAIT_TO_EXECUTE = "Wait_To_Execute";

/** Message to wait for the execution step*/
static const string SM_LATENCY_MESSAGE = "latency-message";

/** Message to wait for the execution step*/
static const string SM_DEADLINE = "Deadline";

// ---------- SIMCAN Message Initial Length ---------- //

/** Initial message length*/
static const int64 MSG_INITIAL_LENGTH = 1;



/**
 * @class SIMCAN_Message SIMCAN_Message.h "SIMCAN_Message.h"
 *
 * Class that represents a SIMCAN_Message.
 *
 * @author Alberto N&uacute;&ntilde;ez Covarrubias
 * @date 02-10-2007
 */
class SIMCAN_Message: public SIMCAN_Message_Base{


	protected:

		/** Pointer to parent request message or NULL if has no parent */
		cMessage *parentRequest;

		/** Message trace */
		vector <pair <string, vector<TraceComponent> > > trace;


	public:


	   /**
		* Destructor.
		*/
		virtual ~SIMCAN_Message();


	   /**
		* Constructor of SIMCAN_Message
		* @param name Message name
		* @param kind Message kind
		*/
		SIMCAN_Message (const char *name=NULL, int kind=0);


	   /**
		* Constructor of SIMCAN_Message
		* @param other Message
		*/
		SIMCAN_Message(const SIMCAN_Message& other);


	   /**
		* = operator
		* @param other Message
		*/
		SIMCAN_Message& operator=(const SIMCAN_Message& other);


	   /**
		* Method that makes a copy of a SIMCAN_Message
		*/
		virtual SIMCAN_Message *dup() const;

	   /**
		* Empty method
		*/
		virtual void setTraceArraySize(unsigned int size);


       /**
		* Empty method
		*/
    	virtual unsigned int getTraceArraySize() const;

       /**
		* Empty method
		*/
    	virtual TraceComponent& getTrace(unsigned int k);


       /**
		* Empty method
		*/
    	virtual void setTrace(unsigned int k, const TraceComponent& trace_var);


	   /**
		* Add a module to message trace
		* @param module Added mdule.
		* @param gate Gate ID to <b>module</b>.
		* @param currentRequest Current request number.
		*/
		void addModuleToTrace (int module, int gate, reqNum_t currentRequest);


	   /**
		* Removes the last module from the message trace, including all its request numbers.
		* If last trace component is not a Module, then return an empty string.
		*/
		void removeLastModuleFromTrace ();


	   /**
		* Add a node to message trace.
		* @param node Added node.
		*/
		void addNodeToTrace (string node);


	   /**
		* Removes the last node from message trace, including all its modules and request numbers.
		*/
		void removeLastNodeFromTrace ();


	   /**
		* Add a request to message trace.
		* @param request Added request.
		*/
		void addRequestToTrace (int request);


	   /**
		* Removes last request from message trace.
		*/
		void removeLastRequestFromTrace ();


	   /**
		* Get the las Gate ID from message trace.
		* @return If all OK, the return last gate ID, else return SIMCAN_ERROR
		*/
		int getLastGateId ();


	   /**
		* Get the las module ID from message trace.
		* @return If all OK, the return last module ID, else return SIMCAN_ERROR
		*/
		int getLastModuleId ();


	   /**
		* Get the current request.
		* @return Current request or SIMCAN_ERROR if an error occurs.
		*/
		int getCurrentRequest ();


	   /**
		* Get the parent request message
		* @return Pointer to parent request message or NULL if not exists a parent message.
		*/
		cMessage* getParentRequest () const;


	   /**
		* Set the parent request message
		* @param parent Pointer to parent request message.
		*/
		void setParentRequest (cMessage* parent);


	   /**
		* Parse current trace to string
		* @return String with the corresponding trace.
		*/
		string traceToString ();


	   /**
		* Parse all parameters of current message to string.
		* @param printContents Print message contents.
		* @return String with the corresponding contents.
		*/
		virtual string contentsToString (bool printContents);

	   /**
		* Parse a SIMCAN_Message operation to string.
		* @param operation SIMCAN_Message operation
		* @return Operation SIMCAN_Message in string format.
		*/
		string operationToString (unsigned int operation);

	   /**
		* Parse a SIMCAN_Message operation to string.
		* @return Operation SIMCAN_Message in string format.
		*/
		string operationToString ();		
		
	   /**
		* Return if this is response message in string format.
		* @return "true" if this is a response message or "false" in other case.
		*/
		string getIsResponse_string ();

	   /**
		* Return if this message contains a remote operation in string format.
		* @return "true" if this message contains a remote operation or "false" in other case.
		*/
		string getRemoteOperation_string ();

	   /**
		* Calculates if message trace is empty.
		* @return True if message trace is empty or false in another case.
		*/
		bool isTraceEmpty ();

	   /**
		* Method that calculates if current subRequest fits with its parent trace.
		*
		*/
	   	bool fitWithParent ();

	   /**
	   	* Method that calculates if this message is son of <b>parentMsg</b>.
	   	* @return True if current message if son of parentMsg, or false in another case.
	   	*/
	   	bool fitWithParent (cMessage *parentMsg);

	   /**
	    * Add a node to trace
	    * @param host Hostname
	    * @param nodeTrace Node trace
	    */
		void addNodeTrace (string host, vector<TraceComponent> nodeTrace);

	   /**
		* Get the hostname of node k
		* @param k kth Node
		* @return hostname of kthe node
		*/
		string getHostName (int k);

	   /**
		* Get the kthe traceNode
		* @param k kth node
		* @return kth node trace
		*/
		vector<TraceComponent> getNodeTrace (int k);

	   /**
		* Serializes a SIMCAN_Message.
		* @param b Communication buffer
		*/
	    virtual void parsimPack(cCommBuffer *b);

	   /**
		* Deserializes a SIMCAN_Message.
		* @param b Communication buffer
		*/
	    virtual void parsimUnpack(cCommBuffer *b);
};

#endif
