/**
 * @project apophniapp
 * @file include/utils/ThreadContainer.hpp
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
 *  ThreadContainer.hpp : Thread Container
 *
 */

#ifndef _APN_UTILS_THREADCONTAINER_HPP_
#define _APN_UTILS_THREADCONTAINER_HPP_

#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <zmq.hpp>
#include "CfgFileOptions.hpp"

namespace apn {
namespace utils {
template<class T>
class ThreadContainer {
	typedef std::function<void(void)> VoidFunctionT;

public:
	/**
	* Constructor deleted
	*/
	ThreadContainer()=default;
	ThreadContainer(const ThreadContainer&)=delete;

	/**
	* Constructor : the used constructor
	*
	* @param config
	*   apn::utils::CfgFileOptions::pointer config pointer
	*
	* @param context
	*   zmq::context_t* zmq context
	*
	* @param onthrow
	*   std::function<void(void)> onthrow
	*
	* @return
	*   none
	*/
	ThreadContainer(apn::utils::CfgFileOptions::pointer config, zmq::context_t* context,
	                std::function<void(void)> onthrow
	               ) : mythread([&] {
		try {
			auto server = T::create(context);
			this->section = server->GetSection();
			typename T::ParamsListT require = server->GetRequire();
			typename T::ParamsListT params;
			for (std::size_t i=0; i<require.size(); ++i)
			{
				params.push_back ( config->Find<std::string>(section,require[i]));
			}
			this->mystop=std::bind(&T::stop,server);
			DLOG(INFO) << section << " Server Started " << std::endl;
			server->init(params); // should block
			DLOG(INFO) << section << " Server Interrupted " << std::endl;
		} catch (...)
		{
			onthrow();
		}
	})
	{
	}

	/**
	*
	* ok : check if things are ok
	*
	* @return
	*   none
	*/
	bool ok()
	{
		return ((mystop != nullptr) && (mythread.joinable()));
	}

	/**
	* destructor
	*/
	~ThreadContainer ()
	{
		if (mystop != nullptr) mystop();
		DLOG(INFO) << section << " Server Stopped " << std::endl;
		if (mythread.joinable()) mythread.join();
		DLOG(INFO) << section << " Server Joined " << std::endl;
	}

protected:
	std::thread mythread;
	VoidFunctionT mystop;
	std::string section;

};

} // namespace utils
} // namespace apn
#endif /* _APN_UTILS_THREADCONTAINER_HPP_ */
