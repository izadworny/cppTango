static const char *RcsId = "$Header$";

//+============================================================================
//
// file :               attrdesc.cpp
//
// description :        C++ source code for the BlackBoxElt and BlackBox
//			classes. These classes are used to implement the 
//			tango device server black box. There is one
//			black box for each Tango device. This black box
//			keeps info. on all the activities on a device.
//			A client is able to retrieve these data via a Device
//			attribute
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// $Revision$
//
// $Log$
// Revision 2.7  2002/12/16 10:14:41  taurel
// - New method get_device_list() in Util class
// - Util::get_class_list takes DServer device into account
// - Util::get_device_by_name() takes DServer device into account
// - Util::get_device_list_by_class() takes DServer device into account
// - New parameter to the attribute::set_value() method to enable CORBA to free
// memory allocated for the attribute
//
// Revision 2.6  2002/10/17 07:43:05  taurel
// Fix bug in history stored by the polling thread :
// - We need one copy of the attribute data to build an history!!! It is true
// also for command which return data created by the DeviceImpl::create_xxx
// methods. Chnage in pollring.cpp/pollring.h/dserverpoll.cpp/pollobj.cpp
// and pollobj.h
//
// Revision 2.5  2002/10/15 11:27:18  taurel
// Fix bugs in device.cpp file :
// - Protect the state and status CORBA attribute with the device monitor
// Add the "TgLibVers" string as a #define in tango_config.h
//
// Revision 2.4  2002/08/12 15:06:53  taurel
// Several big fixes and changes
//   - Remove HP-UX specific code
//   - Fix bug in polling alogorithm which cause the thread to enter an infinite
//     loop (pollthread.cpp)
//   - For bug for Win32 device when trying to set attribute config
//     (attribute.cpp)
//
// Revision 2.3  2002/07/02 15:22:23  taurel
// Miscellaneous small changes/bug fixes for Tango CPP release 2.1.0
//     - classes reference documentation now generated using doxygen instead of doc++
//     - A little file added to the library which summarizes version number.
//       The RCS/CVS "ident" command will now tells you that release library x.y.z is composed
//       by C++ client classes set release a.b and C++ server classes set release c.d
//     - Fix incorrect field setting for DevFailed exception re-thrown from a CORBA exception
//     - It's now not possible to poll the Init command
//     - It's now possible to define a default class doc. per control system
//       instance (using property)
//     - The test done to check if attribute value has been set before it is
//       returned to caller is done only if the attribute quality is set to VALID
//     - The JTCInitialize object is now stored in the Util
//     - Windows specific : The tango.h file now also include winsock.h
//
// Revision 2.2  2002/04/30 10:50:40  taurel
// Don't check alarm on attribute if attribute quality factor is INVALID
//
// Revision 2.1  2002/04/29 12:24:02  taurel
// Fix bug in attribute::set_value method and on the check against min and max value when writing attributes
//
// Revision 2.0  2002/04/09 14:45:08  taurel
// See Tango WEB pages for list of changes
//
// Revision 1.6  2001/10/08 09:03:10  taurel
// See tango WEB pages for list of changes
//
// Revision 1.5  2001/07/04 12:27:09  taurel
// New methods re_throw_exception(). Read_attributes supports AllAttr mnemonic A new add_attribute()method in DeviceImpl class New way to define attribute properties New pattern to prevent full re-compile For multi-classes DS, it is now possible to use the Util::get_device_by_name() method in device constructor Adding << operator ovebloading Fix devie CORBA ref. number when device constructor sends an excep.
//
// Revision 1.4  2001/05/04 09:28:12  taurel
// Fix bugs in DServer::restart() method and in Util::get_device_by_name() method
//
// Revision 1.3  2001/03/30 08:03:43  taurel
// Fix bugs in attributes. For linux, add signal_handler in its own thread, change the way to kill server. For all system, change DevRestart philosophy.
//
// Revision 1.2  2001/03/09 08:20:13  taurel
// Fix bug in the MultiClassAttribute::init_class_attribute() method. Also remove the DbErr_DeviceNotDefined define.
//
// Revision 1.1.1.1  2001/02/27 08:46:20  taurel
// Imported sources
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-============================================================================


#include <tango.h>
#include <attrdesc.h>

namespace Tango
{

//+-------------------------------------------------------------------------
//
// method : 		Attr::Attr 
// 
// description : 	Constructor for the Attr class.
//			This constructor simply set the internal values
//
//--------------------------------------------------------------------------

Attr::Attr(const char *att_name,long att_type,AttrWriteType att_writable,
           const char *assoc)
:name(att_name),type(att_type),writable(att_writable),assoc_name(assoc)
{
	format = Tango::SCALAR;
	ext = new AttrExt();
	check_type();
	
	if ((writable == Tango::WRITE) && (assoc_name != AssocWritNotSpec))
	{
		cout3 << "Attr::Attr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Associated attribute is not supported" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"Attr::Attr");
	}
	
	if ((writable == Tango::READ_WITH_WRITE) && (assoc_name == AssocWritNotSpec))
	{
		cout3 << "Attr::Attr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Associated attribute not defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"Attr::Attr");
	}
	
	if (writable == READ_WRITE)
		assoc_name = name;
		
}

Attr::Attr(const char *att_name,long att_type,DispLevel level,
	   AttrWriteType att_writable, const char *assoc)
:name(att_name),type(att_type),writable(att_writable),assoc_name(assoc)
{
	format = Tango::SCALAR;
	ext = new AttrExt(level);
	check_type();
	
	if ((writable == Tango::WRITE) && (assoc_name != AssocWritNotSpec))
	{
		cout3 << "Attr::Attr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Associated attribute is not supported" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"Attr::Attr");
	}
	
	if ((writable == Tango::READ_WITH_WRITE) && (assoc_name == AssocWritNotSpec))
	{
		cout3 << "Attr::Attr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Associated attribute not defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"Attr::Attr");
	}
	
	if (writable == READ_WRITE)
		assoc_name = name;
		
}
Attr::~Attr()
{
	delete ext;
}

//+-------------------------------------------------------------------------
//
// method : 		Attr::check_type 
// 
// description : 	This method checks data type and throws an exception
//			in case of unsupported data type
//
//--------------------------------------------------------------------------

void Attr::check_type()
{
	bool unsuported = true;
	
	if (type == Tango::DEV_SHORT)
		unsuported = false;
	else if (type == Tango::DEV_LONG)
		unsuported = false;
	else if (type == Tango::DEV_DOUBLE)
		unsuported = false;
	else if (type == Tango::DEV_STRING)
		unsuported = false;

	if (unsuported == true)
	{
		cout3 << "Attr::check_type throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Data type is not supported" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"Attr::check_type");
	}
}

//+-------------------------------------------------------------------------
//
// method : 		Attr::set_default_properties
// 
// description : 	This method set the default user properties in the
//			Attr object. At this level, each attribute property
//			is represented by one instance of the Attrproperty
//			class.
//
//--------------------------------------------------------------------------

void Attr::set_default_properties(UserDefaultAttrProp &prop_list)
{
	if (prop_list.label.empty() == false)
		user_default_properties.push_back(AttrProperty("label",prop_list.label));

	if (prop_list.description.empty() == false)
		user_default_properties.push_back(AttrProperty("description",prop_list.description));

	if (prop_list.unit.empty() == false)
		user_default_properties.push_back(AttrProperty("unit",prop_list.unit));

	if (prop_list.standard_unit.empty() == false)
		user_default_properties.push_back(AttrProperty("standard_unit",prop_list.standard_unit));

	if (prop_list.display_unit.empty() == false)
		user_default_properties.push_back(AttrProperty("display_unit",prop_list.display_unit));
		
	if (prop_list.format.empty() == false)
		user_default_properties.push_back(AttrProperty("format",prop_list.format));

	if (prop_list.min_value.empty() == false)
		user_default_properties.push_back(AttrProperty("min_value",prop_list.min_value));

	if (prop_list.max_value.empty() == false)
		user_default_properties.push_back(AttrProperty("max_value",prop_list.max_value));

	if (prop_list.min_alarm.empty() == false)
		user_default_properties.push_back(AttrProperty("min_alarm",prop_list.min_alarm));

	if (prop_list.max_alarm.empty() == false)
		user_default_properties.push_back(AttrProperty("max_alarm",prop_list.max_alarm));

}

//+-------------------------------------------------------------------------
//
// method : 		SpectrumAttr::SpectrumAttr 
// 
// description : 	Constructor for the SpectrumAttr class.
//			This constructor simply set the internal values
//
//--------------------------------------------------------------------------

SpectrumAttr::SpectrumAttr(const char *att_name,long att_type,long x)
:Attr(att_name,att_type),ext(NULL)
{
	format = Tango::SPECTRUM;
	if (x <= 0)
	{
		cout3 << "SpectrumAttr::SpectrumAttr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Maximum x dim. wrongly defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"SpectrumAttr::SpectrumAttr");
	}
	max_x = x;	
}

SpectrumAttr::SpectrumAttr(const char *att_name,long att_type,long x,DispLevel level)
:Attr(att_name,att_type,level),ext(NULL)
{
	format = Tango::SPECTRUM;
	if (x <= 0)
	{
		cout3 << "SpectrumAttr::SpectrumAttr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Maximum x dim. wrongly defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"SpectrumAttr::SpectrumAttr");
	}
	max_x = x;	
}

//+-------------------------------------------------------------------------
//
// method : 		ImageAttr::ImageAttr 
// 
// description : 	Constructor for the ImageAttr class.
//			This constructor simply set the internal values
//
//--------------------------------------------------------------------------

ImageAttr::ImageAttr(const char *att_name,long att_type,long x,long y)
:SpectrumAttr(att_name,att_type,x),ext(NULL)
{
	format = Tango::IMAGE;
	if (y <= 0)
	{
		cout3 << "ImageAttr::ImageAttr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Maximum y dim. wrongly defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"ImageAttr::ImageAttr");
	}
	max_y = y;	
}

ImageAttr::ImageAttr(const char *att_name,long att_type,long x,
		     long y,DispLevel level)
:SpectrumAttr(att_name,att_type,x,level),ext(NULL)
{
	format = Tango::IMAGE;
	if (y <= 0)
	{
		cout3 << "ImageAttr::ImageAttr throwing exception" << endl;
		TangoSys_OMemStream o;
		
		o << "Attribute : " << name << ": "; 
		o << " Maximum y dim. wrongly defined" << ends;
		Except::throw_exception((const char *)"API_AttrWrongDefined",
				      o.str(),
				      (const char *)"ImageAttr::ImageAttr");
	}
	max_y = y;	
}

} // End of Tango namespace
