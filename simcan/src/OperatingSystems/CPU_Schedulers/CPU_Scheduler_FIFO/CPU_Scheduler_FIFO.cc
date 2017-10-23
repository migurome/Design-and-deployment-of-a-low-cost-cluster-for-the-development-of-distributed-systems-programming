#include "CPU_Scheduler_FIFO.h"

Define_Module (CPU_Scheduler_FIFO);



CPU_Scheduler_FIFO::~CPU_Scheduler_FIFO(){
		
	requestsQueue.clear();			
}


void CPU_Scheduler_FIFO::initialize(){

	std::ostringstream osStream;
	int i;


	    // Set the moduleIdName
		osStream << "CPU_Scheduler_FIFO." << getId();
		moduleIdName = osStream.str();

	    // Init the super-class
	    cSIMCAN_Base::initialize();	
	    
	    // Get module parameters
	    numCPUs = par ("numCPUs");
	    
	    // State of CPUs
		isCPU_Idle = new bool [numCPUs];
		
		// Init state to idle!
		for (i=0; i<numCPUs; i++)
			isCPU_Idle[i] = true;	    
	    	    		
		// Init requests queue		
		requestsQueue.clear();	
			
	    
		    // Init the gate IDs to/from Scheduler
		    fromServiceRedirectorGate = gate ("fromServiceRedirector");
		    toServiceRedirectorGate = gate ("toServiceRedirector");
	
		    // Init the gates IDs to/from BlockServers
		    toCPUGate = new cGate* [numCPUs];
		    fromCPUGate = new cGate* [numCPUs];
	
		    for (i=0; i<numCPUs; i++){
				toCPUGate [i] = gate ("toCPU", i);
				fromCPUGate [i] = gate ("fromCPU", i);
		    }    
}


void CPU_Scheduler_FIFO::finish(){

	// Finish the super-class
	cSIMCAN_Base::finish();
}


cGate* CPU_Scheduler_FIFO::getOutGate (cMessage *msg){

	int i;

		// If msg arrive from Output
		if (msg->getArrivalGate()==fromServiceRedirectorGate){
			if (gate("toServiceRedirector")->getNextGate()->isConnected()){
				return (toServiceRedirectorGate);
			}
		}

		// If msg arrive from Inputs
		else if (msg->arrivedOn("fromCPU")){
			for (i=0; i<numCPUs; i++)
				if (msg->arrivedOn ("fromCPU", i))
					return (gate("toCPU", i));
		}

	// If gate not found!
	return NULL;
}


void CPU_Scheduler_FIFO::processSelfMessage (cMessage *msg){
	showErrorMessage ("Unknown self message [%s]", msg->getName());
}


void CPU_Scheduler_FIFO::processRequestMessage (SIMCAN_Message *sm){	

	int i, cpuIndex;
	SIMCAN_App_CPU_Message *sm_cpu;
	

		// Casting to debug!
		sm_cpu = check_and_cast<SIMCAN_App_CPU_Message *>(sm);
		
		// Assign infinite quantum
		sm_cpu->setQuantum(INFINITE_QUANTUM);
				
		// Search for an empty cpu core
		cpuIndex = searchIdleCPU();
		
		// All CPUs are busy
		if (cpuIndex == NOT_FOUND){
			
			if (DEBUG_CPU_Scheduler_FIFO)
				showDebugMessage ("Enqueing computing block. All CPUs are busy: %s", sm_cpu->contentsToString(DEBUG_MSG_CPU_Scheduler_FIFO).c_str());
			
			// Enqueue current computing block
			requestsQueue.insert (sm_cpu);
		}
		
		// At least, one cpu core is idle
		else{
			
			if (DEBUG_CPU_Scheduler_FIFO)
				showDebugMessage ("Sending computing block to CPU[%d]:%s", cpuIndex, sm_cpu->contentsToString(DEBUG_MSG_CPU_Scheduler_FIFO).c_str());
			
			// Assign cpu core
			sm_cpu->setNextModuleIndex(cpuIndex);			
			
			// Update state!
			isCPU_Idle[cpuIndex]=false;		
			sendRequestMessage (sm_cpu, toCPUGate[cpuIndex]);
		}
}


void CPU_Scheduler_FIFO::processResponseMessage (SIMCAN_Message *sm){

	unsigned int cpuIndex;	
	cMessage* unqueuedMessage;
	SIMCAN_Message *nextRequest;	
	SIMCAN_App_CPU_Message *sm_cpu;
	SIMCAN_App_CPU_Message *sm_cpuNext;
	
		// Cast
		sm_cpu = check_and_cast<SIMCAN_App_CPU_Message *>(sm);

		// Update cpu state!
		cpuIndex = sm->getNextModuleIndex();

		if ((cpuIndex >= numCPUs) || (cpuIndex < 0))
			showErrorMessage ("CPU index error (%d). There are %d CPUs attached. %s\n",
								cpuIndex,
								numCPUs,
								sm->contentsToString(true).c_str());
		else
			isCPU_Idle[cpuIndex] = true;
			
		if (DEBUG_CPU_Scheduler_FIFO)
			showDebugMessage ("Computing block Finished. Sending back to app:%s", sm_cpu->contentsToString(DEBUG_MSG_CPU_Scheduler_FIFO).c_str());

		// There are pending requests
		if (!requestsQueue.empty()){
			
			// Pop
			unqueuedMessage = (cMessage *) requestsQueue.pop();

			// Dynamic cast!
			nextRequest = check_and_cast<SIMCAN_Message *>(unqueuedMessage);

			// set the cpu for the return
			nextRequest->setNextModuleIndex(cpuIndex);

			// Update state!
			isCPU_Idle[cpuIndex]=false;	
			
			sm_cpuNext = check_and_cast<SIMCAN_App_CPU_Message *>(nextRequest);		
						
			if (DEBUG_CPU_Scheduler_FIFO)
				showDebugMessage ("Sending computing block to CPU[%d]:%s", cpuIndex, sm_cpuNext->contentsToString(DEBUG_MSG_CPU_Scheduler_FIFO).c_str());
														
			// Send!
			sendRequestMessage (nextRequest, toCPUGate[cpuIndex]);
		}	

	// Send current response to OS
	sendResponseMessage (sm);
}


int CPU_Scheduler_FIFO::searchIdleCPU (){
	
	int i;
	bool found;
	int result;
	
		// Init
		i = 0;
		found = false;
	
		// Search for an idle CPU
		while ((i<numCPUs) && (!found)){			
			
			if (isCPU_Idle[i])
				found = true;
			else
				i++;
		}
				
		// Result
		if (found)
			result = i;
		else
			result = NOT_FOUND;		
	
	return result;
}

