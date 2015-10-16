/**
 * @project apophniapp
 * @file include/utils/ServerBase.hpp
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
 *  ServerBase.hpp : Server ABC
 *
 */

#ifndef _APN_UTILS_SERVERBASE_HPP_
#define _APN_UTILS_SERVERBASE_HPP_

#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>
#include <zmq.hpp>
#include <initializer_list>
#include <glog/logging.h>

namespace apn {
namespace utils {
class ServerBase : public boost::noncopyable {
public:
	typedef std::vector<std::string> ParamsListT;

	/**
	* destructor
	*/
	virtual ~ServerBase () {}

	/**
	* init : initialize
	*
	* @param params
	*   ParamsListT parameters to init
	*
	* @return
	*   none
	*/
	virtual void init(ParamsListT params)=0;

	/**
	 * GetSection : section required
	 *
	 * @return
	 *   std::string section
	 */
	virtual std::string GetSection() =0;

	/**
	 * GetRequire : parameters required
	 *
	 * @return
	 *   ParamsListT
	 */
	virtual ParamsListT GetRequire()=0;

	/**
	* stop : shutdown
	*
	* @return
	*   none
	*/
	virtual void stop()=0;
protected:
	bool is_init;
	zmq::context_t* context_;
	ServerBase(zmq::context_t* context)
		: is_init(false),
		context_(context)
	{}

};
} // namespace utils
} // namespace apn
#endif /* _APN_UTILS_SERVERBASE_HPP_ */
