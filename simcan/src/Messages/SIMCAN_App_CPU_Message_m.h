//
// Generated file, do not edit! Created by nedtool 4.6 from Messages/SIMCAN_App_CPU_Message.msg.
//

#ifndef _SIMCAN_APP_CPU_MESSAGE_M_H_
#define _SIMCAN_APP_CPU_MESSAGE_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
	#include "SIMCAN_Message.h"
			
// }}

/**
 * Class generated from <tt>Messages/SIMCAN_App_CPU_Message.msg:15</tt> by nedtool.
 * <pre>
 * ///
 * /// SIMCAN Message for CPU requests
 * ///
 * message SIMCAN_App_CPU_Message extends SIMCAN_Message
 * {
 *     @customize(true);
 * 
 *     simtime_t cpuTime = 0.0;			// CPU execution time
 *     unsigned long totalMIs = 0;	// Number of instructions to be executed.
 *     unsigned long remainingMIs = 0;// Remaining instructions to be executed.
 *     int quantum = 0;					// Quantum used in Round-Robin algorithm (measured in ticks)
 *     int cpuPriority = 0;				// Priority 
 * }
 * </pre>
 *
 * SIMCAN_App_CPU_Message_Base is only useful if it gets subclassed, and SIMCAN_App_CPU_Message is derived from it.
 * The minimum code to be written for SIMCAN_App_CPU_Message is the following:
 *
 * <pre>
 * class SIMCAN_App_CPU_Message : public SIMCAN_App_CPU_Message_Base
 * {
 *   private:
 *     void copy(const SIMCAN_App_CPU_Message& other) { ... }

 *   public:
 *     SIMCAN_App_CPU_Message(const char *name=NULL, int kind=0) : SIMCAN_App_CPU_Message_Base(name,kind) {}
 *     SIMCAN_App_CPU_Message(const SIMCAN_App_CPU_Message& other) : SIMCAN_App_CPU_Message_Base(other) {copy(other);}
 *     SIMCAN_App_CPU_Message& operator=(const SIMCAN_App_CPU_Message& other) {if (this==&other) return *this; SIMCAN_App_CPU_Message_Base::operator=(other); copy(other); return *this;}
 *     virtual SIMCAN_App_CPU_Message *dup() const {return new SIMCAN_App_CPU_Message(*this);}
 *     // ADD CODE HERE to redefine and implement pure virtual functions from SIMCAN_App_CPU_Message_Base
 * };
 * </pre>
 *
 * The following should go into a .cc (.cpp) file:
 *
 * <pre>
 * Register_Class(SIMCAN_App_CPU_Message);
 * </pre>
 */
class SIMCAN_App_CPU_Message_Base : public ::SIMCAN_Message
{
  protected:
    simtime_t cpuTime_var;
    unsigned long totalMIs_var;
    unsigned long remainingMIs_var;
    int quantum_var;
    int cpuPriority_var;

  private:
    void copy(const SIMCAN_App_CPU_Message_Base& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SIMCAN_App_CPU_Message_Base&);
    // make constructors protected to avoid instantiation
    SIMCAN_App_CPU_Message_Base(const char *name=NULL, int kind=0);
    SIMCAN_App_CPU_Message_Base(const SIMCAN_App_CPU_Message_Base& other);
    // make assignment operator protected to force the user override it
    SIMCAN_App_CPU_Message_Base& operator=(const SIMCAN_App_CPU_Message_Base& other);

  public:
    virtual ~SIMCAN_App_CPU_Message_Base();
    virtual SIMCAN_App_CPU_Message_Base *dup() const {throw cRuntimeError("You forgot to manually add a dup() function to class SIMCAN_App_CPU_Message");}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual simtime_t getCpuTime() const;
    virtual void setCpuTime(simtime_t cpuTime);
    virtual unsigned long getTotalMIs() const;
    virtual void setTotalMIs(unsigned long totalMIs);
    virtual unsigned long getRemainingMIs() const;
    virtual void setRemainingMIs(unsigned long remainingMIs);
    virtual int getQuantum() const;
    virtual void setQuantum(int quantum);
    virtual int getCpuPriority() const;
    virtual void setCpuPriority(int cpuPriority);
};


#endif // ifndef _SIMCAN_APP_CPU_MESSAGE_M_H_
