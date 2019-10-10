/*
 * html_analysis v2.1 edit in 2019.5.7
 * v2.1==add GetLine() to use string for the line's container, to avoid using array's overflow
 * add thread mutex protect
 */
#ifndef HTML_ANALYSIS_H
#define HTML_ANALYSIS_H

#include <unistd.h>

#include <iostream>
#include <regex>
#include <sstream>// io string stream
#include <string>
#include <list>
#include <queue>
//#include <atomic>// thread mutex

namespace MyFuncs
{
	template <typename T>
	class HtmlAnalyze
	{
	public:
		HtmlAnalyze(const std::string& buffer) : isr_stream(buffer)
		{
		}
		~HtmlAnalyze()
		{
		}

		void GetUrlsFromStream(T&);
		
	private:
		std::string read_buffer;
		  
		std::istringstream isr_stream;
		
		std::string FindUrl(const std::string& buffer);
		std::string GetLine(void);
	};

	#include "html_analysis.inl"//here is the template definition
	
}
//
#endif /*html_analysis.h*/
