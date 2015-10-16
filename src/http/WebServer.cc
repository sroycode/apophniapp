/**
 * @project apophniapp
 * @file src/http/WebServer.cc
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
 *  WebServer.cc : Web Server Implementation
 *
 */

#include <syslog.h>
#include <unistd.h>
#include <memory>

#include "WebServer.hpp"

apn::http::WebServer::WebServer(zmq::context_t* context)
	: apn::utils::ServerBase(context)
{
	DLOG(INFO) << "WebServer Created" << std::endl;
}

apn::http::WebServer::~WebServer ()
{
	DLOG(INFO) << "WebServer Deleted" << std::endl;
}

std::string apn::http::WebServer::GetSection()
{
	return "http";
}

apn::http::WebServer::ParamsListT apn::http::WebServer::GetRequire()
{
	return {"host","port","threads"};
}

void apn::http::WebServer::init(apn::http::WebServer::ParamsListT params)
{
	if (params.size() != this->GetRequire().size() )
		throw std::runtime_error("WebServer: params and required size mismatch");
	DLOG(INFO) << "WebServer Started" << std::endl;
	server = std::make_shared<HttpServerT>(
	    std::stoul(params[1]), // port
	    std::stoul(params[2]) // threads
	);
	is_init=true;
	this->DefaultServices(context_,server);
	server->start();
}

void apn::http::WebServer::stop()
{
	server->stop();
	DLOG(INFO) << "WebServer Stopped" << std::endl;
}
