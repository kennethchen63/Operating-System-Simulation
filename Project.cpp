// Kenneth Chen
// Home Project

#include <iostream>
#include <vector>
#include <sstream>
#include "Computer.h"

using namespace std;


int main()
{
	
	long long RAM;
	int HDD;
	bool input = true;
	string commands;
	string command1;
	string command2;
	Computer Running;
	
	cout << "How much RAM memory is there on the simulated computer?" << endl;
	cin >> RAM;
	if(RAM > 4000000000 || RAM < 0)
	{
		cout << "Invalid amount of memory." << endl;
		input = false;
	}
	else
	{
		cout << "How many hard disks does the simulated computer have?" << endl;
		cin >> HDD;
		if(HDD < 0)
		{
			cout << "Invalid amount of disks." << endl;
			input = false;
		}
	}
	
	
	if(input)
	{
		Running.create(RAM, HDD);
		while(getline(cin, commands))
		{
			stringstream command(commands);
			// Obtains the first command
			command >> command1;
			// Obtains the second command
			command >> command2;
			if(command1 == "A")
			{
				Running.create_common_process(command2);
			}
			else if(command1 == "AR")
			{
				Running.create_rt_process(command2);
			}
			else if(command1 == "Q")
			{
				Running.time_end();
			}
			else if(command1 == "t")
			{
			Running.terminate();
			}
			else if(command1 == "d")
			{
				int check = stoi(command2);
				if(check >= HDD)
				{
					cout << "There is no disk " << command2 << "." << endl;
				}
				else
				{
					Running.disk_request(command2);
				}
			}
			else if(command1 == "D")
			{
				int check = stoi(command2);
				if(check >= HDD)
				{	
					cout << "There is no disk " << command2 << "." << endl;
				}
				else
				{
					Running.disk_finish(command2);
				}
			}
			else if(command1 == "S" && command2 == "r")
			{
				Running.show_cpu();
			}
			else if(command1 == "S" && command2 == "i")
			{
				Running.show_disk();
			}
			else if(command1 == "S" && command2 == "m")
			{
				Running.show_memory();
			}
			else
			{
				if(command1 != "")
				{
					cout << "Invalid command. " << endl;	
				}
			}
		}
	}
	else
	{
		cout << "Please try again. " << endl;
	}
}
