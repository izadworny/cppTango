/*
 *
 * Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
 *						European Synchrotron Radiation Facility
 *                      BP 220, Grenoble 38043
 *                      FRANCE
 *
 * This file is part of Tango.
 *
 * Tango is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Tango is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Tango.  If not, see <http://www.gnu.org/licenses/>.
 *
 * log4tango.h
 *
 * NL - SOLEIL.
 *
 */

#ifndef _LOG4TANGO_H_
#define _LOG4TANGO_H_


//-------------------------------------------------------------
// REQUIRED LOG4TANGO STUFFS
//-------------------------------------------------------------
#include <log4tango/Logger.h>
#include <log4tango/RollingFileAppender.h>
#include <log4tango/OstreamAppender.h>
#include <log4tango/PatternLayout.h>
#include <log4tango/XmlLayout.h>

//-------------------------------------------------------------
// REQUIRED TANGO LOGGING STUFFS
//-------------------------------------------------------------
#include <logstream.h>
#include <logging.h>

#include <tango_current_function.h>

#define TANGO_EXPAND_ARGS(...) __VA_ARGS__
#define TANGO_STRIP_PARENS(X) X

//-------------------------------------------------------------
// LOGGING MACROS (FOR DEVICE DEVELOPERS)
//-------------------------------------------------------------
#define LOG_FATAL(X) \
    get_logger()->fatal( \
        ::Tango::logging_detail::basename(__FILE__), __LINE__, \
        TANGO_STRIP_PARENS( TANGO_EXPAND_ARGS X ))

#define LOG_ERROR(X) \
    API_LOGGER->error( \
        ::Tango::logging_detail::basename(__FILE__), __LINE__, \
        TANGO_STRIP_PARENS( TANGO_EXPAND_ARGS X ))

#define LOG_WARN(X) \
    get_logger()->warn( \
        ::Tango::logging_detail::basename(__FILE__), __LINE__, \
        TANGO_STRIP_PARENS( TANGO_EXPAND_ARGS X ))

#define LOG_INFO(X) \
    get_logger()->info( \
        ::Tango::logging_detail::basename(__FILE__), __LINE__, \
        TANGO_STRIP_PARENS( TANGO_EXPAND_ARGS X ))

#define LOG_DEBUG(X) \
    get_logger()->debug( \
        ::Tango::logging_detail::basename(__FILE__), __LINE__, \
        TANGO_STRIP_PARENS( TANGO_EXPAND_ARGS X ))

#define DEV_FATAL_STREAM(device) \
  if (device->get_logger()->is_fatal_enabled()) \
    device->get_logger()->fatal_stream() \
      << log4tango::LogInitiator::_begin_log \
      << log4tango::LoggerStream::SourceLocation{ \
          ::Tango::logging_detail::basename(__FILE__), __LINE__}

#define DEV_ERROR_STREAM(device) \
  if (device->get_logger()->is_error_enabled()) \
    device->get_logger()->error_stream() \
      << log4tango::LogInitiator::_begin_log \
      << log4tango::LoggerStream::SourceLocation{ \
          ::Tango::logging_detail::basename(__FILE__), __LINE__}

#define DEV_WARN_STREAM(device) \
  if (device->get_logger()->is_warn_enabled()) \
    device->get_logger()->warn_stream() \
      << log4tango::LogInitiator::_begin_log \
      << log4tango::LoggerStream::SourceLocation{ \
          ::Tango::logging_detail::basename(__FILE__), __LINE__}

#define DEV_INFO_STREAM(device) \
  if (device->get_logger()->is_info_enabled()) \
    device->get_logger()->info_stream() \
      << log4tango::LogInitiator::_begin_log \
      << log4tango::LoggerStream::SourceLocation{ \
          ::Tango::logging_detail::basename(__FILE__), __LINE__}

#define DEV_DEBUG_STREAM(device) \
  if (device->get_logger()->is_debug_enabled()) \
    device->get_logger()->debug_stream() \
      << log4tango::LogInitiator::_begin_log \
      << log4tango::LoggerStream::SourceLocation{ \
          ::Tango::logging_detail::basename(__FILE__), __LINE__}

#define ENDLOG \
  log4tango::LogSeparator::_end_log

#define FATAL_STREAM DEV_FATAL_STREAM(this)
#define ERROR_STREAM DEV_ERROR_STREAM(this)
#define WARN_STREAM DEV_WARN_STREAM(this)
#define INFO_STREAM DEV_INFO_STREAM(this)
#define DEBUG_STREAM DEV_DEBUG_STREAM(this)

//-------------------------------------------------------------
// A CLASS TO LOG IN THE NAME OF DEVICE (USING DEV'S LOGGER)
//-------------------------------------------------------------
namespace Tango {

class LogAdapter
{

public:

  LogAdapter(Tango::DeviceImpl *dev);

  virtual ~LogAdapter();

  inline log4tango::Logger* get_logger (void) {
    return logger_;
  }

private:
  log4tango::Logger* logger_;
};

} // namespace Tango


#endif // _LOG4TANGO_H_

