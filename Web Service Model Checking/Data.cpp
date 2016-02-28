#include <iostream>
#include <string>

using namespace std;

class Date
{
	public:
	int date, month, year;			//dd-mm-yyyy

	
		Date()
		{
						// Date Default Constructor
		}

		Date(int d,int m,int y)
		{
			date = d;
			month = m;		// To assign dd mm yyyy
			year = y;
		}

		Date(const Date &d)
		{
			date = d.date;
			month = d.month;	// Copy Constructor
			year = d.year;
		}

		bool operator<(Date d2)
		{
			if(year < d2.year)
				return true;
			else if(year > d2.year)
				return false;
			
			if(month < d2.month)
				return true;
			else if(month > d2.month)		// Operator < overloaded
				return false;

			if(date < d2.date)
				return true;
			else if(date > d2.date)
				return false;

			return false;
		}

		bool operator>(Date d2)
		{
			if(year > d2.year)
				return true;
			else if(year < d2.year)
				return false;
			
			if(month > d2.month)			// Operator > overloaded
				return true;
			else if(month < d2.month)
				return false;

			if(date > d2.date)
				return true;
			else if(date < d2.date)
				return false;

			return false;
		}

		bool operator==(Date d2)
		{
			if(year == d2.year && month == d2.month && date == d2.date)
				return true;						// Operator == overloaded
			return false;
		}

		void displayDate()
		{
			cout<<date<<" / "<<month<<" / "<<year;		// Display Date in dd/mm/yyyy
		}

};



class ServiceInput
{
	public:
	int type;					// int dec text date
	string name;					// property name
	
	long i1;
	double i2;					// depending on type values of i1 i2 i3 i4 will be assigned
	string i3;
	Date i4;

	
		 ServiceInput()
		 {
			i1 = 0;
			i2 = 0;
			i3 = "\0";
			Date *ix = new Date(0,0,0);		// default property constructor
			i4 = *ix;
		 }

		ServiceInput(const ServiceInput& SI)
		{
			type = SI.type;
			name = SI.name;
			i1 = SI.i1;				// copy constructor
			i2 = SI.i2;
			i3 = SI.i3;
			i4 = SI.i4;
		}

		 ServiceInput(int t, string n, long val)
		 {	
			type = t;
			name = n;				// integer assign
			i1 = val;
		 }

		ServiceInput(int t, string n, double val)
		 {	
			type = t;
			name = n;				// decimal assign
			i2 = val;
		 }

		ServiceInput(int t, string n, string val)
		 {	
			type = t;
			name = n;				// text assign
			i3 = val;
		 }

		ServiceInput(int t, string n, Date val)
		 {	
			type = t;
			name = n;				// date assign
			i4 = val;
		 }


	/*
		long - 1
		double - 2		
		string - 3
		Date - 4
	*/	

		int typeReturn()
		{
			return type;
		} 

		string nameReturn()
		{
			return name;
		}

		void displayName()
		{
			cout<<name;
		}
};

ServiceInput assignProperty(int t, string n)
{
	ServiceInput S(t,n,"\0");
	return S;
}




		


