//----------------------------------------------------------------------------
// File:		driver.cpp
// 
// Description: Test Driver for the slist class
// 
// Programmer:  Anthony Waddell
//
// Functions:   main()
// 
// Environment: Hardware: PC, i7
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Comments:	Only provided full function definitions for functions that 
//				had to be implemented by myself (insert and remove)
//-------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "slist.h"

using namespace std;

//---------------------------------------------------------------------------
// Function:	main()
// Title:		Using Step List
// Description: This file contains function main() which tests slist class
//
// Programmer:	Anthony Waddell
// Date:		11-17-17
// Version:		1.0
// Environment: Hardware: PC, i7        
//				Software: OS: Windows 10 
//				Compiles under Microsoft Visual C++ 2015
// 
// Input:		N/A
// Output:		Various operations performed on a step list output to console
// Calls:		SList<Object>; constructor
//				insert()
//				show()
//				size()
//				remove()
//				find()
//				getCost()
// Called By:	n/a
// Parameters:  None
// Returns:		EXIT_SUCCESS  upon successful execution
//
// History Log:	11-13-17 AW Began Project
//				11-17-17 AW Completed project
// Known Bugs:  random not seeded
//----------------------------------------------------------------------------
int main() 
{
	// Test of constructor
	SList<int> *intList = new SList<int>;
	delete intList;

	SList<string> facultyList;

	// Test of insert
	facultyList.insert("unknown");
	facultyList.insert("erdly");
	facultyList.insert("sung");
	facultyList.insert("olson");
	facultyList.insert("zander");
	facultyList.insert("berger");
	facultyList.insert("cioch");
	facultyList.insert("fukuda");
	facultyList.insert("stiber");
	facultyList.insert("jackels");

	// Test of size and show
	cout << "#faculty members: " << facultyList.size() << endl;
	facultyList.show();
	cout << endl;

	// Test of remove
	cout << "deleting unknown" << endl;
	facultyList.remove("unknown");
	cout << "#faculty members: " << facultyList.size() << endl;
	facultyList.show();
	cout << endl;

	// Test of find
	cout << "finding stiber = " << facultyList.find("stiber") << endl;
	cout << endl;

	// Test of copy constructor
	cout << "create another list" << endl;
	SList<string> studentList = facultyList;
	cout << "finding stiber = " << facultyList.find("stiber") << endl;
	cout << "#faculty members: " << facultyList.size() << endl;
	cout << endl;

	cout << "cost of find = " << facultyList.getCost() << endl;

	// Hold screen
	system("pause");
	return EXIT_SUCCESS;
}
