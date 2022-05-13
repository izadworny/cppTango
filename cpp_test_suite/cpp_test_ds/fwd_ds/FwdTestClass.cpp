/*----- PROTECTED REGION ID(FwdTestClass.cpp) ENABLED START -----*/
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL$";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
//=============================================================================
//
// file :        FwdTestClass.cpp
//
// description : C++ source for the FwdTestClass.
//               A singleton class derived from DeviceClass.
//               It implements the command and attribute list
//               and all properties and methods required
//               by the FwdTest once per process.
//
// project :
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
//
//
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#include <FwdTestClass.h>

/*----- PROTECTED REGION END -----*/	//	FwdTestClass.cpp

//-------------------------------------------------------------------
/**
 *	Create FwdTestClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_FwdTest_class(const char *name) {
		return FwdTest_ns::FwdTestClass::init(name);
	}
}

namespace FwdTest_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
FwdTestClass *FwdTestClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		FwdTestClass::FwdTestClass(std::string &s)
 * description : 	constructor for the FwdTestClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
FwdTestClass::FwdTestClass(std::string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering FwdTestClass constructor" << std::endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(FwdTestClass::constructor) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::constructor

	cout2 << "Leaving FwdTestClass constructor" << std::endl;
}

//--------------------------------------------------------
/**
 * method : 		FwdTestClass::~FwdTestClass()
 * description : 	destructor for the FwdTestClass
 */
//--------------------------------------------------------
FwdTestClass::~FwdTestClass()
{
	/*----- PROTECTED REGION ID(FwdTestClass::destructor) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		FwdTestClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
FwdTestClass *FwdTestClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			std::string s(name);
			_instance = new FwdTestClass(s);
		}
		catch (std::bad_alloc &)
		{
			throw;
		}
	}
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		FwdTestClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
FwdTestClass *FwdTestClass::instance()
{
	if (_instance == NULL)
	{
		std::cerr << "Class is not initialised !!" << std::endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================

//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum FwdTestClass::get_class_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum FwdTestClass::get_default_device_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum FwdTestClass::get_default_class_property(std::string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void FwdTestClass::set_default_property()
{
	std::string	prop_name;
	std::string	prop_desc;
	std::string	prop_def;
	std::vector<std::string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
}

//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void FwdTestClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	std::string	classname = get_name();
	std::string	header;
	std::string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	std::string	str_title("");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	std::vector<std::string>	str_desc;
	str_desc.push_back("");
	description << str_desc;
	data.push_back(description);

	//	put cvs or svn location
	std::string	filename("FwdTest");
	filename += "Class.cpp";

	// check for cvs information
	std::string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!= std::string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			std::string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!= std::string::npos && start>0)
			{
				std::string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!= std::string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}

	// check for svn information
	else
	{
		std::string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!= std::string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				std::string	strloc = src_path.substr(start, (end-start));

				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag

	// CVS tag
	std::string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	std::string	endstr(" $");

	end   = tagname.find(endstr);
	if (end!= std::string::npos && end>start)
	{
		std::string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}

	// SVN tag
	std::string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();

	end   = svnpath.find(endstr);
	if (end!= std::string::npos && end>start)
	{
		std::string	strloc = svnpath.substr(start, end-start);

		std::string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!= std::string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			std::string	strtag = strloc.substr(start, end-start-1);

			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	std::string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	std::vector<std::string> inheritance;
	inheritance.push_back("Tango::Device_4Impl");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void FwdTestClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(FwdTestClass::device_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << std::endl;
		device_list.push_back(new FwdTest(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		FwdTest *dev = static_cast<FwdTest *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(FwdTestClass::device_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void FwdTestClass::attribute_factory(std::vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(FwdTestClass::attribute_factory_before) ENABLED START -----*/

	FwdAttrScaSh	*att1 = new FwdAttrScaSh("fwd_short_rw");
	Tango::UserDefaultFwdAttrProp	att1_prop;
	att1_prop.set_label("Gasp a fwd attribute");
	att1->set_default_properties(att1_prop);
	att_list.push_back(att1);

	FwdAttrScaStr	*att2 = new FwdAttrScaStr("fwd_string_w");
	att_list.push_back(att2);

	FwdAttrSpecDb	*att3 = new FwdAttrSpecDb("fwd_spec_double");
	att_list.push_back(att3);

	FwdAttrImaStr	*att4 = new FwdAttrImaStr("fwd_ima_string_rw");
	att_list.push_back(att4);

	FwdAttrScaSta	*att5 = new FwdAttrScaSta("fwd_state");
	att_list.push_back(att5);

	FwdAttrScaStr	*att6 = new FwdAttrScaStr("fwd_string_rw");
	att_list.push_back(att6);

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::attribute_factory_before
	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(FwdTestClass::attribute_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void FwdTestClass::command_factory()
{
	/*----- PROTECTED REGION ID(FwdTestClass::command_factory_before) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::command_factory_before


	/*----- PROTECTED REGION ID(FwdTestClass::command_factory_after) ENABLED START -----*/

	//	Add your own code

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		FwdTestClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void FwdTestClass::create_static_attribute_list(std::vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		std::string att_name(att_list[i]->get_name());
		std::transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << std::endl;

	/*----- PROTECTED REGION ID(FwdTestClass::create_static_att_list) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		FwdTestClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void FwdTestClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, std::vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((std::string)(*devlist_ptr)[i]).c_str());
		FwdTest *dev = static_cast<FwdTest *> (dev_impl);

		std::vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		std::vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			std::string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			std::vector<std::string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				cout2 << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << std::endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(FwdTestClass::erase_dynamic_attributes) ENABLED START -----*/

	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : FwdTestClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *FwdTestClass::get_attr_object_by_name(std::vector<Tango::Attr *> &att_list, std::string attname)
{
	std::vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; it++)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(FwdTestClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	FwdTestClass::Additional Methods
} //	namespace
