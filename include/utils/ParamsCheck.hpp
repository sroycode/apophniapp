/**
 * @project apophniapp
 * @file src/utils/ParamsCheck.hpp
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
 *  ParamsCheck.hpp : Check params are valid
 *
 */

#ifndef _APN_UTILS_PARAMSCHECK_HPP_
#define _APN_UTILS_PARAMSCHECK_HPP_

#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <zmq.hpp>
#include <utils/CfgFileOptions.hpp>

namespace apn {
namespace utils {
template<class T>
class ParamsCheck {

public:
	/**
	* Constructor deleted
	*/
	ParamsCheck()=default;
	ParamsCheck(const ParamsCheck&)=delete;

	/**
	* Constructor : the used constructor
	*
	* @param config
	*   apn::utils::CfgFileOptions::pointer config pointer
	*
	* @param context
	*   zmq::context_t* zmq context
	*
	* @return
	*   none
	*/
	ParamsCheck(apn::utils::CfgFileOptions::pointer config, zmq::context_t* context) {
		auto server = T::create(context);
		std::string section = server->GetSection();
		typename T::ParamsListT require = server->GetRequire();
		typename T::ParamsListT params;
		for (std::size_t i=0; i<require.size(); ++i)
		{
			params.push_back ( config->Find<std::string>(section,require[i]));
		}
	}

	/**
	* destructor
	*/
	virtual ~ParamsCheck () {}

protected:

};

} // namespace utils
} // namespace apn
#endif /* _APN_UTILS_PARAMSCHECK_HPP_ */
