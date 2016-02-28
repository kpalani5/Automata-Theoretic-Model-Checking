#include <iostream>
#include <string>

using namespace std;

class LTLExpression;

class LTLOperator
{
public:
	string op;			// Actual operator
	int opno;			
	int ary;			// unary or binary
	LTLExpression* var1;		
	LTLExpression* var2;		// binary

	LTLOperator()			// default
	{
	}

	LTLOperator(int opn1,LTLExpression* v1,LTLExpression* v2)		// To assign variables to operators
	{
		opno = opn1;
		var1 = v1;
		var2 = v2;

		/*
			Not		!	0	Unary
			Next  		X  	1  	Unary
			Globally 	G  	2  	Unary
			Finally  	F  	3  	Unary
			Until 		U  	4  	Binary
			Release		R	5	Binary
			And		^	6	Binary
			Or		v	7	Binary
		*/
		
		switch(opno)
		{
			case 1: op = "X";
				ary = 1;
				break;

			case 2: op = "G";
				ary = 1;
				break;

			case 3: op = "F";
				ary = 1;
				break;

			case 4: op = "U";
				ary = 2;
				break;

			case 5: op = "R";
				ary = 2;
				break;

			case 6: op = "^";
				ary = 2;
				break;

			case 7: op = "v";
				ary = 2;
				break;
			
			case 0: op = "!";
				ary = 1;
				break;

			default: break;
		}
	}

	LTLOperator(const LTLOperator& LOp)		// copy
	{
		op = LOp.op;
		opno = LOp.opno;
		ary = LOp.ary;
		var1 = LOp.var1;
		var2 = LOp.var2;
	} 

};

class LTLExpression
{

public:	
	LTLExpression* variables;		// list of variables
	LTLOperator* operators;			// list of operators
	int varct;				// number of variables
	int opct;				// number of operators
	string formula;				// readable format
	LTLExpression* next;			// pointer to next expression in list

	
	LTLExpression()				// default
	{
		varct = 0;
		opct = 0;
		next = NULL;
		formula = "\0";
	}

	LTLExpression(int vct,int oct)		// assign expression
	{
		varct = vct;
		opct = oct;
		variables = new LTLExpression[varct];
		for(int i = 0; i < varct; i++)
		{
			variables[i].varct = 1;	
			variables[i].operators=NULL;
		}
		if(opct != 0)
			operators = new LTLOperator[opct];
		next = NULL;
		formula = "\0";
	}

	LTLExpression(const LTLExpression& E)			// copy
	{
		varct = E.varct;
		opct = E.opct;
		variables = new LTLExpression[varct];
		operators = new LTLOperator[opct];
		int i;
		for(i=0; i<varct; i++)
		{
			if(E.variables != NULL)
			{
				variables[i].varct = 1;
				variables[i].opct = 0;
				variables[i].formula = E.variables[i].formula;
				variables[i].next = NULL;
			}
		}

		for(i=0; i<opct; i++)
		{
			operators[i] = E.operators[i];
		}
		
		formula = E.formula + "\0";
		next = E.next;
	}


	void BuildExpression()			// expression being form (tree structure)
	{
		LTLExpression* subFormulas = new LTLExpression[varct*opct];
		int subct = 0;
		int i,j;

		for(i = 0; i < varct*opct; i++)		// Maximum possible number of subformulas
		{
			subFormulas[i].opct = 0;
			subFormulas[i].varct = 1;
		}

		for(i = 0; i < varct; i++)			// Reading all variables
		{
			cout<<"\nEnter name of variable "<<i+1<<": ";
			cin>>variables[i].formula;
		}
	
		for(i = 0; i < opct; i++)			// Reading all operators
		{
			cout<<"1. Not \t\t2. Next\t\t3. Globally\t4. Finally";
			cout<<"\n5. Until \t6. Release \t7. And \t\t8. Or";
			cout<<"\nEnter operator choice: ";
		 	int opch,varch1,varch2;
			cin>>opch;
			for(j = 0; j < varct; j++)			// Display all variables
			{
				cout<<j+1<<". "<<variables[j].formula;
				cout<<"\n";
			}

			for(j = varct; j < varct + subct; j++)		// Display all sub formulas
			{
				cout<<j+1<<". "<<subFormulas[j-varct].formula;
				cout<<"\n";
			}

			cout<<"Enter first operand number: ";
			cin>>varch1;
			
			LTLExpression* varval1;
			LTLExpression* varval2;

			if(varch1 <= varct)		// If variable chosen is a simple variable
			{
				varval1 = new LTLExpression(variables[varch1-1].varct,variables[varch1-1].opct);
				varval1->formula = variables[varch1-1].formula;
				int k;
				for(k = 0; k < variables[varch1-1].varct; k++)
				{
					varval1->variables[k] = variables[varch1-1];
				}
			}
			else				// If variable chosen is a subformula
			{
				varval1 = new LTLExpression(subFormulas[varch1-1-varct].varct,subFormulas[varch1-1-varct].opct);
				varval1->formula = subFormulas[varch1-1-varct].formula;
				int k;
				for(k = 0; k < subFormulas[varch1-1-varct].varct; k++)
				{
					varval1->variables[k] = subFormulas[varch1-1-varct].variables[k];
				}
				for(k = 0; k < subFormulas[varch1-1-varct].opct; k++)
				{
					varval1->operators[k] = subFormulas[varch1-1-varct].operators[k];
				}
			}
			
			if(opch > 4)			// If operator is binary
			{
				cout<<"Enter second operand number: ";
				cin>>varch2;

				if(varch2 <= varct)
				{
					varval2 = new LTLExpression(variables[varch2-1].varct,variables[varch2-1].opct);
					varval2->formula = variables[varch2-1].formula;
					int k;
					for(k = 0; k < variables[varch2-1].varct; k++)
					{
						varval2->variables[k] = variables[varch2-1];
					}
				}
				else
				{
					varval2 = new LTLExpression(subFormulas[varch2-1-varct].varct,subFormulas[varch2-1-varct].opct);
					varval2->formula = subFormulas[varch2-1-varct].formula;
					int k;
					for(k = 0; k < subFormulas[varch2-1-varct].varct; k++)
					{
						varval2->variables[k] = subFormulas[varch2-1-varct].variables[k];
					}
					for(k = 0; k < subFormulas[varch2-1-varct].opct; k++)
					{
						varval2->operators[k] = subFormulas[varch2-1-varct].operators[k];
					}
				} 
			}
			else
			{
				varval2 = NULL;
			}
			
			
			LTLOperator* tmp = new LTLOperator(opch-1,varval1,varval2);
			operators[i] = *tmp;
			
			if(opch > 4)			// Creating new subformula
			{
				subFormulas[subct].varct = varval1->varct + varval2->varct;
				subFormulas[subct].opct = varval1->opct + varval2->opct + 1;
				subFormulas[subct].variables = new LTLExpression[subFormulas[subct].varct];
				subFormulas[subct].operators = new LTLOperator[subFormulas[subct].opct];
				int k;
				for(k = 0; k < varval1->varct ; k++)
				{
					subFormulas[subct].variables[k] = varval1->variables[k];
				}
				for(k = 0; k < varval2->varct ; k++)
				{
					subFormulas[subct].variables[varval1->varct + k] = varval2->variables[k];
				}
				for(k = 0; k < varval1->opct ; k++)
				{
					subFormulas[subct].operators[k] = varval1->operators[k];
				}
				for(k = 0; k < varval2->opct ; k++)
				{
					subFormulas[subct].operators[varval1->opct + k] = varval2->operators[k];
				}
				subFormulas[subct].operators[varval1->opct + varval2->opct] = operators[i];
				subFormulas[subct].formula = varval1->formula + operators[i].op + varval2->formula;
			}
			else
			{
				subFormulas[subct].varct = varval1->varct;
				subFormulas[subct].opct = varval1->opct + 1;
				subFormulas[subct].variables = new LTLExpression[subFormulas[subct].varct];
				subFormulas[subct].operators = new LTLOperator[subFormulas[subct].opct];
				int k;
				for(k = 0; k < varval1->varct ; k++)
				{
					subFormulas[subct].variables[k] = varval1->variables[k];
				}
				for(k = 0; k < varval1->opct ; k++)
				{
					subFormulas[subct].operators[k] = varval1->operators[k];
				}
				subFormulas[subct].operators[varval1->opct] = operators[i];
				subFormulas[subct].formula = operators[i].op + varval1->formula;
			}
			subct++; 
		}
		if(opct != 0)					// Latest Subformula = formula
			formula = subFormulas[subct-1].formula;	
		else if(varct != 0)
			formula = variables[0].formula;
		else
			formula = "\0";
	}
};
