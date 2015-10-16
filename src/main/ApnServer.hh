/**
 * @project apophniapp
 * @file src/main/ApnServer.hh
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
 *  ApnServer.hh : ApnServer Headers File
 *
 */

#ifndef _APN_APNSERVER_HPP_
#define _APN_APNSERVER_HPP_

#include <ctime>
#include <iostream>

#include <unistd.h>
#include <string>
#include <vector>
#include <string>
#include <exception>
#include <thread>
#include <memory>
#include <functional>
#include <zmq.hpp>

#define APN_DEFAULT_EXE_NAME "apnserver"
#define APN_DEFAULT_EXE_VERSION 1.0.0
#define APN_DEFAULT_EXE_COPYRIGHT "Copyright (c) 2015-2016"

#define APN_DEFAULT_STRN_HELP "help"
#define APN_DEFAULT_STRN_CONFIG "config"
#define APN_DEFAULT_STRN_SYSTEM "system"
#define APN_DEFAULT_STRN_DAEMON "daemon"

#define APN_DEFAULT_ZMQ_INITTHREADS 1
#define APN_DEFAULT_ZMQ_SYSTEMTHREADS 10

#endif
