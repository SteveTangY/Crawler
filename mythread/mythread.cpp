/*
 * 
 */
#include "mythread.h"
//
namespace MyFuncs
{
	//v3's new fun. let user can set thread nums
	bool MyThread::SetThreadNum(const int& num)
	{
		if ( (num > THREAD_MAX) || (num <= 0) )
			return false;
		else
		{
			thread_num = num;
			return true;
		}	
	}
	//
	void MyThread::MutexCondInit(const std::string& init_set)
	{
		destroy_flag = init_set;
		
		if ((init_set == init_mutex)|(init_set == init_both))
		{
			if (pthread_mutex_init(&mymutex, NULL) != 0)
				throw;
		}
		
		if ((init_set == init_cond)|(init_set == init_both))
		{
			for (int i=0; i < thread_num; i++)
			{
				if (pthread_cond_init(&mycond[i], NULL) != 0)
					throw;
			}
		}
	}
	//in v2.3 new func
	int MyThread::FindAvoilThread(void)
	{
		int i = 0;
		for (; i < thread_num; i++)
		{
			if (threads[i] == avoil_thread_id.front())
			{
				avoil_thread_id.pop();
				//break;
				return i;
			}
		}
		return -1;
	}
	//in v2, add funs which can create more than thread_max threads and add mod to set when create thread.
	//in v2.3 can create new thread more smart
	bool MyThread::ThreadCreate(void* (*thread_func)(void*) , void* arg, char mod)
	{
		if (thread_count == thread_num)
		{
			thread_count = 0;
			if (wait_flag == false)
				wait_flag = true;
		}
		
		if (wait_flag == true)
		{
			//wait for the thread exit
			//std::cout << "begin wait thread" << std::endl;//log
			if (!avoil_thread_id.empty())
			{
				thread_count = FindAvoilThread();
				if (thread_count < thread_num && thread_count >= 0)
				{
					pthread_join(threads[thread_count], NULL);//make sure thread exited
				}
				else
				{
					thread_count = 0;//clear wrong data
					std::cout << "create thread fault because more than thread num..." << std::endl;//log
					return false;
				}
			}
			else
			{
				//std::cout << "create thread fault because no avoid thread..." << std::endl;//log
				return false;
			}
		}
		
		if (pthread_create(&threads[thread_count], NULL, thread_func, arg) != 0)
		{
			std::cout << "create thread fault..." << std::endl;//log
			return false;
		} 
		else
		{
			if (mod == mod_detach)
				pthread_detach(threads[thread_count]);
			thread_count++;
			//usleep(5);//avoid some bugs when built up serveral threads
			//std::cout << "create a thread successfully..." << std::endl;//log
			return true;
		}
	}
	//
	void MyThread::Lock(void)
	{
		if (destroy_flag == "")
		{
			std::cout << "must init mutex first..." << std::endl;//log
			exit(1);
		}
		pthread_mutex_lock(&mymutex);
		return;
	}
	//
	void MyThread::Unlock(void)
	{
		if (destroy_flag == "")
		{
			std::cout << "must init mutex first..." << std::endl;//log
			exit(1);
		}
		pthread_mutex_unlock(&mymutex);
		return;
	}
	//in v2.2 can choose multiple cond to use
	void MyThread::Wait(const int& cond_num)
	{
		if ((cond_num-1) > thread_num)
		{
			std::cout << "error:cond num should not more than thread num" << std::endl;
			throw;
		}
		
		if (destroy_flag == "")
		{
			std::cout << "must init cond first..." << std::endl;//log
			exit(1);
		}
		pthread_cond_wait(&mycond[cond_num-1], &mymutex);
		return;
	}
	//in v2.2 can choose multiple cond to use
	void MyThread::Signal(const int& cond_num)
	{
		if ((cond_num-1) > thread_num)
		{
			std::cout << "error:cond num should not more than thread num" << std::endl;
			throw;
		}
		
		if (destroy_flag == "")
		{
			std::cout << "must init cond first..." << std::endl;//log
			exit(1);
		}
		pthread_cond_signal(&mycond[cond_num-1]);
		return;
	}
	//in v2.3 new func
	void MyThread::ThreadExit(void* arg)
	{
		pthread_mutex_lock(&selfMutex);// protect itself to avoid some bug
		pthread_t thread_id = pthread_self();// get this thread's id
		//std::cout << "finish get pthread_self() in " << __FILE__ << __LINE__ << std::endl;//log
		avoil_thread_id.push(thread_id);
		//std::cout << "finish push thread id in " << __FILE__ << __LINE__ << std::endl;//log
		pthread_mutex_unlock(&selfMutex);
		//std::cout << " in " << __FILE__ << __LINE__ << std::endl;//log
		pthread_exit(arg);
	}
	//
	void MyThread::Exit(void)
	{
		for(int i=0; i < THREAD_MAX; i++)
		{
			pthread_detach(threads[i]);
		}
		return;
	}
	
}
