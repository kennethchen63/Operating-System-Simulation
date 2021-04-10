// Kenneth Chen
// Home Project
// Process class

#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>

using namespace std;

class Process
{
	public:
		Process()
		{
			type_ = "Common";
			size_ = 0;
			PID = 1;
			status_ = "Idle";
			mem_start = 0;
			mem_end = 0;
		}
		
		/*
			Function to create a common process
		*/
		
		void create_common(long long size, int id, string status, long long used)
		{
			type_ = "Common";
			size_ = size;
			PID = id;
			status_ = status;
			mem_start += used;
			mem_end += used + size - 1;
		}
		
		/*
			Function to create a common process to fill in a memory hole
		*/
		
		void create_hole_common(long long size, int id, string status, long long start)
		{
			type_ = "Common";
			size_ = size;
			PID = id;
			status_ = status;
			mem_start = start;
			mem_end = start + size - 1;
		}
		
		/*
			Function to create a RT process
		*/
		
		void create_rt(long long size, int id, string status, long long used)
		{
			type_ = "RT";
			size_ = size;
			PID = id;
			status_ = status;
			mem_start += used;
			mem_end += used + size - 1;
		}
		
		/*
			Function to create a RT process to fill a memory hole
		*/
		
		void create_hole_rt(long long size, int id, string status, long long start)
		{
			type_ = "RT";
			size_ = size;
			PID = id;
			status_ = status;
			mem_start = start;
			mem_end = start + size - 1;
		}
		
		/*
			Function to check if the process is currently running
		*/
		
		bool check_status()
		{
			if(status_ == "Idle")
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		
		/*
			Sets the process to idle
		*/
		
		void set_idle()
		{
			status_ = "Idle";
		}
		
		/*
			Sets the process to running
		*/
		
		void set_run()
		{
			status_ = "Running";
		}
		
		/*
			Returns the memory size
		*/
		
		long long obtain_size() const
		{
			return size_;
		}
		
		/*
			Returns the memory start range
		*/
		
		long long obtain_mem_start() const
		{
			return mem_start;
		}
		
		/*
			Returns the memory end range
		*/
		
		long long obtain_mem_end() const
		{
			return mem_end;
		}
		/*
		
			Prints the PID 
		*/
		
		int print_id() const
		{
			return PID;
		}
		
		/*
			Prints the type of process
		*/
		
		string print_type() const
		{
			return type_;
		}
		
		/*
			Prints the status of the process (running or idle)
		*/
		
		string print_status() const
		{
			return status_;
		}
		
		/*
			Checks if the process is RT
		*/
		
		bool check_rt() const
		{
			if(type_ == "RT")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		/*
			Prints the starting memory byte
		*/
		
		long long print_mem_start() const
		{
			return mem_start;
		}
		
		/*
			Prints the ending memory byte
		*/
		
		long long print_mem_end() const
		{
			return mem_end;
		}
		
	private:
		string type_;
		long long size_;
		int PID;
		string status_;
		long long mem_start;
		long long mem_end;		
};

#endif
