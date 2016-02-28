				*******************************************************************************************
					AUTOMATA THEORETIC MODEL CHECKING OF WEB SERVICE SYSTEMS - PROJECT THESIS
				*******************************************************************************************
				-------------------------------------------------------------------------------------------
					Project by Abishek Sudarshan K, Bharath Raj T and Karthik Perumal P
				-------------------------------------------------------------------------------------------


*****************************
*** --- DISK CONTENTS --- ***
*****************************

The contents of the disk are:

Project Code - 	Folder consisting of the project files:
		1) Data.cpp
		2) Services.cpp
		3) ServiceToAutomata.cpp
		4) LTLFormula.cpp
		5) LTLAutomata.cpp
		6) ModelChecking.cpp
		7) Example.txt
		8) makefile

Project Report

Project References

Documentation


***********************************
*** --- RUNNING ENVIRONMENT --- ***
***********************************

The preferable operating system to run the project code will be any LINUX operating system.

The code is entirely C++ and the version of C++ should be atleast C++11.

In other operating systems, the latest version of C++ is needed and also any IDE.



****************************
*** --- INSTRUCTIONS --- ***
****************************

Running on LINUX systems

1. Download and Extract Web Service Model Checking Folder.

2. In the Terminal, change directory to the Web Service Model Checking Folder.

3. Type the command 'make'.

4. Type the command './ModelChecking'.



*************************************
*** --- USING THE APPLICATION --- ***
*************************************

1. A text file, Example.txt has been provided along with the code. This file contains the TrainReservation service automaton example.

2. Choose the Example.txt file for the Service file. Any other web service automaton can also be used.

3. Next, the formula has to be specified. Choose number of variables and operators and build the formula.

4. Generate the product automaton and perform model checking.

5. The same service can be checked for multiple formulae.