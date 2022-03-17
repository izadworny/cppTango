//=============================================================================
//
// file :		Command.h
//
// description :	Include for the Device root classes.
//			One class is declared in this file :
//				The Command class
//
// project :		TANGO
//
// author(s) :		A.Gotz + E.Taurel
//
// Copyright (C) : 2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
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
//=============================================================================

#ifndef _COMMAND_H
#define _COMMAND_H

#include <tango.h>

namespace Tango
{

typedef bool (DeviceImpl::*ALLO_PTR)(const CORBA::Any &);

/**
 * This class is a class representing a command in the TANGO device server
 * pattern. it is an abstract class. It is the root class for all command
 * related classes for command implemented with the inheritance model or
 * with the template command model
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

class Command
{
public:
  /**@name Constructors
   * Miscellaneous constructors */
  //@{
  /**
   * Constructs a newly allocated Command object.
   *
   * The default constructor
   */
  Command()
      : ext(new CommandExt)
  {
  }

  /**
   * Constructs a newly allocated Command object for a command from its
   * name and its input and output parameter types.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	s	The command name
   * @param	in	The command input parameter type
   * @param	out 	The command output parameter type
   * @param	level	The command display level
   *
   */
  Command(const std::string &s, Tango::CmdArgType in, Tango::CmdArgType out, Tango::DispLevel level = Tango::OPERATOR);

  /**
   * Constructs a newly allocated Command object for a command from its
   * name, its input and output parameter types plus parameters description
   *
   * @param 	s	The command name
   * @param	in	The command input parameter type
   * @param	out 	The command output parameter type
   * @param	in_desc	The input parameter description
   * @param	out_desc The output parameter description
   * @param	level	The command display level
   *
   */
  Command(const std::string &s, Tango::CmdArgType in, Tango::CmdArgType out, const std::string &in_desc,
          const std::string &out_desc, Tango::DispLevel level = Tango::OPERATOR);

  //@}

  /**@name Destructor
   * Only one desctructor is defined for this class */
  //@{
  /**
   * The object desctructor.
   */
  virtual ~Command() {}

  //@}

  /**@name Miscellaneous methods */
  //@{
  /**
   * Execute the command.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client. This method is abstract and
   * must be redefined in each sub-class
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object.
   * @return The CORBA Any object returned to the client.
   * @exception DevFailed If the execution method failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  virtual CORBA::Any *execute(DeviceImpl *dev, const CORBA::Any &in_any) = 0;

  /**
   * Check if the command is allowed in the actual device state.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client to check if the command is
   * allowed in the actual device state. This method is the default is_allowed
   * method which always allows the command to be executed. It is possible to
   * re-define it if this default behaviour does not fulfill the device needs.
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object. This
   * data is passed to this method in case it is necessary to take the command
   * allowed decision
   * @return A boolean set to true is the command is allowed. Otherwise, the
   * return value is false.
   */
  virtual bool is_allowed(DeviceImpl *dev, const CORBA::Any &in_any)
  {
    (void) dev;
    (void) in_any;
    return true;
  }

  /**
   * Init command parameters type.
   *
   * This method is used only for command implemented using the Tango template
   * command model. In this base class, it does nothing and is re-define in
   * sub-classes.
   *
   */
  virtual void init_types() {}

  //@}

  /**@name Get/Set object members.
   * These methods allows the external world to get/set DeviceImpl instance
   * data members
   */
  //@{
  /**
   * Return the command name.
   *
   * @return The command name
   */
  std::string &get_name() { return name; }

  /**
   * Set the command name.
   *
   * @param new_name The new command name
   */
  void set_name(const std::string &new_name) { name = new_name; }

  /**
   * Return the command name in lower case letters.
   *
   * @return The command name
   */
  std::string &get_lower_name() { return lower_name; }

  /**
   * Return the input parameter type.
   *
   * @return The input parameter type
   */
  Tango::CmdArgType get_in_type() { return in_type; }

  /**
   * Return the output parameter type.
   *
   * @return The output parameter type
   */
  Tango::CmdArgType get_out_type() { return out_type; }

  /**
   * Return the input parameter description.
   *
   * @return The input parameter description
   */
  std::string &get_in_type_desc() { return in_type_desc; }

  /**
   * Return the output parameter description.
   *
   * @return The output parameter description
   */
  std::string &get_out_type_desc() { return out_type_desc; }

  /**
   * Return the command display level.
   *
   * @return The command display level
   */
  Tango::DispLevel get_disp_level() { return cmd_disp_level; }

  /**
   * Set the input parameter description field.
   *
   * @param desc The input parameter description
   */
  void set_in_type_desc(std::string desc) { in_type_desc = desc; }

  /**
   * Set the output parameter description field.
   *
   * @param desc The output parameter description
   */
  void set_out_type_desc(std::string desc) { out_type_desc = desc; }

  /**
   * Set the command display level.
   *
   * @param level The command display level
   */
  void set_disp_level(Tango::DispLevel level) { cmd_disp_level = level; }

  /**
   * Set the command polling period.
   *
   * @param per The command polling period (in mS)
   */
  void set_polling_period(long per) { poll_period = per; }

  /**
   * Get the command polling period.
   *
   * @return The command polling period (in mS)
   */
  long get_polling_period() { return poll_period; }

  //@}

  /**@name Extract methods.
   * All these methods extract data from the CORBA Any object received as
   * command input data
   */
  //@{
  /**
   * Extract a boolean data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted boolean data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevBoolean &data);

  /**
   * Extract a short data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted short data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevShort &data);

  /**
   * Extract a long data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted long data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevLong &data);

  /**
   * Extract a 64 bits long data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted 64 bits long data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevLong64 &data);

  /**
   * Extract a float data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted float data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevFloat &data);

  /**
   * Extract a double data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted double data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevDouble &data);

  /**
   * Extract an unsigned short data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned short data
   * @exception DevFailed If the Any object does not contanis a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevUShort &data);

  /**
   * Extract an unsigned long data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned long data
   * @exception DevFailed If the Any object does not contanis a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevULong &data);

  /**
   * Extract an unsigned 64 bits long data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned 64 bits long data
   * @exception DevFailed If the Any object does not contanis a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevULong64 &data);

  /**
   * Extract a string from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted string data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevString &data);

  /**
   * Extract a const string from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted string data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const char *&data);

  /**
   * Extract a char array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted char array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarCharArray *&data);

  /**
   * Extract a short array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted short array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarShortArray *&data);

  /**
   * Extract a long array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted long array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarLongArray *&data);

  /**
   * Extract a 64 bits long array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted 64 bits long array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarLong64Array *&data);

  /**
   * Extract a float array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted float array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarFloatArray *&data);

  /**
   * Extract a double array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted double array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarDoubleArray *&data);

  /**
   * Extract a unsigned short array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned char array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarUShortArray *&data);

  /**
   * Extract a unsigned long array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned long array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarULongArray *&data);

  /**
   * Extract a unsigned 64 bits long array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted unsigned 64 bits long array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarULong64Array *&data);

  /**
   * Extract a string array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted string array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarStringArray *&data);

  /**
   * Extract a string array from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted string array
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarBooleanArray *&data);

  /**
   * Extract a DevVarLongStringArray data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted DevVarLongStringArray data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarLongStringArray *&data);

  /**
   * Extract a DevVarDoubleStringArray data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted DevVarDoubleStringArray data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevVarDoubleStringArray *&data);

  /**
   * Extract a Tango device state data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted device state data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, Tango::DevState &data);

  /**
   * Extract a Tango DevEncoded data from a CORBA Any object.
   *
   * @param in The CORBA Any object
   * @param data Reference to the extracted DevEncoded data
   * @exception DevFailed If the Any object does not contains a data of the
   * waited type.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void extract(const CORBA::Any &in, const Tango::DevEncoded *&data);
  //@}

  /**@name Insert methods.
   * All these methods create a CORBA Any object and insert data into this
   * object
   */
  //@{
  /**
   * Create an  empty CORBA Any object.
   *
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert();

  /**
   * Create a CORBA Any object and insert a Tango::DevBoolean data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevBoolean data);

  /**
   * Create a CORBA Any object and insert a Tango::DevShort data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevShort data);

  /**
   * Create a CORBA Any object and insert a Tango::DevLong data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevLong data);

  /**
   * Create a CORBA Any object and insert a Tango::DevLong64 data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevLong64 data);

  /**
   * Create a CORBA Any object and insert a Tango::DevFloat data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevFloat data);

  /**
   * Create a CORBA Any object and insert a Tango::DevDouble data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevDouble data);

  /**
   * Create a CORBA Any object and insert a Tango::DevUShort data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevUShort data);

  /**
   * Create a CORBA Any object and insert a Tango::DevULong data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevULong data);

  /**
   * Create a CORBA Any object and insert a Tango::DevULong64 data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevULong64 data);

  /**
   * Create a CORBA Any object and insert a Tango::DevString data in it.
   *
   * This method will also de-allocate the string passed as parameter.
   *
   * @param data The string to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevString data);

  /**
   * Create a CORBA Any object and insert a Tango::DevString data in it.
   * Te parameter type is char * and not Tango::DevString because the const
   * C++ modifier applied to a Tango::DevString make the pointer constant and
   * not the pointed to characters to be constant.
   *
   * @param data The string to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(const char *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarCharArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarCharArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarCharArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarCharArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarCharArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarShortArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarShortArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarShortArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarShortArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarShortArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLongArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLongArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLongArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarLongArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLongArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLong64Array data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLong64Array &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLong64Array data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarLongArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLong64Array *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarFloatArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarFloatArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarFloatArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarFloatArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarFloatArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarDoubleArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarDoubleArray &data);

  /**
   * Create a CORBA CORBA::Any object and insert a Tango::DevVarDoubleArray data
   * in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarDoubleArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarDoubleArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarUShortArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarUShortArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarUShortArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarUShortArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarUShortArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarULongArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarULongArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarULongArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarULongArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarULongArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarULong64Array data in
   * it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarULong64Array &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarULong64Array data in
   * it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarULongArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarULong64Array *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarStringArray data in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarStringArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarStringArray data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarStringArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarStringArray *data);

  // TODO templates
  /**
   * Create a CORBA Any object and insert a Tango::DevVarBooleanArray data in
   * it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(DevVarBooleanArray *data);
  /**
   * Create a CORBA Any object and insert a Tango::DevVarBooleanArray data in
   * it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarBooleanArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(DevVarBooleanArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLongStringArray data in
   * it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLongStringArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarLongStringArray data in
   * it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarLongStringArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarLongStringArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarDoubleStringArray data
   * in it.
   *
   * This method will do a <b>deep copy</b> of the array into the Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarDoubleStringArray &data);

  /**
   * Create a CORBA Any object and insert a Tango::DevVarDoubleStringArray data
   * in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarDoubleStringArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevVarDoubleStringArray *data);

  /**
   * Create a CORBA Any object and insert a Tango::DevState data in it.
   *
   * @param data The data to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevState data);

  /**
   * Create a CORBA Any object and insert a Tango::DevEncoded data in it.
   *
   * This method <b>consumes</b> the memory used by the array.
   * When the CORBA layer will destroy the Any object, the memory allocated
   * for the array will also be freed. This is the recommended method to
   * insert Tango::DevVarDoubleStringArray data type into a CORBA Any object.
   *
   * @param data The array to be inserted into the Any object
   * @exception DevFailed If the Any object creation failed.
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *insert(Tango::DevEncoded *data);
  //@}

protected:
  /**@name Class data members */
  //@{
  /**
   * The command name
   */
  std::string name;
  /**
   * The command name in lower case
   */
  std::string lower_name;
  /**
   * The command input parameter type
   */
  Tango::CmdArgType in_type;
  /**
   * The command output parameter type
   */
  Tango::CmdArgType out_type;
  /**
   * The command input parameter description
   */
  std::string in_type_desc;
  /**
   * The command output parameter type
   */
  std::string out_type_desc;
  //@}

private:
  class CommandExt
  {
  public:
    CommandExt() {}
  };

  void alloc_any(CORBA::Any *&);
  void throw_bad_type(const char *);

  std::unique_ptr<CommandExt> ext; // Class extension

  //
  // Ported from the extension class
  //

  Tango::DispLevel cmd_disp_level; // Display  level
  long poll_period;                // Polling period
};

//=============================================================================
//
//			The TemplCommand class
//
//
// description :	This class is a derived class of the Command class.
//			It is used to create a command from a pointer to a
//			object method which will execute the command.
//			This class is for command without inout  nor output
//			paremeters.
//			This class is also a base class for the template Command
//			class
//
//=============================================================================

/**
 * This class is a class representing a command in the template command model
 * without input or output parameter
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

class TemplCommand : public Command
{
public:
  /**@name Constructors
   * Miscellaneous constructors */
  //@{
  /**
   * Constructs a newly allocated Command object.
   *
   * The default constructor
   */
  TemplCommand()
      : ext(nullptr)
  {
  }

  /**
   * Constructs a newly allocated TemplCommand object for a command with a
   * name and an execution method.
   * This constructor set the command input and output type to Tango::DEV_VOID.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	level		The command display level
   *
   */
  TemplCommand(const std::string &cmd_name, void (DeviceImpl::*exe_method)(), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommand object for a command with a
   * name, an execution method and a command allowed method.
   * This constructor set the command input and output type to Tango::DEV_VOID
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	level		The command display level
   *
   */
  TemplCommand(const std::string &cmd_name, void (DeviceImpl::*exe_method)(),
               bool (DeviceImpl::*state_method)(const CORBA::Any &), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommand object for a command with a
   * name, an execution method and a description for the
   * input and output command parameters.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	in_desc		The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommand(const std::string &cmd_name, void (DeviceImpl::*exe_method)(), const std::string &in_desc,
               const std::string &out_desc, Tango::DispLevel level = Tango::OPERATOR);

  /**
   * Constructs a newly allocated TemplCommand object for a command with a
   * name, an execution method, a command allowed method and a description for
   * the input and output command parameters. This constructor set the command
   * input and output type to Tango::DEV_VOID.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	in_desc		The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommand(const std::string &cmd_name, void (DeviceImpl::*exe_method)(),
               bool (DeviceImpl::*state_method)(const CORBA::Any &) = NULL, const std::string &in_desc = "",
               const std::string &out_desc = "", Tango::DispLevel level = Tango::OPERATOR);
  //@}

  TemplCommand(const std::string &, Tango::DispLevel);
  TemplCommand(const std::string &, const std::string & = "", const std::string & = "", DispLevel = Tango::OPERATOR);

  /**@name Miscellaneous methods */
  //@{
  /**
   * Choose the correct TANGO data type constant according to data type
   *
   * The TANGO constant is determined using the type_info object passed
   * as first argument of the method. This data is compared to each defined
   * Tango type.
   *
   * @param data_type The type to be analysed
   * @param type A reference where Tango data type constant must be stored
   * @exception DevFailed If the type is not a Tango data type
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  void set_type(const std::type_info &data_type, Tango::CmdArgType &type);

  /**
   * Invoke the command execution method given at object creation time.
   *
   * This method is automatically called by the TANGO core classes when the
   * associated command is requested by a client.
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object. For
   * command created with this TemplCommand class, this Any object does not
   * contain usefull data
   * @return The CORBA Any object returned to the client. For command created
   * with this TemplCommand class, this any object does not contain data.
   * @exception DevFailed If the execution method failed
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *execute(DeviceImpl *dev, const CORBA::Any &in_any);

  /**
   * Invoke the command allowed method given at object creation time.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client to check if the command is
   * allowed in the actual device state. If the user give a command allowed
   * method at object creation time, this method will be invoked.
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object. For
   * command created with this TemplCommand class, this Any object does not
   * contain data
   * @return A boolean set to true is the command is allowed. Otherwise, the
   * return value is false. This return value is always set to true if the user
   * does not supply a method to be excuted. If a method has been supplied, the
   * return value is the value returned by the user supplied mehod.
   */
  bool is_allowed(DeviceImpl *dev, const CORBA::Any &in_any);
  //@}

private:
  class TemplCommandExt
  {
  };

  void (DeviceImpl::*exe_ptr)();
  std::unique_ptr<TemplCommandExt> ext; // Class extension

protected:
  /**@name Class data members */
  //@{
  /**
   * The command allowed method object reference
   */
  bool (DeviceImpl::*allowed_ptr)(const CORBA::Any &);
  //@}
};

//=============================================================================
//
//			The TemplCommandInOut class
//
//
// description :	This class is a derived class of the Command class.
//			It is used to create a command from a pointer to a
//			object method which will execute the command.
//			This class is for command without inout  nor output
//			paremeters.
//			This class is also a base class for the template Command
//			class
//
//=============================================================================

/**
 * This class is a class representing a command in the template command model
 * with output and input parameters. The class template
 * parameters (called INARG and OUTARG) are the command input parameter type
 * and the command output parameter type.
 *<p>Synopsis : template <typename INARG,typename OUTARG> class
 *TemplCommandInOut:public TemplCommand;</p> <p>Usage : new
 *TemplCommandInOut<Tango::DevLong,Tango::DevFloat>(...);</p>
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

template <typename INARG, typename OUTARG>
class TemplCommandInOut : public TemplCommand
{
public:
  /**@name Constructors
   * Miscellaneous constructors */
  //@{
  /**
   * Constructs a newly allocated TemplCommandInOut object for a command with a
   * name and an execution method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	level		The command display level
   *
   */
  TemplCommandInOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(INARG), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandInOut object for a command with a
   * name, an execution method and a command allowed method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	level		The command display level
   *
   */
  TemplCommandInOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(INARG),
                    bool (DeviceImpl::*state_method)(const CORBA::Any &), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandInOut object for a command with a
   * name, an execution method and a description for the
   * input and output command parameters.
   * The input and output command data type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	in_desc		The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandInOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(INARG), const std::string &in_desc,
                    const std::string &out_desc, Tango::DispLevel level = Tango::OPERATOR);

  /**
   * Constructs a newly allocated TemplCommandInOut object for a command with a
   * name, an execution method, a command allowed method and a description for
   * the input and output command parameters. The input and output command data
   * type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	in_desc	The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandInOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(INARG),
                    bool (DeviceImpl::*state_method)(const CORBA::Any &) = NULL, const std::string &in_desc = "",
                    const std::string &out_desc = "", Tango::DispLevel level = Tango::OPERATOR);

  //@}

  ~TemplCommandInOut() {}

  /**@name Miscellaneous methods */
  //@{
  /**
   * Initialise command input and output types.
   *
   * Set the command output type to Tango::DEV_VOID. The command input type is
   * automatically determined from the class template specialisation
   *
   */
  void init_types();

  /**
   * Invoke the command execution method given at object creation time.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client. It first unpacks the incoming
   * data. Then, it invokes the user supplied
   * command execution method and packs the returned data into the outgoing
   * CORBA Any object
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object.
   * @return The CORBA Any object returned to the client.
   * @exception DevFailed If the execution method failed
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *execute(DeviceImpl *dev, const CORBA::Any &in_any);
  //@}

private:
  class TemplCommandInOutExt
  {
  };

  OUTARG (DeviceImpl::*exe_ptr_inout)(INARG);
  std::unique_ptr<TemplCommandInOut> ext; // Class extension
};

//+-------------------------------------------------------------------------
//
// method : 		TempCommandInOut class constructors
//
// description : 	instance constructor
//
//--------------------------------------------------------------------------
template <typename INARG, typename OUTARG>
TemplCommandInOut<INARG, OUTARG>::TemplCommandInOut(const std::string &s, OUTARG (DeviceImpl::*f)(INARG),
                                                    Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_inout(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename INARG, typename OUTARG>
TemplCommandInOut<INARG, OUTARG>::TemplCommandInOut(const std::string &s, OUTARG (DeviceImpl::*f)(INARG),
                                                    bool (DeviceImpl::*a)(const CORBA::Any &), Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_inout(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

template <typename INARG, typename OUTARG>
TemplCommandInOut<INARG, OUTARG>::TemplCommandInOut(const std::string &s, OUTARG (DeviceImpl::*f)(INARG),
                                                    const std::string &in_desc, const std::string &out_desc,
                                                    Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_inout(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename INARG, typename OUTARG>
TemplCommandInOut<INARG, OUTARG>::TemplCommandInOut(const std::string &s, OUTARG (DeviceImpl::*f)(INARG),
                                                    bool (DeviceImpl::*a)(const CORBA::Any &),
                                                    const std::string &in_desc, const std::string &out_desc,
                                                    Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_inout(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

//+-------------------------------------------------------------------------
//
// method : 		init_types
//
// description : 	Initialise input and output type from the template
//			class specialisation parameters.
//
//--------------------------------------------------------------------------

template <typename INARG, typename OUTARG>
void TemplCommandInOut<INARG, OUTARG>::init_types()
{
  //
  // Set intput type
  //
  set_type(typeid(INARG), in_type);

  //
  // Set output type
  //

  set_type(typeid(OUTARG), out_type);
}

//+-------------------------------------------------------------------------
//
// method : 		execute
//
// description : 	Execute the method associated with the command
//			(stored in the exe_ptr data)
//
// input : - dev_ptr : pointer to the device on which the command must be
//		       executed
//	   - in_any : Incoming command data
//
// This method returns a pointer to an Any object with the command outing
// data.
//
//--------------------------------------------------------------------------

template <typename INARG, typename OUTARG>
CORBA::Any *TemplCommandInOut<INARG, OUTARG>::execute(DeviceImpl *dev_ptr, const CORBA::Any &in_any)
{
  //
  // Execute the command associated method
  //

  INARG in_data;
  extract(in_any, in_data);
  OUTARG out_data = (dev_ptr->*exe_ptr_inout)(in_data);
  return insert(out_data);
}

//=============================================================================
//
//			The TemplCommandIn class
//
//
// description :	This class is a derived class of the Command class.
//			It is used to create a command from a pointer to a
//			object method which will execute the command.
//			This class is for command without inout  nor output
//			paremeters.
//			This class is also a base class for the template Command
//			class
//
//=============================================================================

/**
 * This class is a class representing a command in the template command model
 * with input parameter but without output parameter. The class template
 * parameter (called INARG) is the command input parameter type.
 *<p>Synopsis : template typename &lt;INARG&gt; class TemplCommandIn:public
 *TemplCommand;</p> <p>Usage : new
 *TemplCommandIn&lt;Tango::DevLong&gt;(...);</p>
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

template <typename INARG>
class TemplCommandIn : public TemplCommand
{
public:
  /**@name Constructors
   * Miscellaneous constructors */
  //@{
  /**
   * Constructs a newly allocated TemplCommandIn object for a command with a
   * name and an execution method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	level		The command display level
   *
   */
  TemplCommandIn(const std::string &cmd_name, void (DeviceImpl::*exe_method)(INARG), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandIn object for a command with a
   * name, an execution method and a command allowed method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	level		The command display level
   *
   */
  TemplCommandIn(const std::string &cmd_name, void (DeviceImpl::*exe_method)(INARG),
                 bool (DeviceImpl::*state_method)(const CORBA::Any &), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandIn object for a command with a
   * name, an execution method and a description for the
   * input and output command parameters.
   * The input and output command data type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	in_desc		The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandIn(const std::string &cmd_name, void (DeviceImpl::*exe_method)(INARG), const std::string &in_desc,
                 const std::string &out_desc, Tango::DispLevel level = Tango::OPERATOR);

  /**
   * Constructs a newly allocated TemplCommandIn object for a command with a
   * name, an execution method, a command allowed method and a description for
   * the input and output command parameters. The input and output command data
   * type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	in_desc	The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandIn(const std::string &cmd_name, void (DeviceImpl::*exe_method)(INARG),
                 bool (DeviceImpl::*state_method)(const CORBA::Any &) = NULL, const std::string &in_desc = "",
                 const std::string &out_desc = "", Tango::DispLevel level = Tango::OPERATOR);

  //@}

  ~TemplCommandIn() {}

  /**@name Miscellaneous methods */
  //@{
  /**
   * Initialise command input and output types.
   *
   * Set the command output type to Tango::DEV_VOID. The command input type is
   * automatically determined from the class template specialisation
   *
   */
  void init_types();

  /**
   * Invoke the command execution method given at object creation time.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client. It unpacks the data stored in
   * the CORBA Any object and invoke the user supplied command execution
   * method
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object.
   * @return The CORBA Any object returned to the client. For command created
   * with this TemplCommandIn class, this any object does not contain data.
   * @exception DevFailed If the execution method failed
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *execute(DeviceImpl *dev, const CORBA::Any &in_any);
  //@}

private:
  class TemplCommandInExt
  {
  };

  void (DeviceImpl::*exe_ptr_in)(INARG);
  std::unique_ptr<TemplCommandInExt> ext; // Class extension
};

//+-------------------------------------------------------------------------
//
// method : 		TempCommandIn class constructors
//
// description : 	instance constructor
//
//--------------------------------------------------------------------------

template <typename INARG>
TemplCommandIn<INARG>::TemplCommandIn(const std::string &s, void (DeviceImpl::*f)(INARG), Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_in(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename INARG>
TemplCommandIn<INARG>::TemplCommandIn(const std::string &s, void (DeviceImpl::*f)(INARG),
                                      bool (DeviceImpl::*a)(const CORBA::Any &), Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_in(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

template <typename INARG>
TemplCommandIn<INARG>::TemplCommandIn(const std::string &s, void (DeviceImpl::*f)(INARG), const std::string &in_desc,
                                      const std::string &out_desc, Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_in(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename INARG>
TemplCommandIn<INARG>::TemplCommandIn(const std::string &s, void (DeviceImpl::*f)(INARG),
                                      bool (DeviceImpl::*a)(const CORBA::Any &), const std::string &in_desc,
                                      const std::string &out_desc, Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_in(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

//+-------------------------------------------------------------------------
//
// method : 		init_types
//
// description : 	Initialise input and output type from the template
//			class specialisation parameters.
//
//--------------------------------------------------------------------------

template <typename INARG>
void TemplCommandIn<INARG>::init_types()
{
  out_type = Tango::DEV_VOID;
  //
  // Set intput type
  //
  set_type(typeid(INARG), in_type);
}

//+-------------------------------------------------------------------------
//
// method : 		execute
//
// description : 	Execute the method associated with the command
//			(stored in the exe_ptr data)
//
// input : - dev_ptr : pointer to the device on which the command must be
//		       executed
//	   - in_any : Incoming command data
//
// This method returns a pointer to an Any object with the command outing
// data.
//
//--------------------------------------------------------------------------

template <typename INARG>
CORBA::Any *TemplCommandIn<INARG>::execute(DeviceImpl *dev_ptr, const CORBA::Any &in_any)
{
  //
  // Execute the command associated method
  //

  INARG in_data;
  extract(in_any, in_data);
  (dev_ptr->*exe_ptr_in)(in_data);
  return insert();
}

//=============================================================================
//
//			The TemplCommandOut class
//
//
// description :	This class is a derived class of the Command class.
//			It is used to create a command from a pointer to a
//			object method which will execute the command.
//			This class is for command without inout nor output
//			parameters.
//			This class is also a base class for the template Command
//			class
//
//=============================================================================

/**
 * This class is a class representing a command in the template command model
 * with output parameter but without input parameter. The class template
 * parameter (called OUTARG) is the command output parameter type.
 *<p>Synopsis : template &lt;typename OUTARG&gt; class TemplCommandOut:public
 *TemplCommand;</p> <p>Usage : new
 *TemplCommandOut&lt;Tango::DevLong&gt;(...);</p>
 *
 *
 * @headerfile tango.h
 * @ingroup Server
 */

template <typename OUTARG>
class TemplCommandOut : public TemplCommand
{
public:
  /**@name Constructors
   * Miscellaneous constructors */
  //@{
  /**
   * Constructs a newly allocated TemplCommandOut object for a command with a
   * name and an execution method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	level		The command display level
   *
   */
  TemplCommandOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandOut object for a command with a
   * name, an execution method and a command allowed method.
   * The input and output command data type are automatically determined.
   * The input and output parameter description are set to the default String
   * "Uninitialised".
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	level		The command display level
   *
   */
  TemplCommandOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(),
                  bool (DeviceImpl::*state_method)(const CORBA::Any &), Tango::DispLevel level);

  /**
   * Constructs a newly allocated TemplCommandOut object for a command with a
   * name, an execution method and a description for the
   * input and output command parameters.
   * The input and output command data type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	in_desc		The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(), const std::string &in_desc,
                  const std::string &out_desc, Tango::DispLevel level = Tango::OPERATOR);

  /**
   * Constructs a newly allocated TemplCommandIn object for a command with a
   * name, an execution method, a command allowed method and a description for
   * the input and output command parameters. The input and output command data
   * type are automatically determined.
   *
   * @param 	cmd_name	The command name
   * @param	exe_method	Pointer to the command execution method
   * @param	state_method 	Pointer to the command allowed method
   * @param	in_desc	The command input parameter description
   * @param	out_desc	The command output parameter description
   * @param	level		The command display level
   *
   */
  TemplCommandOut(const std::string &cmd_name, OUTARG (DeviceImpl::*exe_method)(),
                  bool (DeviceImpl::*state_method)(const CORBA::Any &) = NULL, const std::string &in_desc = "",
                  const std::string &out_desc = "", Tango::DispLevel level = Tango::OPERATOR);

  //@}

  ~TemplCommandOut() {}

  /**@name Miscellaneous methods */
  //@{
  /**
   * Initialise command input and output types.
   *
   * Set the command output type to Tango::DEV_VOID. The command input type is
   * automatically determined from the class template specialisation
   *
   */
  void init_types();

  /**
   * Invoke the command execution method given at object creation time.
   *
   * This method is automtically called by the TANGO core classes when the
   * associated command is requested by a client. It invokes the user supplied
   * command execution method and packs the returned data into the outgoing
   * CORBA Any object
   *
   * @param dev The device on which the command must be executed
   * @param in_any The incoming data still packed in a CORBA Any object. For
   * command created with this TemplCommandOut class, this Any object does not
   * contain usefull data
   * @return The CORBA Any object returned to the client.
   * @exception DevFailed If the execution method failed
   * Click <a
   * href="https://tango-controls.readthedocs.io/en/latest/development/advanced/IDL.html#exceptions">here</a>
   * to read <b>DevFailed</b> exception specification
   */
  CORBA::Any *execute(DeviceImpl *dev, const CORBA::Any &in_any);
  //@}

private:
  class TemplCommandOutExt
  {
  };

  OUTARG (DeviceImpl::*exe_ptr_out)();
  std::unique_ptr<TemplCommandOutExt> ext; // Class extension
};

//+-------------------------------------------------------------------------
//
// method : 		TempCommandOut class constructors
//
// description : 	instance constructor
//
//--------------------------------------------------------------------------

template <typename OUTARG>
TemplCommandOut<OUTARG>::TemplCommandOut(const std::string &s, OUTARG (DeviceImpl::*f)(), Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_out(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename OUTARG>
TemplCommandOut<OUTARG>::TemplCommandOut(const std::string &s, OUTARG (DeviceImpl::*f)(),
                                         bool (DeviceImpl::*a)(const CORBA::Any &), Tango::DispLevel level)
    : TemplCommand(s, level),
      exe_ptr_out(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

template <typename OUTARG>
TemplCommandOut<OUTARG>::TemplCommandOut(const std::string &s, OUTARG (DeviceImpl::*f)(), const std::string &in_desc,
                                         const std::string &out_desc, Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_out(f),
      ext(nullptr)
{
  allowed_ptr = NULL;
  init_types();
}

template <typename OUTARG>
TemplCommandOut<OUTARG>::TemplCommandOut(const std::string &s, OUTARG (DeviceImpl::*f)(),
                                         bool (DeviceImpl::*a)(const CORBA::Any &), const std::string &in_desc,
                                         const std::string &out_desc, Tango::DispLevel level)
    : TemplCommand(s, in_desc, out_desc, level),
      exe_ptr_out(f),
      ext(nullptr)
{
  allowed_ptr = a;
  init_types();
}

//+-------------------------------------------------------------------------
//
// method : 		init_types
//
// description : 	Initialise input and output type from the template
//			class specialisation parameters.
//
//--------------------------------------------------------------------------

template <typename OUTARG>
void TemplCommandOut<OUTARG>::init_types()
{
  in_type = Tango::DEV_VOID;
  //
  // Set output type
  //

  set_type(typeid(OUTARG), out_type);
}

//+-------------------------------------------------------------------------
//
// method : 		execute
//
// description : 	Execute the method associated with the command
//			(stored in the exe_ptr data)
//
// input : - dev_ptr : pointer to the device on which the command must be
//		       executed
//	   - in_any : Incoming command data
//
// This method returns a pointer to an Any object with the command outing
// data.
//
//--------------------------------------------------------------------------

template <typename OUTARG>
CORBA::Any *TemplCommandOut<OUTARG>::execute(DeviceImpl *dev_ptr, TANGO_UNUSED(const CORBA::Any &in_any))
{
  //
  // Execute the command associated method
  //

  return insert((dev_ptr->*exe_ptr_out)());
}

} // namespace Tango

#endif // _COMMAND_H
