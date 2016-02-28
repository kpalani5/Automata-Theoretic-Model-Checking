#include "Services.cpp"
#include <fstream>

string FileName;
ifstream FileStream;

class State
{
	public:	
	string serviceName;		// service to which state belongs to
	int stateNumber;		// identify each state uniquely
	string stateName;		// name of state
	bool isStart;			// is it a start state?
	bool isFinish;			// is it a finish state?

	
		State* nextState;	// Linked list of states = pointer
		
		State()
		{			// default
		}

		void DisplayState()	// display state details
		{
			cout<<"State Number: "<<stateNumber<<"\t State Name: "<<stateName<<"\t";
			if(isStart)
				cout<<"Start State";
			if(isFinish)
				cout<<"Finish State";
		}
	
		State(string servN, int no, string stateN, bool s, bool f) 	// Constructor to assign
		{
			serviceName = servN;
			stateNumber = no;
			stateName = stateN;
			isStart = s;
			isFinish = f;
		}

		int StateNumber()
		{
			return stateNumber;
		}

		string StateName()
		{
			return stateName;
		}

		void setFinish()
		{
			isFinish = true;
		}
};

class TransitionCondition
{
	public:
		string ConditionName;			// name of condition
		ServiceInput decidingProperty;		// property which decides
		int condition;				
		/*
			Result = True

			1 - equal to
			2 - lesser than
			3 - greater than

			Result = False
		
			1 - not equal to
			2 - greater than or equal to
			3 - lesser than or equal to
		*/ 
		bool conditionResult;

		long inp1;
		double inp2;
		string inp3;
		Date inp4;

		// if decidingProperty.value <condition + conditionResult output> inp1/inp2/inp3/inp4 go from inputState to outputState

		TransitionCondition()				//default
		{
		}

		TransitionCondition(string nm, ServiceInput prop)  	// assign condition
		{
			ConditionName = nm;
			decidingProperty = prop;
			cout<<"\n1. Equal to (=)\n2. Lesser Than (<)\n3. Greater Than (>)\n4. Not Equal to (!=)\n";
			cout<<"5. Greater than or equal to (>=)\n6. Lesser than or equal to (<=)\n7. Any of These\nChoose condition: ";
			getline(FileStream,nm);
			condition = stoi(nm,NULL,10);
			cout<<condition;

			if(condition >= 4 && condition <=6)
			{
				condition -= 3;
				conditionResult = false;
			}
			else
				conditionResult = true;	

			if(condition != 7)
			{
				int yy,mm,dd;
				string nm1;
				switch(decidingProperty.typeReturn())
				{
					case 1: {
							
							cout<<"\nInteger value for condition limit: ";
							getline(FileStream,nm1);
							inp1 = stoi(nm1,NULL,10);
							cout<<inp1;
						}
						break;
					case 2: {
							cout<<"\nDecimal value for condition limit: ";
							getline(FileStream,nm1);
							size_t sza;
							inp2 = stod(nm1,&sza);
							cout<<inp2;
						}
						break;
					case 3: {
							cout<<"\nText value for condition limit: ";
							getline(FileStream,inp3);
							cout<<inp3;
						}
						break; 
					case 4: {
							cout<<"\nDate value for condition limit: ";
							cout<<"\nYear: ";
							getline(FileStream,nm1);
							yy = stoi(nm1,NULL,10);
							cout<<yy;
							cout<<"\nMonth: ";
							getline(FileStream,nm1);
							mm = stoi(nm1,NULL,10);
							cout<<mm;
							cout<<"\nDate: ";
							getline(FileStream,nm1);
							dd = stoi(nm1,NULL,10);
							cout<<dd;
							Date tmp(dd,mm,yy);
							inp4 = tmp;
						}
						break;
					default: break;
				}
			}		
		}
	
		TransitionCondition(const TransitionCondition& T)			// copy
		{
			decidingProperty = T.decidingProperty;
			condition = T.condition;
			conditionResult = T.conditionResult;
			inp1 = T.inp1;
			inp2 = T.inp2;
			inp3 = T.inp3;
			inp4 = T.inp4;
			ConditionName = T.ConditionName;
		}

		void DisplayCondition()			// display condition
		{
			decidingProperty.displayName();
			cout<<" ";
			if(!conditionResult)
				condition += 3;
			switch(condition)
			{
				case 1: cout<<"=";break;
				case 2: cout<<"<";break;
				case 3: cout<<">";break;
				case 4: cout<<"!=";break;
				case 5: cout<<">=";break;
				case 6: cout<<"<=";break;
				case 7: cout<<"epsilon";break;
				default: cout<<"";break;
			}
			
			cout<<" ";
			if(!conditionResult)
				condition -= 3;

			switch(decidingProperty.typeReturn())
			{
				case 1: cout<<inp1;break;
				case 2: cout<<inp2;break;
				case 3: cout<<inp3;break;
				case 4: inp4.displayDate();break;
				default: cout<<"";break;
			}
		}
	
};

class Transition
{
	public:
	string serviceName;		//service which transition belongs to
	int inputState;			// outgoing state
	int outputState;		// incoming state
	TransitionCondition TCondition; // which condition leads to transition
	
		Transition* nextTransition;		// pointer to list
		
		Transition()
		{			// default
		}

		Transition(const Transition& T)
		{
			serviceName = T.serviceName;
			inputState = T.inputState;		// copy
			outputState = T.outputState;
			TCondition = T.TCondition;
		}

		void DisplayTransition()			//display transition
		{
			cout<<"Input State Number: "<<inputState<<"\t Output State Number: "<<outputState;
			cout<<"\nCondition: Property ";
			TCondition.DisplayCondition();	
		}

		Transition(string servN, int ip, int op, TransitionCondition prop)		//assign transition
		{
			serviceName = servN;
			inputState = ip;
			outputState = op;
			TCondition = prop;			
		}
};

class ServiceAutomata
{
	public:
	Service S;				// Service name, list of properties and conditions
	State* firstState;			// pointer to first state (list of states)
	Transition* firstTransition;		// pointer to first transition (list of transitions)
	int stateCount;				// number of states
	int transitionCount;			// number of transitions
	
	
		ServiceAutomata()		// default
		{
			stateCount = 0;
			transitionCount = 0;
			firstState = NULL;
			firstTransition = NULL;
		}

		ServiceAutomata(const ServiceAutomata& SA)		// copy
		{
			S = SA.S;
			firstState = SA.firstState;
			firstTransition = SA.firstTransition;
			stateCount = SA.stateCount;
			transitionCount = SA.transitionCount;
		}

		Service InitializeService(Service S, string n, int ct)		// obtaining each property of service
		{
			S.serviceName = n;
			S.propertyCount = ct;
			S.property = new ServiceInput[S.propertyCount];			

			for (int i = 0; i < S.propertyCount; i++)
			{

				string n1;
				cout<<"\n\nName of Property: ";
				getline(FileStream,n1);
				cout<<n1;

				cout<<"\nProperty "<<i+1<<":\n1. Integer\n2. Decimal\n3. Text\n4. Date\nChoose Type: ";
				int t;
				string nm1;
				getline(FileStream,nm1);
				t = stoi(nm1,NULL,10);
				cout<<t;
				S.property[i] = assignProperty(t,n1);
			}
			return S;
		}


		void ConstructAutomata()		// construction of service automaton
		{

			cout<<"Enter name of file: ";
			cin>>FileName;
	
			FileStream.open(FileName);

			stateCount = 0;
			transitionCount = 0;
			firstState = NULL;
			firstTransition = NULL;
			int pch;
			
			// Obtaining service details

			string nm1;
			cout<<"\nName of Service: ";
			getline(FileStream,nm1);
			cout<<nm1;

			int ct1;
			cout<<"\n\nNumber of Service properties: ";
			getline(FileStream,nm1);
			ct1 = stoi(nm1,NULL,10);
			cout<<ct1;

			S = InitializeService(S,nm1,ct1);

			int propertyCount = S.returnCount();
			ServiceInput* properties = new ServiceInput[propertyCount];
			properties = S.Property();

			cout<<"\n\nNumber of conditions: ";
			int conditionCount;
			getline(FileStream,nm1);
			conditionCount = stoi(nm1,NULL,10);
			cout<<conditionCount;

			conditionCount *= 2;							// To find negation of conditions
			TransitionCondition* transCondition = new TransitionCondition[conditionCount];
			
			int loopvar;
			for(loopvar = 0; loopvar < conditionCount/2; loopvar++)			// read all conditions
			{
				string nmc;
				cout<<"\n\nName of condition: ";
				getline(FileStream,nmc);
				cout<<nmc<<"\n";

				for(int i = 0; i < propertyCount; i++)
				{
					cout<<i+1<<". "<<properties[i].nameReturn()<<"\n";
				}
				
				cout<<"Property which causes the transition: ";
				getline(FileStream,nm1);
				pch = stoi(nm1,NULL,10);
				cout<<pch;
				
				TransitionCondition* tmpcon = new TransitionCondition(nmc,properties[pch-1]);
				transCondition[loopvar] = *tmpcon;
			}
	
			int prevloopvar = 0;
			for(loopvar = conditionCount/2; loopvar < conditionCount; loopvar++)		// generate condition negations
			{
				TransitionCondition* tmpcon = new TransitionCondition(transCondition[prevloopvar]);
				tmpcon->ConditionName = "!" + transCondition[prevloopvar].ConditionName;
				tmpcon->conditionResult = !tmpcon->conditionResult;
				transCondition[loopvar] = *tmpcon;
				prevloopvar++;
			}

			// Initial State
						
			firstState = new State(S.ServiceName(),1,"Start",true,false);
			firstState->nextState = NULL;
			stateCount++; 

			State* lastState = firstState;
			Transition* lastTransition;			

			// Other States

			cout<<"\nStates of the Automaton: \n";
			int cont;

			do
			{
					string stn;
					cout<<"\nNew state reached: ";
					getline(FileStream,stn);
					cout<<stn;
					
					State* newState = new State(S.ServiceName(),stateCount+1,stn,false,false);
					newState->nextState = NULL;
					cout<<"\n"<<newState->StateName()<<"\nIs this state a final state? ";

					string ynch;
					getline(FileStream,ynch);
					cout<<ynch;

					if(ynch == "y"||ynch == "Y")
						newState->setFinish();
					cout<<"\n";
					stateCount++;

					lastState->nextState = newState;
					lastState = newState;

					cout<<"\nEnter 1 to continue defining states. Enter 0 to stop : ";
					getline(FileStream,nm1);
					cont = stoi(nm1,NULL,10);
					cout<<cont;
			}while(cont == 1);
				
			cout<<"\nTransitions of the Automaton: \n";
			do
			{
				int ich,och;
							 	
				// Input State
			
				State* statePtr = firstState;
				cout<<"\n";	
				while(statePtr != NULL)
				{
					cout<<"\n"<<statePtr->StateNumber()<<". "<<statePtr->StateName();
					statePtr = statePtr->nextState;
				}
				cout<<"\n\nChoice of input state: ";
				getline(FileStream,nm1);
				ich = stoi(nm1,NULL,10);
				cout<<ich;

				// Output State

				
					statePtr = firstState;
					while(statePtr != NULL)
					{
						cout<<"\n"<<statePtr->StateNumber()<<". "<<statePtr->StateName();
						statePtr = statePtr->nextState;
					}
					cout<<"\n\nChoice of output state: ";
					getline(FileStream,nm1);
					och = stoi(nm1,NULL,10);
					cout<<och<<"\n";
				
				// Transition Conditions

				for(int i = 0; i < conditionCount; i++)
				{
					cout<<i+1<<". ";
					transCondition[i].DisplayCondition();
					cout<<"\n";
				}
				int cch;
				cout<<"\nChoice of Condition: ";
				getline(FileStream,nm1);
				cch = stoi(nm1,NULL,10);
				cout<<cch;			
				
				if(firstTransition == NULL)
				{
					firstTransition = new Transition(S.ServiceName(),ich,och,transCondition[cch-1]);
					firstTransition->nextTransition = NULL;
					transitionCount++;
					lastTransition = firstTransition;
				}
				else
				{
				  	Transition *newTransition = new Transition(S.ServiceName(),ich,och,transCondition[cch-1]);
					newTransition->nextTransition = NULL;
					transitionCount++;
					lastTransition->nextTransition = newTransition;
					lastTransition = newTransition; 
				}

				cout<<"\nEnter 1 to continue defining transitions. Enter 0 to stop : ";
				getline(FileStream,nm1);
				cont = stoi(nm1,NULL,10);
				cout<<cont;
			} while(cont == 1);
			cout<<"\n\n";
		}

		void DisplayAutomata()		// Display Automaton
		{
			// Service
			
			cout<<"Service Name: "<<S.ServiceName();
			cout<<"\nNumber of Properties: "<<S.returnCount();

			// States
			
			cout<<"\n\nNumber of States: "<<stateCount;
			cout<<"\n";
			State* stPtr = firstState;
			while(stPtr != NULL)
			{
				stPtr->DisplayState();
				cout<<"\n";
				stPtr = stPtr->nextState;
			}

			// Transitions
			
			cout<<"\nNumber of Transitions: "<<transitionCount;
			cout<<"\n";
			Transition* trPtr = firstTransition;
			while(trPtr != NULL)
			{
				trPtr->DisplayTransition();
				cout<<"\n";
				trPtr = trPtr->nextTransition;
			}		
		}
};

