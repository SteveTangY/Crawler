/*
 *Here are the funtions defination 
 * page file--cond 1, url data--cond 2
 */
#include "GetUrlResource.h"

namespace Reptile
{
namespace
{
#define WAIT_DATE_TIME_MS 500
#define CONTAINER_WAIT_MAX 6
}
	//
	int URL::count = 80;//set how many page to get
	int URL::target_port = 80;
	int URL::ready_quit_count = 0;// when url container is empty, this will offer a quit chance
	std::string URL::page_file_name = "./htmls/pages.txt";
	//std::queue<std::string> URL::urls;
	std::list<std::string> URL::urls;
	std::fstream URL::pages;
	MyFuncs::MyThread URL::thread;
	
	void URL::SetCounts(const int& mycount)
	{
		count = mycount;
		return;
	}
	//
	bool URL::SetThreadNum(const int& num)
	{
		return thread.SetThreadNum(num);
	}
	//
	void URL::SetPageSaveDir(const std::string& save_dir)
	{
		//string buf = save_dir;
		page_file_name = save_dir + "pages.txt";
		return;
	}
	//
	void URL::SetConnectPort(const int& port)
	{
		target_port = port;
		return;
	}
	//  
	void URL::SavePages(const std::string& buffer)
	{
		if (pages.is_open())
		{   
			thread.Wait(1);
		}   
		thread.Lock();//it's important in cond used
		std::cout << "begin savepages" << std::endl;//log
		pages.open(page_file_name, std::ios::out|std::ios::app);
		if (!pages.is_open())
		{   
			std::cout << "open page file failt at savepages" << std::endl;//log
			return;
		}   
		try 
		{   
			pages << buffer;
		}   
		catch (int e)
		{   
		
		}   
		pages.close();
		thread.Unlock();//it's important in cond used
		thread.Signal(1);
		std::cout << "exit savepages" << std::endl;//log
		return;
	}

	//
	void* URL::Crawling(void* arg)
	{
		int sock = 0;
		//std::cout << "start static_cast" << std::endl;//log
		std::string* url = static_cast<std::string*>(arg);
		std::cout << *url << std::endl;//log
		std::string pages_buffer = "";

		//check if url is used
		
		//get page by sock,  count--
		count--;
		MyFuncs::MySocket msGetPage(*url);
		//MyFuncs::MySocket* msGetPage = new MyFuncs::MySocket(*url);
		//std::cout << "finish create MySocket" << std::endl;//log
		msGetPage.GET(pages_buffer);
		//msGetPage->GET(pages_buffer);

		//std::cout << "finish GET from MySocket" << std::endl;//log
		//get urls from stream
		if (pages_buffer != "")
		{
			MyFuncs::HtmlAnalyze<std::list<std::string> > haAnaly(pages_buffer);
			std::cout << "finish create HtmlAnalyze" << std::endl;//log
			haAnaly.GetUrlsFromStream(urls);
			std::cout << "finish GetUrlFromStream" << std::endl;//log
		}
		//std::cout << "ready to delete MySocket in " << __FILE__ << __LINE__ << std::endl;//log
		//delete msGetPage;	
		//save page to file
		//SavePages(pages_buffer);
		//
		std::cout << "exit Crawling, sizeof list:" << urls.size() << " count:" << count << std::endl;//log
		//pthread_exit(0);
		thread.ThreadExit(static_cast<void*>(0));
	}
	//
	void URL::StartWithSeedUrl(const std::string& url)
	{
		urls.push_back(url);
		std::string url2 = "";
		while (count > 0)
		{
			//pop url and create new thread
			if (!urls.empty())
			{
				//thread.Lock();//here is a bug ,if uncomment it will into deadlock
				url2 = urls.front();
				if (thread.ThreadCreate(Crawling, (void*)&url2, thread.mod_join))
				{
					std::cout << "create a new thread to start" << std::endl;//log
					urls.pop_front();
					std::cout << "next url is:" << urls.front() << std::endl;//log
				}					
				//thread.Unlock();
				if (ready_quit_count != 0)
				{
					ready_quit_count = 0;
				}
			}
			else
			{
				//std::cout << "bgein wait data" ;//<< std::endl;//log
				//thread.Wait(2);//here is a bug that if uncomment it, the program will run slow and seems go into deadlock
				usleep(WAIT_DATE_TIME_MS*1000);
				ready_quit_count++;
				if (ready_quit_count > CONTAINER_WAIT_MAX)
				{
					//
					break;
				}
			}
			//check if work is done		
			
		}
		std:: cout << "exit StarWithSeedUrl" << std::endl;//log
		return;
	}

	
}


