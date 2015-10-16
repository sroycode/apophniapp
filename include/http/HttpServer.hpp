/**
 * @project apophniapp
 * @file include/http/HttpServer.hpp
 * @author  S Roychowdhury <sroycode @ gmail DOT com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * http://www.gnu.org/copyleft/gpl.html
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 *  HttpServer.hpp : HTTP Server ( Modified from https://github.com/eidheim/Simple-Web-Server)
 *
 */

#ifndef _APN_HTTP_HTTPSERVER_HPP_
#define _APN_HTTP_HTTPSERVER_HPP_

#include "HttpServerBase.hpp"

#ifdef APOPHNIAPP_BUILD_WITH_SSL
#include <boost/asio/ssl.hpp>
#endif


namespace apn {
namespace http {

template<class socket_type>
class HttpServer : public HttpServerBase<socket_type> {};
typedef boost::asio::ip::tcp::socket HTTP;

template<>
class HttpServer<HTTP> : public HttpServerBase<HTTP> {
public:
	HttpServer(unsigned short port, size_t num_threads=1, size_t timeout_request=5, size_t timeout_content=300) :
		HttpServerBase<HTTP>::HttpServerBase(port, num_threads, timeout_request, timeout_content) {}
private:
	void accept()
	{
		//Create new socket for this connection
		//Shared_ptr is used to pass temporary objects to the asynchronous functions
		std::shared_ptr<HTTP> socket(new HTTP(io_service));
		acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& ec) {
			//Immediately start accepting a new connection
			accept();
			if(!ec) {
				boost::asio::ip::tcp::no_delay option(true);
				socket->set_option(option);
				read_request_and_content(socket);
			}
		});
	}
};


#ifdef APOPHNIAPP_BUILD_WITH_SSL
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;
template<>
class HttpServer<HTTPS> : public HttpServerBase<HTTPS> {
public:
	HttpServer(unsigned short port, size_t num_threads, const std::string& cert_file, const std::string& private_key_file,
	           size_t timeout_request=5, size_t timeout_content=300,
	           const std::string& verify_file=std::string()) :
		HttpServerBase<HTTPS>::HttpServerBase(port, num_threads, timeout_request, timeout_content),
		context(boost::asio::ssl::context::sslv23)
	{
		context.use_certificate_chain_file(cert_file);
		context.use_private_key_file(private_key_file, boost::asio::ssl::context::pem);

		if(verify_file.size()>0)
			context.load_verify_file(verify_file);
	}

private:
	boost::asio::ssl::context context;

	void accept()
	{
		//Create new socket for this connection
		//Shared_ptr is used to pass temporary objects to the asynchronous functions
		std::shared_ptr<HTTPS> socket(new HTTPS(io_service, context));

		acceptor.async_accept((*socket).lowest_layer(), [this, socket](const boost::system::error_code& ec) {
			//Immediately start accepting a new connection
			accept();

			if(!ec) {
				boost::asio::ip::tcp::no_delay option(true);
				socket->lowest_layer().set_option(option);

				//Set timeout on the following boost::asio::ssl::stream::async_handshake
				std::shared_ptr<boost::asio::deadline_timer> timer;
				if(timeout_request>0)
					timer=set_timeout_on_socket(socket, timeout_request);
				(*socket).async_handshake(boost::asio::ssl::stream_base::server, [this, socket, timer]
				(const boost::system::error_code& ec) {
					if(timeout_request>0)
						timer->cancel();
					if(!ec)
						read_request_and_content(socket);
				});
			}
		});
	}
};
#endif

} // namespace http
} // namespace apn
#endif	/* _APN_HTTP_HTTPSERVER_HPP_ */
