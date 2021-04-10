// Kenneth Chen
// Home Project
// Computer class

#ifndef COMPUTER_H
#define COMPUTER_H

#include <iostream>
#include "Process.h"
#include "Hole.h"
#include <vector>

using namespace std;

class Computer
{
	public:
		Computer()
		{
			memory = 0;
			disks = 0;
			PID_counter = 1;
			mem_count = 0;
		}
		
		/*
			Creates the computer with the specified RAM and amount of disks
		*/
		
		void create(long long RAM, int disk_number)
		{
			memory = RAM;
			disks = disk_number;
			vector<vector<Process>> io_size(disk_number);
			io = io_size;
		}
		
		/*
			Creates a common process 		
		*/
		
		void create_common_process(string size)
		{
			bool deleted = false;	// Checks if the created process was created in a memory hole
			bool create = true;		// Checks if the process was successfully created
			bool found = false;		// Checks for the first memory hole that fits the allocated memory size
			Process c_p;
			long long p_size = stoll(size, nullptr, 10);
			// If there are any memory holes
			for(size_t b = 0; b < memory_holes.size(); b++)
			{
				// Checks if the process size is smaller than the memory hole and acts accordingly
				// by adjusting the memory hole's start and size values
				if(p_size < memory_holes[b].obtain_size() && !found)
				{
					c_p.create_hole_common(p_size, PID_counter, "Idle", memory_holes[b].obtain_start());
					memory_holes[b].alter(p_size);
					deleted = true;
					found = true;
				}
				// Checks if the process size is the same size as the hole, then puts the process
				// in the hole and deletes the hole
				else if(p_size == memory_holes[b].obtain_size())
				{
					c_p.create_hole_common(p_size, PID_counter, "Idle", memory_holes[b].obtain_start());
					memory_holes.erase(memory_holes.begin() + b);
					deleted = true;
				}
			}
			if(mem_count + p_size > memory && !deleted)
			{
				cout << "There is not enough memory." << endl;
				create = false;
			}
			// If there are no processes and no holes
			else if(common.size() == 0 && RT.size() == 0 && memory_holes.size() == 0)
			{
				c_p.create_common(p_size, PID_counter, "Running", mem_count);
			}
			// If the process cannot fit in any holes
			else if(!deleted)
			{
				c_p.create_common(p_size, PID_counter, "Idle", mem_count);
			}
			// If the process was created, increment the PID counter and put it in the common queue
			if(create)
			{
				PID_counter++;
				common.push_back(c_p);
			}
			// If the process was not created in a memory hole, increase the memory counter
			if(!deleted && create)
			{
				mem_count += p_size;
			}
		}
		
		/*
			Creates a Real time process
		*/
		
		void create_rt_process(string size)
		{
			bool deleted = false;		// Checks if the process was created in a memory hole
			bool create = true;			// Checks if the process was successfully created
			bool found = false;			// Checks for the first memory hole that fits the allocated memory size
			Process rt_p;
			long long p_size = stoll(size, nullptr, 10);
			// Checks through all the memory holes
			for(size_t b = 0; b < memory_holes.size(); b++)
			{
				// If the size is smaller than the memory hole
				if(p_size < memory_holes[b].obtain_size() && !found)
				{
					// If there are no RT processes, it will be set as running and edits the memory hole size
					if(RT.size() == 0)
					{
						rt_p.create_hole_rt(p_size, PID_counter, "Running", memory_holes[b].obtain_start());
						memory_holes[b].alter(p_size);
						deleted = true;
						found = true;
						// Resets all the common processes to idle
						if(common.size() > 0)
						{
							reset_all_common();
						}
					}
					// If there is any RT processes, it will be set as idle and edits the memory hole size
					else
					{
						rt_p.create_hole_rt(p_size, PID_counter, "Idle", memory_holes[b].obtain_start());
						memory_holes[b].alter(p_size);
						found = true;
						deleted = true;	
					}
				}
				// If the size is the same size as the memory hole
				else if(p_size == memory_holes[b].obtain_size())
				{
					// If there are no RT processes, it will be set as running and deletes the memory hole
					if(RT.size() == 0)
					{
						rt_p.create_hole_rt(p_size, PID_counter, "Running", memory_holes[b].obtain_start());
						memory_holes.erase(memory_holes.begin() + b);
						deleted = true;	
						if(common.size() > 0)
						{
							reset_all_common();
						}
					}
					// If there is any RT processes, it will be set as idle and deletes the memory hole
					else
					{
					 	rt_p.create_hole_rt(p_size, PID_counter, "Idle", memory_holes[b].obtain_start());
						memory_holes.erase(memory_holes.begin() + b);
						deleted = true;	
					}
				}
			}
			if(mem_count + p_size > memory && !deleted)
			{
				cout << "There is not enough memory." << endl;
				create = false;
			}
			// If there are no processes and no memory holes
			else if(common.size() == 0 && RT.size() == 0 && memory_holes.size() == 0)
			{
				rt_p.create_rt(p_size, PID_counter, "Running", mem_count);
			}
			// If there are no RT processes and no memory holes
			else if(!deleted && RT.size() == 0)
			{
				rt_p.create_rt(p_size, PID_counter, "Running", mem_count);
				if(common.size() > 0)
				{
					reset_all_common();
				}
			}
			// If there is at least 1 RT process
			else if(!deleted && RT.size() > 0)
			{
				rt_p.create_rt(p_size, PID_counter, "Idle", mem_count);
			}
			// If the RT process was created, increment the PID counter and put it in the RT queue
			if(create)
			{
				PID_counter++;
				RT.push_back(rt_p);
			}
			// If the RT process was not created in a memory hole, increase the memory counter
			if(!deleted && create)
			{
				mem_count += p_size;
			}
		}
		
		/*
			Sets the status of common proccesses back to idle if there are any RT processes
		*/
		
		void reset_all_common()
		{
			for(size_t i = 0; i < common.size(); i++)
			{
				if(common[i].check_status())
				{
					common[i].set_idle();
				}
			}
		}
		
		/*
			Resets all except the currently running RT processes to idle
		*/
		
		void reset_RT()
		{
			for(size_t j = 1; j < RT.size(); j++)
			{
				if(RT[j].check_status())
				{
					RT[j].set_idle();
				}
			}
		}
		
		/*
			Resets all except the currently running common processes to idle
		*/
		
		void reset_common()
		{
			for(size_t i = 1; i < common.size(); i++)
			{
				if(common[i].check_status())
				{
					common[i].set_idle();
				}
			}
		}
		
		/*
			Sorts the memory holes and ranges in ascending order
		*/
		
		void insert_sort(Hole h)
		{
			bool big = true;
			int counter = 0;
			// If there is nothing in the vector, there is nothing to sort
			if(memory_holes.size() < 1)
			{
				memory_holes.push_back(h);
			}
			// If there is at least memory hole to sort
			else
			{
				for(size_t b = 0; b < memory_holes.size(); b++)
				{
					// Checks if the new hole is less than any of the holes in the vector
					if(h.obtain_start() < memory_holes[b].obtain_start())
					{
						big = false;
						counter = b;
					}
				}
				// If the new hole is bigger than all the holes, push it to the back
				if(big)
				{
					memory_holes.push_back(h);
				}
				// If the new hole is smaller than any others, place it in the correct spot
				else
				{
					memory_holes.insert(memory_holes.begin() + counter, h);
				}
			}
		}
		
		/*
			Function to implement the end of a process's time slice
		*/
		
		void time_end()
		{
			// Counter used to keep track of which process is next in queue to be run
			int counter = 0;
			// Checks if there are more than 1 RT processes and adjusts accordingly
			if(RT.size() > 1 && common.size() == 0)
			{
				for(size_t i = 0; i < RT.size(); i++)
				{
					if(RT[i].check_status())
					{
						RT[i].set_idle();
						counter = (1 + i) % RT.size();
						i++;
					}
				}
				RT[counter].set_run();
			}
			// Checks if there is only 1 RT process and has common processes
			else if(RT.size() == 1 && common.size() > 0)
			{
				// Doesn't do anything if there is only 1 RT process and there are common processes
			}
			// Checks if there are 0 RT processes and there are more than 1 common processes
			else if(RT.size() == 0 && common.size() > 1)
			{
				for(size_t j = 0; j < common.size(); j++)
				{
					if(common[j].check_status())
					{
						common[j].set_idle();
						counter = (1 + j) % common.size();
						j++;
					}
				}
				common[counter].set_run();
			}
			// If there are at least 1 RT process and at least 1 common process
			else if(RT.size() > 1 && common.size() > 0)
			{
				for(size_t i = 0; i < RT.size(); i++)
				{
					if(RT[i].check_status())
					{
						RT[i].set_idle();
						counter = (1 + i) % RT.size();
						i++;
					}
				}
				RT[counter].set_run();
			}
		}
		
		/*
			The currently running process has terminated
		*/
		
		void terminate()
		{
			int counter = 0;
			Hole deleted;
			// If there is more than 1 RT process
			if(RT.size() > 1)
			{
				for(size_t i = 0; i < RT.size(); i++)
				{
					if(RT[i].check_status())
					{
						deleted.set_size(RT[i].obtain_size());
						deleted.set_start(RT[i].obtain_mem_start());
						deleted.set_end(RT[i].obtain_mem_end());
						insert_sort(deleted);
						RT.erase(RT.begin() + i);
						counter = i % RT.size();
						i++;
					}
				}
				RT[counter].set_run();
			}
			// If there is only 1 RT process and at least 1 common process
			else if(RT.size() == 1 && common.size() > 0)
			{
				// Checks if there RT process is running or not
				if(RT[0].check_status())
				{
					deleted.set_size(RT[0].obtain_size());
					deleted.set_start(RT[0].obtain_mem_start());
					deleted.set_end(RT[0].obtain_mem_end());
					insert_sort(deleted);
					RT.erase(RT.begin());
					common[0].set_run();
				}
				else
				{
					for(size_t j = 0; j < common.size(); j++)
					{
						if(common[j].check_status())
						{
							deleted.set_size(common[j].obtain_size());
							deleted.set_start(common[j].obtain_mem_start());
							deleted.set_end(common[j].obtain_mem_end());
							insert_sort(deleted);
							common.erase(common.begin() + j);
						}
					}
					RT[0].set_run();
				}
			}
			// If there is only 1 RT process and 0 common processes
			else if(RT.size() == 1 && common.size() == 0)
			{
				RT.erase(RT.begin());
				mem_count = 0;
				// Clears all the memory holes
				memory_holes.clear();
			}
			// If there is 0 RT processes and at least 1 common process
			else if(RT.size() == 0 && common.size() > 1)
			{
				for(size_t j = 0; j < common.size(); j++)
				{
					if(common[j].check_status())
					{
						deleted.set_size(common[j].obtain_size());
						deleted.set_start(common[j].obtain_mem_start());
						deleted.set_end(common[j].obtain_mem_end());
						insert_sort(deleted);
						common.erase(common.begin() + j);
						counter = j % common.size();
						j++;
					}
				}
				common[counter].set_run();
			}
			// If there is 0 RT processes and only 1 common process
			else if(RT.size() == 0 && common.size() == 1)
			{
				common.erase(common.begin());
				mem_count = 0;
				// Clears all the memory holes
				memory_holes.clear();
			}	
			// If there is at least 2 memory holes to compare in case of merging
			if(memory_holes.size() > 1)
			{
				for(size_t b = 0; b < memory_holes.size(); b++)
				{
					// If the memory holes can be merged, create a new memory hole
					Hole merge;
					if(memory_holes[b].obtain_end() + 1 == memory_holes[b+1].obtain_start())
					{
						merge.set_size(memory_holes[b].obtain_size() + memory_holes[b+1].obtain_size());
						merge.set_start(memory_holes[b].obtain_start());
						merge.set_end(memory_holes[b+1].obtain_end());
						// Delete the 2 merged holes, insert the new hole into the vector and recheck it
						memory_holes.erase(memory_holes.begin() + b + 1);
						memory_holes.erase(memory_holes.begin() + b);
						insert_sort(merge);
						b--;
					}
				}
			}
		}
		
		/*
			The specified running disk has finished running
		*/
		
		void disk_finish(string disk_number)
		{
			int number = stoi(disk_number);
			// If there is only 1 process in the i/o queue
			if(io[number].size() == 1)
			{
				// If it is a real time process
				if(io[number][0].check_rt())
				{
					RT.insert(RT.end(), io[number][0]);
					io[number].erase(io[number].begin());
					reset_RT();
					reset_all_common();
				}
				// If it is a common process
				else
				{
					common.insert(common.end(), io[number][0]);
					io[number].erase(io[number].begin());
					if(RT.size() > 0)
					{
						reset_all_common();
					}
					else
					{
						reset_common();
					}
				}
			}
			else if(io[number].size() == 0)
			{
				cout << "There were no processes in disk " << number << "." << endl;
			}
			// if there is more than 1 process in the i/o queue
			else
			{
				// If the process is a RT process
				if(io[number][0].check_rt())
				{
					RT.insert(RT.end(), io[number][0]);
					io[number].erase(io[number].begin());
					io[number][0].set_run();
					reset_RT();
					reset_all_common();
				}
				// If the process is a common process
				else
				{
					common.insert(common.end(), io[number][0]);
					io[number].erase(io[number].begin());
					io[number][0].set_run();
					if(RT.size() > 0)
					{
						reset_all_common();
					}
					else
					{
						reset_common();
					}
				}
			}				
		}
		
		/*
			Function implementing disk usage
		*/
		
		void disk_request(string disk_number)
		{
			int r_counter = 0;
			int number = stoi(disk_number);
			// If there are no processes
			if(RT.size() == 0 && common.size() == 0)
			{
				cout << "There are no processes currently running to request disk usage." << endl;
			}
			// If there is more than 1 RT process
			else if(RT.size() > 1)
			{
				for(size_t i = 0; i < RT.size(); i++)
				{
					if(RT[i].check_status())
					{
						io[number].push_back(RT[i]);
						r_counter = i % RT.size();
						RT.erase(RT.begin() + i);
					}
				}
				RT[r_counter].set_run();
			}
			// If there is only 1 RT process and at least 1 common process
			else if(RT.size() == 1 && common.size() > 0)
			{
				io[number].push_back(RT[0]);
				RT.erase(RT.begin());
				common[0].set_run();
			}
			// If there are no RT processes and more than 1 common process
			else if(RT.size() == 0 && common.size() > 1)
			{
				for(size_t j = 0; j < common.size(); j++)
				{
					if(common[j].check_status())
					{
						io[number].push_back(common[j]);
						r_counter = j % common.size();
						common.erase(common.begin() + j);
					}
				}
				common[r_counter].set_run();
			}
			// If there is only 1 RT process and no common processes
			else if(RT.size() == 1 && common.size() == 0)
			{
				io[number].push_back(RT[0]);
				RT.erase(RT.begin());
			}
			// If there is no RT processes and 1 common process
			else if(RT.size() == 0 && common.size() == 1)
			{
				io[number].push_back(common[0]);
				common.erase(common.begin());
			}
			// Resets all the process's status to idle if they are not the first one in queue
			for(size_t x = 0; x < io.size(); x++)
			{
				for(size_t y = 1; y < io[x].size(); y++)
				{
					if(io[x][y].check_status())
					{
						io[x][y].set_idle();
					}
				}
			}
		}
		
		/*
			Prints the current CPU queue
		*/
		
		void show_cpu() 
		{
			string cpu_char = " ";
			// If there are no processes running
			if(RT.size() == 0 && common.size() == 0)
			{
				cout << "CPU: Empty" << endl;
				cout << "RT-queue: Empty " << endl;
				cout << "Common-queue: Empty " << endl;
			}
			else
			{
				cout << "CPU: ";
				// If there is at least 1 RT process
				if(RT.size() > 1)
				{
					int counter = 0;	// Used to loop back around to output the remaining processes in the queue
					bool found = false;
					for(size_t i = 0; i < RT.size(); i++)
					{
						// Prints out the currently running RT process
						if(RT[i].check_status())
						{
							cout << "P" << RT[i].print_id() << endl;
							counter = i;
							found = true;
							cout << "RT-queue: ";
						}
						// Prints out the rest of the RT processes in queue
						if(found && !RT[i].check_status())
						{
							if(i != RT.size()-1 || counter > 0)
							{
								cout << "P" << RT[i].print_id() << " <- ";		
							}
							else
							{
								cout << "P" << RT[i].print_id();
							}
						}
					}
					// Loops back around the queue to print out the remaining processes in the queue (when Q is used)
					for(size_t v = 0; v < counter; v++)
					{
						if(!RT[v].check_status() && v!= counter-1)
						{
							cout << "P" << RT[v].print_id() << " <- ";
						}
						else
						{
							cout << "P" << RT[v].print_id();
						}
					}
					// Prints out the common processes
					cout << endl << "Common-queue: ";
					// If there are no common processes
					if(common.size() == 0)
					{
						cout << "Empty" << endl;
					}
					// There is at least 1 common process
					else
					{
						for(size_t j = 0; j < common.size(); j++)
						{
							if(j != common.size()-1)
							{
								cout << "P" << common[j].print_id() << " <- ";	
							}
							else
							{
								cout << "P" << common[j].print_id();
							}
						}
						cout << endl;				
					}
				}
				// If there is only 1 RT process
				else if(RT.size() == 1)
				{
					cout << "P" << RT[0].print_id() << endl;
					cout << "RT-queue: Empty" << endl;
					cout << "Common-queue: ";
					// If there is no common processes
					if(common.size() == 0)
					{
						cout << "Empty" << endl;
					}
					// If there is atleast 1 common process
					else
					{
						for(size_t j = 0; j < common.size(); j++)
						{
							if(j != common.size()-1)
							{
								cout << "P" << common[j].print_id() << " <- ";	
							}
							else
							{
								cout << "P" << common[j].print_id();
							}
						}
						cout << endl;				
					}
				}
				// If there is 0 RT processes and only 1 common process
				else if(RT.size() == 0 && common.size() == 1)
				{
					cout << "P" << common[0].print_id() << endl;
					cout << "RT-queue: Empty" << endl;
					cout << "Common-queue: Empty" << endl;
				}
				// If there are no RT processes and at least 2 common processes
				else
				{	
					int counter = 0;	// Used to loop back around to print out the remaining processes 
					int found = false;
					for(size_t j = 0; j < common.size(); j++)
					{
						if(common[j].check_status())
						{
							cout << "P" << common[j].print_id() << endl;
							cout << "RT-queue: Empty" << endl;
							cout << "Common-queue: ";
							counter = j;
							found = true;
						}
						if(found && !common[j].check_status())
						{
							if(j != common.size()-1 || counter > 0)
							{
								cout << "P" << common[j].print_id() << " <- ";	
							}
							else
							{
								cout << "P" << common[j].print_id();
							}
						}
					}
					// Prints out the remaining processes in queue (when Q is used)
					for(size_t v = 0; v < counter; v++)
					{
						if(!common[v].check_status() && v != counter - 1)
						{
							cout << "P" << common[v].print_id() << " <- ";
						}
						else
						{
							cout << "P" << common[v].print_id();
						}
					}
					cout << endl;
				}
			}
		}
		
		/*
			Prints the current disk I/O queue
		*/
		
		void show_disk()
		{
			bool empty = true;
			for(size_t x = 0; x < io.size(); x++)
			{
				// If there is only 1 process in the specified HDD
				if(io[x].size() == 1)
				{
					empty = false;
					cout << "HDD " << x << ": P" << io[x][0].print_id() << endl;
					cout << "I/O " << x << ": Empty " << endl;
				}
				// If there is more than 1 process in the specified HDD
				else
				{
					for(size_t y = 0; y < io[x].size(); y++)
					{
						empty = false;
						if(io[x][y].check_status())
						{
							cout << "HDD " << x << ": P" << io[x][y].print_id() << endl;
							cout << "I/O " << x << ": ";
						}
						else
						{
							if(y != io[x].size() - 1)
							{
								cout << "P" << io[x][y].print_id() << " <- ";	
							}
							else
							{
								cout << "P" << io[x][y].print_id() << endl;
							}
						}
					}	
				}
			}
			if(empty)
			{
				cout << "All HDD and I/O queues are empty." << endl;		
			}
		}
		
		/*
			Prints the current memory with the running processes
		*/
		
		void show_memory()
		{
			print_memory.clear();
			bool empty = true;
			cout << "PID       M_Start     M_End" << endl;
			// Sorts the RT processes first in ascending memory ranges
			for(size_t i = 0; i < RT.size(); i++)
			{
				empty = false;
				memory_sort(RT[i]);
			}
			// Then sorts the common processes in ascending memory ranges
			for(size_t j = 0; j < common.size(); j++)
			{
				empty = false;
				memory_sort(common[j]);
			}
			// Lastly sorts the processes in IO queue in ascending memory ranges
			for(size_t x = 0; x < io.size(); x++)
			{
				for(size_t y = 0; y < io[x].size(); y++)
				{
					empty = false;
					memory_sort(io[x][y]);
				}
			}
			
			if(empty)
			{
				cout << "Empty     Empty       Empty" << endl;
			}
			// Prints out the processes and their memory ranges
			else
			{
				for(size_t z = 0; z < print_memory.size(); z++)
				{
					cout << print_memory[z].print_id() << "         ";
					cout << print_memory[z].print_mem_start() << "         ";
					cout << print_memory[z].print_mem_end() << endl;
				}
			}
		}
		
		/*
			Function to sort the processes in ascending order based on their memory ranges
		*/
		void memory_sort(Process p)
		{
			bool big = true;
			int counter = 0;
			// If there is no processes, no sorting is needed
			if(print_memory.size() < 1)
			{
				print_memory.push_back(p);
			}
			else
			{
				for(size_t q = 0; q < print_memory.size(); q++)
				{
					if(!big)
					{
						// Prevents the loop from overwriting itself
					}
					// If the process's starting memory is less than the process in the vector
					else if(p.obtain_mem_start() < print_memory[q].obtain_mem_start())
					{
						big = false;
						counter = q;
					}
				}
				// If the process's memory is bigger than all of the processes, push it to the back
				if(big)
				{
					print_memory.push_back(p);
				}
				// If it is not the biggest process, place it in the correct order
				else
				{
					print_memory.insert(print_memory.begin() + counter, p);
				}
			}
		}
		
	private:
		long long memory;
		int disks;
		vector<Process> common;		// Common processes
		vector<Process> RT;			// RT processes
		vector<vector<Process>> io;		// HDD and I/O queue
		vector<Hole> memory_holes;		// Vector to store all the memory holes
		vector<Process> print_memory;	// Vector to print out the processors and memory ranges in ascending order
		int PID_counter;				// keeps track of the PID
		long long mem_count;			// keeps track of how much memory is used
};

#endif
