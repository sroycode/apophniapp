/**
 * @project apophnia++
 * @file src/Work.hpp
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
 * Work : WorkHorse Header
 *
 */

#ifndef _APNR_WORK_HPP_
#define _APNR_WORK_HPP_
#define APNR_WORK_PROGNO 2001

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <apn/WebObject.hpp>

namespace apnr {
class Work : private boost::noncopyable, public boost::enable_shared_from_this<Work> {
public:
	typedef boost::shared_ptr<Work> pointer;
	/**
	 * create : static construction creates new first time
	 *
	 * @param progname
	 *   String executable name
	 *
	 * @param referdir
	 *   String reference directory
	 *
	 * @param cachedir
	 *   String cache directory
	 *
	 * @return
	 *   none
	 */
	static pointer create(std::string progname, std::string referdir, std::string cachedir) {
		return pointer(new Work(progname,referdir,cachedir));
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
	 * virtual destructor
	 */
	virtual ~Work () {}
	/**
	 * run: mandatory function for web interface
	 *
	 * @param W
	 *   WebObject W
	 *
	 * @return
	 *   Bool status
	 */
	bool run(apn::WebObject::pointer W);
private:
	const std::string referdir_;
	const std::string cachedir_;
	/**
	 * Constructor : private Constructor
	 *
	 * @param progname
	 *   String executable name
	 *
	 * @param referdir
	 *   String reference directory
	 *
	 * @param cachedir
	 *   String cache directory
	 *
	 * @return
	 *   none
	 */
	Work(std::string progname, std::string referdir, std::string cachedir);
};
}
#endif /* _APNR_WORK_HPP_ */
