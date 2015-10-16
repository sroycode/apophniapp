/**
 * @project apophniapp
 * @file src/main/ApnServer.cc
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
 *  ApnServer.cc : ApnServer File
 *
 */

#include <glog/logging.h>

#include <exception>
#include <stdexcept>
#include <boost/asio/signal_set.hpp>
#include <utils/CmdLineOptions.hpp>
#include <utils/CfgFileOptions.hpp>
#include <utils/ParamsCheck.hpp>
#include <utils/ThreadContainer.hpp>

#include "ApnServer.hh"
#include "../http/WebServer.hpp"

/** main */
int main(int argc, char *argv[])
{

	/** Logging **/
	google::InitGoogleLogging(argv[0]);

	/** definitions */
	apn::utils::CmdLineOptions::pointer MyCLO;
	apn::utils::CfgFileOptions::pointer MyCFG;
	apn::utils::CmdLineOptions::eList L = {
		{APN_DEFAULT_STRN_CONFIG,"c","set config file",true},
		{APN_DEFAULT_STRN_DAEMON,"d","set if daemon",true}
	};

	try {
		// read command line
		MyCLO = apn::utils::CmdLineOptions::create(argc,argv,L);
		if(MyCLO->Check(APN_DEFAULT_STRN_HELP) || (!MyCLO->Check(APN_DEFAULT_STRN_CONFIG))) {
			std::cout << MyCLO->GetHelpLine() << std::endl;
			exit(0);
		}
		// bool daemon = MyCLO->Check(APN_DEFAULT_STRN_DAEMON);
		// read config file
		MyCFG = apn::utils::CfgFileOptions::create(MyCLO->Find<std::string>(APN_DEFAULT_STRN_CONFIG));
		MyCFG->Show();
		if (MyCFG->Check<std::string>(APN_DEFAULT_STRN_SYSTEM, "accesslog")) {
			google::SetLogDestination(
			    google::GLOG_INFO, MyCFG->Find<std::string>(APN_DEFAULT_STRN_SYSTEM, "accesslog").c_str() );
		}
		if (MyCFG->Check<std::string>(APN_DEFAULT_STRN_SYSTEM, "errorlog")) {
			google::SetLogDestination(
			    google::GLOG_ERROR, MyCFG->Find<std::string>(APN_DEFAULT_STRN_SYSTEM, "errorlog").c_str() );
			google::SetLogDestination(
			    google::GLOG_FATAL, MyCFG->Find<std::string>(APN_DEFAULT_STRN_SYSTEM, "errorlog").c_str() );
		}

	} catch(std::runtime_error& e) {
		std::cerr << "Runtime Exit " << e.what() << std::endl;
		exit(1);
	} catch(...) {
		std::cerr << "Unknown Exit" << std::endl;
		exit(1);
	}

#ifdef APOPHNIAPP_BUILD_WITH_SSL
	DLOG(INFO) << "Built With HTTPS" << std::endl;
#endif
	/** If not exited Start All Servers */

	try {
		/** initialize zmq context  as shared ptr and get the raw pointer 
		  * this is a very dirty hack but i was getting sick of handling this
		  * raw pointer graceful closure ( see zmq mailing list) */
		std::shared_ptr<zmq::context_t> shared_context = std::make_shared<zmq::context_t>(APN_DEFAULT_ZMQ_INITTHREADS);
		zmq::context_t* context = shared_context.get();

		boost::asio::io_service m_io_service;
		boost::asio::io_service::work m_work(m_io_service);
		boost::asio::signal_set m_signals(m_io_service,SIGINT,SIGTERM,SIGHUP);
		m_signals.async_wait(std::bind(&boost::asio::io_service::stop, &m_io_service));

		std::exception_ptr eptr=nullptr;
		std::function<void(void)> onthrow = [&]() {
			if (!eptr) eptr=std::current_exception();
			if (!m_io_service.stopped()) m_io_service.stop();
		};

		{
			apn::utils::ParamsCheck<apn::http::WebServer>(MyCFG,context);
		}
		/** Start Server function*/
		apn::utils::ThreadContainer<apn::http::WebServer> webserver(MyCFG,context,onthrow);
		usleep(100);
		
		if (eptr) {
			DLOG(INFO) << "Exception Thrown Before" << std::endl;
			std::rethrow_exception(eptr);
		}
		m_io_service.run();
		/** Interrupted */
		DLOG(INFO) << "Interrupted " << std::endl;
		if (eptr) {
			DLOG(INFO) << "Exception Thrown After" << std::endl;
			std::rethrow_exception(eptr);
		}

		/** Delete Context Not Needed :-) */


	} catch(std::runtime_error& e) {
		DLOG(INFO) << "Main Exit " << e.what() << std::endl;
	} catch(...) {
		DLOG(INFO) << " Unknown Exit" << std::endl;
	}

	DLOG(INFO) << "EXITING" << std::endl;
	return 0;
}

