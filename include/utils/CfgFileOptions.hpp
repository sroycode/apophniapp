
/**
 * @project apophniapp
 * @file include/utils/CfgFileOptions.hpp
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
 *  CfgFileOptions.hpp : Config File Reader
 *
 */

#ifndef _APN_UTILS_CFGFILEOPTIONS_HPP_
#define _APN_UTILS_CFGFILEOPTIONS_HPP_

#include <fstream>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <memory>
#include <iostream>
#include <iterator>
#include "Convert.hpp"

#define APN_CFGFILE_COLON ':'
#define APN_CFGFILE_SEMICOLON ';'
#define APN_CFGFILE_SQB_OPEN '['
#define APN_CFGFILE_SQB_CLOSE ']'
#define APN_CFGFILE_HASH '#'
#define APN_CFGFILE_EQUALTO '='
#define APN_CFGFILE_MIDLINE_TRIM " \t\r\n"
#define APN_CFGFILE_ENDLINE_TRIM " \t\r\n;"
#define APN_CFGFILE_BLANK_STR ""
#define APN_CFGFILE_DELIM_STR ":"

namespace apn {
namespace utils {
class CfgFileOptions {
public:
	typedef std::shared_ptr<CfgFileOptions> pointer;
	typedef std::pair<std::string,std::string> ssPairT;
	typedef std::map<std::string,std::string> ssMapT;
	typedef std::list<std::string> sListT;

	/**
	 * create
	 *
	 * @param InputFile
	 *   String Input File Name
	 *
	 */
	static pointer create(const std::string InputFile) {
		pointer p(new CfgFileOptions(InputFile));
		return p;
	}

	~CfgFileOptions() {}

	/**
	 * Check : Check if Value Exists
	 *
	 * @param Section
	 *   String Section to find it in
	 *
	 * @param Name
	 *   String Name to find
	 *
	 * @return
	 *   bool
	 */
	template<typename T>
	bool Check(const std::string Section, const std::string Name)
	{
		try {
			std::string s = Find<std::string>(Section,Name);
			return true;
		} catch (std::runtime_error& e) {
			return false;
		}
		return false; // never reaches
	}

	/**
	 * Find: Find an entry
	 *
	 * @param Section
	 *   String Section to find it in
	 *
	 * @param Name
	 *   String Name to find
	 *
	 * @param NoThrow
	 *   Bool get default-val if true, defult false.
	 *
	 * @return
	 *   Type
	 */
	template<typename T>
	T Find(const std::string Section, const std::string Name, bool NoThrow=false)
	{
		auto it = SectionMap.find(MakeKey(Section,Name));
		if (it != SectionMap.end()) {
			return Convert::AnyToAny<std::string,T>(it->second,NoThrow);
		}

		/** else look for inherited section, duplicate is checked so inf loop not possible */
		auto jt = InheritMap.find(Section);
		while ( jt !=InheritMap.end()) {
			it = SectionMap.find(MakeKey(jt->second,Name));
			if (it != SectionMap.end()) {
				return Convert::AnyToAny<std::string,T>(it->second,NoThrow);
			}

			/** recurse if not found */
			jt = InheritMap.find(jt->second);
		}
		if(!NoThrow)
			throw std::runtime_error(std::string("Invalid Config Param : ") + Name);
		return T();
	}

	/**
	 * FindList: Find several entries
	 *
	 * @param Section
	 *   String Section to find it in
	 *
	 * @param NameList
	 *   sListT Names to find
	 *
	 * @param NoThrow
	 *   Bool get default-val if true, defult false.
	 *
	 * @return
	 *   Type
	 */
	template<typename T>
	T FindList(const std::string Section, const sListT NameList, bool NoThrow=false)
	{
		T ValueList;
		for(auto name : NameList) {
			ValueList.push_back( this->Find<T::value_type>(Section,name,NoThrow));
		}
		return ValueList;
	}

	/**
	 * Show: Show all entries
	 *
	 * @return
	 *   none
	 */
	void Show()
	{
		for (auto const& it : SectionMap) {
			std::cerr << it.first << " = " << it.second << std::endl;
		}
	}

	/**
	 * Update: Update or Add an entry
	 *
	 * @param Section
	 *   String Section
	 * @param Name
	 *   String Name
	 * @param Value
	 *   String Value
	 *
	 * @return
	 *   none
	 */
	void Update(const std::string Section, const std::string Name, std::string Value)
	{
		SectionMap.insert(ssMapT::value_type(MakeKey(Section,Name),Value));
	}

	/**
	 * MakeKey: Merge Section and Name
	 *
	 * @param Section
	 *   String Section to find it in
	 *
	 * @param Name
	 *   String Name to find
	 *
	 * @return
	 *   String merged
	 */
	inline std::string MakeKey(std::string Section, std::string Name)
	{
		return Section + APN_CFGFILE_DELIM_STR + Name;
	}
private:
	ssMapT SectionMap; /** Section + name = value  */
	ssMapT InheritMap; /** Section and inherits */

	/**
	 * Constructor : the default constructor
	 */
	CfgFileOptions() = default;
	CfgFileOptions(const CfgFileOptions&)=delete;

	/**
	 * Constructor : the used constructor
	 *
	 * @param InputFile
	 *   String Input File Name
	 *
	 */
	CfgFileOptions(const std::string InputFile)
	{
		std::ifstream file(InputFile.c_str());
		if(!file.is_open())
			throw std::runtime_error("Cannot Open Configfile");
		std::string line, inSection;
		while(std::getline(file,line)) {
			boost::algorithm::trim(line);
			if(! line.length()) continue;
			if(line[0] == APN_CFGFILE_HASH) continue;
			if(line[0] == APN_CFGFILE_SEMICOLON) continue;
			if(line[0] == APN_CFGFILE_SQB_OPEN) {
				if(!line.find(APN_CFGFILE_SQB_CLOSE))
					throw std::runtime_error(std::string("Configfile Syntax Error ")+line);

				/** section name */
				inSection=line.substr(1,line.find(APN_CFGFILE_SQB_CLOSE)-1);
				boost::algorithm::trim(inSection);
				std::string inHerits = APN_CFGFILE_BLANK_STR;
				ssMapT::const_iterator it = InheritMap.find(inSection);
				if(it!=InheritMap.end())
					throw std::runtime_error(std::string("Duplicate Section : ")+inSection);

				/** inherits name if exists */
				if(line.length() > line.find_last_of(APN_CFGFILE_COLON)) {
					inHerits = line.substr(line.find_last_of(APN_CFGFILE_COLON)+1);
					boost::algorithm::trim(inHerits);
					it = InheritMap.find(inHerits);
					if(it==InheritMap.end())
						throw std::runtime_error(std::string("Invalid Inherit : ")+ inSection);
				}
				InheritMap.insert(ssMapT::value_type(inSection,inHerits));
				continue;
			}
			size_t EqPos = line.find(APN_CFGFILE_EQUALTO); /** position */
			if(EqPos==std::string::npos)
				throw std::runtime_error(std::string("Invalid Line : ")+ line);
			std::string myname = line.substr(0,EqPos);
			boost::algorithm::trim(myname);
			std::string myval = line.substr(EqPos+1);
			boost::algorithm::trim(myval);
			Update(inSection,myname,myval);
		}
		file.close();
	}
};
} // namespace utils
} // namespace apn
#endif
