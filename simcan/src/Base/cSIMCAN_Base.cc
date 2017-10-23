#include "cSIMCAN_Base.h"

const string cSIMCAN_Base::STARTED_MODULES_FILE="Started_Modules.txt";
const string cSIMCAN_Base::LOG_MESSAGES_FILE="Log_Messages.txt";
const string cSIMCAN_Base::DEBUG_MESSAGES_FILE="Debug_Messages.txt";
const string cSIMCAN_Base::ERROR_MESSAGES_FILE="Error_Messages.txt";
const string cSIMCAN_Base::RESULTS_MESSAGES_FILE="Results.txt";
const string cSIMCAN_Base::OUTPUT_DIRECTORY="output";


cSIMCAN_Base::~cSIMCAN_Base(){
	
	struct stat info;
		
		// Delete latency message
		cancelAndDelete (latencyMessage);
		
		// Write messages to file?
		if (WRITE_MESSAGES_TO_FILE){
	
			// Close all files!
			close (fdStartedModules);
			close (fdLog);
			close (fdDebug);
			close (fdError);			
		}		

		// Check the size of result file!
		stat (resultPath, &info);

		if (info.st_size == 0)
			unlink (resultPath);
		else
			close (fdResults);
}


void cSIMCAN_Base::initialize(){

	char module_path [NAME_SIZE];
	char currentHostName [NAME_SIZE];
	DIR *currentDir;
	string currentRunPath;

		// Init common attributes
		currentRequest = 0;
		latencyMessage = NULL;
		queue.clear();


		// Check if output folder exists...
		checkDir (OUTPUT_DIRECTORY.c_str());

		// Check the current run directory
		currentRunPath = checkCurrentRunDir();

		// Results messages!
		sprintf (resultPath, "%s/Results.txt", currentRunPath.c_str());

		// Create log files...
		if (WRITE_MESSAGES_TO_FILE){

			// Creates current module folder!			
			#ifdef WITH_PARSIM
				bzero (currentHostName, NAME_SIZE);
				gethostname (currentHostName, NAME_SIZE);
				sprintf (module_path, "%s/%s-%d", currentRunPath.c_str(), currentHostName, getId());
				
			#else
				sprintf (module_path, "%s/%d", currentRunPath.c_str(), getId());						
			#endif			
			
			checkDir (module_path);

			// Creates Started module file!
			if (SHOW_INITIAL_PARAMETERS_IN_MODULES){
				sprintf (startedModulesPath, "%s/%s", currentRunPath.c_str(), STARTED_MODULES_FILE.c_str());				
			}

			// Creates log messages file!
			if (SHOW_LOG_MESSAGES){

				sprintf (logPath, "%s/%s",module_path, LOG_MESSAGES_FILE.c_str());
				fdLog = open (logPath, O_CREAT | O_TRUNC | O_RDWR, 00777);

				// Error opening log file
				if (fdLog<0){
					perror ("Error opening log File");
					opp_error ("Error opening log File");
				}
				else
					close (fdLog);
			}

			// Creates debug messages file!
			if (SHOW_DEBUG_MESSAGES){

				sprintf (debugPath, "%s/%s", module_path, DEBUG_MESSAGES_FILE.c_str());
				fdDebug = open (debugPath, O_CREAT | O_TRUNC | O_RDWR, 00777);

				// Error opening debug file
				if (fdDebug<0){
					perror ("Error opening debug File");
					opp_error ("Error opening debug File");
				}
				else
					close (fdDebug);
			}

			// Creates error messages file!
			if (SHOW_ERROR_MESSAGES){

				sprintf (errorPath, "%s/%s", module_path, ERROR_MESSAGES_FILE.c_str());
				fdError = open (errorPath, O_CREAT | O_TRUNC | O_RDWR, 00777);

				// Error opening error file
				if (fdError<0){
					perror ("Error opening error File");
					opp_error ("Error opening error File");
				}
				else
					close (fdError);
			}
		}


	// Log that this module starts
	showStartedModule (" ");
}


void cSIMCAN_Base::handleMessage(cMessage *msg){

	SIMCAN_Message *sm;		
	

		// If msg is a Self Message...
		if (msg->isSelfMessage())
			processSelfMessage (msg);

		// Not a self message...
		else{

			// Casting to SIMCAN_Message
			if ((sm = dynamic_cast<SIMCAN_Message *>(msg)) != NULL){												

				// Request message, upload message trace and send to destination module!
				if (!sm->getIsResponse()){
				
					// Update message trace
					updateMessageTrace (sm);

					// Insert into queue
					queue.insert (sm);

					// If not processing any request...
					processCurrentRequestMessage ();
				}

				// Response message!
				else{
					processResponseMessage (sm);
				}
			}
						
			// msg is not a SIMCAN Message!
			else
				showErrorMessage ("Unknown received message [%s]", msg->getName());
		}
}


void cSIMCAN_Base::finish (){
}


void cSIMCAN_Base::sendRequestMessage (SIMCAN_Message *sm, cGate* gate){

	// If trace is empty, add current hostName, module and request number
	if (sm->isTraceEmpty()){
		sm->addNodeToTrace (getHostName());
		updateMessageTrace (sm);
	}

	// Send the message!
	send (sm, gate);

	// Process next request!
	processCurrentRequestMessage ();
}


void cSIMCAN_Base::sendResponseMessage (SIMCAN_Message *sm){

	int gateId;

		// Get the gateId to send back the message
		gateId = sm->getLastGateId ();

		// Removes the current module from trace...
		sm->removeLastModuleFromTrace ();

		// Send back the message
		send (sm, gateId);

		// Process next request!
		processCurrentRequestMessage ();
}


void cSIMCAN_Base::showStartedModule (const char *args, ...){
	
	char currentHostName [NAME_SIZE];
	va_list ap;
	bool isEnd;
	char aux [2*NAME_SIZE];
	char aux2 [2*NAME_SIZE];
	char aux3 [2*NAME_SIZE];
	char type;
	int ret;
	std::ostringstream msgLine;

	char *s;
	int c;
	int d;
	double f;
	unsigned int u;
	unsigned long int U;
	unsigned long long int w;


		if (SHOW_INITIAL_PARAMETERS_IN_MODULES){

			// Init
			isEnd = 0;
			va_start(ap, args);
			strcpy (aux, args);
			msgLine.str("");
			
			//#ifdef WITH_PARSIM
				//bzero (currentHostName, NAME_SIZE);
				//gethostname (currentHostName, NAME_SIZE);
				//msgLine << "ModuleID:" << currentHostName << "-" << getId() << " -> " << getFullPath() << endl;
				
			//#else
				msgLine << "ModuleID:" << getId() << " -> " << getFullPath() << endl;						
			//#endif

				while (!isEnd){

                   	// Parse current line!
					ret = sscanf (aux, "%[^%\n]%%%c%[^\n]", aux2, &type, aux3);

						// End of args!
						if (ret==0)
							isEnd=1;

						// One parameter...
						else if (ret==1){
							msgLine << aux2;
							isEnd=1;
						}

						// Two parameters...
						else if ((ret==2) || (ret==3)){

							msgLine << aux2;

							// Type?
							switch (type){

								// String!
								case 's':
									s = va_arg(ap, char *);
									msgLine << s;
									break;

								// Char!
								case 'c':
									c = va_arg(ap, int);
									msgLine << (char)c;
									break;

								// Int!
								case 'd':
                             		d = va_arg(ap, int);
                             		msgLine << d;
                             		break;

                             	// Double!
								case 'f':
                             		f = va_arg(ap, double);
                             		msgLine << f;
                             		break;

                             	// Unsigned int!
								case 'u':
                             		u = va_arg(ap, unsigned int);
                             		msgLine << u;
                             		break;

                             	// Unsigned long int!
								case 'U':
                             		U = va_arg(ap, unsigned long int);
                             		msgLine << U;
                             		break;

                             	// Unsigned long long int!
								case 'w':
                             		w = va_arg(ap, unsigned long long int);
                             		msgLine << w;
                             		break;
							}


							if (ret==3)
								strcpy (aux, aux3);
							else
								isEnd=1;
						}
				}

				// End of parsing!
				msgLine << endl;
				va_end(ap);

				// Show message in GUI
				if (SHOW_GUI_MESSAGES)
					EV 	<< msgLine.str();

				// Show message in console
				if (SHOW_CONSOLE_MESSAGES)
					printf ("%s", msgLine.str().c_str());					
					
					
				// Write message to file
				if (WRITE_MESSAGES_TO_FILE){					
					
					// If file exists, open it, else, create it!
					if ((fdStartedModules = open (startedModulesPath, O_APPEND | O_RDWR, 00777)) == -1)
						fdStartedModules = open (startedModulesPath, O_CREAT | O_APPEND | O_RDWR, 00777);
	
					// Error opening started modules file
					if (fdStartedModules<0){
						perror ("Error opening Started Modules File");
						opp_error ("Error opening Started Modules File");
						exit(0);
					}			
					else{
						write (fdStartedModules, msgLine.str().c_str(), msgLine.str().length());
						close (fdStartedModules);
					}
				}					
		}
}


void cSIMCAN_Base::showLogMessage (string logMsg){

	std::ostringstream msgLine;

		if (SHOW_LOG_MESSAGES){

			// Prepare the message line!
			msgLine.str("");
			msgLine << "[" << simTime() << "]" << "(" << getId() << ") LOG: " << logMsg << endl;

			// Show message in GUI
			if (SHOW_GUI_MESSAGES)
				EV 	<< msgLine.str();

			// Show message in console
			if (SHOW_CONSOLE_MESSAGES)
				printf ("%s", msgLine.str().c_str());
				
			// Write message to file
			if (WRITE_MESSAGES_TO_FILE){

				fdLog = open (logPath, O_APPEND | O_RDWR, 00777);

				if (fdLog<0){
					perror ("Error opening Log File (on write).");
					opp_error ("Error opening Log File (on write)");
					exit(0);
				}
				else{
					write (fdLog, msgLine.str().c_str(), msgLine.str().length());
					close (fdLog);
				}
			}
		}
}


void cSIMCAN_Base::showDebugMessage (const char *args, ...){

	va_list ap;
	bool isEnd;
	char aux [2*NAME_SIZE];
	char aux2 [2*NAME_SIZE];
	char aux3 [2*NAME_SIZE];
	char type;
	int ret;
	char lineAux[2024];
	std::ostringstream msgLine;

	char *s;
	int c;
	int d;
	double f;
	unsigned int u;
	unsigned long int U;
	unsigned long long int w;


		if (SHOW_DEBUG_MESSAGES){

			// Init
			isEnd = 0;
			va_start(ap, args);
			strcpy (aux, args);
			msgLine.str("");
			msgLine << "[" << simTime() << "]" << "(" << getHostName() << "." << moduleIdName << ") DEBUG: ";

				while (!isEnd){

                   	// Parse current line!
					ret = sscanf (aux, "%[^%\n]%%%c%[^\n]", aux2, &type, aux3);

						// End of args!
						if (ret==0)
							isEnd=1;

						// One parameter...
						else if (ret==1){
							msgLine << aux2;
							isEnd=1;
						}

						// Two parameters...
						else if ((ret==2) || (ret==3)){

							msgLine << aux2;

							// Type?
							switch (type){

								// String!
								case 's':
									s = va_arg(ap, char *);
									msgLine << s;
									break;

								// Char!
								case 'c':
									c = va_arg(ap, int);
									msgLine << (char)c;
									break;

								// Int!
								case 'd':
                             		d = va_arg(ap, int);
                             		msgLine << d;
                             		break;

                             	// Double!
								case 'f':
                             		f = va_arg(ap, double);
                             		msgLine << f;
                             		break;

                             	// Unsigned int!
								case 'u':
                             		u = va_arg(ap, unsigned int);
                             		msgLine << u;
                             		break;

                             	// Unsigned long int!
								case 'U':
                             		U = va_arg(ap, unsigned long int);
                             		msgLine << U;
                             		break;

                             	// Unsigned long long int!
								case 'w':
                             		w = va_arg(ap, unsigned long long int);
                             		msgLine << w;
                             		break;
							}


							if (ret==3)
								strcpy (aux, aux3);
							else
								isEnd=1;
						}
				}

				// End of parsing!
				msgLine << endl;
				va_end(ap);

				// Show message in GUI
				if (SHOW_GUI_MESSAGES)
					EV 	<< msgLine.str();

				// Show message in console
				if (SHOW_CONSOLE_MESSAGES)
					printf ("%s", msgLine.str().c_str());					

				// Write message to file
				if (WRITE_MESSAGES_TO_FILE){

					fdDebug = open (debugPath, O_APPEND | O_RDWR, 00777);

					if (fdDebug<0){
						perror ("Error opening Debug File (on write).");
						opp_error ("Error opening Debug File (on write)");
						exit(0);
					}
					else{
						write (fdDebug, msgLine.str().c_str(), msgLine.str().length());
						close (fdDebug);
					}
				}
		}
}


void cSIMCAN_Base::showBigDebugMessage (string message){

//	// TODO This function does not work well.
//	// Simply, splits it into a const slices and call showDebugMessage function!
//
//	size_t init_ptr;
//	size_t last_ptr;
//	size_t last_found;
//	size_t current_found;
//
//
//		// Init
//		init_ptr = last_found = current_found =0;
//		last_ptr = NAME_SIZE;
//
//		while (init_ptr<message.size()){
//
//			current_found = message.find_last_of ("\n", last_ptr);
//
//			// Found!
//			if (current_found!=string::npos){
//
//				//printf ("Salto encontrado -> ");
//
//				// Previous block...
//				if (current_found==last_found){
//					showDebugMessage (message.substr(init_ptr, NAME_SIZE).c_str());
//					//printf ("%s\n", message.substr(init_ptr, NAME_SIZE).c_str());
//					init_ptr += NAME_SIZE+1;
//					last_ptr = init_ptr + NAME_SIZE;
//				}
//
//				// New block...
//				else{
//					showDebugMessage (message.substr(init_ptr, current_found-init_ptr).c_str());
//					//printf ("%s\n", message.substr(init_ptr, current_found-init_ptr).c_str());
//					last_found = current_found;
//					init_ptr = current_found+1;
//					last_ptr = init_ptr + NAME_SIZE;
//				}
//			}
//
//			// not found!
//			else{
//				showDebugMessage (message.substr(init_ptr, NAME_SIZE).c_str());
//				//printf ("Santo no encontrado -> %s\n", message.substr(init_ptr, NAME_SIZE).c_str());
//				init_ptr += NAME_SIZE +1;
//				last_ptr = init_ptr + NAME_SIZE;
//			}
//		}
}


void cSIMCAN_Base::showErrorMessage (const char *args, ...){

	va_list ap;
	bool isEnd;
	char aux [NAME_SIZE];
	char aux2 [NAME_SIZE];
	char aux3 [NAME_SIZE];
	char type;
	int ret;
	std::ostringstream msgLine;

	char *s;
	int c;
	int d;
	double f;
	unsigned int u;
	unsigned long int U;
	unsigned long long int w;


		if (SHOW_ERROR_MESSAGES){

			// Init
			isEnd = 0;
			va_start(ap, args);
			strcpy (aux, args);
			msgLine.str("");
			msgLine << "[" << simTime() << "]" << "(" << getFullPath() << ") ERROR: ";

				while (!isEnd){

                   	// Parse current line!
					ret = sscanf (aux, "%[^%\n]%%%c%[^\n]", aux2, &type, aux3);

						// End of args!
						if (ret==0)
							isEnd=1;

						// One parameter...
						else if (ret==1){
							msgLine << aux2;
							isEnd=1;
						}

						// Two parameters...
						else if ((ret==2) || (ret==3)){

							msgLine << aux2;

							// Type?
							switch (type){

								// String!
								case 's':
									s = va_arg(ap, char *);
									msgLine << s;
									break;

								// Char!
								case 'c':
									c = va_arg(ap, int);
									msgLine << (char)c;
									break;

								// Int!
								case 'd':
                             		d = va_arg(ap, int);
                             		msgLine << d;
                             		break;

                             	// Double!
								case 'f':
                             		f = va_arg(ap, double);
                             		msgLine << f;
                             		break;

                             	// Unsigned int!
								case 'u':
                             		u = va_arg(ap, unsigned int);
                             		msgLine << u;
                             		break;

                             	// Unsigned long int!
								case 'U':
                             		U = va_arg(ap, unsigned long int);
                             		msgLine << U;
                             		break;

                             	// Unsigned long long int!
								case 'w':
                             		w = va_arg(ap, unsigned long long int);
                             		msgLine << w;
                             		break;
							}


							if (ret==3)
								strcpy (aux, aux3);
							else
								isEnd=1;
						}
				}

				// End of parsing!
				msgLine << endl;
				va_end(ap);

				// Show message in GUI
				if (SHOW_GUI_MESSAGES)
					EV 	<< msgLine.str();

				// Show message in console
				if (SHOW_CONSOLE_MESSAGES)
					printf ("%s", msgLine.str().c_str());
				
				// Write message to file
				if (WRITE_MESSAGES_TO_FILE){

					fdError = open (errorPath, O_APPEND | O_RDWR, 00777);

					if (fdError<0){
						perror ("Error opening Error File (on write).");
						opp_error ("Error opening Error File (on write)");
						exit(0);
					}
					else{
						write (fdError, msgLine.str().c_str(), msgLine.str().length());
						close (fdError);
					}
				}
		}

	// Throws an exception to stop the execution!
	throw cRuntimeError(msgLine.str().c_str());
}


void cSIMCAN_Base::showResultMessage (const char *args, ...){

	va_list ap;
	bool isEnd;
	char aux [NAME_SIZE];
	char aux2 [NAME_SIZE];
	char aux3 [NAME_SIZE];
	char type;
	int ret;
	std::ostringstream msgLine;

	char *s;
	int c;
	int d;
	double f;
	unsigned int u;
	unsigned long int U;
	unsigned long long int w;


		// Init
		isEnd = 0;
		va_start(ap, args);
		strcpy (aux, args);
		msgLine.str("");
		msgLine << "[" << simTime() << "]" << "(" << getFullPath() << ") ";

			while (!isEnd){

               	// Parse current line!
				ret = sscanf (aux, "%[^%\n]%%%c%[^\n]", aux2, &type, aux3);

					// End of args!
					if (ret==0)
						isEnd=1;

					// One parameter...
					else if (ret==1){
						msgLine << aux2;
						isEnd=1;
					}

					// Two parameters...
					else if ((ret==2) || (ret==3)){

						msgLine << aux2;

						// Type?
						switch (type){

							// String!
							case 's':
								s = va_arg(ap, char *);
								msgLine << s;
								break;

							// Char!
							case 'c':
								c = va_arg(ap, int);
								msgLine << (char)c;
								break;

							// Int!
							case 'd':
                         		d = va_arg(ap, int);
                         		msgLine << d;
                         		break;

                         	// Double!
							case 'f':
                         		f = va_arg(ap, double);
                         		msgLine << f;
                         		break;

                         	// Unsigned int!
							case 'u':
                         		u = va_arg(ap, unsigned int);
                         		msgLine << u;
                         		break;

                         	// Unsigned long int!
							case 'U':
                         		U = va_arg(ap, unsigned long int);
                         		msgLine << U;
                         		break;

                         	// Unsigned long long int!
							case 'w':
                         		w = va_arg(ap, unsigned long long int);
                         		msgLine << w;
                         		break;
						}


						if (ret==3)
							strcpy (aux, aux3);
						else
							isEnd=1;
					}
			}

			// End of parsing!
			msgLine << endl;
			va_end(ap);

			// Show message in GUI
			if (SHOW_GUI_MESSAGES)
				EV 	<< msgLine.str();

			// Show message in console
			if (SHOW_CONSOLE_MESSAGES)
				printf ("%s", msgLine.str().c_str());				

			// Open or create results file!
			if ((fdResults = open (resultPath, O_APPEND | O_RDWR, 00777)) == -1)
				fdResults = open (resultPath, O_CREAT | O_APPEND | O_RDWR, 00777);

			// Error opening results file?
			if (fdResults<0){
				perror ("Error opening Result File (on write)");
				opp_error ("Error opening Result File (on write)");
				exit(0);
			}
			else{
				write (fdResults, msgLine.str().c_str(), msgLine.str().length());
				close (fdResults);
			}
}


void cSIMCAN_Base::updateMessageTrace (SIMCAN_Message *sm){

	int gateId;

		// If gate==NULL, first module, there is no previoud gate!
		if ((getOutGate (sm))==NULL)
			gateId = NO_GATE_ID;
		else
			gateId = getOutGate (sm)->getId();


		// Add the current module to trace...
		sm->addModuleToTrace (getId(), gateId, currentRequest);

		// Set the update trace
		currentRequest++;
}


string cSIMCAN_Base::getHostName(){

	cModule *current_Module;
	cModule *parent_Module;
	string hostName;
	bool found;
	bool end;


		// Init!
		found = end = false;
		current_Module = this;

		// While not found and not raise the top of the hierarchy...
		while ((!found) && (!end)){

			// Get the current module's parent module!
			parent_Module = current_Module->getParentModule();

			// End of the hierarchy! or get the hostName!
			if (parent_Module == NULL)
				end = true;
			else{
				//const char *newHostName = parent_Module->par ("hostName");
				const char *newHostName = parent_Module->getAncestorPar("hostName");

				if (newHostName != NULL){
					hostName = newHostName;
					found = true;
				}
			}
		}

	// If hostName not found!
	if (end)
		showErrorMessage ("Host name not found!");

	return hostName;
}


void cSIMCAN_Base::processCurrentRequestMessage (){

	SIMCAN_Message *sm;
	cMessage *unqueuedMessage;

		// There is no pending request!
		if (!isPendingRequest()){

			// While exists enqueued requests
			if (!queue.empty()){

				// Pop!
				unqueuedMessage = (cMessage *) queue.pop();

				// Dynamic cast!
				sm = check_and_cast<SIMCAN_Message *>(unqueuedMessage);

				// Process
				processRequestMessage (sm);
			}
		}
}


bool cSIMCAN_Base::isPendingRequest (){

	bool result;

		// Init...
		result = false;

		// Check if latencyMsg is not NULL!!!
		if (latencyMessage!=NULL){
			if (latencyMessage->isScheduled())
				result = true;
		}

	return result;
}


void cSIMCAN_Base::checkDir (const char *path){

	DIR *logDir;

		// Check if log Directory exists
		logDir = opendir (path);

		// If dir do not exists, then create it!
		if (logDir==NULL){
			mkdir (path, 00777);
			//closedir (logDir);
		}
		else{
			closedir (logDir);
		}
}


string cSIMCAN_Base::checkCurrentRunDir (){

	struct tm * timeinfo;
	time_t rawtime;
	char run_path [NAME_SIZE];
	string runPath;


		// Check if current run directory exists...
		time (&rawtime);
		timeinfo = localtime (&rawtime);

		sprintf (run_path, "%s/run.%04d-%02d-%02d_%02d-%02d-%02d", OUTPUT_DIRECTORY.c_str(),
											timeinfo->tm_year+1900,
											timeinfo->tm_mon+1,
											timeinfo->tm_mday,
											timeinfo->tm_hour,
											timeinfo->tm_min,
											timeinfo->tm_sec);



		checkDir (run_path);

		runPath = run_path;

	return runPath;
}


string cSIMCAN_Base::boolToString (bool value){

	string result;

		if (value)
			result = "true";
		else
			result = "false";

	return result;
}

