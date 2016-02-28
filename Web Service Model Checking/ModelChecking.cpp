#include "ServiceToAutomata.cpp"
#include "LTLAutomata.cpp"

class BuchiStates
{
	public:
		string bStateName;		// State name
		bool AcceptingState;		// Accepting or not?
		
		BuchiStates* bNextState;	// next state in list

		BuchiStates()			// default
		{
			bStateName = "\0";
			AcceptingState = false;
			bNextState = NULL;
		}

		BuchiStates(const BuchiStates& B)	// copy
		{
			bStateName = B.bStateName;
			AcceptingState = B.AcceptingState;
			bNextState = B.bNextState;
		}
};

class BuchiTransitionSets
{
	public:
		int bFormulaCount;			// Number of formulas
		string* bTransFormulas;			// List of formulas

		BuchiTransitionSets* bNextSet;		// Pointer to next set

		BuchiTransitionSets()			// default
		{
			bFormulaCount = 0;
			bNextSet = NULL;
		}

		BuchiTransitionSets(int ct)		// assign transition set
		{
			bFormulaCount = ct;
			bTransFormulas = new string[ct];
			bNextSet = NULL;
		}

		BuchiTransitionSets(const BuchiTransitionSets& B)	// copy
		{
			bFormulaCount = B.bFormulaCount;
			bTransFormulas = new string[bFormulaCount];
			for(int i = 0; i < bFormulaCount; i++)
			{
				bTransFormulas[i] = B.bTransFormulas[i];
			}
			bNextSet = B.bNextSet;
		}
};

class BuchiTransitions
{
	public:
		string bInputStateName;			// outgoing
		string bOutputStateName;		// incoming
		BuchiTransitionSets* bTransSets;	// list of sets
		int bSetCount;				// number of sets

		BuchiTransitions* bNextTransition;	// pointer to next transition in list

		BuchiTransitions()			// default
		{
			bTransSets = NULL;
			bNextTransition = NULL;
			bSetCount = 0;
		}

		BuchiTransitions(const BuchiTransitions& B)	// copy
		{
			bInputStateName = B.bInputStateName;
			bOutputStateName = B.bOutputStateName;
			bTransSets = B.bTransSets;
			bSetCount = B.bSetCount;
			bNextTransition = B.bNextTransition;
		}
};

class BuchiAutomaton
{
	public:
		BuchiStates* bStates;			// List of states
		BuchiTransitions* bTransitions;		// List of transitions
		int bStateCount;			// number of states
		int bTransitionCount;			// number of transitions

		BuchiAutomaton()			// default
		{
			bStates = NULL;
			bTransitions = NULL;
			bStateCount = 0;
			bTransitionCount = 0;
		}

		BuchiAutomaton(const BuchiAutomaton& B)		// copy
		{
			bStates = B.bStates;
			bTransitions = B.bTransitions;
			bStateCount = B.bStateCount;
			bTransitionCount = B.bTransitionCount;
		}
};

BuchiAutomaton* ServiceToBuchi(ServiceAutomata S)		// Service automaton construction
{
	BuchiAutomaton* B = new BuchiAutomaton();

	B->bStates = new BuchiStates();
	B->bStates->bStateName = S.firstState->stateName;
	B->bStates->AcceptingState = true;
	BuchiStates* bprev = B->bStates;
	B->bStateCount = 1;

	State* sptr = S.firstState->nextState;
	while(sptr != NULL)
	{
		BuchiStates* btmp = new BuchiStates();
		btmp->bStateName = sptr->stateName;
		btmp->AcceptingState = true;
		bprev->bNextState = btmp;
		bprev = btmp;
		sptr = sptr->nextState;
		B->bStateCount++;
	}
	
	BuchiTransitions* btptr;
	BuchiTransitions* btprev;
	sptr = S.firstState;
	while(sptr != NULL)
	{
		State* sptr2 = S.firstState;
		while(sptr2 != NULL)
		{
			Transition* tptr = S.firstTransition;
			while(tptr != NULL)
			{
				if(sptr->stateNumber == tptr->inputState && sptr2->stateNumber == tptr->outputState)
				{
					int btflag = 0;
					if(B->bTransitions == NULL)
					{
						B->bTransitions = new BuchiTransitions();
						btprev = B->bTransitions;
						btptr = B->bTransitions;
					}
					else
					{
						btptr = NULL;
						BuchiTransitions* bttmp = B->bTransitions;
						while(bttmp != NULL)
						{
						if(bttmp->bInputStateName == sptr->stateName && bttmp->bOutputStateName == sptr2->stateName)
							btptr = bttmp;
						bttmp = bttmp->bNextTransition; 
						}
						if(btptr == NULL)
						{
							btptr = new BuchiTransitions();	
							btflag = 1;
						}
					}
					if(btptr->bTransSets == NULL)
					{
						btptr->bInputStateName = sptr->stateName;
						btptr->bOutputStateName = sptr2->stateName;
						btptr->bTransSets = new BuchiTransitionSets();
						btptr->bTransSets->bFormulaCount++;
					}
					else
					{
						btptr->bTransSets->bFormulaCount++;
					}
					if(btflag == 1)
					{
						btprev->bNextTransition = btptr;
						btprev = btptr;
					}
				}
				tptr = tptr->nextTransition;
			}
			sptr2 = sptr2->nextState;
		}
		sptr = sptr->nextState;
	}

	btptr = B->bTransitions;
	while(btptr != NULL)
	{
		btptr->bSetCount = 1;
		btptr->bTransSets->bTransFormulas = new string[btptr->bTransSets->bFormulaCount];
		for(int i = 0; i < btptr->bTransSets->bFormulaCount; i++)
			btptr->bTransSets->bTransFormulas[i] = "UNASSIGNED";
		btptr = btptr->bNextTransition;
		B->bTransitionCount++;
	}

	sptr = S.firstState;
	while(sptr != NULL)
	{
		State* sptr2 = S.firstState;
		while(sptr2 != NULL)
		{
			Transition* tptr = S.firstTransition;
			while(tptr != NULL)
			{
				if(sptr->stateNumber == tptr->inputState && sptr2->stateNumber == tptr->outputState)
				{
					BuchiTransitions* bttmp = B->bTransitions;
					while(bttmp != NULL)
					{
						if(bttmp->bInputStateName == sptr->stateName && bttmp->bOutputStateName == sptr2->stateName)
							btptr = bttmp;
						bttmp = bttmp->bNextTransition; 
					}
					int i = 0;
					while(btptr->bTransSets->bTransFormulas[i] != "UNASSIGNED")
					{
						i++;
						if(i >= btptr->bTransSets->bFormulaCount)
							break;
					}
					if(i < btptr->bTransSets->bFormulaCount)
					{
						btptr->bTransSets->bTransFormulas[i] = tptr->TCondition.ConditionName;
					}
				}
				tptr = tptr->nextTransition;
			}
			sptr2 = sptr2->nextState;
		}
		sptr = sptr->nextState;
	}
	return B;
}

BuchiTransitions* FormulaTransitionToBuchiTransition(GraphTransition* F)		// Formula Transition conversion
{
	BuchiTransitions* B = new BuchiTransitions();
	B->bInputStateName = F->N1.name;
	B->bOutputStateName = F->N2.name;
	B->bTransSets = new BuchiTransitionSets();
	BuchiTransitionSets* btptr = B->bTransSets;
	BuchiTransitionSets* btprev = B->bTransSets;
	int btflag = 1;
	FormulaSet* fptr = F->TrueSets;
	while(fptr != NULL)
	{
		if(btflag == 0)
			btptr = new BuchiTransitionSets();
		
		btptr->bFormulaCount = fptr->forct;
		btptr->bTransFormulas = new string[btptr->bFormulaCount];
		LTLExpression* eptr = fptr->TrueFormulas;
		int i = 0;
		while(eptr != NULL)
		{
			if(i < btptr->bFormulaCount)
				btptr->bTransFormulas[i] = eptr->formula;
			i++;
			eptr = eptr->next;
		}
		if(btflag == 0)
		{
			btprev->bNextSet = btptr;
			btprev = btptr;
		}
		else
			btflag = 0;
		B->bSetCount++;
		fptr = fptr->nextSet;
	}
	return B;
}

BuchiAutomaton* FormulaToBuchi(FormulaAutomata F)			// Formula automaton conversion
{
	BuchiAutomaton* B = new BuchiAutomaton();
	
	B->bStates = new BuchiStates();
	B->bStates->bStateName = F.NodeSet->name;
	B->bStates->AcceptingState = F.NodeSet->Accepting;
	BuchiStates* bprev = B->bStates;
	B->bStateCount = 1;

	GraphNode* gnptr = F.NodeSet->nextNode;
	while(gnptr != NULL)
	{
		BuchiStates* btmp = new BuchiStates();
		btmp->bStateName = gnptr->name;
		btmp->AcceptingState = gnptr->Accepting;
		bprev->bNextState = btmp;
		bprev = btmp;
		gnptr = gnptr->nextNode;
		B->bStateCount++;
	}

	B->bTransitions = FormulaTransitionToBuchiTransition(F.TransitionSet);
	BuchiTransitions* btprev = B->bTransitions;
	GraphTransition* gtptr = F.TransitionSet->nextEdge;
	B->bTransitionCount = 1;
	
	while(gtptr != NULL)
	{
		BuchiTransitions* bttmp = FormulaTransitionToBuchiTransition(gtptr);
		btprev->bNextTransition = bttmp;
		btprev = bttmp;
		gtptr = gtptr->nextEdge;
		B->bTransitionCount++;
	}
	return B;
}

void DisplayBuchi(BuchiAutomaton* B)			// Display buchi automaton
{
	cout<<"States: "<<B->bStateCount<<"\n\n";
	BuchiStates* bstmp = B->bStates;
	while(bstmp != NULL)
	{
		cout<<"Name: "<<bstmp->bStateName<<"\t\t"<<"Accepting: ";
		if(bstmp->AcceptingState)
			cout<<"YES";
		else
			cout<<"NO";
		cout<<"\n";
		bstmp = bstmp->bNextState;
	}

	cout<<"\nTransitions: "<<B->bTransitionCount<<"\n\n";
	BuchiTransitions* bttmp = B->bTransitions;
	while(bttmp != NULL)
	{
		cout<<"Input State: "<<bttmp->bInputStateName<<"\t\tOutput State: "<<bttmp->bOutputStateName<<"\n";
		cout<<"True Sets of Formulas: "<<bttmp->bSetCount<<"\n";
		BuchiTransitionSets* btstmp = bttmp->bTransSets;
		while(btstmp != NULL)
		{
			cout<<"{";
			for(int i = 0; i < btstmp->bFormulaCount; i++)
			{
				cout<<btstmp->bTransFormulas[i];
				if(i != btstmp->bFormulaCount-1)
					cout<<",";
			}
			cout<<"}";
			cout<<"\t\t";
			btstmp = btstmp->bNextSet;
		}
		cout<<"\n";
		bttmp = bttmp->bNextTransition;
	}
}

bool BuchiSetEquality(string A[], string B[], int ct)		// To check if sets are equal
{
	int i,j;
	for(i = 0; i < ct; i++)
	{
		for(j = 0; j < ct; j++)
		{
			if(A[i] == B[j])
				j = ct+20;
		}
		if(j < ct+10)
			return false;
	}
	return true;
}

bool BuchiSetExists(BuchiTransitions* B, BuchiTransitionSets* S)		// Set exists in list
{
	BuchiTransitionSets* tmp = B->bTransSets;
	while(tmp != NULL)
	{
		if(S->bFormulaCount == tmp->bFormulaCount)
		{
			if(BuchiSetEquality(S->bTransFormulas,tmp->bTransFormulas,S->bFormulaCount))
				return true;
		}
	}
	return false;
}

bool BuchiStateExists(BuchiAutomaton* B, BuchiStates* S)		// State exists in list
{
	BuchiStates* tmp = B->bStates;
	while(tmp != NULL)
	{
		if(S->bStateName == tmp->bStateName)
			return true;
		tmp = tmp->bNextState;
	}
	return false;
}

bool BuchiTransitionExists(BuchiAutomaton* B, BuchiTransitions* T)		// Transition exists in list
{
	BuchiTransitions* tmp = B->bTransitions;
	while(tmp != NULL)
	{
		if(T->bInputStateName == tmp->bInputStateName && T->bOutputStateName == tmp->bOutputStateName)
			return true;
		tmp = tmp->bNextTransition;
	}
	return false;
}

BuchiTransitions* BuchiEquivalentTransition(BuchiAutomaton* B, BuchiTransitions* T)		// Find equivalent transitions
{
	BuchiTransitions* tmp = B->bTransitions;
	while(tmp != NULL)
	{
		if(T->bInputStateName == tmp->bInputStateName && T->bOutputStateName == tmp->bOutputStateName)
			return tmp;
		tmp = tmp->bNextTransition;
	}
	return tmp;
}

bool CheckAcceptance(BuchiAutomaton* F, string S)		// check if accepting state
{
	BuchiStates* tmp = F->bStates;
	while(tmp != NULL)
	{
		if(tmp->bStateName == S)
			return tmp->AcceptingState;
		tmp = tmp->bNextState;
	}
	return false;
}

BuchiAutomaton* ProductAutomaton(BuchiAutomaton* S, BuchiAutomaton* F)		// Form product automaton
{
	BuchiAutomaton* P = new BuchiAutomaton();
	BuchiStates* Ptmp;
	BuchiStates* Pprev;
	BuchiTransitions* Ptrantmp;
	BuchiTransitions* Ptranprev;
	BuchiTransitions* sbt = S->bTransitions;
	while(sbt != NULL)
	{
		BuchiTransitions* fbt = F->bTransitions;
		while(fbt != NULL)
		{
			BuchiTransitionSets* sbts = sbt->bTransSets;
			while(sbts != NULL)
			{
				BuchiTransitionSets* fbts = fbt->bTransSets;
				while(fbts != NULL)
				{
					if(sbts->bFormulaCount == fbts->bFormulaCount)
					{
						if(BuchiSetEquality(sbts->bTransFormulas,fbts->bTransFormulas,sbts->bFormulaCount))
						{
							if(P->bStates == NULL)
							{
								P->bStates = new BuchiStates();
								P->bStates->bStateName = sbt->bInputStateName + "," + fbt->bInputStateName;
								P->bStates->AcceptingState = CheckAcceptance(F,fbt->bInputStateName);
								Pprev = P->bStates;
								P->bStateCount++;
							}
							else
							{
								Ptmp = new BuchiStates();
								Ptmp->bStateName = sbt->bInputStateName + "," + fbt->bInputStateName;
								if(!BuchiStateExists(P,Ptmp))
								{
									Ptmp->AcceptingState = CheckAcceptance(F,fbt->bInputStateName);
									Pprev->bNextState = Ptmp;
									Pprev = Ptmp;
									P->bStateCount++;
								}
							}
							Ptmp = new BuchiStates();
							Ptmp->bStateName = sbt->bOutputStateName + "," + fbt->bOutputStateName;
							if(!BuchiStateExists(P,Ptmp))
							{
								Ptmp->AcceptingState = CheckAcceptance(F,fbt->bOutputStateName);
								Pprev->bNextState = Ptmp;
								Pprev = Ptmp;
								P->bStateCount++;
							}
							
							if(P->bTransitions == NULL)
							{
								P->bTransitions = new BuchiTransitions();
							P->bTransitions->bInputStateName = sbt->bInputStateName + "," + fbt->bInputStateName;
						     P->bTransitions->bOutputStateName = sbt->bOutputStateName + "," + fbt->bOutputStateName;
							P->bTransitions->bTransSets = sbts;
							P->bTransitions->bTransSets->bNextSet = NULL;
							P->bTransitions->bSetCount++;
							Ptranprev = P->bTransitions;
							P->bTransitionCount++;
							}
							else
							{
								Ptrantmp = new BuchiTransitions();
							Ptrantmp->bInputStateName = sbt->bInputStateName + "," + fbt->bInputStateName;
						     Ptrantmp->bOutputStateName = sbt->bOutputStateName + "," + fbt->bOutputStateName;
							Ptrantmp->bTransSets = sbts;
							Ptrantmp->bSetCount++;
							Ptrantmp->bTransSets->bNextSet = NULL;
							if(!BuchiTransitionExists(P,Ptrantmp))
							{
								Ptranprev->bNextTransition = Ptrantmp;
								Ptranprev = Ptrantmp;
								P->bTransitionCount++;
							}
							else
							{
								BuchiTransitions* Pptr = BuchiEquivalentTransition(P,Ptrantmp);
								
								if(!BuchiSetExists(Pptr,sbts))
								{
									BuchiTransitionSets* tstemp = Pptr->bTransSets;
									
									while(tstemp->bNextSet != NULL)
										tstemp = tstemp->bNextSet;
									BuchiTransitionSets* tsnew = sbts;
									tsnew->bNextSet = NULL;
									tstemp->bNextSet = tsnew;
									Pptr->bSetCount++;
								}
							}
							}
						}
					}
					fbts = fbts->bNextSet;
				}
				sbts = sbts->bNextSet;
			}
			fbt = fbt->bNextTransition;
		}
		sbt = sbt->bNextTransition;
	}

	return P;
}

class AdjacencyList				
{
	public:
	string aStateName;			// Name of state
	BuchiStates* AdjacentStates;		// Set of adjacent states

	AdjacencyList()
	{
		AdjacentStates = NULL;
	}

	AdjacencyList(string S)
	{
		aStateName = S;
		AdjacentStates = NULL;
	}
	
	AdjacencyList(const AdjacencyList& A)
	{
		aStateName = A.aStateName;
		AdjacentStates = A.AdjacentStates;
	}

	AdjacencyList(BuchiStates* S, BuchiAutomaton* B)		// Creating adjacency list for DFS
	{
		aStateName = S->bStateName;
		BuchiTransitions* bttmp = B->bTransitions;
		AdjacentStates = NULL;
		while(bttmp != NULL)
		{
			if(bttmp->bInputStateName == S->bStateName)
			{
				BuchiStates* bstmp = AdjacentStates;
				BuchiStates* bsprev = AdjacentStates;
				while(bstmp != NULL)
				{
					bsprev = bstmp;
					bstmp = bstmp->bNextState;
				}
				bstmp = new BuchiStates();
				bstmp->bStateName = bttmp->bOutputStateName;
				bstmp->AcceptingState = CheckAcceptance(B,bttmp->bOutputStateName);
				if(AdjacentStates == NULL)
					AdjacentStates = bstmp;
				else
					bsprev->bNextState = bstmp;
			}
			bttmp = bttmp->bNextTransition;
		}
	}
};

bool DFSCycleCheck(string S1, string S2, AdjacencyList A[], int ct, string acc)		// Check for cycles
{
	int i;
	for(i = 0; i < ct; i++)
	{
		if(A[i].aStateName == S2)
		{
			BuchiStates* bstmp = A[i].AdjacentStates;
			while(bstmp != NULL)
			{
				if(bstmp->AcceptingState)
					acc = "Y";
				if(bstmp->bStateName == S1)
					return true;
				else if(DFSCycleCheck(S1,bstmp->bStateName,A,ct,acc))
					return true;
				bstmp = bstmp->bNextState;
			}
		}
	}
	return false;
}

bool CycleCheck(BuchiAutomaton* P)			// Check for Accepting cycles
{
	BuchiStates* bstmp;
	BuchiTransitions* bttmp = P->bTransitions;
	while(bttmp != NULL)
	{
		if(bttmp->bInputStateName == bttmp->bOutputStateName)
		{
			bstmp = P->bStates;
			while(bstmp != NULL)
			{
				if(bstmp->bStateName == bttmp->bInputStateName && bstmp->AcceptingState)
					return true;
				bstmp = bstmp->bNextState;
			}
		}
		bttmp = bttmp->bNextTransition;
	}
	
	int act = 0;
	AdjacencyList* NodesDFS = new AdjacencyList[P->bStateCount];
	bstmp = P->bStates;
	while(bstmp != NULL)
	{
		AdjacencyList* atmp = new AdjacencyList(bstmp,P);
		NodesDFS[act++] = *atmp;
		bstmp = bstmp->bNextState;
	}

	int i;
	for(i = 0; i < act; i++)
	{
		BuchiStates* bsptr = NodesDFS[i].AdjacentStates;
		while(bsptr != NULL)
		{
			string acc = "N";
			if(DFSCycleCheck(NodesDFS[i].aStateName,bsptr->bStateName,NodesDFS,act,acc))
				if(acc == "Y")
					return true;
			bsptr = bsptr->bNextState;
		}
	}
	return false;  		
}

int main()
{
	cout<<"Service: \n";
	ServiceAutomata S;
	S.ConstructAutomata();
	BuchiAutomaton* ServiceBuchi = ServiceToBuchi(S);
	DisplayBuchi(ServiceBuchi);
	cout<<"\n\nFormula: \n";
	int o,v;
	cout<<"Enter number of variables: ";
	cin>>v;
	cout<<"\n\nEnter number of operators: ";
	cin>>o;
	cout<<"\n";
	if(v > 0)
	{
		LTLExpression* E = new LTLExpression(v,o);
		E->BuildExpression();
		cout<<E->formula;
		FormulaAutomata* F = new FormulaAutomata(*E);
		BuchiAutomaton* FormulaBuchi = FormulaToBuchi(*F);
		DisplayBuchi(FormulaBuchi);
		cout<<"\n\n\nProduct Automaton: \n";
		BuchiAutomaton* ProductBuchi = ProductAutomaton(ServiceBuchi,FormulaBuchi);
		DisplayBuchi(ProductBuchi);
		cout<<"\n\nChecking for Cycles: \n";
		if(CycleCheck(ProductBuchi))
			cout<<"Cycles exist in the graph. The system DOES NOT MEET the specification.\n";
		else
			cout<<"There are no cycles in the graph. The system MEETS the specification.\n";
	}
	return 0;
}

