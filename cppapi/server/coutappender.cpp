//+=============================================================================
//
// file :         coutappender.cpp
//
// description :
//
// project :      TANGO
//
// author(s) :    N.Leclercq - SOLEIL
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
// Revision 3.6  2010/09/09 13:44:46  taurel
// - Add year 2010 in Copyright notice
//
// Revision 3.5  2009/01/21 12:49:04  taurel
// - Change CopyRights for 2009
//
// Revision 3.4  2008/10/06 15:00:36  taurel
// - Changed the licensing info from GPL to LGPL
//
// Revision 3.3  2008/10/03 06:51:36  taurel
// - Add some licensing info in each files
//
// Revision 3.2  2007/04/20 14:40:28  taurel
// - Ported to Windows 64 bits x64 architecture
//
// Revision 3.1  2003/05/28 14:55:08  taurel
// Add the include (conditionally) of the include files generated by autoconf
//
// Revision 3.0  2003/03/25 16:41:57  taurel
// Many changes for Tango release 3.0 including
// - Added full logging features
// - Added asynchronous calls
// - Host name of clients now stored in black-box
// - Three serialization model in DS
// - Fix miscellaneous bugs
// - Ported to gcc 3.2
// - Added ApiUtil::cleanup() and destructor methods
// - Some internal cleanups
// - Change the way how TangoMonitor class is implemented. It's a recursive
//   mutex
//
// Revision 2.2  2003/03/11 17:55:48  nleclercq
// Switch from log4cpp to log4tango
//
// Revision 2.1  2003/02/17 14:57:39  taurel
// Added the new Tango logging stuff (Thanks Nicolas from Soleil)
//
//-=============================================================================

#include <tango.h>

#include <coutappender.h>

namespace Tango
{
  CoutAppender::CoutAppender (const std::string& name)
    : log4tango::LayoutAppender(name)
  {
    //no-op ctor
  }

  CoutAppender::~CoutAppender ()
  {
    //no-op dtor
  }

  int CoutAppender::_append (const log4tango::LoggingEvent& event)
  {
#ifdef _TG_WINDOWS_
    CoutBuf *dbg_win;
    try {
      dbg_win = Util::instance(false)->get_debug_object();
    } catch (...) {
      dbg_win = 0;
    }
    if (dbg_win)
      dbg_win->dbg_out(get_layout().format(event).c_str());
    else
#endif
      ::printf("%s\n", get_layout().format(event).c_str());
    return 0;
  }

} // namespace tango
