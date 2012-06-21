/**
 * @project apophnia++
 * @file src/MimeTypes.hpp
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
 * Mime Types
 *
 */

#ifndef _APNR_MIME_TYPES_HPP_
#define _APNR_MIME_TYPES_HPP_
#define APNR_MIME_TYPES_PROGNO 2003

#include <string>
#include <algorithm>

namespace apnr {
namespace mime_types {

struct mime_type_mapping {
	const char* extension;
	const char* type;
	const char* name;
} mime_type_mappings[] = {
	{ "gif", "image/gif", "GIF" },
	{ "jpg", "image/jpeg", "JPEG" },
	{ "jpeg", "image/jpeg" , "JPEG" },
	{ "png", "image/png", "PNG" },
	{ 0, 0, 0 } // Marks end of list.
};

/**
 * get_type: get type corresp to extn
 *
 * @param extension
 *   String extension by address
 *
 * @return
 *   String
 */
std::string get_type(const std::string& extension)
{
	std::string e = extension;
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	for (mime_type_mapping* m = mime_type_mappings; m->extension; ++m) {
		if (m->extension == e) {
			return m->type;
		}
	}
	throw apn::GenericException(APNR_MIME_TYPES_PROGNO,"invalid extn","");
	return "";
}

/**
 * get_name: get name corresp to extn
 *
 * @param extension
 *   String extension by address
 *
 * @return
 *   String
 */
std::string get_name(const std::string& extension)
{
	std::string e = extension;
	std::transform(e.begin(), e.end(), e.begin(), ::tolower);
	for (mime_type_mapping* m = mime_type_mappings; m->extension; ++m) {
		if (m->extension == e) {
			return m->name;
		}
	}
	throw apn::GenericException(APNR_MIME_TYPES_PROGNO,"invalid extn","");
	return "";
}

/**
 * get_all_extns : get all extensions
 *
 * @return
 *   Type
 */
template<class T>
T get_all_extns()
{
	T t;
	for (mime_type_mapping* m = mime_type_mappings; m->extension; ++m) {
		t.push_back(typename T::value_type(m->extension));
	}
	return t;
}

} // namespace mime_types
} // namespace apnr
#endif
