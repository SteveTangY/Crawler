/*
 * the web crawler v3 edit in 2019.3.7  
 * 
 */ 
#ifndef GETURLRESOURCE_H
#define GETURLRESOURCE_H

//myfuncs
#include "mythread/mythread.h"
#include "mysocket/mysocket.h"
#include "html_analysis/html_analysis.h"

//usual
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>
//#include <thread>//enable C++11 thread
#include <pthread.h>//enable posix thread
#include <list>//double forward list
#include <vector>//
#include <regex>//

//net tools
#include <netdb.h>//enable DNS
#include <sys/socket.h>//socket
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//inet_addr
#include <openssl/ssl.h>//basic ssl functions
#include <openssl/err.h>//

#define WAIT_FILE_US 10
#define RECV_BUFF_SIZE 20

namespace Reptile
{
	//forward reference
	class REGEX;
	class HTTPS;
	
	class URL
	{
	public:
		URL()
		{
			thread.MutexCondInit(thread.init_both);//init mutex and cond value
		}
		~URL()
		{}
		void StartWithSeedUrl(const std::string& url);
		void SetPageSaveDir(const std::string& save_dir);
		void SetCounts(const int& mycount);
		void SetConnectPort(const int& port);
		bool SetThreadNum(const int& num);
	private:
		static MyFuncs::MyThread thread;
		//static std::queue<std::string> urls;
		static std::list<std::string> urls;
		static std::fstream pages;
		static std::string page_file_name;
		static int ready_quit_count;
		static int count;
		static int target_port;//set host's port

		static void SavePages(const std::string& buffer);
		static void* Crawling(void* arg);
	};


	
}
#endif

