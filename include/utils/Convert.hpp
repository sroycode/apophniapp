/**
 * @project apophniapp
 * @file include/utils/Convert.hpp
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
 *  Convert.hpp : Various Converters
 *
 */

#ifndef _APN_UTILS_CONVERT_HPP_
#define _APN_UTILS_CONVERT_HPP_

#include <exception>
#include <boost/utility/value_init.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/algorithm/string.hpp>

namespace apn {
namespace Convert {

/**
 * AnyToAny: Get the Corresp value in a different Type
 *
 * @param InData
 *   InputType Input Data
 *
 * @param NoThrow
 *   (optional) bool get default if error, default FALSE
 *
 * @return
 *   OutputType
 */
template<typename InDataT , typename OutDataT>
OutDataT AnyToAny(InDataT InData, bool NoThrow=false)
{
	OutDataT OutData = OutDataT();

	try {
		OutData=boost::lexical_cast<OutDataT>(InData);

	} catch(std::exception& e) {
		if(! NoThrow)
			throw std::runtime_error("conversion error");

	} catch(...) {
		if(! NoThrow)
			throw std::runtime_error("conversion error");
	}

	return OutData;
}

} // namespace Convert
} // namespace apn
#define APN_DEFAULT_STR2SIZET(AAA) apn::Convert::AnyToAny<std::string,std::size_t>(AAA)
#define APN_DEFAULT_STR2UINT(AAA) apn::Convert::AnyToAny<std::string,unsigned int>(AAA)

#endif // _APN_UTILS_CONVERT_HPP_
