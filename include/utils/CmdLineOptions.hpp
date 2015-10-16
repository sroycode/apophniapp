
/**
 * @project apophniapp
 * @file include/utils/CmdLineOptions.hpp
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
 *  CmdLineOptions.hpp : Command Line Options
 *
 */

#ifndef _APN_UTILS_CMDLINEOPTIONS_HPP_
#define _APN_UTILS_CMDLINEOPTIONS_HPP_
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <initializer_list>
#include "Convert.hpp"
#define APN_CMDLINE_LONG_DASH "--"
#define APN_CMDLINE_SHORT_DASH "-"
#define APN_CMDLINE_TABSP "   "
#define APN_CMDLINE_EOL "\n"

namespace apn {
namespace utils {
class CmdLineOptions {
public:
	typedef std::shared_ptr<CmdLineOptions> pointer;

	/**
	 * @brief: Element: struct for Holding Commandline option elems
	 */
	struct Element {
		std::string shortform;
		std::string longform;
		std::string description;
		bool hasparams;
	};
	typedef std::list<Element> eList;

	/**
	 * create
	 *
	 * @param argc
	 *   int number of parameters supplied (from command line)
	 *
	 * @param argv
	 *   char*[] parameters supplied (from command line)
	 *
	 * @param AddOptions
	 *   List_of_Elements of Valid Options
	 *
	 * @return
	 *   none
	 *
	 */
	static pointer create(int argc, char* argv[], eList L) {
		pointer p(new CmdLineOptions(argc,argv,L));
		return p;
	}

	~CmdLineOptions() {}

	/**
	 * GetHelpLine :  Get Help Line
	 *
	 * @return
	 *   String
	 */
	std::string GetHelpLine()
	{
		return HelpLine;
	}

	/**
	 * Check : Check if Value Exists
	 *
	 * @param ParamName
	 *   String Name of Param to retrieve
	 *
	 * @return
	 *   bool
	 */
	bool Check(std::string ParamName)
	{
		ssMap::const_iterator it = OptionsMap.find(ParamName);
		return (it!=OptionsMap.end());
	}

	/**
	 * Find : Find Value
	 *
	 * @param ParamName
	 *   String Name of Param to retrieve
	 * @param NoThrow
	 *   bool Find default if absent
	 *
	 * @return
	 *   Param
	 */
	template<class T>
	T Find(std::string ParamName, bool NoThrow=false)
	{
		ssMap::const_iterator it = OptionsMap.find(ParamName);

		if(it==OptionsMap.end()) {
			if(!NoThrow)
				throw std::runtime_error(std::string("Bad Commandline parameter")+ParamName);
			else return T();
		}

		try {
			return apn::Convert::AnyToAny<std::string,T>(it->second);
		} catch(std::runtime_error& e) {
			if(!NoThrow)
				throw std::runtime_error(std::string("Missing Commandline parameter ")+ParamName);
			else return T();
		}
		return T();
	}
private:
	typedef std::map<std::string,std::string> ssMap;
	ssMap OptionsMap; /** param name val */
	std::string HelpLine;

	/**
	 * Constructor : default Constructor 
	 *
	 */
	CmdLineOptions() = default;
	CmdLineOptions(const CmdLineOptions&)=delete;

	/**
	 * Constructor : used Constructor
	 *
	 * @param argc
	 *   int number of parameters supplied (from command line)
	 *
	 * @param argv
	 *   char*[] parameters supplied (from command line)
	 *
	 * @param AddOptions
	 *   List_of_Elements of Valid Options
	 *
	 * @return
	 *   none
	 */
	CmdLineOptions(int argc, char* argv[], eList AddOptions)
	{
		ssMap NameMap;
		typedef std::map<std::string,bool> sbMap;
		sbMap HasOptMap;
		HelpLine = std::string("Allowed Options" APN_CMDLINE_EOL);
		AddOptions.insert(AddOptions.begin(), {"help","h","print help line",false});
		for(auto const & El : AddOptions) {

			/** short form */
			std::string ShortForm = APN_CMDLINE_SHORT_DASH + std::string(El.longform);
			ssMap::const_iterator it = NameMap.find(ShortForm);
			if (it==NameMap.end())
				NameMap.insert(ssMap::value_type(ShortForm,El.shortform));
			else
				throw std::runtime_error(std::string("Repeated Param Shortform ")+ShortForm);
			HelpLine+= std::string(APN_CMDLINE_TABSP + ShortForm);

			/** long form */
			std::string LongForm = APN_CMDLINE_LONG_DASH + std::string(El.shortform);
			it = NameMap.find(LongForm);
			if(it==NameMap.end())
				NameMap.insert(ssMap::value_type(LongForm,El.shortform));
			else
				throw std::runtime_error(std::string("Repeated Param Longform ")+LongForm);
			HelpLine+= std::string(APN_CMDLINE_TABSP "[" + LongForm + "]");

			/** explanation */
			HasOptMap.insert(sbMap::value_type(El.shortform,El.hasparams));
			HelpLine+= std::string(APN_CMDLINE_TABSP + El.description +  APN_CMDLINE_EOL);
		}
		std::string TokenString;
		for(int i=0; i<argc; ++i) {
			TokenString+=std::string((i==0)?"":" ")+std::string(argv[i]);
		}
		std::vector<std::string> TokenVec;
		boost::split(TokenVec,TokenString,boost::is_any_of(" =\n\t"));
		for(size_t i=1; i<TokenVec.size(); ++i) {
			ssMap::const_iterator it = NameMap.find(TokenVec.at(i));
			if(it==NameMap.end())
				throw std::runtime_error(std::string("Unknown Option ")+TokenVec.at(i));
			if(HasOptMap[it->second]) {
				if(++i>=TokenVec.size())  throw std::runtime_error(std::string("No Value for Option ")+it->second);
				OptionsMap.insert(ssMap::value_type(it->second,TokenVec.at(i)));
			} else {
				OptionsMap.insert(ssMap::value_type(it->second,std::string()));
			}
		}
	}
};
} // namespace utils
} // namespace apn
#endif
