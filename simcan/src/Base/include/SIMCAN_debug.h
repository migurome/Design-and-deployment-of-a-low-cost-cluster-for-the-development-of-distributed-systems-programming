#ifndef _SIMCAN_DEBUG_H_
#define _SIMCAN_DEBUG_H_

#include <string>
using std::string;


/************************* GLOBAL FLAGS *************************/

/** Flag to show a complete message trace*/
#define PRINT_SM_TRACE 0


/** Flag to show messages in GUI */
#define SHOW_GUI_MESSAGES 0


/** Flag to show messages in console */
#define SHOW_CONSOLE_MESSAGES 1


/** Flag to write messages to file */
#define WRITE_MESSAGES_TO_FILE 0


/** Flag to enable the started modules messages */
#define SHOW_INITIAL_PARAMETERS_IN_MODULES 0


/** Flag to enable the log messages */
#define SHOW_LOG_MESSAGES 0


/** Flag to enable the debug messages */
#define SHOW_DEBUG_MESSAGES 1


/** Flag to enable the debug messages */
#define SHOW_ERROR_MESSAGES 0


/************************* APPLICATIONS *************************/

//// APP_PIPELINE
	#define DEBUG_ApplicationPipeline 	false	// Shows basic debug messages
	#define DEBUG_MSG_ApplicationPipeline false	// Shows all messages entirely (including all parameters)


//// APP MATRIX
    #define DEBUG_ApplicationMatrix     false    // Shows basic debug messages
    #define DEBUG_MSG_ApplicationMatrix false   // Shows all messages entirely (including all parameters)

    #define DEBUG_MatrixHPC     false            // Shows basic debug messages
    #define DEBUG_MSG_MatrixHPC false           // Shows all messages entirely (including all parameters)

//// TRACE_LOCAL_APPLICATION
	#define DEBUG_LocalApplication          false	// Shows basic debug messages
	#define DEBUG_MSG_LocalApplication      false	// Shows all messages enterely (including all parameters)


//// TRACE_MATRIX_MULTIPLICATION
    #define DEBUG_MatrixMultiplication      false     // Shows basic debug messages
    #define DEBUG_MSG_MatrixMultiplication  false    // Shows all messages enterely (including all parameters)


//// TRACE_APPLICATION_HPC
	#define DEBUG_ApplicationHPC            false	// Shows basic debug messages
	#define DEBUG_MSG_ApplicationHPC        false	// Shows all messages enterely (including all parameters)


//// NFS_CLIENT
	#define DEBUG_NFS_Client false					// Shows basic debug messages
	#define DEBUG_MSG_NFS_Client false				// Shows all messages enterely (including all parameters)
	#define DEBUG_SMS_NFS_Client false				// Shows the contents of the SMS structure
	
//// PFS_CLIENT
	#define DEBUG_PFS_Client false					// Shows basic debug messages
	#define DEBUG_MSG_PFS_Client false				// Shows all messages enterely (including all parameters)
	#define DEBUG_SMS_PFS_Client false				// Shows the contents of the SMS structure
	

//// NFS_SERVER
	#define DEBUG_NFS_Server false					// Shows basic debug messages
	#define DEBUG_MSG_NFS_Server false				// Shows all messages enterely (including all parameters)


//// APP_SYSTEM_REQUESTS
	#define DEBUG_AppSystem false					// Shows basic debug messages
	#define DEBUG_MSG_AppSystem false				// Shows all messages enterely (including all parameters)


//// MPI_APP_BASE
	#define DEBUG_MPIApp false						// Shows basic debug messages
	#define DEBUG_MSG_MPIApp false					// Shows all messages enterely (including all parameters)


/************************* FILE CONFIGURATION *************************/		
		
#define DEBUG_FILE_CONFIG_printServers      false
#define DEBUG_FILE_CONFIG_printPreload      false
#define DEBUG_FILE_CONFIG_printIORCfg       false
#define DEBUG_FILE_CONFIG_printMPIEnvCfg    false
		
		
		
/************************* DISKS *************************/

	#define DEBUG_Disk false					    // Shows basic debug messages
	#define DEBUG_DETAILED_Disk false				// Shows detailed debug messages
	#define DEBUG_MSG_Disk false					// Shows all messages enterely (including all parameters)
	#define DEBUG_BRANCHES_Disk false				// Shows the block list of each request message


/************************* CPU SCHEDULERS *************************/

	#define DEBUG_CPU_Scheduler_FIFO false				// Shows basic debug messages
	#define DEBUG_MSG_CPU_Scheduler_FIFO false			// Shows all messages enterely (including all parameters)
	#define DEBUG_CPU_Scheduler_RR false				// Shows basic debug messages
	#define DEBUG_MSG_CPU_Scheduler_RR false			// Shows all messages enterely (including all parameters)

/************************* CPU Cores *************************/

	#define DEBUG_CPUcore false					// Shows basic debug messages
	#define DEBUG_MSG_CPUcore false				// Shows all messages enterely (including all parameters)


/************************* FILE SYSTEM *************************/

	#define DEBUG_FS false							// Shows basic debug messages
	#define DEBUG_DETAILED_FS false					// Shows detailed debug messages
	#define DEBUG_PARANOIC_FS false					// Shows the FS layout after each FS call
	#define DEBUG_MSG_FS false						// Shows all messages enterely (including all parameters)
	#define DEBUG_TABLES_FS false					// Shows the Weibull tables contents
	#define DEBUG_BRANCHES_FS false					// Shows the block list of each request message
	#define DEBUG_FS_Files false					// Shows the block list of each request message


/************************* BASIC FILE SYSTEM *************************/

	#define DEBUG_Basic_FS false							// Shows basic debug messages
	#define DEBUG_DETAILED_Basic_FS false					// Shows detailed debug messages
	#define DEBUG_PARANOIC_Basic_FS false					// Shows the FS layout after each FS call
	#define DEBUG_MSG_Basic_FS false						// Shows all messages enterely (including all parameters)
	#define DEBUG_BRANCHES_Basic_FS false					// Shows the block list of each request message
	#define DEBUG_FS_Basic_Files false						// Shows the block list of each request message


/************************* PARALLEL FILE SYSTEM *************************/

	#define DEBUG_Parallel_FS false					// Shows basic debug messages
	#define DEBUG_DETAILED_Parallel_FS false			// Shows detailed debug messages
	#define DEBUG_MSG_Parallel_FS false				// Shows all messages enterely (including all parameters)
	#define DEBUG_SMS_Parallel_FS false				// Shows the contents of the current SMS request
	#define DEBUG_ALL_SMS_Parallel_FS false			// Shows the contents of all SMS
	

/************************* VFS *************************/

	#define DEBUG_VFS false				            // Shows basic debug messages
	#define DEBUG_MSG_VFS false			// Shows all messages enterely (including all parameters)
	

/************************* MAIN MEMORY *************************/

	#define DEBUG_Main_Memory false					// Shows basic debug messages
	#define DEBUG_DETAILED_Main_Memory false		// Shows detailed debug messages
	#define DEBUG_MSG_Main_Memory false				// Shows all messages enterely (including all parameters)
	#define DEBUG_SMS_Main_Memory false				// Shows the contents of the current SMS request
	#define DEBUG_ALL_SMS_Main_Memory false			// Shows the contents of all SMS
	#define DEBUG_SHOW_CONTENTS_Main_Memory false	// Shows the contents of the complete memory
	
/************************* BASIC MAIN MEMORY *************************/

	#define DEBUG_Basic_Main_Memory false					// Shows basic debug messages
	#define DEBUG_DETAILED_Basic_Main_Memory false		    // Shows detailed debug messages
	#define DEBUG_MSG_Basic_Main_Memory false				// Shows all messages enterely (including all parameters)

/************************* Cache Block Latencies MEMORY *************************/

	
	#define DEBUG_CacheBlock_Latencies_Memory false					// Shows basic debug messages
	#define DEBUG_DETAILED_CacheBlock_Latencies_Memory false		// Shows detailed debug messages
	#define DEBUG_MSG_CacheBlock_Latencies_Memory false			    // Shows all messages enterely (including all parameters)
	#define DEBUG_SMS_CacheBlock_Latencies_Memory false			    // Shows the contents of the current SMS request
	#define DEBUG_ALL_SMS_CacheBlock_Latencies_Memory false			// Shows the contents of all SMS
	#define DEBUG_SHOW_CONTENTS_CacheBlock_Latencies_Memory false	// Shows the contents of the complete memory



/************************* NETWORK SERVICES *************************/

	#define DEBUG_Network_Service false				// Shows basic debug messages
	#define DEBUG_MSG_Network_Service false			// Shows all messages enterely (including all parameters)
	#define DEBUG_TCP_Service_Client false			// Shows basic debug messages
	#define DEBUG_TCP_Service_MSG_Client false		// Shows all messages enterely (including all parameters)
	#define DEBUG_TCP_Service_Server false			// Shows basic debug messages
	#define DEBUG_TCP_Service_MSG_Server false		// Shows all messages enterely (including all parameters)


/************************* SERVICE REDIRECTOR *************************/

	#define DEBUG_Service_Redirector false			// Shows basic debug messages
	#define DEBUG_MSG_Service_Redirector false		// Shows all messages enterely (including all parameters)


/************************* BLOCK MANAGERS *************************/

	#define DEBUG_Block_Manager false				// Shows basic debug messages
	#define DEBUG_MSG_Block_Manager false			// Shows all messages enterely (including all parameters)
	#define DEBUG_BRANCHES_Block_Manager false		// Shows the block list of each request message
	#define DEBUG_SMS_Block_Manager false			// Shows the contents of the current SMS request
	#define DEBUG_ALL_SMS_Block_Manager false		// Shows the contents of all SMS



/************************* BLOCK SCHEDULER *************************/

	#define DEBUG_Block_Scheduler false				// Shows basic debug messages
	#define DEBUG_MSG_Block_Scheduler false			// Shows all messages enterely (including all parameters)
	#define DEBUG_BRANCHES_Block_Scheduler false	// Shows the block list of each request message
	#define DEBUG_SMS_Block_Scheduler false			// Shows the contents of the current SMS request
	


#endif /*SIMCANTYPES_H_*/
