// Kenneth Chen
// Home Project
// Hole class

#ifndef HOLE_H
#define HOLE_H

#include <iostream>

class Hole
{
	public:
		Hole()
		{
			size_ = 0;
			mem_start_ = 0;
			mem_end_ = 0;
		}
		
		/*
			Changes the hole size if it is not completely filled
		*/
		
		void alter(long long alter_amount)
		{
			size_ -= alter_amount;
			mem_start_ += alter_amount;
		}
		
		/*
			Returns the size of the hole
		*/
		
		long long obtain_size()
		{
			return size_;
		}
		
		/*
			Returns the starting memory range for the hole
		*/
		
		long long obtain_start()
		{
			return mem_start_;
		}
		
		/*
			Returns the ending memory range for the hole
		*/
		
		long long obtain_end()
		{
			return mem_end_;
		}
		
		/*
			Sets the hole size
		*/
		
		void set_size(long long size)
		{
			size_ = size;
		}
		
		/*
			Sets the hole's starting memory range
		*/
		
		void set_start(long long start)
		{
			mem_start_ = start;
		}
		
		/*
			Sets the hole's ending memory range
		*/
		
		void set_end(long long end)
		{
			mem_end_ = end;
		}
		
	private:
		
		long long size_;
		long long mem_start_;
		long long mem_end_;
};

#endif
