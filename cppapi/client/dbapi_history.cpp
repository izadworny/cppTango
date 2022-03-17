//
// dbapi_history.cpp - C++ source code file for TANGO dbapi class DbHistory
//
// programmer 	- JL Pons (pons@esrf.fr)
//
// original 	- Feb 2007
//
// Copyright (C) :      2007,2008,2009,2010,2011,2012,2013,2014,2015
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
// Revision 3.6  2010/09/09 13:43:38  taurel
// - Add year 2010 in Copyright notice
//
// Revision 3.5  2009/01/21 12:45:15  taurel
// - Change CopyRights for 2009
//
// Revision 3.4  2008/10/06 15:02:17  taurel
// - Changed the licensing info from GPL to LGPL
//
// Revision 3.3  2008/10/02 16:09:25  taurel
// - Add some licensing information in each files...
//
// Revision 3.2  2008/03/11 14:36:44  taurel
// - Apply patches from Frederic Picca about compilation with gcc 4.2
//
// Revision 3.1  2007/04/30 14:05:47  jlpons
// Added new commands to the database client interface.
//
//
//

#include <tango.h>

using namespace CORBA;

namespace Tango
{

//-----------------------------------------------------------------------------
//
// DbHistory::DbHistory() - Constructs a property
//
//-----------------------------------------------------------------------------

DbHistory::DbHistory(std::string _propname, std::string _date, const std::vector<std::string> &svalues)
{
  propname = _propname;
  date     = format_mysql_date(_date);
  deleted  = (svalues.size() == 0);
  make_db_datum(svalues);
}

//-----------------------------------------------------------------------------
//
// DbHistory::DbHistory() - Constructs an attribute property.
//
//-----------------------------------------------------------------------------

DbHistory::DbHistory(std::string _propname, std::string _attname, std::string _date,
                     const std::vector<std::string> &svalues)
{
  propname = _propname;
  attname  = _attname;
  date     = format_mysql_date(_date);
  deleted  = (svalues.size() == 0);
  make_db_datum(svalues);
}

//-----------------------------------------------------------------------------
//
// DbHistory::get_name() - Returns property name.
//
//-----------------------------------------------------------------------------

std::string DbHistory::get_name() { return propname; }

//-----------------------------------------------------------------------------
//
// DbHistory::get_attribute_name() - Returns attribute name.
//                                   Used when retrieving attribute property.
//
//-----------------------------------------------------------------------------

std::string DbHistory::get_attribute_name() { return attname; }

//-----------------------------------------------------------------------------
//
// DbHistory::get_date() - Returns the update date.
//
//-----------------------------------------------------------------------------

std::string DbHistory::get_date() { return date; }

//-----------------------------------------------------------------------------
//
// DbHistory::get_value() - Returns the value.
//
//-----------------------------------------------------------------------------

DbDatum DbHistory::get_value() { return value; }

//-----------------------------------------------------------------------------
//
// DbHistory::is_deleted() - Return true if the property is deleted.
//
//-----------------------------------------------------------------------------

bool DbHistory::is_deleted() { return deleted; }

//-----------------------------------------------------------------------------
//
// DbHistory::format_mysql_date() - Format mysql date using format
//                                  DD/MM/YYYY hh:mm:ss
//
//-----------------------------------------------------------------------------

std::string DbHistory::format_mysql_date(std::string _date)
{
  // Handle MySQL date formating
  if(_date.find("-") != std::string::npos)
    return _date.substr(8, 2) + "/" + _date.substr(5, 2) + "/" + _date.substr(0, 4) + " " + _date.substr(11, 2) + ":" +
           _date.substr(14, 2) + ":" + _date.substr(17, 2);
  else
    return _date.substr(6, 2) + "/" + _date.substr(4, 2) + "/" + _date.substr(0, 4) + " " + _date.substr(8, 2) + ":" +
           _date.substr(10, 2) + ":" + _date.substr(12, 2);
}

//-----------------------------------------------------------------------------
//
// DbHistory::make_db_datum() - Build the value as DbDatum
//
//-----------------------------------------------------------------------------

void DbHistory::make_db_datum(const std::vector<std::string> &values)
{
  value.name = propname;
  value.value_string.resize(values.size());
  for(unsigned int i = 0; i < values.size(); i++)
    value.value_string[i] = values[i];
}

} // namespace Tango
