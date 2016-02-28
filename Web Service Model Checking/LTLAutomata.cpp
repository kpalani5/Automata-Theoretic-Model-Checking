#include "LTLFormula.cpp"
#include <string>

using namespace std;

int UniqueNodeNumber = 1;

class GraphNode				// Based on Gerth Peled Vardi Wolper algorithm
{
public:
	
	string name;
	GraphNode* incoming;
	string father;

	LTLExpression* New;
	LTLExpression* Old;
	LTLExpression* Next;

	GraphNode* nextNode;
	bool Accepting;

	GraphNode()			// default
	{
		New = NULL;
		Old = NULL;
		Next = NULL;
		nextNode = NULL;
		incoming = NULL;
		name = "\0";
		father = "\0";
		Accepting = false;
	}

	GraphNode(const GraphNode& G)		// copy
	{
		name = G.name;
		incoming = G.incoming;
		father = G.father;
		New = G.New;
		Old = G.Old;
		Next = G.Next;
		nextNode = G.nextNode;
		Accepting = G.Accepting;
	}
		
};

int SetCount(LTLExpression* Set)		// Number of expressions in set
{
	if(Set == NULL)
		return 0;

	int ct = 0;
	LTLExpression* tmp = Set;

	while(tmp != NULL)
	{
		ct++;
		tmp = tmp->next;
	}
	
	return ct;
}

bool SetEquality(LTLExpression* N1, LTLExpression* N2)		// To check if set N1 = set N2
{
	LTLExpression* tmp1 = N1;
	LTLExpression* tmp2 = N2;

	if(N1 == NULL && N2 == NULL)
		return true;
		
	if(SetCount(N1) != SetCount(N2))
		return false;
	
	while(tmp1 != NULL)
	{
		tmp2 = N2;
		int flag = 0;
		while(tmp2 != NULL)
		{
			if(tmp1->formula != tmp2->formula)
				tmp2 = tmp2->next;
			else
			{
				tmp1 = tmp1->next;
				flag = 1;
				tmp2 = NULL;
			}
		}
		if(flag == 0)
			return false;
	}

	return true; 	
}

void SetToArray(LTLExpression Array[], LTLExpression* Set)		// To convert set of expressions to array
{
	LTLExpression* tmp = Set;
	
	int ct = 0;

	while(tmp != NULL)
	{
		Array[ct] = *tmp;
		ct++;
		tmp = tmp->next;
	}
}

LTLExpression* ArrayToSet(LTLExpression* Set, LTLExpression Array[], int ct)		// Array of expressions to set
{
	if(ct == 0)
		return NULL;
	Set = new LTLExpression(Array[0]);
	LTLExpression* last = Set;
	for(int i = 1; i < ct; i++)
	{
		LTLExpression* tmp = new LTLExpression(Array[i]);
		last->next = tmp;
		last = tmp;
	}
	last->next = NULL;
	return Set;
}

LTLExpression* ExpressionUnion(LTLExpression* N1, LTLExpression* N2)		// Union of expression sets
{
	if(N1 != NULL)
	{
	LTLExpression* tmp = N1;
	LTLExpression* last;
	while(tmp != NULL)
	{
		last = tmp;
		tmp = tmp->next;
	}

	last->next = N2;
	

	int ct = SetCount(N1);
	LTLExpression* Array = new LTLExpression[ct];
	SetToArray(Array,N1);
	LTLExpression* Array2 = new LTLExpression[ct];

	int i,j;
	int ct2 = 1;
	Array2[0] = Array[0];
	for(i=1;i<ct;i++)
	{
		int flag = 0;
		for(j=0;j<ct;j++)
		{
			if(i != j && Array[i].formula == Array[j].formula)
				flag = 1;
		}

		if(flag == 0)
			Array2[ct2++] = Array[i];
	}

	N1 = ArrayToSet(N1,Array2,ct2);
	}
	else
	{
		N1 = new LTLExpression(*N2);
	}
	return N1;
}

int ListCount(GraphNode* List)			// Number of graph nodes in list
{
	if(List == NULL)
		return 0;

	int ct = 0;
	GraphNode* tmp = List;

	while(tmp != NULL)
	{
		ct++;
		tmp = tmp->nextNode;
	}
	
	return ct;
}

void ListToArray(GraphNode Array[], GraphNode* List)		// Convert list of graph nodes to array
{
	GraphNode* tmp = List;
	
	int ct = 0;

	while(tmp != NULL)
	{
		Array[ct] = *tmp;
		ct++;
		tmp = tmp->nextNode;
	}
}

GraphNode* ArrayToList(GraphNode* List, GraphNode Array[], int ct)		// Convert array of graph nodes to list
{
	if(ct != 0)
	{
		List = new GraphNode(Array[0]);
		GraphNode* last = List;
		for(int i=1; i<ct; i++)
		{
			GraphNode* tmp = new GraphNode(Array[i]);
			last->nextNode = tmp;
			last = tmp;
		}
	}
	return List;
}

GraphNode* NodeUnion(GraphNode* N1, GraphNode* N2)		// Union of two graph node lists
{
	if(N1 != NULL)
	{
	GraphNode* tmp = N1;
	GraphNode* last;
	while(tmp != NULL)
	{
		last = tmp;
		tmp = tmp->nextNode;
	}

	last->nextNode = N2;
	
	int ct = ListCount(N1);
	GraphNode* Array = new GraphNode[ct];
	ListToArray(Array,N1);
	GraphNode* Array2 = new GraphNode[ct];

	int i,j;
	int ct2 = 1;
	Array2[0] = Array[0];
	for(i=1;i<ct;i++)
	{
		int flag = 0;
		for(j=0;j<ct;j++)
		{
			if(i != j && Array[i].name == Array[j].name)
				flag = 1;
		}

		if(flag == 0)
			Array2[ct2++] = Array[i];
	}

	N1 = ArrayToList(N1,Array2,ct2);
	}
	else
	{
		N1 = new GraphNode(*N2);
	}	
	return N1;
}

LTLExpression* SetComplement(LTLExpression* N, LTLExpression* E)		// Finding complement between two sets
{
	if(E == NULL)
		return N;
	int ct = SetCount(E);
	LTLExpression* arr = new LTLExpression[ct];
	SetToArray(arr,E);
	
	LTLExpression* R = N;

	for(int k = 0; k < ct; k++)
	{
		string form = arr[k].formula;
		LTLExpression* tmp = R;
		if(tmp->formula == form)
		{
			R = tmp->next;
		}
		else
		{
			LTLExpression* prev = R;
			tmp = tmp->next;
			while(tmp != NULL)
			{
				if(tmp->formula == form)
				{
					prev->next = tmp->next;
					break;
				}
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}
	return R;
}

LTLExpression* New1(LTLExpression E)			// Gerth peled algorithm
{
	LTLExpression* R;
	if(E.operators[E.opct-1].opno == 4)
	{
		R = new LTLExpression(*E.operators[E.opct-1].var1);
	}
	else if(E.operators[E.opct-1].opno == 5)
	{
		R = new LTLExpression(*E.operators[E.opct-1].var2);
	}
	else 
	{
		R = new LTLExpression(*E.operators[E.opct-1].var1);
	}
	return R;
}

LTLExpression* New2(LTLExpression E)		// Gerth peled algorithm
{
	LTLExpression* R;
	if(E.operators[E.opct-1].opno == 4)
	{
		R = E.operators[E.opct-1].var2;
	}
	else if(E.operators[E.opct-1].opno == 5)
	{
		R = new LTLExpression(*E.operators[E.opct-1].var1);
		LTLExpression* S = new LTLExpression(*E.operators[E.opct-1].var2);
		R->next = S;
	}
	else 
	{
		R = E.operators[E.opct-1].var2;
	}
	return R;
}

LTLExpression* Next1(LTLExpression E)		// Gerth peled algorithm
{
	LTLExpression* R;
	if(E.operators[E.opct-1].opno == 4)
	{
		R = new LTLExpression(E);
	}
	else if(E.operators[E.opct-1].opno == 5)
	{
		R = new LTLExpression(E);
	}
	else 
	{
		R = NULL;
	}
	return R;
}

LTLExpression* And1(LTLExpression E)		// gerth peled algorithm
{
	LTLExpression* R = E.operators[E.opct-1].var1;
	LTLExpression* S = E.operators[E.opct-1].var2;
	R->next = S;
	return R;
}

bool NegationExists(LTLExpression* Set, LTLExpression E)		// To check if negation of a formula exists in the set
{
	LTLExpression R;
	if(E.opct != 0 && E.operators[E.opct-1].opno == 0)
	{
		R = *E.operators[E.opct-1].var1;
	}
	else
	{
		R.varct = E.varct;
		R.opct = E.opct + 1;
		R.variables = new LTLExpression[R.varct];
		R.operators = new LTLOperator[R.opct];
		int k;
		for(k = 0; k < E.varct; k++)
		{
			R.variables[k] = E.variables[k];
		}
		for(k = 0; k < E.opct; k++)
		{
			R.operators[k] = E.operators[k];
		}
		LTLExpression* Nl = NULL;
		LTLExpression* tmpE4 = new LTLExpression(E);
		LTLOperator* tm = new LTLOperator(0,tmpE4,Nl);
		R.operators[R.opct - 1] = *tm;
		R.formula = "!" + E.formula;
	}
	
	LTLExpression* Arr = new LTLExpression[SetCount(Set)];
	SetToArray(Arr,Set);
	int j;
	for(j = 0; j < SetCount(Set); j++)
	{
		if(Arr[j].formula == R.formula)
		{
			return true;
		}
	}
	return false;
}

LTLExpression* Neg(LTLExpression E)			// find negation of formula
{
	LTLExpression* R;
	if(E.opct != 0 && E.operators[E.opct-1].opno == 0)
	{
		R = new LTLExpression(*E.operators[E.opct-1].var1);
	}
	else
	{
		R = new LTLExpression();
		R->varct = E.varct;
		R->opct = E.opct + 1;
		R->variables = new LTLExpression[R->varct];
		R->operators = new LTLOperator[R->opct];
		int k;
		for(k = 0; k < E.varct; k++)
		{
			R->variables[k] = E.variables[k];
		}
		for(k = 0; k < E.opct; k++)
		{
			R->operators[k] = E.operators[k];
		}
		LTLExpression* Nl = NULL;
		LTLExpression* tmpE4 = new LTLExpression(E);
		LTLOperator* tm = new LTLOperator(0,tmpE4,Nl);
		R->operators[R->opct - 1] = *tm;
		R->formula = "!" + E.formula;
	}
	
	return R;
}

void DisplayGraph(GraphNode* Nodes)				// display the graph
{
	GraphNode* tmp = Nodes;
	while(tmp != NULL)
	{
		cout<<"Name: "<<tmp->name<<"\t"<<"Father: "<<tmp->father<<"\t"<<"Accepting State: ";
		if(tmp->Accepting)
			cout<<"YES";
		else
			cout<<"NO";
		cout<<"\n";
		GraphNode* tmpin = tmp->incoming;
		cout<<"Incoming Nodes: ";
		while(tmpin != NULL)
		{
			cout<<tmpin->name<<" ";
			tmpin = tmpin->nextNode;
		}
		cout<<"\n";
		LTLExpression* tmpold = tmp->Old;
		cout<<"Formulas in Old Set: ";
		while(tmpold != NULL)
		{
			cout<<tmpold->formula<<" ";
			tmpold = tmpold->next;
		}
		cout<<"\n";
		LTLExpression* tmpnxt = tmp->Next;
		cout<<"Formulas in Next Set: ";
		while(tmpnxt != NULL)
		{
			cout<<tmpnxt->formula<<" ";
			tmpnxt = tmpnxt->next;
		}
		cout<<"\n\n";
		tmp = tmp->nextNode;
	}
}

// expand and createGraph are a part of the GPVW algorithm

GraphNode* expand(GraphNode Node, GraphNode* NodeSet)
{
	cout<<"\n";
	if(Node.New == NULL)
	{
		int ct = ListCount(NodeSet);
		GraphNode* NodeSetArray = new GraphNode[ct];
		ListToArray(NodeSetArray,NodeSet);

		int i;
		int flag = 0;
		for(i=0; i<ct; i++)
		{
			if(SetEquality(Node.Old,NodeSetArray[i].Old) && SetEquality(Node.Next,NodeSetArray[i].Next))
			{
				GraphNode* tmpArr = new GraphNode[ListCount(NodeSetArray[i].incoming)];
				ListToArray(tmpArr,NodeSetArray[i].incoming);
				NodeSetArray[i].incoming = ArrayToList(NodeSetArray[i].incoming,tmpArr,ListCount(NodeSetArray[i].incoming));
				NodeUnion(NodeSetArray[i].incoming,Node.incoming);
				flag = 1;
			}
		}
		if(flag == 1)
		{
			NodeSet = ArrayToList(NodeSet,NodeSetArray,ct);
			return NodeSet;
		}
		else
		{
			GraphNode* N = new GraphNode(Node);
			if(N->Old == NULL && N->Next == NULL)
				N->Accepting = true;
			NodeSet = NodeUnion(NodeSet,N);

			cout<<"\n Name for created node: ";
			string nm = "N" + to_string(UniqueNodeNumber);
			UniqueNodeNumber++;
			cout<<nm;

			GraphNode M;
			M.name = nm;
			M.father = nm;
			M.incoming = new GraphNode(*N);
			
			if(Node.Next != NULL)
			{
				LTLExpression* tmpArr1 = new LTLExpression[SetCount(Node.Next)];
				SetToArray(tmpArr1,Node.Next);
				M.New = ArrayToSet(M.New,tmpArr1,SetCount(Node.Next));
			}

			return expand(M,NodeSet);
		}
	}
	else
	{
		LTLExpression E;
		E = *Node.New;
		E.next = NULL;
		LTLExpression* tmpE = new LTLExpression(E);
		Node.New = SetComplement(Node.New,tmpE);
		
		if((E.opct ==0) || (E.varct == 1 && E.operators[E.opct-1].opno < 1))
		{
			if(NegationExists(Node.Old,E) || E.formula == "False")
			{
				return NodeSet;
			}
			else
			{
				LTLExpression* tmpE3 = new LTLExpression(E);
				Node.Old = ExpressionUnion(Node.Old,tmpE3);
				if((E.varct > 1 && E.operators[E.opct-1].opno == 0) || E.opct > 1)
				{
					Node.New = ExpressionUnion(Node.New,Neg(*tmpE3));
				}
				return expand(Node,NodeSet);
			}
		}
		else if(E.operators[E.opct-1].opno == 4 ||E.operators[E.opct-1].opno == 5 ||E.operators[E.opct-1].opno == 7)
		{
			cout<<"\n Name for created node 1: ";
			string nm1 = "N" + to_string(UniqueNodeNumber);
			UniqueNodeNumber++;
			cout<<nm1;

			cout<<"\n Name for created node 2: ";
			string nm2 = "N" + to_string(UniqueNodeNumber);
			UniqueNodeNumber++;
			cout<<nm2;

			GraphNode M1;
			M1.name = nm1;
			M1.father = Node.name;
			M1.incoming = Node.incoming;

			LTLExpression* tmpArr1 = new LTLExpression[SetCount(Node.New)];
			SetToArray(tmpArr1,Node.New);
			M1.New = ArrayToSet(M1.New,tmpArr1,SetCount(Node.New));
			M1.New = ExpressionUnion(M1.New,SetComplement(New1(E),Node.Old));

			LTLExpression* tmpArr2 = new LTLExpression[SetCount(Node.Old)];
			SetToArray(tmpArr2,Node.Old);
			M1.Old = ArrayToSet(M1.Old,tmpArr2,SetCount(Node.Old));
			LTLExpression* tmpE1 = new LTLExpression(E);
			M1.Old = ExpressionUnion(M1.Old,tmpE1);
			
			LTLExpression* tmpArr3 = new LTLExpression[SetCount(Node.Next)];
			SetToArray(tmpArr3,Node.Next);
			M1.Next = ArrayToSet(M1.Next,tmpArr3,SetCount(Node.Next));
			if(Next1(E) != NULL)
				M1.Next = ExpressionUnion(M1.Next,Next1(E));
			
			GraphNode M2;
			M2.name = nm2;
			M2.father = Node.name;
			M2.incoming = Node.incoming;

			LTLExpression* tmpArr4 = new LTLExpression[SetCount(Node.New)];
			SetToArray(tmpArr4,Node.New);
			M2.New = ArrayToSet(M2.New,tmpArr4,SetCount(Node.New));
			M2.New = ExpressionUnion(M2.New,SetComplement(New2(E),Node.Old));

			LTLExpression* tmpArr5 = new LTLExpression[SetCount(Node.Old)];
			SetToArray(tmpArr5,Node.Old);
			M2.Old = ArrayToSet(M2.Old,tmpArr5,SetCount(Node.Old));
			LTLExpression* tmpE2 = new LTLExpression(E);
			M2.Old = ExpressionUnion(M2.Old,tmpE2);
			
			LTLExpression* tmpArr6 = new LTLExpression[SetCount(Node.Next)];
			SetToArray(tmpArr6,Node.Next);
			M2.Next = ArrayToSet(M2.Next,tmpArr6,SetCount(Node.Next));

			return expand(M2,expand(M1,NodeSet));
		}
		else if(E.operators[E.opct-1].opno == 6)
		{
			GraphNode M1;
			M1.name = Node.name;
			M1.father = Node.father;
			M1.incoming = Node.incoming;

			LTLExpression* tmpArr1 = new LTLExpression[SetCount(Node.New)];
			SetToArray(tmpArr1,Node.New);
			M1.New = ArrayToSet(M1.New,tmpArr1,SetCount(Node.New));
			M1.New = ExpressionUnion(M1.New,SetComplement(And1(E),Node.Old));

			LTLExpression* tmpArr2 = new LTLExpression[SetCount(Node.Old)];
			SetToArray(tmpArr2,Node.Old);
			M1.Old= ArrayToSet(M1.Old,tmpArr2,SetCount(Node.Old));
			LTLExpression* tmpE1 = new LTLExpression(E);
			M1.Old = ExpressionUnion(M1.Old,tmpE1);
			
			LTLExpression* tmpArr3 = new LTLExpression[SetCount(Node.Next)];
			SetToArray(tmpArr3,Node.Next);
			M1.Next = ArrayToSet(M1.Next,tmpArr3,SetCount(Node.Next));

			return expand(M1,NodeSet);
		}
		else if(E.operators[E.opct-1].opno == 1)
		{
			GraphNode M1;
			M1.name = Node.name;
			M1.father = Node.father;
			M1.incoming = Node.incoming;

			LTLExpression* tmpArr1 = new LTLExpression[SetCount(Node.New)];
			SetToArray(tmpArr1,Node.New);
			M1.New = ArrayToSet(M1.New,tmpArr1,SetCount(Node.New));

			LTLExpression* tmpArr2 = new LTLExpression[SetCount(Node.Old)];
			SetToArray(tmpArr2,Node.Old);
			M1.Old = ArrayToSet(M1.Old,tmpArr2,SetCount(Node.Old));
			LTLExpression* tmpE1 = new LTLExpression(E);
			M1.Old = ExpressionUnion(M1.Old,tmpE1);
			
			LTLExpression* tmpArr3 = new LTLExpression[SetCount(Node.Next)];
			SetToArray(tmpArr3,Node.Next);
			M1.Next = ArrayToSet(M1.Next,tmpArr3,SetCount(Node.Next));
			M1.Next = ExpressionUnion(M1.Next,E.operators[E.opct-1].var1);

			return expand(M1,NodeSet);
		}
		else if(E.operators[E.opct-1].opno == 2)
		{
			LTLExpression* Exp = new LTLExpression(E.varct+1,E.opct);
			LTLExpression* False = new LTLExpression(1,0);
			False->formula = "False";
			Exp->variables[0] = *False;
			int i;
			for(i = 0; i < E.varct; i++)
			{
				Exp->variables[i+1] = E.variables[i];
			}
			LTLExpression* tmpE4 = new LTLExpression(*E.operators[E.opct-1].var1);
			LTLOperator* tm = new LTLOperator(5,False,tmpE4);
			for(i = 0; i < E.opct-1; i++)
			{
				Exp->operators[i] = E.operators[i];
			}
			Exp->operators[Exp->opct - 1] = *tm;
			
			Exp->formula = "FalseR" + tmpE4->formula;
			Exp->next = Node.New;
			Node.New = Exp;

			return expand(Node,NodeSet);
		}
		else if(E.operators[E.opct-1].opno == 3)
		{
			LTLExpression* Exp = new LTLExpression(E.varct+1,E.opct);
			LTLExpression* True = new LTLExpression(1,0);
			True->formula = "True";
			Exp->variables[0] = *True;
			int i;
			for(i = 0; i < E.varct; i++)
			{
				Exp->variables[i+1] = E.variables[i];
			}
			LTLExpression* tmpE4 = new LTLExpression(*E.operators[E.opct-1].var1);
			LTLOperator* tm = new LTLOperator(4,True,tmpE4);
			for(i = 0; i < E.opct-1; i++)
			{
				Exp->operators[i] = E.operators[i];
			}
			Exp->operators[Exp->opct - 1] = *tm;
			
			Exp->formula = "TrueU" + tmpE4->formula;
			Exp->next = Node.New;
			Node.New = Exp;

			return expand(Node,NodeSet);
		}
	}
	return NodeSet;
}

GraphNode* CreateGraph(LTLExpression V)
{
	cout<<"\n Name for created node: ";
	string nm = "N" + to_string(UniqueNodeNumber);
	UniqueNodeNumber++;
	cout<<nm;
	
	GraphNode *init = new GraphNode();
	init->name = "init";
	init->father = "init";
	init->incoming = NULL;

	GraphNode S;
	S.name = nm;
	S.father = nm;
	S.incoming = init;
	S.New = &V;
	
	GraphNode* NodeSet = NULL;

	return expand(S,NodeSet);
}

class FormulaSet
{
	public:
		int forct;			// Number of formulas
		LTLExpression* TrueFormulas;	// set of formulas
	
		FormulaSet* nextSet;		// pointer to next formula in set

	FormulaSet()				// default
	{
		forct = 0;
		TrueFormulas = NULL;
		nextSet = NULL;
	}

	FormulaSet(int c)			// assign
	{
		forct = c;
		TrueFormulas = NULL;
		nextSet = NULL;
	}

	FormulaSet(const FormulaSet& F)		// copy
	{
		forct = F.forct;
		TrueFormulas = F.TrueFormulas;
		nextSet = F.nextSet;
	}	
};

bool ExistingInSet(LTLExpression E, LTLExpression* Set)		// To check if expression exists in set
{
	LTLExpression* tmp = Set;
	while(tmp != NULL)
	{
		if(E.formula == tmp->formula)
			return true;
		tmp = tmp->next;
	}
	return false;
}

bool ExistingInTrueSet(FormulaSet T, FormulaSet* TSet)		// To check if formula is a part of set
{
	FormulaSet* tmp = TSet;
	while(tmp != NULL)
	{
		if(T.forct == tmp->forct)
		{
			int flag1 = 0;
			LTLExpression* tmpE1 = T.TrueFormulas;
			
			while(tmpE1 != NULL)
			{
				int flag2 = 0;
				LTLExpression* tmpE2 = tmp->TrueFormulas;
				while(tmpE2 != NULL)
				{
					if(tmpE1->formula == tmpE2->formula)
						flag2 = 1;
					tmpE2 = tmpE2->next;
				}
				if(flag2 == 1)
					tmpE1 = tmpE1->next;
				else
				{
					flag1 = 1;
					tmpE1 = NULL;
				}
			}
			if(flag1 == 0)
				return true;	
		}
		tmp = tmp->nextSet;
	}
	return false;
}

LTLExpression* FormSet(LTLExpression* Set, LTLExpression E, LTLExpression* OldSet)		// Create Formula Set recursively
{
	LTLExpression* tmp = Set;
	LTLExpression* prev = Set;
	while(tmp != NULL)
	{
		prev = tmp;
		tmp = tmp->next;
	}

	if(E.varct == 1 && E.opct == 0)
	{
		tmp = new LTLExpression(E);
		tmp->next = NULL;
		if(!NegationExists(OldSet,E) && !ExistingInSet(E,Set))
		{
			if(prev != NULL)
				prev->next = tmp;
			prev = tmp;
			if(Set == NULL)
				Set = prev;
		}
	}
	else if(E.operators[E.opct-1].opno == 1)
	{
		Set = FormSet(Set,*E.operators[E.opct-1].var1,OldSet);
	}
	else if(E.operators[E.opct-1].opno >= 4 && E.operators[E.opct-1].opno <= 7)
	{
		LTLExpression M1 = *E.operators[E.opct-1].var1;
		LTLExpression M2 = *E.operators[E.opct-1].var2;
		Set = FormSet(Set,M1,OldSet);
		Set = FormSet(Set,M2,OldSet);
	}
	return Set;
}

FormulaSet* CreateTrueSet(FormulaSet* TSets, LTLExpression E, LTLExpression* OldSet)		// Initial formula set creation
{
	FormulaSet* sets = TSets;
	FormulaSet* prev = TSets;
	while(sets != NULL)
	{
		prev = sets;
		sets = sets->nextSet;
	}
	if(E.varct == 1 && E.opct == 0)
	{
		if(!NegationExists(OldSet,E))
		{
			sets = new FormulaSet(1);
			sets->TrueFormulas = new LTLExpression(E);
			sets->TrueFormulas->next = NULL;
			if(!ExistingInTrueSet(*sets,TSets))
			{
				if(prev != NULL)
					prev->nextSet = sets;
				prev = sets;
				if(TSets == NULL)
					TSets = prev;
			}
		}
	}
	else if(E.operators[E.opct-1].opno == 1)
	{
		sets = new FormulaSet(1);
		sets->TrueFormulas = new LTLExpression(1,0);
		sets->TrueFormulas->formula = "phi";
		sets->TrueFormulas->next = NULL;
		if(!ExistingInTrueSet(*sets,TSets))
		{
			if(prev != NULL)
				prev->nextSet = sets;
			prev = sets;
			if(TSets == NULL)
					TSets = prev;
			
		}
		return CreateTrueSet(TSets,*E.operators[E.opct-1].var1,OldSet);
	}
	else if(E.operators[E.opct-1].opno >= 4 && E.operators[E.opct-1].opno <= 7)
	{
		sets = new FormulaSet();
		sets->TrueFormulas = FormSet(sets->TrueFormulas,E,OldSet);
		sets->forct = SetCount(sets->TrueFormulas);
		if(!ExistingInTrueSet(*sets,TSets))
		{
			if(prev != NULL)
				prev->nextSet = sets;
			prev = sets;
			if(TSets == NULL)
					TSets = prev;
		}
	}
	return TSets;
}

class GraphTransition
{
	public:
		GraphNode N1;			// incoming
		GraphNode N2;			// outgoing
		FormulaSet* TrueSets;		// set of formulas which are true
	
		GraphTransition* nextEdge;	// next transition in list of transitions

	GraphTransition()			// default
	{
		TrueSets = NULL;
		nextEdge = NULL;
	}

	GraphTransition(GraphNode* G1, GraphNode* G2)		// assign transition
	{
		N1 = *G1;
		N2 = *G2;
		nextEdge = NULL;
		
		LTLExpression* tmp = G2->Old;
		while(tmp != NULL)
		{
			LTLExpression M = *tmp;
			M.next = NULL;
			TrueSets = CreateTrueSet(TrueSets,M,G2->Old);
			tmp = tmp->next;
		}
	}
	
	GraphTransition(const GraphTransition& G)		// copy
	{
		N1 = G.N1;
		N2 = G.N2;
		TrueSets = G.TrueSets;
		nextEdge = G.nextEdge;
	}
};

class FormulaAutomata
{
	public:
		GraphNode* NodeSet;			// set of nodes
		GraphTransition* TransitionSet;		// set of transitions
		
	FormulaAutomata()
	{
		NodeSet = NULL;
		TransitionSet = NULL;
	}

	FormulaAutomata(LTLExpression E)
	{
		TransitionSet = NULL;
		NodeSet = CreateGraph(E);
		GraphNode* tmp = NodeSet;

		GraphTransition* trans;
		GraphTransition* prev;

		while(tmp->nextNode != NULL)
		{
			tmp = tmp->nextNode;
		}
		GraphNode* newtmp = new GraphNode();
		newtmp->name = "init";
		newtmp->father = "init";
		tmp->nextNode = newtmp;

		tmp = NodeSet;
		while(tmp != NULL)
		{
			GraphNode* inc = tmp->incoming;
			while(inc != NULL)
			{
				if(TransitionSet == NULL)
				{
					TransitionSet = new GraphTransition(inc,tmp);
					trans = TransitionSet;
				}
				else
				{
					trans = new GraphTransition(inc,tmp);
					prev->nextEdge = trans;
				}					
				prev = trans;
				inc = inc->nextNode;
			}
			tmp = tmp->nextNode;
		}

		GraphTransition* tmpT = TransitionSet;
		FormulaSet* sets = new FormulaSet(1);
		sets->TrueFormulas = new LTLExpression(1,0);
		sets->TrueFormulas->formula = "phi";
		FormulaSet* prevF = sets;
		FormulaSet* nextF = sets;
		while(tmpT != NULL)
		{
			FormulaSet* tmpF = tmpT->TrueSets;
			while(tmpF != NULL)
			{
				nextF = new FormulaSet(*tmpF);
				nextF->nextSet = NULL;
				if(!ExistingInTrueSet(*nextF,sets))
				{
					prevF->nextSet = nextF;
					prevF = nextF;
				}
				tmpF = tmpF->nextSet;
			}
			tmpT = tmpT->nextEdge;
		}

		tmpT = TransitionSet;
		while(tmpT != NULL)
		{
			if(tmpT->TrueSets == NULL && tmpT->N2.Accepting)
			{
				tmpT->TrueSets = sets;
			}
			tmpT = tmpT->nextEdge;
		}
	}
};

void DisplayGraphTransitions(GraphTransition* G)		// Display Graph Transitions
{
	GraphTransition* tmp = G;
	while(tmp != NULL)
	{
		cout<<"Input State: "<<tmp->N1.name<<"\t";
		cout<<"Output State: "<<tmp->N2.name<<"\n";
		cout<<"True Formula Sets in the Transition:\n";
		FormulaSet* tmpS = tmp->TrueSets;
		while(tmpS != NULL)
		{
			cout<<"{";
			LTLExpression* tmpE = tmpS->TrueFormulas;
			while(tmpE != NULL)
			{
				cout<<tmpE->formula;
				tmpE = tmpE->next;
				if(tmpE != NULL)
					cout<<",";
			}
			cout<<"}\t\t";
			tmpS = tmpS->nextSet;
		}
		cout<<"\n\n"; 
		tmp = tmp->nextEdge;
	}
}

