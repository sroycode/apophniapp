/**
 * @project apophniapp
 * @file include/http/SecureWebServer.hpp
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
 *  SecureWebServer.hpp : Web Server Headers
 *
 */

#ifndef _APN_HTTP_SECUREWEBSERVER_HPP_
#define _APN_HTTP_SECUREWEBSERVER_HPP_

#include "utils/ServerBase.hpp"
#include "http/HttpServer.hpp"


namespace apn {
namespace http {

class SecureWebServer :
	public apn::utils::ServerBase,
	public std::enable_shared_from_this<SecureWebServer> {
public:
	typedef std::shared_ptr<SecureWebServer> pointer;
	typedef HttpServer<HTTPS> HttpServerT;

	/**
	* create : static construction creates new first time
	*
	* @param context
	*   zmq::context_t* zmq context
	*
	* @return
	*   none
	*/
	static pointer create(zmq::context_t* context) {
		pointer p(new SecureWebServer(context));
		return p;
	}

	/**
	* share : return instance
	*
	* @return
	*   none
	*/
	pointer share() {
		return shared_from_this();
	}

	/**
	* destructor
	*/
	virtual ~SecureWebServer ();

	/**
	* init : initialize
	*
	* @param params
	*   ParamsListT parameters to init
	*
	* @return
	*   none
	*/
	void init(ParamsListT params);

	/**
	 * GetSection : section required
	 *
	 * @return
	 *   std::string section
	 */
	std::string GetSection();

	/**
	 * GetRequire : parameters required
	 *
	 * @return
	 *   ParamsListT
	 */
	ParamsListT GetRequire();

	/**
	* stop : shutdown
	*
	* @return
	*   none
	*/
	void stop();

protected:
	std::shared_ptr<HttpServerT> server;

private:

	/**
	* Constructor : private default Constructor
	*
	* @param context
	*   zmq::context_t* zmq context
	*
	*/
	SecureWebServer(zmq::context_t* context);

};
} // namespace http
} // namespace apn
#endif /* _APN_HTTP_SECUREWEBSERVER_HPP_ */
