/*----- PROTECTED REGION ID(FwdTestClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        FwdTestClass.h
//
// description : Include for the FwdTest root class.
//               This class is the singleton class for
//                the FwdTest device class.
//               It contains all properties and methods which the 
//               FwdTest requires only once e.g. the commands.
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
// $Author$
//
// $Revision$
// $Date$
//
// $HeadURL$
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef FwdTestClass_H
#define FwdTestClass_H

#include <tango.h>
#include <FwdTest.h>


/*----- PROTECTED REGION END -----*/	//	FwdTestClass.h


namespace FwdTest_ns
{
/*----- PROTECTED REGION ID(FwdTestClass::classes for dynamic creation) ENABLED START -----*/

class FwdAttrScaSh: public Tango::FwdAttr
{
public:
    FwdAttrScaSh(const string &_n):FwdAttr(_n) {};
	~FwdAttrScaSh() {};
};

class FwdAttrScaStr: public Tango::FwdAttr
{
public:
	FwdAttrScaStr(const string &_n):FwdAttr(_n) {};
	~FwdAttrScaStr() {};
};

class FwdAttrSpecDb: public Tango::FwdAttr
{
public:
    FwdAttrSpecDb(const string &_n):FwdAttr(_n) {};
	~FwdAttrSpecDb() {};
};

class FwdAttrImaStr: public Tango::FwdAttr
{
public:
    FwdAttrImaStr(const string &_n):FwdAttr(_n) {};
	~FwdAttrImaStr() {};
};

class FwdAttrScaSta: public Tango::FwdAttr
{
public:
	FwdAttrScaSta(const std::string &_n):FwdAttr(_n) {};
	~FwdAttrScaSta() {};
};

/*----- PROTECTED REGION END -----*/	//	FwdTestClass::classes for dynamic creation

/**
 *	The FwdTestClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  FwdTestClass : public Tango::DeviceClass
#else
class FwdTestClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(FwdTestClass::Additionnal DServer data members) ENABLED START -----*/
	
	
	/*----- PROTECTED REGION END -----*/	//	FwdTestClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static FwdTestClass *init(const char *);
		static FwdTestClass *instance();
		~FwdTestClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		FwdTestClass(string &);
		static FwdTestClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	FwdTest_H
