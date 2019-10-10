#include "mysocket.h"

namespace MyFuncs
{
	//
	bool MySocket::SocketInit()
	{
		struct hostent* ss_hostent = NULL;
		struct sockaddr_in t_addr;
		
		//get the ip from host name	
		ss_hostent = gethostbyname(host.c_str());
		if (ss_hostent == NULL)
		{
			std::cout << "error in getting host's ip addr at SocketInit(mysocket.cpp)" << std::endl;//log
			return false;
		}
		
		//create socket connect
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1)
		{
			std::cout << "error in creating sock at SocketInit(mysocket.cpp)" << std::endl;//log
			return false;
		}
		t_addr.sin_port = htons(default_port);
		t_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(in_addr*)ss_hostent->h_addr));
		t_addr.sin_family = AF_INET;
		if (connect(sock, (struct sockaddr*)&t_addr, sizeof(sockaddr_in)) < 0)
		{
			std::cout << "error in connecting socket at SocketInit(mysocket.cpp)" << std::endl;//log
			return false;
		}
		
		if (!is_https)
		{
			//if is not https link, end function and return
			//std::cout << "create socket successfully" << std::endl;//log
			return true;
		}
		else
		{
			//if is https link, then init the basic client's ssl
			SSL_library_init();//
			OpenSSL_add_ssl_algorithms();//
			if (1)
			{
				ssl_ctx = SSL_CTX_new(SSLv23_client_method());//
			}
			if (ssl_ctx == NULL)
			{
				std::cout << "fail to create ssl ctx at SocketInit(mysocket.cpp)" << std::endl;//log
				return false;
			}
			ssl = SSL_new(ssl_ctx);//
			if (ssl == NULL)
			{
				std::cout << "fail to create new ssl at SocketInit(mysocket.cpp)" << std::endl;//log
				return false;
			}
			if (SSL_set_fd(ssl, sock) < 0)
			{
				std::cout << "error in SSL_set_fd at SocketInit(mysocket.cpp)" << std::endl;//log
				return false;
			}
			if (SSL_connect(ssl) != 1)
			{
				std::cout << "error in SSL_connect at SocketInit(mysocket.cpp)" << std::endl;//log
				return false;
			}
			else
			{
				//std::cout << "create ssl successfully" << std::endl;//log
				return true;
			}
		}
		return false;
	}
	//
	void MySocket::GET(std::string& buffer)
	{
		// in v1.1 when user using GET, it will auto run SockInit()
		if (SocketInit() == false)
		{
			std::cout << __FILE__ << "," << __LINE__ << " SockInit() fail" << std::endl;//log
			return;
		}
		
		std::string request = "GET " + path + " HTTP/1.1\r\n" +
				"Host: " + host + "\r\n" +
				"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:65.0) Gecko/20100101 Firefox/65.0\r\n" + 
				//"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n" +
				//"Accept-Language: en-US,en;q=0.5\r\n" +
				//"Accept-Encoding: gzip, deflate\r\n" +
				"DNT: 1\r\n" +
				"Connection: close\r\n" +
				"\r\n";
		
		if (!is_https)
		{
			//deal with http
			if (send(sock, request.c_str(), request.length(), 0) < 0)
			{
				//send http request header
				std::cout << "error in sending requeset at GET" << std::endl;//log
				return;
			}
			
			while (recv(sock, recv_buffer, RECV_BUFFER_SIZE, 0) > 0)
			{
				//recv http respond
				buffer += recv_buffer;
			}
		}
		else
		{
			//deal with https
			if (SSL_write(ssl, request.c_str(), request.length()) < 0)
			{
				std::cout << "error in SSL_write at SocketInit(mysocket.cpp)" << std::endl;//log
				return;
			}
		
			int rec = SSL_read(ssl, recv_buffer, RECV_BUFFER_SIZE);
			while (rec >= 0)
			{
				//recv https respond
				buffer += recv_buffer;
				if (rec <= 0)
					break;
				rec = SSL_read(ssl, recv_buffer, RECV_BUFFER_SIZE);
			}
		}
		return;
	}
	//
	std::string MySocket::GetPathFromUrl() const
	{
		std::smatch sm;
		std::string buf = url;
		
		//remove http head
		if (std::regex_search(buf, sm, std::regex("://")))
			buf = sm.suffix().str();
		//remove host and get path
		if (std::regex_search(buf, sm, std::regex("[\\w\.]*")))
			buf = sm.suffix().str();
		if (buf == "")
			return "/";
		//remove special signs like "
		if (std::regex_search(buf, sm, std::regex("[^\"]*")))
			return sm.str();
		else
			return "/";
		//checked
	}
	//
	std::string MySocket::GetHostFromUrl() const
	{
		std::smatch sm;
		std::string buf = url;
		
		if (std::regex_search(buf, sm, std::regex("://")))
			buf = sm.suffix().str();
		if (std::regex_search(buf, sm, std::regex("[\\w\.]*")))
			buf = sm.str();
		else
			return " ";
		return buf;
		//checked
	}
	






	
}

