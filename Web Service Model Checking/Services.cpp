#include "Data.cpp"
#include <fstream>

class Service
{
	public:
	ServiceInput* property;			// List of properties
	string serviceName;			// name of service
	int propertyCount;			// number of properties

	
		Service()
		{				// default
		}

		Service(const Service& S)
		{
			serviceName = S.serviceName;			// copy
			propertyCount = S.propertyCount;
			property = S.property;
		}

		string ServiceName()
		{
			return serviceName;
		}

		int returnCount()
		{
			return propertyCount;
		}

		ServiceInput* Property()
		{
			return property;
		}
};


