/*
 * 
 */ 
#include "GetUrlResource.h"
#include <iostream>
#include <stdexcept>


void ShowOptions(void)
{
	std::cout << "Welcome use the multithread web crawler.\n"
			<< "you must use -u to set seed url or the tool won't start.\n"
			//<< "===============================================================================\n"
			<< "-u is used to set the start url.e.g:-u http://www.bing.com.\n"
			//<< "===============================================================================\n"
			<< "--threads is used to set how many threads work, or just use its default value.e.g:--threads 4.\n"
			//<< "===============================================================================\n"
			<< "-p is use to set the default port when crawler is working.but the port will change by program.e.g:-p 80.\n"
			//<< "===============================================================================\n"
			<< "thanks for your using."
			<< std::endl;
	
	return;
}


int main(int argc, char** argv)
{
	Reptile::URL myurl;
	//string str_seed_url = "https://blog.csdn.net/hnlyyk/article/details/48025621";
	std::string str_seed_url = "";
	std::string str_command = "";
	int num = 0;
	
	if (argc < 2)
	{
		std::cout << "wrong command, please use '--help' to check options" << std::endl;
		return 0;
	}
	
	for (int i=1; i < argc; i++)
	{
		str_command = argv[i];
		if (str_command == "--help")
		{
			ShowOptions();
		} 
		else if (str_command == "-u")
		{
			//-u, set the seed url
			i++;
			str_seed_url = argv[i];
		}
		else if (str_command == "--threads")
		{
			i++;
			str_command = argv[i];
			try
			{
				num = std::stoi(str_command);
				if (myurl.SetThreadNum(num) == false)
				{
					std::cout << "error when set threads.exit..." << std::endl;//log
					exit(1);
				}
			}
			catch (std::invalid_argument e)
			{
				std::cout << "wrong number format when use --threads" << std::endl;//log
				exit(1);
			}
			std::cout << "set thread's num to " << num << std::endl;
		}
		else if (str_command == "-p")
		{
			i++;
			str_command = argv[i];
			try
			{
				num = std::stoi(str_command);
				myurl.SetConnectPort(num);
			}
			catch (std::invalid_argument e)
			{
				std::cout << "wrong number format when use -p" << std::endl;//log
				exit(1);
			}
		}
	}
	
	if (str_seed_url != "")
	{
		std::cout << "begin StartWithSeedUrl" << std::endl;//log
		myurl.StartWithSeedUrl(str_seed_url);
	}
	//thread_exit(NULL);//don't need this
	std::cout << "exit main" << std::endl;//log
	return 0;
}

