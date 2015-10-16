/**
 * @project apophniapp
 * @file include/http/HttpClient.hpp
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
 *  HttpClient.hpp : HTTP Client ( Modified from https://github.com/eidheim/Simple-Web-Server)
 *
 */

#ifndef _APN_HTTP_HTTPCLIENT_HPP_
#define _APN_HTTP_HTTPCLIENT_HPP_

#include "ClientBase.hpp"

#ifdef APOPHNIAPP_BUILD_WITH_SSL
#include <boost/asio/ssl.hpp>
#endif

namespace apn {
namespace http {

template<class socket_type>
class HttpClient : public ClientBase<socket_type> {};

typedef boost::asio::ip::tcp::socket HTTP;

template<>
class HttpClient<HTTP> : public ClientBase<HTTP> {
public:
	HttpClient(const std::string& server_port_path) : ClientBase<HTTP>::ClientBase(server_port_path, 80)
	{
		socket=std::make_shared<HTTP>(asio_io_service);
	}

private:
	void connect()
	{
		if(socket_error || !socket->is_open()) {
			boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
			boost::asio::connect(*socket, asio_resolver.resolve(query));

			boost::asio::ip::tcp::no_delay option(true);
			socket->set_option(option);

			socket_error=false;
		}
	}
};

#ifdef APOPHNIAPP_BUILD_WITH_SSL
typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;
template<>
class HttpClient<HTTP> : public ClientBase<HTTPS> {
public:
	HttpClient(const std::string& server_port_path, bool verify_certificate=true,
	           const std::string& cert_file=std::string(), const std::string& private_key_file=std::string(),
	           const std::string& verify_file=std::string()) :
		ClientBase<HTTPS>::ClientBase(server_port_path, 443), asio_context(boost::asio::ssl::context::sslv23)
	{
		if(verify_certificate)
			asio_context.set_verify_mode(boost::asio::ssl::verify_peer);
		else
			asio_context.set_verify_mode(boost::asio::ssl::verify_none);

		if(cert_file.size()>0 && private_key_file.size()>0) {
			asio_context.use_certificate_chain_file(cert_file);
			asio_context.use_private_key_file(private_key_file, boost::asio::ssl::context::pem);
		}

		if(verify_file.size()>0)
			asio_context.load_verify_file(verify_file);

		socket=std::make_shared<HTTPS>(asio_io_service, asio_context);
	};

private:
	boost::asio::ssl::context asio_context;

	void connect()
	{
		if(socket_error || !socket->lowest_layer().is_open()) {
			boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
			boost::asio::connect(socket->lowest_layer(), asio_resolver.resolve(query));

			boost::asio::ip::tcp::no_delay option(true);
			socket->lowest_layer().set_option(option);

			socket->handshake(boost::asio::ssl::stream_base::client);
			socket_error=false;
		}
	}
};
#endif

#endif	/* CLIENT_HTTPS_HPP */

} // namespace webserver
} // namespace apn
#endif	/* _APN_HTTP_HTTPCLIENT_HPP_ */
