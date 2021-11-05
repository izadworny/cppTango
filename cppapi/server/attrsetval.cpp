//====================================================================================================================
//
// file :               attrsetval.cpp
//
// description :        C++ source code for the Attribute class set_value() method family.
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// Copyright (C) :      2013,2014,2015
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify it under the terms of the GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with Tango.
// If not, see <http://www.gnu.org/licenses/>.
//
//
//====================================================================================================================

#include <tango.h>
#include <attribute.h>
#include <classattribute.h>
#include <eventsupplier.h>

#include <functional>
#include <algorithm>

#ifdef _TG_WINDOWS_
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif /* _TG_WINDOWS_ */

namespace Tango
{

//+-------------------------------------------------------------------------
//
// method :      Attribute::set_value
//
// description : Set the attribute read value and quality. This method
//               automatically set the date when it has been called
//               This method is overloaded several times for all the
//               supported attribute data type
//
// in :          p_data : The attribute read value
//               x : The attribute x dimension (default is 1)
//               y : The atttribute y dimension (default is 0)
//               release : A flag set to true if memory must be
//                        de-allocated (default is false)
//
//--------------------------------------------------------------------------

void Attribute::set_value(Tango::DevShort *p_data,long x,long y,bool release)
{

//
// Throw exception if type is not correct
//

	if (data_type != Tango::DEV_SHORT &&
		data_type != Tango::DEV_ENUM)
	{
		delete_data_if_needed(p_data,release);

		std::stringstream ss;
		ss << "Invalid data type for attribute " << name;

		TANGO_THROW_EXCEPTION(API_AttrOptProp, ss.str());
	}

//
// Check that data size is less than the given max
//

	if ((x > max_x) || (y > max_y))
	{
		delete_data_if_needed(p_data,release);

		std::stringstream ss;
		ss << "Data size for attribute " << name << " exceeds given limit";

		TANGO_THROW_EXCEPTION(API_AttrOptProp, ss.str());
	}

//
// Compute data size and set default quality to valid.
//

	dim_x = x;
	dim_y = y;
	set_data_size();
	quality = Tango::ATTR_VALID;

//
// Throw exception if pointer is null and data_size != 0
//

	if (data_size != 0)
	{
		CHECK_PTR(p_data,name);
	}

//
// For DevEnum, check that the enum labels are defined. Also check the enum value
//

	if (data_type == DEV_ENUM)
	{
		if (enum_labels.size() == 0)
		{
			delete_data_if_needed(p_data,release);

			std::stringstream ss;
			ss << "Attribute " << name << " data type is enum but no enum labels are defined!";

			TANGO_THROW_EXCEPTION(API_AttrOptProp, ss.str());
		}

		int max_val = enum_labels.size() - 1;
		for (std::uint32_t i = 0;i < data_size;i++)
		{
			if (p_data[i] < 0 || p_data[i] > max_val)
			{
				delete_data_if_needed(p_data,release);

				std::stringstream ss;
				ss << "Wrong value for attribute " << name;
				ss << ". Element " << i << " (value = " << p_data[i] << ") is negative or above the limit defined by the enum (" << max_val << ").";

				TANGO_THROW_EXCEPTION(API_AttrOptProp, ss.str());
			}
		}
	}

//
// If the data is wanted from the DevState command, store it in a sequence.
// If the attribute  has an associated writable attribute, store data in a
// temporary buffer (the write value must be added before the data is sent
// back to the caller)
//

	if (date == false)
	{
		value.sh_seq = new Tango::DevVarShortArray(data_size,data_size,p_data,release);
	}
	else
	{
		if ((is_writ_associated() == true))
		{
			if (data_format == Tango::SCALAR)
			{
				tmp_sh[0] = *p_data;
				delete_data_if_needed(p_data,release);
			}
			else
			{
				value.sh_seq = new Tango::DevVarShortArray(data_size);
				value.sh_seq->length(data_size);
				::memcpy(value.sh_seq->get_buffer(false),p_data,data_size * sizeof(Tango::DevShort));
				if (release == true)
					delete [] p_data;
			}
		}
		else
		{
			if ((data_format == Tango::SCALAR) && (release == true))
			{
				Tango::DevShort *tmp_ptr = new Tango::DevShort[1];
				*tmp_ptr = *p_data;
				value.sh_seq = new Tango::DevVarShortArray(data_size,data_size,tmp_ptr,release);
				if (is_fwd_att() == true)
					delete [] p_data;
				else
					delete p_data;
			}
			else
				value.sh_seq = new Tango::DevVarShortArray(data_size,data_size,p_data,release);
		}
	}
	value_flag = true;

//
// Reset alarm flags
//

	alarm.reset();

//
// Get time
//

	set_time();
}

void Attribute::set_value(Tango::DevString *p_data_str,Tango::DevUChar *p_data,long size,bool release)
{
	if (p_data_str == NULL || p_data == NULL)
	{
		TangoSys_OMemStream o;
		o << "Data pointer for attribute " << name << " is NULL!" << std::ends;
		TANGO_THROW_EXCEPTION(API_AttrOptProp, o.str());
	}

	if (release == false)
	{
		enc_help.encoded_format = Tango::string_dup(*p_data_str);
		enc_help.encoded_data.replace(size,size,p_data,false);

		set_value(&enc_help);
	}
	else
	{
		DevEncoded *enc_ptr = new DevEncoded;
		enc_ptr->encoded_format = Tango::string_dup(*p_data_str);
		delete [] *p_data_str;
		enc_ptr->encoded_data.replace(size,size,p_data,true);

		set_value(enc_ptr,1,0,true);
	}
}

void Attribute::set_value(Tango::EncodedAttribute *attr)
{
	CHECK_PTR(attr,name);

	Tango::DevString *f    = attr->get_format();
	Tango::DevUChar  *d    = attr->get_data();
	long              size = attr->get_size();

	if( *f==NULL )
	{
		TangoSys_OMemStream o;
		o << "DevEncoded format for attribute " << name << " not specified" << std::ends;
		TANGO_THROW_EXCEPTION(API_AttrOptProp, o.str());
	}

	if( size==0 || !d )
	{
		TangoSys_OMemStream o;
		o << "DevEncoded data for attribute " << name << " not specified" << std::ends;
		TANGO_THROW_EXCEPTION(API_AttrOptProp, o.str());
	}

	set_value(f,d,size,false);

	if (attr->get_exclusion() == true)
	{
		set_user_attr_mutex(attr->get_mutex());
	}
}

//---------------------------------------------------------------------------

void Attribute::set_value_date_quality(Tango::DevString *p_data_str,Tango::DevUChar *p_data,long size,time_t t,
				    Tango::AttrQuality qual,
				    bool release)
{
	set_value(p_data_str,p_data,size,release);
	set_quality(qual,false);
	set_date(t);
}

#ifdef _TG_WINDOWS_
void Attribute::set_value_date_quality(Tango::DevString *p_data_str,Tango::DevUChar *p_data,long size,struct _timeb &t,
				    Tango::AttrQuality qual,
				    bool release)
{
	set_value(p_data_str,p_data,size,release);
	set_quality(qual,false);
	set_date(t);
}
#else
void Attribute::set_value_date_quality(Tango::DevString *p_data_str,Tango::DevUChar *p_data,long size,struct timeval &t,
				    Tango::AttrQuality qual,
				    bool release)
{
	set_value(p_data_str,p_data,size,release);
	set_quality(qual,false);
	set_date(t);
}
#endif
} // End of Tango namespace
