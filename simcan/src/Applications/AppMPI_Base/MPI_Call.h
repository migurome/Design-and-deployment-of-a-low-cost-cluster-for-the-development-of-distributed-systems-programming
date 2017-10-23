#ifndef __MPI_CALL__
#define __MPI_CALL__


#include "SIMCAN_MPI_Message.h"
#include <string>
using std::string;

/**
 * Class that represents a MPI Call.
 */
class MPI_Call{
	
	
	protected:

		/** Current process MPI Call */
		unsigned int call;					
		
		/** Current process MPI Call Sender */
		unsigned int sender;	
		
		/** Current process MPI Call receiver */
		unsigned int receiver;
		
		/** Root process on collective calls */
		unsigned int root;	
		
		/** File name */
		string fileName;	
		
		/** Offset */
		int offset;	
		
		/** Sequence number */
		unsigned int sequence;

		/** result */
		int result;

		/** Current process MPI Call bufferSize */
		int bufferSize;	
		
		/** Number of pending acks for collective calls */
		int pendingACKs;	  
		
		
	public:
		
		MPI_Call();		
		void clear();
		void copyMPICall (MPI_Call *dest);
		string toString();
		string callToString();
		
		unsigned int getCall ();
		void setCall (unsigned int newCall);
		
		unsigned int getSender ();
		void setSender (unsigned int newSender);
		
		unsigned int getReceiver ();
		void setReceiver (unsigned int newReceiver);
		
		unsigned int getRoot ();
		void setRoot (unsigned int newRoot);
		
		string getFileName ();
		void setFileName (string newFileName);
		
		int getOffset ();
		void setOffset (int newOffset);	
		
		int getSequence ();
		void setSequence (int newSequence);

		int getResult ();
		void setResult (int newResult);

		int getBufferSize ();
		void setBufferSize (int newBufferSize);	
		
		int getPendingACKs ();
		void setPendingACKs (int newPendingAcks);	
		void arrivesACK ();
};


#endif /*__MPI_CALL__*/
