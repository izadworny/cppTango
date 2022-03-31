//===================================================================================================================
//
// file :		Device_5.h
//
// description :	Include for the Device root classes in its release 5 (IDL release 5, Tango release 9)
//
// project :		TANGO
//
// author(s) :		E.Taurel
//
// Copyright (C) :      2014,2015
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
// $Revision: 20742 $
//
//===================================================================================================================

#ifndef _DEVICE_5_H
#define _DEVICE_5_H

#include <tango.h>

namespace Tango
{

class WPipe;

//==================================================================================================================
//
//			The Device_5Impl class
//
//
// description :
// 		This class is derived directly from the Tango::Device_skel class generated by CORBA. It represents the CORBA
//		servant which will be accessed by the client. It implements all the methods and attributes defined in the
// 		IDL interface for Device.
//
//==================================================================================================================


/**
 * Base class for all TANGO device since version 5.
 *
 * This class inherits from DeviceImpl class which itself inherits from
 * CORBA classes where all the network layer is implemented.
 * This class has been created since release 7 of Tango library where the IDL
 * Tango module has been modified in order to create a Device_4 interface
 * which inherits from the original Device interface
 *
 * $Author: taurel $
 * $Revision: 20742 $
 *
 * @headerfile tango.h
 * @ingroup Server
 */

// NOLINTNEXTLINE(readability-identifier-naming)
class Device_5Impl : public virtual POA_Tango::Device_5,
		     public Device_4Impl
{
public:

/**@name Constructors
 * Miscellaneous constructors */
//@{
/**
 * Constructs a newly allocated Device_5Impl object from its name.
 *
 * The device description field is set to <i>A Tango device</i>. The device
 * state is set to unknown and the device status is set to
 * <b>Not Initialised</b>
 *
 * @param 	device_class	Pointer to the device class object
 * @param	dev_name	The device name
 *
 */
	Device_5Impl(DeviceClass *device_class,const std::string &dev_name);

/**
 * Constructs a newly allocated Device_5Impl object from its name and its description.
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
	Device_5Impl(DeviceClass *device_class,const std::string &dev_name,const std::string &desc);

/**
 * Constructs a newly allocated Device_5Impl object from all its creation
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
	Device_5Impl(DeviceClass *device_class,
	           const std::string &dev_name,const std::string &desc,
	           Tango::DevState dev_state,const std::string &dev_status);

/**
 * Constructs a newly allocated Device_5Impl object from all its creation
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
	Device_5Impl(DeviceClass *device_class,
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
	virtual ~Device_5Impl() {}
//@}


/**@name CORBA operation methods
 * Method defined to implement TANGO device CORBA operation */
//@{

/**
 * Read attribute(s) value.
 *
 * Invoked when the client request the read_attributes_5 CORBA operation.
 * It returns to the client one AttributeValue structure for each wanted
 * attribute.
 *
 * @param names The attribute(s) name list
 * @param source The data source. This parameter is new in Tango release 5. It
 * allows a client to choose the data source between the device itself or the
 * data cache for polled attribute.
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @return A sequence of AttributeValue_5 structure. One structure is initialised
 * for each wanted attribute with the attribute value, the date and the attribute
 * value quality. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeValue</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::AttributeValueList_5 *read_attributes_5(const Tango::DevVarStringArray& names,
								     Tango::DevSource source,
									 const Tango::ClntIdent &cl_ident);
/**
 * Write then read attribute(s) value.
 *
 * Invoked when the client request the write_read_attributes CORBA operation.
 * It sets the attribute(s) with the new value(s) passed as parameter
 * and then read the attribute(s) and return the read value to the caller.
 *
 * @param values The attribute(s) new value(s). One structure is initialised
 * for each wanted attribute with the attribute value. The attribute quality and
 * date are not used by this method.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeValue</b> structure definition.
 * @param r_names Names of the attribute(s) to be read
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @return A sequence of AttributeValue_5 structure. One structure is initialised
 * for each wanted attribute with the attribute value, the date and the attribute
 * value quality. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeValue_5</b> structure definition.
 * @exception DevFailed Thrown if the command does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */

    	virtual Tango::AttributeValueList_5* write_read_attributes_5(const Tango::AttributeValueList_4 &values,
																   const Tango::DevVarStringArray &r_names,
																   const Tango::ClntIdent &cl_ident);

/**
 * Get attribute(s) configuration.
 *
 * Invoked when the client request the get_attribute_config_5 CORBA operation.
 * It returns to the client one AttributeConfig_5 structure for each wanted
 * attribute. All the attribute properties value are returned in this
 * AttributeConfig_5 structure. Since Tango release V5, the attribute event
 * related, the attribute warning alarm and attribute rds alarm properties
 * have been added to the returned structures.
 *
 * @param names The attribute(s) name list
 * @return A sequence of AttributeConfig_5 structure. One structure is initialised
 * for each wanted attribute. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeConfig_5</b> structure specification.
 *
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::AttributeConfigList_5 *get_attribute_config_5(const Tango::DevVarStringArray& names);

/**
 * Set attribute(s) configuration.
 *
 * Invoked when the client request the set_attribute_config_5 CORBA operation.
 * It updates the device attribute configuration actually used by the device but
 * this method also updates the Tango database. One structure of the
 * AttributeConfig_3 type is needed for each attribute to update configuration.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>AttributeConfig_5</b> structure specification.
 *
 * @param new_conf The attribute(s) new configuration structure sequence
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @exception DevFailed Thrown if the command does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual void set_attribute_config_5(const Tango::AttributeConfigList_5& new_conf,
										const Tango::ClntIdent &cl_ident);

/**
 * Read attribute value history.
 *
 * Invoked when the client request the read_attribute_history_5 CORBA operation.
 * This operation allows a client to retrieve attribute value history for
 * polled attribute. The depth of the history is limited to the depth of
 * the device server internal polling buffer.
 * It returns to the client one DevAttrHistory structure for each record.
 *
 * @param name The attribute name
 * @param n The record number.
 * @return A DevAttrHistory_5 structure. This strucure contains all the element
 * allowing a client to retrieve for each history record the attribute value,
 * the date and in case of the attribute
 * returns an error when it was read, the DevErrors data.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>DevAttrHistory</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist or is not polled.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::DevAttrHistory_5 *read_attribute_history_5(const char* name,CORBA::Long n);

/**
 * Get pipe(s) configuration.
 *
 * Invoked when the client request the get_pipe_config_5 CORBA operation.
 * It returns to the client one PipeConfig_5 structure for each wanted
 * pipe. All the pipe description is returned in this
 * PipeConfig_5 structure.
 *
 * @param names The pipe(s) name list
 * @return A sequence of PipeConfig structures. One structure is initialised
 * for each wanted pipe. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>PipeConfig</b> structure specification.
 *
 * @exception DevFailed Thrown if the pipe does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::PipeConfigList *get_pipe_config_5(const Tango::DevVarStringArray& names);

/**
 * Set pipe(s) configuration.
 *
 * Invoked when the client request the set_pipe_config_5 CORBA operation.
 * It updates the device pipe configuration actually used by the device but
 * this method also updates the Tango database. One structure of the
 * PipeConfig type is needed for each pipe to update configuration.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>PipeConfig</b> structure specification.
 *
 * @param new_conf The pipe(s) new configuration structure sequence
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @exception DevFailed Thrown if the command does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
    	virtual void set_pipe_config_5(const Tango::PipeConfigList& new_conf,
    										const Tango::ClntIdent &cl_ident);

/**
 * Read pipe value.
 *
 * Invoked when the client request the read_pipe_5 CORBA operation.
 * It returns to the client a DevPipeData structure.
 *
 * @param name The pipe name
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @return A DevPipeData structure. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>DevPipeData</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::DevPipeData *read_pipe_5(const char *name,const Tango::ClntIdent &cl_ident);

/**
 * Write pipe value.
 *
 * Invoked when the client request the write_pipe_5 CORBA operation.
 * It allows a client to send data to a device through a pipe.
 *
 * @param pipe_value The new pipe value
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual void write_pipe_5(const Tango::DevPipeData &pipe_value,const Tango::ClntIdent& cl_ident);

/**
 * Write then read pipe value.
 *
 * Invoked when the client request the write_read_pipe_5 CORBA operation.
 * It allows a client to send data to a device through a pipe then to read data from the device through
 * the same pipe.
 *
 * @param pipe_value The new pipe value
 * @param cl_ident The client identificator. This parameter is new in release 4.
 * It allows device locking feature implemented in Tango V7
 * @return A DevPipeData structure. Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#structs">here</a>
 * to read <b>DevPipeData</b> structure definition.
 * @exception DevFailed Thrown if the attribute does not exist.
 * Click <a href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a> to read
 * <b>DevFailed</b> exception specification
 */
	virtual Tango::DevPipeData *write_read_pipe_5(const Tango::DevPipeData &pipe_value,const Tango::ClntIdent& cl_ident);
//@}

/// @privatesection

private:
    class Device_5ImplExt
    {
    public:
        Device_5ImplExt() {}
        ~Device_5ImplExt() {}
    };

    std::unique_ptr<Device_5ImplExt>     ext_5;           // Class extension

};

} // End of Tango namespace

#endif // _DEVICE_5_H
