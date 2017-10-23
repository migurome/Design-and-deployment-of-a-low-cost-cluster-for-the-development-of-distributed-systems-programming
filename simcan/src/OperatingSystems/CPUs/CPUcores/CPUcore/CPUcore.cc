#include "CPUcore.h"
#define ZERO_DOUBLE 0.000000F


Define_Module (CPUcore);


CPUcore::~CPUcore(){	
}


void CPUcore::initialize(){

	std::ostringstream osStream;


	    // Init the super-class
	    cSIMCAN_Base::initialize();

	    // Set the moduleIdName
	    osStream << "CPUcore." << getId();
	    moduleIdName = osStream.str();
	    
	    // Get the speed parameter
	    speed = par ("speed");
	    tick = par ("tick");
	    ipt = (unsigned long) ceil (speed * tick.dbl());

	    //printf ("[CPU] Speed:%d  - tick:%f  - ipt:%d\n", speed, tick.dbl(), ipt);
		
	    // Gate IDs
	    inGate = gate ("in");
	    outGate = gate ("out");
	    	    
	    pendingMessage = NULL;	    
	    
	    showStartedModule ("CPU Core Speed:%d MIPS.  Tick:%f s.  IPT:%d",
			    			speed,
			    			tick.dbl(),
			    			ipt);
	    
	    // Latency message
	    latencyMessage = new cMessage (SM_LATENCY_MESSAGE.c_str());
}


void CPUcore::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* CPUcore::getOutGate (cMessage *msg){

	// If msg arrive from scheduler
	if (msg->getArrivalGate()==inGate){
		if (outGate->getNextGate()->isConnected()){
			return (outGate);
		}
	}

	// If gate not found!
	return NULL;
}


void CPUcore::processSelfMessage (cMessage *msg){
	
	SIMCAN_App_CPU_Message *sm_cpu;
	
		// Latency message...
		if (!strcmp (msg->getName(), SM_LATENCY_MESSAGE.c_str())){
	
			// Cast!
			sm_cpu = check_and_cast<SIMCAN_App_CPU_Message *>(pendingMessage);
	
			// Init pending message...
			pendingMessage = NULL;
	
			// establish is a response msg (MOD JAVI)
			sm_cpu->setIsResponse(true);			

			// Send message back!
			sendResponseMessage (sm_cpu);
		}
	
		else
			showErrorMessage ("Unknown self message [%s]", msg->getName());	
}


void CPUcore::processRequestMessage (SIMCAN_Message *sm){
	
	SIMCAN_App_CPU_Message *sm_cpu;
	simtime_t cpuTime;
	simtime_t aux;
	
		// Casting to debug!
		sm_cpu = check_and_cast<SIMCAN_App_CPU_Message *>(sm);
		
		if (pendingMessage != NULL)
			showErrorMessage ("Core currently processing another computing block!");		
		
		// Link pending message
		pendingMessage = sm_cpu;	
		
		aux = sm_cpu->getCpuTime();

		// Uses time
		if (sm_cpu->getCpuTime() > 0.0){

			// Executes completely the CB
			if ((sm_cpu->getQuantum() == INFINITE_QUANTUM) || ((((double)sm_cpu->getQuantum())*tick) > sm_cpu->getCpuTime())){
				cpuTime = getMaximumTimeToExecute (sm_cpu->getCpuTime());
				sm_cpu->executeTime (cpuTime);
			}
			
			// Executes a slice
			else{
				cpuTime = tick * sm_cpu->getQuantum();
				sm_cpu->executeTime (cpuTime);
			}
		}

		// Execute current computing block completely.
		else{

			if ((sm_cpu->getQuantum() == INFINITE_QUANTUM) || (sm_cpu->getQuantum() >= ceil (sm_cpu->getRemainingMIs()/ipt))){
				cpuTime = getTimeToExecuteCompletely (sm_cpu->getRemainingMIs());
				sm_cpu->executeMIs (sm_cpu->getRemainingMIs());
				//printf ("[CPU] Ejecuta de golpe! cpuTime:%f - Remaining MIs:%lu\n", cpuTime.dbl(), sm_cpu->getRemainingMIs());
			}

			// Execute the corresponding number of ticks
			else{
				cpuTime = tick.dbl() * sm_cpu->getQuantum();
				sm_cpu->executeMIs (ipt * sm_cpu->getQuantum());
				//printf ("[CPU] Ejecuta una rodaja! cpuTime:%f - Remaining MIs: %lu\n", cpuTime.dbl(), sm_cpu->getRemainingMIs());
			}
		}
		
		
		if (DEBUG_CPUcore)
	    	showDebugMessage ("[%s] Processing Cb. Computing time:%f - %s",
	    						moduleIdName.c_str(),
	    						cpuTime.dbl(),
	    						sm_cpu->contentsToString(DEBUG_MSG_CPUcore).c_str());		
		
		scheduleAt (simTime()+cpuTime, latencyMessage);		
}


void CPUcore::processResponseMessage (SIMCAN_Message *sm){
	showErrorMessage ("This module cannot receive response messages!");
}


simtime_t CPUcore::getTimeToExecuteCompletely (unsigned long reainingMIs){
	
	simtime_t cpuTime;
	double mis;

		mis = (double)reainingMIs;
		cpuTime = mis / (double) speed;

	return cpuTime;
}


simtime_t CPUcore::getMaximumTimeToExecute (simtime_t reainingTime){
	
	simtime_t cpuTime;
	
		cpuTime = ((int) ceil (reainingTime / tick)) * tick;

	return cpuTime;
}


