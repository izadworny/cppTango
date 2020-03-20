//=============================================================================
//
// file :		Device.h
//
// description :	Include for the Device root classes.
//			Three classes are declared in this file :
//				The Device class
//				The DeviceClass class
//
// project :		TANGO
//
// author(s) :		A.Gotz + E.Taurel
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
// $Revision: 18607
//
//=============================================================================

#ifndef _DEVICE_3_H
#define _DEVICE_3_H

#include <tango.h>

namespace Tango
{

class DeviceClass;
class AttributeValueList_4;

//=============================================================================
//
//			The Device_3Impl class
//
//
// description :	This class is derived directly from the Tango::Device_skel
//			class generated by CORBA. It represents the CORBA
//			servant which will be accessed by the client.
//			It implements all the methods
//			and attributes defined in the IDL interface for Device.
//
//=============================================================================

struct AttIdx
{
	long	idx_in_names;
	long	idx_in_multi_attr;
};

/**
 * Base class for all TANGO device since version 3.
 *
 * This class inherits from DeviceImpl class which itself inherits from
 * CORBA classes where all the network layer is implemented.
 * This class has been created since release 3 of Tango library where the IDL
 * Tango module has been modified in order to create a Device_3 interface
 * which inherits from the original Device interface
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

class Device_3Impl : public virtual POA_Tango::Device_3,
		     public Device_2Impl
{
public:

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Device_3Impl object from its name.
 *
 * The device description field is set to <i>A Tango device</i>. The device
 * state is set to unknown and the device status is set to
 * <b>Not Initialised</b>
 *
 * @param 	device_class	Pointer to the device class object
 * @param	dev_name	The device name
 *
 */
	Device_3Impl(DeviceClass *device_class,const std::string &dev_name);

/**
 * Constructs a newly allocated Device_3Impl object from its name and its description.
 *
 * The device
 * state is set to unknown and the device status is set to
 * <i>Not Initialised</i>
 *
 * @param 	device_class	Pointer to the device class object
 * @param	dev_name	The device name
 * @param	desc	The device description
 *
 */
	Device_3Impl(DeviceClass *device_class,const std::string &dev_name,const std::string &desc);

/**
 * Constructs a newly allocated Device_3Impl object from all its creation
 * parameters.
 *
 * The device is constructed from its name, its description, an original state
 * and status
 *
 * @param 	device_class	Pointer to the device class object
 * @param	dev_name	The device name
 * @param	desc 		The device description
 * @param	dev_state 	The device initial state
 * @param	dev_status	The device initial status
 *
 */
	Device_3Impl(DeviceClass *device_class,
	           const std::string &dev_name,const std::string &desc,
	           Tango::DevState dev_state,const std::string &dev_status);

/**
 * Constructs a newly allocated Device_3Impl object from all its creation
 * parameters with some default values.
 *
 * The device is constructed from its name, its description, an original state
 * and status. This constructor defined default values for the description,
 * state and status parameters. The default device description is <i>A TANGO device</i>.
 * The default device state is <i>UNKNOWN</i> and the default device status
 * is <i>Not initialised</i>.
 *
 * @param 	device_class	Pointer to the device class object
 * @param	dev_name	The device name
 * @param	desc	The device desc
 * @param	dev_state 	The device initial state
 * @param	dev_status	The device initial status
 *
 */
	Device_3Impl(DeviceClass *device_class,
	           const char *dev_name,const char *desc = "A TANGO device",
	           Tango::DevState dev_state = Tango::UNKNOWN,
	           const char *dev_status = StatusNotSet);
//@}

/**@name Destructor
 * Only one desctructor is defined for this class */
//@{
/**
 * The device desctructor.
 */
    virtual ~Device_3Impl() {}
//@}


/**@name CORBA operation methods
 * Method defined to implement TANGO device CORBA operation */
//@{

/**
 * Read attribute(s) value.
 *
 * Invoked when the client request the read_attributes_2 CORBA operation.
 * It returns to the client one AttributeValue structure for each wanted
 * attribute.
 *
 * @param names The attribute(s) name list
 * @param source The data source. This parameter is new in Tango release 2. It
 * allows a client to choose the data source between the device itself or the
 * data cache for polled attribute.
 * @return A sequence of AttributeValue structure. One structure is initialised
 * for each wanted attribute with the attribute value, the date and the attribute
 * value quality. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeValue_3</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::AttributeValueList_3 *read_attributes_3(const Tango::DevVarStringArray& names,
							     Tango::DevSource source);

/**
 * Write attribute(s) value.
 *
 * Invoked when the client request the write_attributes CORBA operation.
 * It sets the attribute(s) with the new value(s) passed as parameter.
 *
 * @param values The attribute(s) new value(s). One structure is initialised
 * for each wanted attribute with the attribute value. The attribute quality and
 * date are not used by this method.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeValue</b> structure definition.
 * @exception DevFailed Thrown if the command does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual void write_attributes_3(const Tango::AttributeValueList& values);

/**
 * Read attribute value history.
 *
 * Invoked when the client request the read_attribute_history_3 CORBA operation.
 * This operation allows a client to retrieve attribute value history for
 * polled attribute. The depth of the history is limited to the depth of
 * the device server internal polling buffer.
 * It returns to the client one DevAttrHistory structure for each record.
 *
 * @param name The attribute name
 * @param n The record number.
 * @return A sequence of DevAttrHistory structure. One structure is initialised
 * for each record with the attribute value, the date and in case of the attribute
 * returns an error when it was read, the DevErrors data.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>DevAttrHistory_3</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist or is not polled.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::DevAttrHistoryList_3 *read_attribute_history_3(const char* name,
								  CORBA::Long n);

/**
 * Get device info.
 *
 * Invoked when the client request the info CORBA operation.
 * It updates the black box and returns a DevInfo object
 * with miscellaneous device info
 *
 * @return A DevInfo object
 */
	virtual Tango::DevInfo_3 *info_3();

/**
 * Get attribute(s) configuration.
 *
 * Invoked when the client request the get_attribute_config_3 CORBA operation.
 * It returns to the client one AttributeConfig_3 structure for each wanted
 * attribute. All the attribute properties value are returned in this
 * AttributeConfig_3 structure. Since Tango release 3, the attribute event
 * related, the attribute warning alarm and attribute rds alarm properties
 * have been added to the returned structures.
 *
 * @param names The attribute(s) name list
 * @return A sequence of AttributeConfig_3 structure. One structure is initialised
 * for each wanted attribute. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeConfig_3</b> structure specification.
 *
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::AttributeConfigList_3 *get_attribute_config_3(const Tango::DevVarStringArray& names);

/**
 * Set attribute(s) configuration.
 *
 * Invoked when the client request the set_attribute_config_3 CORBA operation.
 * It updates the device attribute configuration actually used by the device but
 * this method also updates the Tango database. One structure of the
 * AttributeConfig_3 type is needed for each attribute to update configuration.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeConfig_3</b> structure specification.
 *
 * @param new_conf The attribute(s) new configuration structure sequence
 * @exception DevFailed Thrown if the command does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual void set_attribute_config_3(const Tango::AttributeConfigList_3& new_conf);
//@}

/// @privatesection
	void write_attributes_in_db(const std::vector<long> &,const std::vector<long> &);
	void add_state_status_attrs();
	void read_attributes_from_cache(const Tango::DevVarStringArray&,Tango::AttributeIdlData &);
	void delete_dev() {ext_3->delete_dev();}
	void get_attr_props(const char *,std::vector<AttrProperty> &);

protected:
/// @privatesection
	void read_attributes_no_except(const Tango::DevVarStringArray&,Tango::AttributeIdlData &,bool,std::vector<long> &);
	void write_attributes_in_db(const std::vector<long> &,const std::vector<AttIdx> &);
	void add_alarmed(std::vector<long> &);
	void state2attr(Tango::DevState,Tango::AttributeValue_3 &);
	void state2attr(Tango::DevState,Tango::AttributeValue_4 &);
	void state2attr(Tango::DevState,Tango::AttributeValue_5 &);
	void status2attr(Tango::ConstDevString,Tango::AttributeValue_3 &);
	void status2attr(Tango::ConstDevString,Tango::AttributeValue_4 &);
	void status2attr(Tango::ConstDevString,Tango::AttributeValue_5 &);
	void alarmed_not_read(const std::vector<AttIdx> &);

	void write_attributes_34(const Tango::AttributeValueList *,const Tango::AttributeValueList_4 *);

	template <typename T,typename V>
	void set_attribute_config_3_local(const T &,const V &,bool,int);

	template <typename T> void one_error(T &,const char *,const char *,std::string &,Attribute &);
	template <typename T> void one_error(T &,const char *,const char *,std::string &,const char *);

	template <typename T,typename V> void init_polled_out_data(T &,V &);
	template <typename T> void init_out_data(T &,Attribute &,AttrWriteType &);
	template <typename T> void init_out_data_quality(T &,Attribute &,AttrQuality);

	template <typename T> void base_state2attr(T &);
	template <typename T> void base_status2attr(T &);

private:

    class Device_3ImplExt
    {
    public:
        Device_3ImplExt() {}
        virtual ~Device_3ImplExt() {}

        virtual	void		delete_dev() {}
    };

    void real_ctor();

    void handle_read_attributes(
        const Tango::DevVarStringArray&,
        Tango::AttributeIdlData&,
        bool second_try,
        std::vector<long> &idx);
    void call_read_attr_hardware_if_needed(
        const std::vector<AttIdx>&,
        bool state_wanted);
    void update_readable_attribute_value(
        const Tango::DevVarStringArray&,
        Tango::AttributeIdlData&,
        bool second_try,
        std::vector<long>& idx,
        AttIdx&);
    void update_writable_attribute_value(
        const Tango::DevVarStringArray&,
        Tango::AttributeIdlData&,
        bool second_try,
        std::vector<long>& idx,
        AttIdx&);
    void read_and_store_state_for_network_transfer(
        const char* name,
        Tango::AttributeIdlData&,
        int state_idx,
        std::vector<AttIdx>&);
    void read_and_store_status_for_network_transfer(
        const char* name,
        Tango::AttributeIdlData&,
        int status_idx);
    void store_attribute_for_network_transfer(
        const char* name,
        Tango::AttributeIdlData&,
        int index);


    std::unique_ptr<Device_3ImplExt>     ext_3;           // Class extension
};

} // End of Tango namespace

#endif // _DEVICE_H
