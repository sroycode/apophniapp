/**
 * @project apophnia++
 * @file src/Main.cc
 * @author  S Roychowdhury <sroycode AT gmail DOT com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
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
 * Main Program Posix
 *
 */

#include <iostream>
#include <map>
#include <boost/assign/list_of.hpp>
#include "Default.hh"
#include <apn/CmdLineOptions.hpp>
#include <apn/CfgFileOptions.hpp>
#include <apn/ConnServ.hpp>
#include "Work.hpp"

/** definitions */
apn::CmdLineOptions MyCLO;
apn::CfgFileOptions MyCFG;
apn::CmdLineOptions::eList L = boost::assign::tuple_list_of
                               ("config","c","set config file",true)
                               ("verbose","v","set verbosity level",true)
                               ("port","p","set port",true)
                               ("threads","t","no of threads",true)
                               ("referdir","r","refer dir",true)
                               ("cachedir","k","cache dir",true)
                               ;
/** definitions */
template<class T>
T FindSystem(const std::string s, T defval)
{
	T p = defval;
	if (MyCLO.Check(s) )
		p = MyCLO.Find<T>(s);
	else if (MyCFG.Check<T>("system",s) )
		p = MyCFG.Find<T>("system",s);
	return p;
}

int main(int argc, char *argv[])
{

	try {

		MyCLO = apn::CmdLineOptions(argc,argv,L);
		if(MyCLO.Check("help") || (!MyCLO.Check("config"))) {
			std::cout << MyCLO.GetHelpLine() << std::endl;
			exit(0);
		}

		MyCFG = apn::CfgFileOptions(MyCLO.Find<std::string>("config"));
		MyCFG.Show();
		/** port: CommandLine, Config , default */
		int port = FindSystem<int>("port",APNR_DEFAULT_PORT);
		int threads = FindSystem<int>("threads",APNR_DEFAULT_HTTP_THREADS);
		std::string referdir = FindSystem<std::string>("referdir",APNR_DEFAULT_REFERDIR);
		std::string cachedir = FindSystem<std::string>("cachedir",APNR_DEFAULT_CACHEDIR);
		typedef std::map<std::string,std::string> ssMap;
		std::string myloc = std::string(argv[0]);
		/**  work */
		apnr::Work::pointer Sdata = apnr::Work::create(myloc,referdir,cachedir);
		/** http */
		apn::ConnServ<apnr::Work::pointer>::create(threads, port, Sdata->share());

	} catch(apn::GenericException e) {
		std::cerr << e.ErrorCode_ << " " << e.ErrorMsg_ << " " << e.ErrorFor_ << std::endl;
		exit(1);

	} catch(std::exception e) {
		std::cerr << e.what() << std::endl;
		exit(1);

	} catch(...) {
		std::cerr << " Unknown Exit" << std::endl;
		exit(1);
	}

	return 0;
}
