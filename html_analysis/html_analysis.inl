/*
 * 
 */
template <>
class HtmlAnalyze< std::queue<std::string> >
{
	public:
		HtmlAnalyze(const std::string& buffer) : isr_stream(buffer)
		{
		}
		~HtmlAnalyze()
		{
		}

		void GetUrlsFromStream(std::queue<std::string>&);
		
	private:
		std::string read_buffer = "";
		  
		std::istringstream isr_stream;
		
		std::string FindUrl(const std::string& buffer);
		std::string GetLine(void);
};
/*
 * 
 */
template <>
class HtmlAnalyze< std::list<std::string> >
{
	public:
		HtmlAnalyze(const std::string& buffer) : isr_stream(buffer)
		{
		}
		~HtmlAnalyze()
		{
		}

		void GetUrlsFromStream(std::list<std::string>&);
		
	private:
		std::string read_buffer = "";
		  
		std::istringstream isr_stream;
		
		std::string FindUrl(const std::string& buffer);
		std::string GetLine(void);
};



