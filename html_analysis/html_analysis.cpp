#include "html_analysis.h"

#include <mutex>// enable thread mutex protect

namespace MyFuncs
{
	/*
	 * specialization for std::queue
	 */
	//
	std::string HtmlAnalyze< std::queue<std::string> >::FindUrl(const std::string& buffer)
	{
		//std::cout << "in GetHttp_Url" << buffer << std::endl;//log
		std::string buff = buffer;
		std::smatch sm; 
		std::string pattern = "http://\\S*\"|\"https://\\S*\"";
		//std::string buf = buffer;
		if (std::regex_search(buff, sm, std::regex(pattern)))
		  buff = sm.str();
		else 
		  buff = ""; 
		return buff;
		//checked
	}
	//
	std::string HtmlAnalyze< std::queue<std::string> >::GetLine(void)
	{
		char buf = '\0';
		std::string buff = "";
		
		isr_stream.get(buf);
		while ((buf != '\n') && (buf != '\t') && (!isr_stream.eof()))
		{
			buff += buf;
			isr_stream.get(buf);
		}
		
		return buff;
	}
	//
	void HtmlAnalyze< std::queue<std::string> >::GetUrlsFromStream(std::queue<std::string>& newcontainer)
	{
		//std::cout << "begin find url at queue" << std::endl;//log
		std::mutex container_mutex;
		std::string url = ""; 
			  
		while (!isr_stream.eof())
		{   
		  read_buffer = GetLine();
		  url = FindUrl(read_buffer);
		  if (url != "") 
		  {   
			container_mutex.lock();
			newcontainer.push(url);
			container_mutex.unlock();
			//std::cout << url << std::endl;//log
		  }   
		}   
		return;
	}

	/*
	 * specialization for std::list 
	 */
	std::string HtmlAnalyze< std::list<std::string> >::FindUrl(const std::string& buffer)
	{
		//std::cout << "in GetHttp_Url" << buffer << std::endl;//log
		std::string buff = buffer;
		std::smatch sm; 
		std::string pattern = "http://\\S*\"|\"https://\\S*\"";
		//std::string buf = buffer;
		if (std::regex_search(buff, sm, std::regex(pattern)))
		  buff = sm.str();
		else 
		  buff = ""; 
		return buff;
		//checked
	}
	//
	std::string HtmlAnalyze< std::list<std::string> >::GetLine(void)
	{
		char buf = '\0';
		std::string buff = "";
		
		isr_stream.get(buf);
		while ((buf != '\n') && (buf != '\t') && (!isr_stream.eof()))
		{
			buff += buf;
			isr_stream.get(buf);
		}
		
		return buff;
	}
	//
	void HtmlAnalyze< std::list<std::string> >::GetUrlsFromStream(std::list<std::string>& newcontainer)
	{
		//std::cout << "begin find url at queue" << std::endl;//log
		std::mutex container_mutex;
		std::string url = ""; 
			  
		while (!isr_stream.eof())
		{   
		  read_buffer = GetLine();
		  url = FindUrl(read_buffer);
		  if (url != "") 
		  {   
			container_mutex.lock();
			newcontainer.push_back(url);
			container_mutex.unlock();
			//std::cout << url << std::endl;//log
		  }   
		}   
		return;
	}

}

