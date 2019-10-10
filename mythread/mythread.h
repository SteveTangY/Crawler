/*
 * use posix thread to get thread funcs  v2.3
 * v2.3==fix some logic mistake, add thread lock to protect avoil thread queue
 * edit time 2019.5.1
 */
#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <unistd.h>
#include <pthread.h>//use posix thread
#include <queue>//use FIFO queue
#include <iostream>

#define THREAD_MAX 8

namespace MyFuncs
{
	class MyThread
	{
	public:	
		MyThread()
		{
			thread_num = THREAD_MAX / 2;
			thread_count = 0;
			destroy_flag = "";
			wait_flag = false;
			while (pthread_mutex_init(&selfMutex,NULL) != 0 )
			{ 
				std::cout << "can't init selfMutex, retry..." << std::endl;//log 
			}
		}
		~MyThread()
		{
			if ((destroy_flag == init_mutex)|(destroy_flag == init_both))
				pthread_mutex_destroy(&mymutex);
			if ((destroy_flag == init_cond)|(destroy_flag == init_both))
			{
				for (int i=0; i < thread_num; i++)
				{
					pthread_cond_destroy(&mycond[i]);
				}
			}
			pthread_mutex_destroy(&selfMutex);
			Exit();
		}
		const std::string init_mutex = "init_mutex";
		const std::string init_cond = "init_cond";
		const std::string init_both = "init_both";
		const char mod_join = '1';
		const char mod_detach = '2';
		const int thread_num_max = THREAD_MAX;
		bool wait_flag;
		bool SetThreadNum(const int& num);
		bool ThreadCreate(void* (*thread_func)(void*) , void* arg, char mod);
		void MutexCondInit(const std::string& init_set);
		void Lock(void);
		void Unlock(void);
		void Wait(const int& cond_num);
		void Signal(const int& cond_num);
		void Exit(void);
		void ThreadExit(void* arg);
		int FindAvoilThread(void);
	private:
		int thread_count;//must get optimization
		int thread_num;
		std::string destroy_flag;
		std::queue<pthread_t> avoil_thread_id;
		pthread_t threads[THREAD_MAX];
		pthread_mutex_t mymutex, selfMutex;
		pthread_cond_t mycond[THREAD_MAX];
		
	};

}
#endif
