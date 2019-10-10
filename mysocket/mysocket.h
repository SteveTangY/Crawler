/*
 * mysocket v1.1 edit in 2019.5.1
 * ==user no longer need to run SockInit() before using GET() to avoid some mistake.
 * use to download pages by socekt
 */
#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <unistd.h>

#include <iostream>
#include <string>
#include <regex>

#include <netdb.h>//DNS
#include <sys/socket.h>//socket
#include <netinet/in.h>//socketaddr_in
#include <arpa/inet.h>//inet_addr

#include <openssl/ssl.h>//basic ssl functions
#include <openssl/err.h>

#define RECV_BUFFER_SIZE 512

namespace MyFuncs
{
	class MySocket
	{
	public:
		MySocket(const std::string& newurl) : sock(0)
		{
			url = newurl;
			if (std::regex_search(url, std::regex("https")))
			{
				//check if the link is httpS
				is_https = true;
				default_port = 443;
			}
			else
			{
				is_https = false;
				default_port = 80;
			}
			host = GetHostFromUrl();
			path = GetPathFromUrl();
		}
		MySocket(const int& port_num) : sock(0), default_port(port_num)
		{
		}
		~MySocket()
		{
			close(sock);
			if (is_https)
			{
				SSL_shutdown(ssl);
				SSL_CTX_free(ssl_ctx);
				SSL_free(ssl);
			}
		}
		
		std::string url;
		std::string host;
		std::string path;
		
		void GET(std::string& buffer);
	private:
		bool is_https;
		int sock;
		int default_port;
		char recv_buffer[RECV_BUFFER_SIZE];
		SSL* ssl;
		SSL_CTX* ssl_ctx;
		
		bool SocketInit();
		std::string GetPathFromUrl() const;
		std::string GetHostFromUrl() const;
		
	};






}

#endif /*mysocket.h*/
