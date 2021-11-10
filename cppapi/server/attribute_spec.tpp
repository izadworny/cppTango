//+==================================================================================================================
//
// file :               attribute_spec.tpp
//
// description :        C++ source code for the Attribute class template methods when they are specialized
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
// Tango is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version 3 of the License, or
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
//-=================================================================================================================

#ifndef _ATTRIBUTE_SPEC_TPP
#define _ATTRIBUTE_SPEC_TPP

//
// These methods are in a sepearte files because I did not find a way to explicitely instantiate specialized
// templates. Therefore, for template management we have three files:
// 1 - A file with explicit template instanciation (templ_inst.cpp)
// 2 - A file with template method definition (attribute.tpp for Attribute class)
// 3 - A file with template specialization defnition (attribute_spec.tpp (this file))
//
// We did this in order to have explicit instanciation of templates method except for specialized templates
// for which we have instanciation following the inclusion model
// See C++ template book chapter 6
//

namespace Tango
{

//+-------------------------------------------------------------------------------------------------------------------
//
// method :
//		Attribute::set_min_alarm()
//		Template specialization for data type DevEncoded and string
//
// description :
//		Sets minimum alarm attribute property. Throws exception in case the data type of provided property does not
//		match the attribute data type
//
// args :
// 		in :
//			- new_min_alarm : The minimum alarm property to be set
//
//-------------------------------------------------------------------------------------------------------------------

template <>
inline void Attribute::set_min_alarm(const Tango::DevEncoded &)
{
	std::string err_msg = "Attribute properties cannot be set with Tango::DevEncoded data type";
	TANGO_THROW_EXCEPTION(API_MethodArgument, err_msg.c_str());
}

template <>
inline void Attribute::set_min_alarm(const std::string &new_min_alarm_str)
{
	if((data_type == Tango::DEV_STRING) ||
		(data_type == Tango::DEV_BOOLEAN) ||
		(data_type == Tango::DEV_STATE))
		throw_err_data_type("min_alarm",d_name,"Attribute::set_min_alarm()");

	std::string min_alarm_str_tmp = new_min_alarm_str;
	std::string dev_name = d_name;

	Tango::DeviceClass *dev_class = get_att_device_class(d_name);
	Tango::MultiClassAttribute *mca = dev_class->get_class_attr();
	Tango::Attr &att = mca->get_attr(name);
	std::vector<AttrProperty> &def_user_prop = att.get_user_default_properties();
	std::vector<AttrProperty> &def_class_prop = att.get_class_properties();

	size_t nb_class = def_class_prop.size();
	size_t nb_user = def_user_prop.size();

	std::string usr_def_val;
	std::string class_def_val;
	bool user_defaults = false;
	bool class_defaults = false;

    user_defaults = prop_in_list("min_alarm",usr_def_val,nb_user,def_user_prop);

    class_defaults = prop_in_list("min_alarm",class_def_val,nb_class,def_class_prop);

	bool set_value = true;

    if (class_defaults)
    {
		if(TG_strcasecmp(new_min_alarm_str.c_str(),AlrmValueNotSpec) == 0)
		{
		    set_value = false;

		    avns_in_db("min_alarm",dev_name);
            avns_in_att(MIN_ALARM);
		}
		else if ((TG_strcasecmp(new_min_alarm_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_min_alarm_str.c_str(),class_def_val.c_str()) == 0))
        {
            min_alarm_str_tmp = class_def_val;
        }
		else if (strlen(new_min_alarm_str.c_str()) == 0)
		{
		    if (user_defaults)
		    {
                min_alarm_str_tmp = usr_def_val;
		    }
		    else
		    {
		        set_value = false;

                avns_in_db("min_alarm",dev_name);
                avns_in_att(MIN_ALARM);
		    }
		}
    }
	else if(user_defaults)
	{
		if(TG_strcasecmp(new_min_alarm_str.c_str(),AlrmValueNotSpec) == 0)
		{
			set_value = false;

            avns_in_db("min_alarm",dev_name);
            avns_in_att(MIN_ALARM);
		}
		else if ((TG_strcasecmp(new_min_alarm_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_min_alarm_str.c_str(),usr_def_val.c_str()) == 0) ||
				(strlen(new_min_alarm_str.c_str()) == 0))
			min_alarm_str_tmp = usr_def_val;
	}
	else
	{
		if ((TG_strcasecmp(new_min_alarm_str.c_str(),AlrmValueNotSpec) == 0) ||
				(TG_strcasecmp(new_min_alarm_str.c_str(),NotANumber) == 0) ||
				(strlen(new_min_alarm_str.c_str()) == 0))
		{
			set_value = false;

            avns_in_db("min_alarm",dev_name);
            avns_in_att(MIN_ALARM);
		}
	}

	if(set_value)
	{
		if ((data_type != Tango::DEV_STRING) &&
		    (data_type != Tango::DEV_BOOLEAN) &&
		    (data_type != Tango::DEV_STATE) &&
			(data_type != Tango::DEV_ENUM))
		{
			double db;
			float fl;

			TangoSys_MemStream str;
			str.precision(TANGO_FLOAT_PRECISION);
			str << min_alarm_str_tmp;
			switch (data_type)
			{
			case Tango::DEV_SHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				set_min_alarm((DevShort)db);
				break;

			case Tango::DEV_LONG:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				set_min_alarm((DevLong)db);
				break;

			case Tango::DEV_LONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				set_min_alarm((DevLong64)db);
				break;

			case Tango::DEV_DOUBLE:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				set_min_alarm(db);
				break;

			case Tango::DEV_FLOAT:
				if (!(str >> fl && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				set_min_alarm(fl);
				break;

			case Tango::DEV_USHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				(db < 0.0) ? set_min_alarm((DevUShort)(-db)) : set_min_alarm((DevUShort)db);
				break;

			case Tango::DEV_UCHAR:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				(db < 0.0) ? set_min_alarm((DevUChar)(-db)) : set_min_alarm((DevUChar)db);
				break;

			case Tango::DEV_ULONG:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				(db < 0.0) ? set_min_alarm((DevULong)(-db)) : set_min_alarm((DevULong)db);
				break;

			case Tango::DEV_ULONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				(db < 0.0) ? set_min_alarm((DevULong64)(-db)) : set_min_alarm((DevULong64)db);
				break;

			case Tango::DEV_ENCODED:
				if (!(str >> db && str.eof()))
					throw_err_format("min_alarm",dev_name,"Attribute::set_min_alarm()");
				(db < 0.0) ? set_min_alarm((DevUChar)(-db)) : set_min_alarm((DevUChar)db);
				break;
			}
		}
		else
			throw_err_data_type("min_alarm",dev_name,"Attribute::set_min_alarm()");
	}
}

//+-------------------------------------------------------------------------------------------------------------------
//
// method :
//		Attribute::set_max_alarm()
//		Template specialization for data type DevEncoded and string
//
// description :
//		Sets maximum alarm attribute property. Throws exception in case the data type of provided property does not
//		match the attribute data type
//
// args :
// 		in :
//			- new_min_alarm : The minimum alarm property to be set
//
//-------------------------------------------------------------------------------------------------------------------

template <>
inline void Attribute::set_max_alarm(const Tango::DevEncoded &)
{
	std::string err_msg = "Attribute properties cannot be set with Tango::DevEncoded data type";
	TANGO_THROW_EXCEPTION(API_MethodArgument, err_msg.c_str());
}

template <>
inline void Attribute::set_max_alarm(const std::string &new_max_alarm_str)
{
	if((data_type == Tango::DEV_STRING) ||
		(data_type == Tango::DEV_BOOLEAN) ||
		(data_type == Tango::DEV_STATE))
		throw_err_data_type("max_alarm",d_name,"Attribute::set_max_alarm()");

	std::string max_alarm_str_tmp = new_max_alarm_str;
	std::string dev_name = d_name;

	Tango::DeviceClass *dev_class = get_att_device_class(d_name);
	Tango::MultiClassAttribute *mca = dev_class->get_class_attr();
	Tango::Attr &att = mca->get_attr(name);
	std::vector<AttrProperty> &def_user_prop = att.get_user_default_properties();
	std::vector<AttrProperty> &def_class_prop = att.get_class_properties();

	size_t nb_class = def_class_prop.size();
	size_t nb_user = def_user_prop.size();

	std::string usr_def_val;
	std::string class_def_val;
	bool user_defaults = false;
	bool class_defaults = false;

    user_defaults = prop_in_list("max_alarm",usr_def_val,nb_user,def_user_prop);

    class_defaults = prop_in_list("max_alarm",class_def_val,nb_class,def_class_prop);

	bool set_value = true;

    if (class_defaults)
    {
		if(TG_strcasecmp(new_max_alarm_str.c_str(),AlrmValueNotSpec) == 0)
		{
		    set_value = false;

		    avns_in_db("max_alarm",dev_name);
            avns_in_att(MAX_ALARM);
		}
		else if ((TG_strcasecmp(new_max_alarm_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_max_alarm_str.c_str(),class_def_val.c_str()) == 0))
        {
            max_alarm_str_tmp = class_def_val;
        }
		else if (strlen(new_max_alarm_str.c_str()) == 0)
		{
		    if (user_defaults)
		    {
                max_alarm_str_tmp = usr_def_val;
		    }
		    else
		    {
		        set_value = false;

                avns_in_db("max_alarm",dev_name);
                avns_in_att(MAX_ALARM);
		    }
		}
    }
	else if(user_defaults)
	{
		if(TG_strcasecmp(new_max_alarm_str.c_str(),AlrmValueNotSpec) == 0)
		{
			set_value = false;

            avns_in_db("max_alarm",dev_name);
            avns_in_att(MAX_ALARM);
		}
		else if ((TG_strcasecmp(new_max_alarm_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_max_alarm_str.c_str(),usr_def_val.c_str()) == 0) ||
				(strlen(new_max_alarm_str.c_str()) == 0))
			max_alarm_str_tmp = usr_def_val;
	}
	else
	{
		if ((TG_strcasecmp(new_max_alarm_str.c_str(),AlrmValueNotSpec) == 0) ||
				(TG_strcasecmp(new_max_alarm_str.c_str(),NotANumber) == 0) ||
				(strlen(new_max_alarm_str.c_str()) == 0))
		{
			set_value = false;

            avns_in_db("max_alarm",dev_name);
            avns_in_att(MAX_ALARM);
		}
	}

	if(set_value)
	{
		if ((data_type != Tango::DEV_STRING) &&
		    (data_type != Tango::DEV_BOOLEAN) &&
		    (data_type != Tango::DEV_STATE) &&
			(data_type != Tango::DEV_ENUM))
		{
			double db;
			float fl;

			TangoSys_MemStream str;
			str.precision(TANGO_FLOAT_PRECISION);
			str << max_alarm_str_tmp;
			switch (data_type)
			{
			case Tango::DEV_SHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				set_max_alarm((DevShort)db);
				break;

			case Tango::DEV_LONG:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				set_max_alarm((DevLong)db);
				break;

			case Tango::DEV_LONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				set_max_alarm((DevLong64)db);
				break;

			case Tango::DEV_DOUBLE:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				set_max_alarm(db);
				break;

			case Tango::DEV_FLOAT:
				if (!(str >> fl && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				set_max_alarm(fl);
				break;

			case Tango::DEV_USHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				(db < 0.0) ? set_max_alarm((DevUShort)(-db)) : set_max_alarm((DevUShort)db);
				break;

			case Tango::DEV_UCHAR:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				(db < 0.0) ? set_max_alarm((DevUChar)(-db)) : set_max_alarm((DevUChar)db);
				break;

			case Tango::DEV_ULONG:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				(db < 0.0) ? set_max_alarm((DevULong)(-db)) : set_max_alarm((DevULong)db);
				break;

			case Tango::DEV_ULONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				(db < 0.0) ? set_max_alarm((DevULong64)(-db)) : set_max_alarm((DevULong64)db);
				break;

			case Tango::DEV_ENCODED:
				if (!(str >> db && str.eof()))
					throw_err_format("max_alarm",dev_name,"Attribute::set_max_alarm()");
				(db < 0.0) ? set_max_alarm((DevUChar)(-db)) : set_max_alarm((DevUChar)db);
				break;
			}
		}
		else
			throw_err_data_type("max_alarm",dev_name,"Attribute::set_max_alarm()");
	}
}

//+-------------------------------------------------------------------------------------------------------------------
//
// method :
//		Attribute::set_min_warning()
//		Template specialization for data type DevEncoded and string
//
// description :
//		Sets minimum warning attribute property. Throws exception in case the data type of provided property does not
//		match the attribute data type
//
// args :
// 		in :
//			- new_min_alarm : The minimum alarm property to be set
//
//-------------------------------------------------------------------------------------------------------------------

template <>
inline void Attribute::set_min_warning(const Tango::DevEncoded &)
{
	std::string err_msg = "Attribute properties cannot be set with Tango::DevEncoded data type";
	TANGO_THROW_EXCEPTION(API_MethodArgument, err_msg.c_str());
}

template <>
inline void Attribute::set_min_warning(const std::string &new_min_warning_str)
{
	if((data_type == Tango::DEV_STRING) ||
		(data_type == Tango::DEV_BOOLEAN) ||
		(data_type == Tango::DEV_STATE))
		throw_err_data_type("min_warning",d_name,"Attribute::set_min_warning()");

	std::string min_warning_str_tmp = new_min_warning_str;
	std::string dev_name = d_name;

	Tango::DeviceClass *dev_class = get_att_device_class(d_name);
	Tango::MultiClassAttribute *mca = dev_class->get_class_attr();
	Tango::Attr &att = mca->get_attr(name);
	std::vector<AttrProperty> &def_user_prop = att.get_user_default_properties();
	std::vector<AttrProperty> &def_class_prop = att.get_class_properties();

	size_t nb_class = def_class_prop.size();
	size_t nb_user = def_user_prop.size();

	std::string usr_def_val;
	std::string class_def_val;
	bool user_defaults = false;
	bool class_defaults = false;

    user_defaults = prop_in_list("min_warning",usr_def_val,nb_user,def_user_prop);

    class_defaults = prop_in_list("min_warning",class_def_val,nb_class,def_class_prop);

	bool set_value = true;

    if (class_defaults)
    {
		if(TG_strcasecmp(new_min_warning_str.c_str(),AlrmValueNotSpec) == 0)
		{
		    set_value = false;

		    avns_in_db("min_warning",dev_name);
            avns_in_att(MIN_WARNING);
		}
		else if ((TG_strcasecmp(new_min_warning_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_min_warning_str.c_str(),class_def_val.c_str()) == 0))
        {
            min_warning_str_tmp = class_def_val;
        }
		else if (strlen(new_min_warning_str.c_str()) == 0)
		{
		    if (user_defaults)
		    {
                min_warning_str_tmp = usr_def_val;
		    }
		    else
		    {
		        set_value = false;

                avns_in_db("min_warning",dev_name);
                avns_in_att(MIN_WARNING);
		    }
		}
    }
	else if(user_defaults)
	{
		if(TG_strcasecmp(new_min_warning_str.c_str(),AlrmValueNotSpec) == 0)
		{
			set_value = false;

            avns_in_db("min_warning",dev_name);
            avns_in_att(MIN_WARNING);
		}
		else if ((TG_strcasecmp(new_min_warning_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_min_warning_str.c_str(),usr_def_val.c_str()) == 0) ||
				(strlen(new_min_warning_str.c_str()) == 0))
			min_warning_str_tmp = usr_def_val;
	}
	else
	{
		if ((TG_strcasecmp(new_min_warning_str.c_str(),AlrmValueNotSpec) == 0) ||
				(TG_strcasecmp(new_min_warning_str.c_str(),NotANumber) == 0) ||
				(strlen(new_min_warning_str.c_str()) == 0))
		{
			set_value = false;

            avns_in_db("min_warning",dev_name);
            avns_in_att(MIN_WARNING);
		}
	}

	if(set_value)
	{
		if ((data_type != Tango::DEV_STRING) &&
		    (data_type != Tango::DEV_BOOLEAN) &&
		    (data_type != Tango::DEV_STATE) &&
			(data_type != Tango::DEV_ENUM))
		{
			double db;
			float fl;

			TangoSys_MemStream str;
			str.precision(TANGO_FLOAT_PRECISION);
			str << min_warning_str_tmp;
			switch (data_type)
			{
			case Tango::DEV_SHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				set_min_warning((DevShort)db);
				break;

			case Tango::DEV_LONG:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				set_min_warning((DevLong)db);
				break;

			case Tango::DEV_LONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				set_min_warning((DevLong64)db);
				break;

			case Tango::DEV_DOUBLE:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				set_min_warning(db);
				break;

			case Tango::DEV_FLOAT:
				if (!(str >> fl && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				set_min_warning(fl);
				break;

			case Tango::DEV_USHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				(db < 0.0) ? set_min_warning((DevUShort)(-db)) : set_min_warning((DevUShort)db);
				break;

			case Tango::DEV_UCHAR:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				(db < 0.0) ? set_min_warning((DevUChar)(-db)) : set_min_warning((DevUChar)db);
				break;

			case Tango::DEV_ULONG:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				(db < 0.0) ? set_min_warning((DevULong)(-db)) : set_min_warning((DevULong)db);
				break;

			case Tango::DEV_ULONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				(db < 0.0) ? set_min_warning((DevULong64)(-db)) : set_min_warning((DevULong64)db);
				break;

			case Tango::DEV_ENCODED:
				if (!(str >> db && str.eof()))
					throw_err_format("min_warning",dev_name,"Attribute::set_min_warning()");
				(db < 0.0) ? set_min_warning((DevUChar)(-db)) : set_min_warning((DevUChar)db);
				break;
			}
		}
		else
			throw_err_data_type("min_warning",dev_name,"Attribute::set_min_warning()");
	}
}

//+-------------------------------------------------------------------------------------------------------------------
//
// method :
//		Attribute::set_max_warning()
//		Template specialization for data type DevEncoded and string
//
// description :
//		Sets maximum warning attribute property. Throws exception in case the data type of provided property does not
//		match the attribute data type
//
// args :
// 		in :
//			- new_min_alarm : The minimum alarm property to be set
//
//-------------------------------------------------------------------------------------------------------------------

template <>
inline void Attribute::set_max_warning(const Tango::DevEncoded &)
{
	std::string err_msg = "Attribute properties cannot be set with Tango::DevEncoded data type";
	TANGO_THROW_EXCEPTION(API_MethodArgument, err_msg.c_str());
}

template <>
inline void Attribute::set_max_warning(const std::string &new_max_warning_str)
{
	if((data_type == Tango::DEV_STRING) ||
		(data_type == Tango::DEV_BOOLEAN) ||
		(data_type == Tango::DEV_STATE))
		throw_err_data_type("max_warning",d_name,"Attribute::set_max_warning()");

	std::string max_warning_str_tmp = new_max_warning_str;
	std::string dev_name = d_name;

	Tango::DeviceClass *dev_class = get_att_device_class(d_name);
	Tango::MultiClassAttribute *mca = dev_class->get_class_attr();
	Tango::Attr &att = mca->get_attr(name);
	std::vector<AttrProperty> &def_user_prop = att.get_user_default_properties();
	std::vector<AttrProperty> &def_class_prop = att.get_class_properties();

	size_t nb_class = def_class_prop.size();
	size_t nb_user = def_user_prop.size();

	std::string usr_def_val;
	std::string class_def_val;
	bool user_defaults = false;
	bool class_defaults = false;

    user_defaults = prop_in_list("max_warning",usr_def_val,nb_user,def_user_prop);

    class_defaults = prop_in_list("max_warning",class_def_val,nb_class,def_class_prop);

	bool set_value = true;

    if (class_defaults)
    {
		if(TG_strcasecmp(new_max_warning_str.c_str(),AlrmValueNotSpec) == 0)
		{
		    set_value = false;

		    avns_in_db("max_warning",dev_name);
            avns_in_att(MAX_WARNING);
		}
		else if ((TG_strcasecmp(new_max_warning_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_max_warning_str.c_str(),class_def_val.c_str()) == 0))
        {
            max_warning_str_tmp = class_def_val;
        }
		else if (strlen(new_max_warning_str.c_str()) == 0)
		{
		    if (user_defaults)
		    {
                max_warning_str_tmp = usr_def_val;
		    }
		    else
		    {
		        set_value = false;

                avns_in_db("max_warning",dev_name);
                avns_in_att(MAX_WARNING);
		    }
		}
    }
	else if(user_defaults)
	{
		if(TG_strcasecmp(new_max_warning_str.c_str(),AlrmValueNotSpec) == 0)
		{
			set_value = false;

            avns_in_db("max_warning",dev_name);
            avns_in_att(MAX_WARNING);
		}
		else if ((TG_strcasecmp(new_max_warning_str.c_str(),NotANumber) == 0) ||
				(TG_strcasecmp(new_max_warning_str.c_str(),usr_def_val.c_str()) == 0) ||
				(strlen(new_max_warning_str.c_str()) == 0))
			max_warning_str_tmp = usr_def_val;
	}
	else
	{
		if ((TG_strcasecmp(new_max_warning_str.c_str(),AlrmValueNotSpec) == 0) ||
				(TG_strcasecmp(new_max_warning_str.c_str(),NotANumber) == 0) ||
				(strlen(new_max_warning_str.c_str()) == 0))
		{
			set_value = false;

            avns_in_db("max_warning",dev_name);
            avns_in_att(MAX_WARNING);
		}
	}

	if(set_value)
	{
		if ((data_type != Tango::DEV_STRING) &&
		    (data_type != Tango::DEV_BOOLEAN) &&
		    (data_type != Tango::DEV_STATE) &&
			(data_type != Tango::DEV_ENUM))
		{
			double db;
			float fl;

			TangoSys_MemStream str;
			str.precision(TANGO_FLOAT_PRECISION);
			str << max_warning_str_tmp;
			switch (data_type)
			{
			case Tango::DEV_SHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				set_max_warning((DevShort)db);
				break;

			case Tango::DEV_LONG:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				set_max_warning((DevLong)db);
				break;

			case Tango::DEV_LONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				set_max_warning((DevLong64)db);
				break;

			case Tango::DEV_DOUBLE:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				set_max_warning(db);
				break;

			case Tango::DEV_FLOAT:
				if (!(str >> fl && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				set_max_warning(fl);
				break;

			case Tango::DEV_USHORT:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				(db < 0.0) ? set_max_warning((DevUShort)(-db)) : set_max_warning((DevUShort)db);
				break;

			case Tango::DEV_UCHAR:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				(db < 0.0) ? set_max_warning((DevUChar)(-db)) : set_max_warning((DevUChar)db);
				break;

			case Tango::DEV_ULONG:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				(db < 0.0) ? set_max_warning((DevULong)(-db)) : set_max_warning((DevULong)db);
				break;

			case Tango::DEV_ULONG64:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				(db < 0.0) ? set_max_warning((DevULong64)(-db)) : set_max_warning((DevULong64)db);
				break;

			case Tango::DEV_ENCODED:
				if (!(str >> db && str.eof()))
					throw_err_format("max_warning",dev_name,"Attribute::set_max_warning()");
				(db < 0.0) ? set_max_warning((DevUChar)(-db)) : set_max_warning((DevUChar)db);
				break;
			}
		}
		else
			throw_err_data_type("max_warning",dev_name,"Attribute::set_max_warning()");
	}
}



template<>
struct tango_type_traits<Tango::DevShort>
{
    using array_type = Tango::DevVarShortArray;
    using type = Tango::DevShort;
    static constexpr CmdArgType type_value = DEV_SHORT; 
};
template<>
struct tango_type_traits<Tango::DevUShort>
{
    using array_type = Tango::DevVarUShortArray;
    using type = Tango::DevUShort;
    static constexpr CmdArgType type_value = DEV_USHORT; 
};
template<>
struct tango_type_traits<Tango::DevLong>
{
    using array_type = Tango::DevVarLongArray;
    using type = Tango::DevLong;
    static constexpr CmdArgType type_value = DEV_LONG; 
};
template<>
struct tango_type_traits<Tango::DevULong>
{
    using array_type = Tango::DevVarULongArray;
    using type = Tango::DevULong;
    static constexpr CmdArgType type_value = DEV_ULONG; 
};
template<>
struct tango_type_traits<Tango::DevLong64>
{
    using array_type = Tango::DevVarLong64Array;
    using type = Tango::DevLong64;
    static constexpr CmdArgType type_value = DEV_LONG64; 
};
template<>
struct tango_type_traits<Tango::DevULong64>
{
    using array_type = Tango::DevVarULong64Array;
    using type = Tango::DevULong64;
    static constexpr CmdArgType type_value = DEV_ULONG64; 
};
template<>
struct tango_type_traits<Tango::DevDouble>
{
    using array_type = Tango::DevVarDoubleArray;
    using type = Tango::DevDouble;
    static constexpr CmdArgType type_value = DEV_DOUBLE; 
};
template<>
struct tango_type_traits<Tango::DevString>
{
    using array_type = Tango::DevVarStringArray;
    using type = Tango::DevString;
    static constexpr CmdArgType type_value = DEV_STRING; 
};
template<>
struct tango_type_traits<Tango::DevBoolean>
{
    using array_type = Tango::DevVarBooleanArray;
    using type = Tango::DevBoolean;
    static constexpr CmdArgType type_value = DEV_BOOLEAN; 
};
template<>
struct tango_type_traits<Tango::DevFloat>
{
    using array_type = Tango::DevVarFloatArray;
    using type = Tango::DevFloat;
    static constexpr CmdArgType type_value = DEV_FLOAT; 
};
template<>
struct tango_type_traits<Tango::DevUChar>
{
    using array_type = Tango::DevVarUCharArray;
    using type = Tango::DevUChar;
    static constexpr CmdArgType type_value = DEV_UCHAR; 
};
template<>
struct tango_type_traits<Tango::DevState>
{
    using array_type = Tango::DevVarStateArray;
    using type = Tango::DevState;
    static constexpr CmdArgType type_value = DEV_STATE; 
};
template<>
struct tango_type_traits<Tango::DevEncoded>
{
    using array_type = Tango::DevVarEncodedArray;
    using type = Tango::DevEncoded;
    static constexpr CmdArgType type_value = DEV_ENCODED; 
};
template<>
struct tango_type_traits<Tango::DevVarShortArray>
{
    using array_type = Tango::DevVarShortArray;
    using type = Tango::DevVarShortArray;
    static constexpr CmdArgType type_value = DEVVAR_SHORTARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarUShortArray>
{
    using array_type = Tango::DevVarUShortArray;
    using type = Tango::DevVarUShortArray;
    static constexpr CmdArgType type_value = DEVVAR_USHORTARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarLongArray>
{
    using array_type = Tango::DevVarLongArray;
    using type = Tango::DevVarLongArray;
    static constexpr CmdArgType type_value = DEVVAR_LONGARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarULongArray>
{
    using array_type = Tango::DevVarULongArray;
    using type = Tango::DevVarULongArray;
    static constexpr CmdArgType type_value = DEVVAR_ULONGARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarLong64Array>
{
    using array_type = Tango::DevVarLong64Array;
    using type = Tango::DevVarLong64Array;
    static constexpr CmdArgType type_value = DEVVAR_LONG64ARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarULong64Array>
{
    using array_type = Tango::DevVarULong64Array;
    using type = Tango::DevVarULong64Array;
    static constexpr CmdArgType type_value = DEVVAR_ULONG64ARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarCharArray>
{
    using array_type = Tango::DevVarCharArray;
    using type = Tango::DevVarCharArray;
    static constexpr CmdArgType type_value = DEVVAR_CHARARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarFloatArray>
{
    using array_type = Tango::DevVarFloatArray;
    using type = Tango::DevVarFloatArray;
    static constexpr CmdArgType type_value = DEVVAR_FLOATARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarDoubleArray>
{
    using array_type = Tango::DevVarDoubleArray;
    using type = Tango::DevVarDoubleArray;
    static constexpr CmdArgType type_value = DEVVAR_DOUBLEARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarStringArray>
{
    using array_type = Tango::DevVarStringArray;
    using type = Tango::DevVarStringArray;
    static constexpr CmdArgType type_value = DEVVAR_STRINGARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarBooleanArray>
{
    using array_type = Tango::DevVarBooleanArray;
    using type = Tango::DevVarBooleanArray;
    static constexpr CmdArgType type_value = DEVVAR_BOOLEANARRAY; 
};
template<>
struct tango_type_traits<Tango::DevVarStateArray>
{
    using array_type = Tango::DevVarStateArray;
    using type = Tango::DevVarStateArray;
    static constexpr CmdArgType type_value = DEVVAR_STATEARRAY; 
};

template<>
inline Tango::DevVarULong64Array** Attribute::get_value_storage()
{
    return &value.ulg64_seq;
}

template<>
inline Tango::DevVarShortArray** Attribute::get_value_storage()
{
    return &value.sh_seq;
}

template<>
inline Tango::DevVarDoubleArray** Attribute::get_value_storage()
{
    return &value.db_seq;
}

template<>
inline Tango::DevVarStringArray** Attribute::get_value_storage()
{
    return &value.str_seq;
}

template<>
inline Tango::DevVarFloatArray** Attribute::get_value_storage()
{
    return &value.fl_seq;
}

template<>
inline Tango::DevVarBooleanArray** Attribute::get_value_storage()
{
    return &value.boo_seq;
}

template<>
inline Tango::DevVarUShortArray** Attribute::get_value_storage()
{
    return &value.ush_seq;
}

template<>
inline Tango::DevVarCharArray** Attribute::get_value_storage()
{
    return &value.cha_seq;
}

template<>
inline Tango::DevVarLong64Array** Attribute::get_value_storage()
{
    return &value.lg64_seq;
}

template<>
inline Tango::DevVarLongArray** Attribute::get_value_storage()
{
    return &value.lg_seq;
}

template<>
inline Tango::DevVarULongArray** Attribute::get_value_storage()
{
    return &value.ulg_seq;
}

template<>
inline Tango::DevVarStateArray** Attribute::get_value_storage()
{
    return &value.state_seq;
}

template<>
inline Tango::DevVarEncodedArray** Attribute::get_value_storage()
{
    return &value.enc_seq;
}

template<>
inline Tango::DevULong64 (&Attribute::get_tmp_storage())[2]
{
    return tmp_ulo64;
}

template<>
inline Tango::DevLong64 (&Attribute::get_tmp_storage())[2]
{
    return tmp_lo64;
}

template<>
inline Tango::DevULong (&Attribute::get_tmp_storage())[2]
{
    return tmp_ulo;
}

template<>
inline Tango::DevState (&Attribute::get_tmp_storage())[2]
{
    return tmp_state;
}

template<>
inline Tango::DevShort (&Attribute::get_tmp_storage())[2]
{
    return tmp_sh;
}

template<>
inline Tango::DevLong (&Attribute::get_tmp_storage())[2]
{
    return tmp_lo;
}

template<>
inline Tango::DevFloat (&Attribute::get_tmp_storage())[2]
{
    return tmp_fl;
}

template<>
inline Tango::DevDouble (&Attribute::get_tmp_storage())[2]
{
    return tmp_db;
}

template<>
inline Tango::DevString (&Attribute::get_tmp_storage())[2]
{
    return tmp_str;
}

template<>
inline Tango::DevUShort (&Attribute::get_tmp_storage())[2]
{
    return tmp_ush;
}

template<>
inline Tango::DevBoolean (&Attribute::get_tmp_storage())[2]
{
    return tmp_boo;
}

template<>
inline Tango::DevUChar (&Attribute::get_tmp_storage())[2]
{
    return tmp_cha;
}

template<>
inline Tango::DevEncoded (&Attribute::get_tmp_storage())[2]
{
    return tmp_enc;
}

} // End of Tango namespace
#endif // _ATTRIBUTE_SPEC_TPP
