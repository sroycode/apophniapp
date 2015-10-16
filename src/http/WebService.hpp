/**
 * @project apophniapp
 * @file include/http/WebService.hpp
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
 *  WebService.hpp : Web Server Headers
 *
 */

#ifndef _APN_HTTP_WEBSERVICE_HPP_
#define _APN_HTTP_WEBSERVICE_HPP_

#include <zmq.hpp>
#include <fstream>
#include <memory>

namespace apn {
namespace http {

template <class HttpServerT>
class WebService {
public:

	/**
	* DefaultServices : default Services
	*
	* @param context
	*   zmq::context_t* zmq context
	*
	* @param server
	*   HttpServerT server
	*
	* @return
	*   none
	*/

	void DefaultServices(zmq::context_t* context, typename std::shared_ptr<HttpServerT> server) {
		server->default_resource["GET"] =[](typename HttpServerT::Response& response, std::shared_ptr<typename HttpServerT::Request> request) {
			std::stringstream content_stream;
			content_stream << "404 NOT FOUND" << request->method << " " << request->path << " HTTP/" << request->http_version << "";
			content_stream.seekp(0, std::ios::end);
			response <<  "HTTP/" << request->http_version << " 404 NOT FOUND\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
		};

		server->resource["^/[a-z0-9]+/"]["GET"]
			=[](typename HttpServerT::Response& response, std::shared_ptr<typename HttpServerT::Request> request) {
			std::stringstream content_stream;
			content_stream << request->path;
			for(auto& header: request->header) {
				content_stream << header.first << ": " << header.second << "\r\n";
			}
			content_stream.seekp(0, std::ios::end);

			response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
		};
	}
};

} // namespace http
} // namespace rvh

#endif // _APN_HTTP_WEBSERVICE_HPP_
