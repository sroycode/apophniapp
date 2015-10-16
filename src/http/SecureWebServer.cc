/**
 * @project apophniapp
 * @file src/http/SecureWebServer.cc
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
 *  SecureWebServer.cc : Secure Web Server Implementation
 *
 */

#include <syslog.h>
#include <unistd.h>
#include <memory>

#include "SecureWebServer.hpp"

apn::http::SecureWebServer::SecureWebServer(zmq::context_t* context)
	: apn::utils::ServerBase(context)
{
	DLOG(INFO) << "SecureWebServer Created" << std::endl;
}

apn::http::SecureWebServer::~SecureWebServer ()
{
	DLOG(INFO) << "SecureWebServer Deleted" << std::endl;
}

std::string apn::http::SecureWebServer::GetSection()
{
	return "http";
}

apn::http::SecureWebServer::ParamsListT apn::http::SecureWebServer::GetRequire()
{
	return {"host","port","threads","server_crt","server_key"};
}

void apn::http::SecureWebServer::init(apn::http::SecureWebServer::ParamsListT params)
{
	if (params.size() != this->GetRequire().size() )
		throw std::runtime_error("SecureWebServer: params and required size mismatch");
	DLOG(INFO) << "SecureWebServer Started" << std::endl;
	server = std::make_shared<HttpServerT>(
	    std::stoul(params[1]), // port
	    std::stoul(params[2]), // threads
			params[3], // server_crt
			params[3] // server_key
	);
	is_init=true;
	server->start();
}

void apn::http::SecureWebServer::stop()
{
	server->stop();
	DLOG(INFO) << "SecureWebServer Stopped" << std::endl;
}
