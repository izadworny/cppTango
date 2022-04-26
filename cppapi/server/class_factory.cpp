//+===========================================================================
//
// file :		class_factory.cpp
//
// description :	C++ source file for a dummy DServer::class_factory()
//			method. For server, this method is redefined by the
//			DS programmer in its own file and the linker will not
//			use this file. For client where ther is no
//			class_factory() method, this one will be used by the
//			linker
//			This works only if class_factory() is in its own file
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
//
//-===========================================================================

#include <dserver.h>
#ifdef __darwin__
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include <regex>
#endif

namespace Tango
{

//
// There is a trick to build Windows DLL and to use it in a server.
// To build a DLL, you have to pass your code into the linker. The linker
// try to resolve all symbols. Therefore, it resolves the DServer::class_factory
// with this default class_factory. For a server, it's annoying because we
// want the user DServer::class_factory to be called !!
// For all other kinds of lib (Unix type or Windows static), the linker is used
// only when the application is built and symbols are resolved first from the
// supplied object files and only if not found in object files with symbols
// defined in libraries.
// Therefore, the trick for DLL, is inside the default DServer::class_factory
// to force calling the user DServer::class_factory
// To do this, we use the GetProcAddress and we defines in dserver.h file
// the class_factory has "export".
// There is another trick to cast the pointer returned by GetProcAddress
// which is a pointer to function into a pointer to method.
// This is done using an union and initializing the pointeur to method
// with a local method and then modifying it
//

#ifdef _TG_WINDOWS_
typedef void (DServer::*PTR)(void);
typedef union _convertor
{
	PTR d;
	FARPROC s;
}convertor;
#elif __darwin__
typedef void (DServer::*PTR)(void);
typedef union _convertor
{
	PTR d;
	void *s;
} convertor;
#endif


void DServer::class_factory()
{
#ifdef _TG_WINDOWS_
	Tango::Util *tg = Tango::Util::instance();
	std::string exe_name = tg->get_ds_exec_name();
	exe_name = exe_name + ".exe";
	HMODULE mod;
	FARPROC proc;
	convertor conv;
	PTR tmp;

	if ((mod = GetModuleHandle(exe_name.c_str())) == NULL)
	{
		std::cerr << "Oops, no class defined in this server. Exiting ..." << std::endl;
		exit(-1);
	}

//
// Use the mangled name to find the user DServer::class_factory method
//
// Due to the fact that on Windows 64 bits we have both _WIN32 and _WIN64
// defined, start by testing _WIN64 (See tango_config.h)
//

#ifdef _WIN64
	if ((proc = GetProcAddress(mod,"?class_factory@DServer@Tango@@AEAAXXZ")) == NULL)
#elif _WIN32 /* WIN32 */
	if ((proc = GetProcAddress(mod,"?class_factory@DServer@Tango@@AAEXXZ")) == NULL)
#endif
	{
		std::cerr << "Oops, no class defined in this server. Exiting ..." << std::endl;
		exit(-1);
	}
	else
	{
		conv.d = &DServer::stop_polling;
		conv.s = proc;

		tmp = conv.d;
		(this->*tmp)();
	}

#elif __darwin__
    Tango::Util* tg{Tango::Util::instance()};
    const std::string exe_name{tg->get_ds_unmodified_exec_name()};

    // Figure out what the full path of exe_name is.
    char full_path_exe_name[PATH_MAX]{};

    // We'll use this to check if the file found in the path is not a directory.
    struct stat exec_file_stat{};
    if((realpath(exe_name.c_str(), full_path_exe_name) == nullptr) || (stat(full_path_exe_name, &exec_file_stat) != 0) ||  ((exec_file_stat.st_mode & S_IFDIR) == S_IFDIR))
    {
        // OK. We got here because
        // - realpath returned nullptr which indicates that it could
        //   not resolve the full path for the Device Server's executable.
        // - The stat call failed. Not much we can do.
        // - The stat call reveals that there exists a directory of the same
        //   in the path.
        // The shell must have found the executable in one of the directories in
        // ${PATH}, Otherwise this code here would now not be executed.
        // Therefore I will now iterate over all directories in ${PATH}
        // and try to find the executable there.

        // But first get the directories in ${PATH} and split 'em up at ":".
        // Create a handy lambda function for the splitting.
        auto split = [](const std::string& path_env, const std::string delimiter = ":") -> std::vector< std::string >
        {
            std::regex regex{delimiter};
            return { std::sregex_token_iterator(std::begin(path_env), std::end(path_env), regex, -1), std::sregex_token_iterator() };
        };

        // Now fetch the paths from the ${PATHS} environment variable.
        const std::string path_environment{getenv("PATH")};
        // Call the lambda function to split it at every ":".
        auto paths{split(path_environment)};
        // Now the std::vector paths should contain one path per entry.

        // Shortened name of the executable, contains just the name
        // of the executable without any "/" or ".".
        const std::string shortened_exe_name{tg->get_ds_exec_name()};

        // A lambda funtion that checks if a file exists in a path.
        // I will iterate over every entry in paths and call this lambda.
        auto file_exists_in_path = [&full_path_exe_name, &shortened_exe_name](const std::string& path) -> bool
        {
            const std::string full_path{path + "/" + shortened_exe_name};
            // Check if the executable exists in the path and that it is not
            // a directory of the same name.
            struct stat exec_file_stat{};
            if((stat(full_path.c_str(), &exec_file_stat) == 0) &&  ((exec_file_stat.st_mode & S_IFDIR) == 0))
            {
                // Found it!
                return true;
            }

            return false;
        };

        // Call file_exists_in_path for every item in paths until
        // file_exists_in_path returns true and returns an iterator which
        // points to the correct value in paths. This then means that we have
        // found the executable.
        // If the executable has still not been found, then something is wrong
        // and we exit.
        auto paths_iterator{std::find_if(std::begin(paths), std::end(paths), file_exists_in_path)};
        if(paths_iterator != std::end(paths))
        {
            // Found the executable, update the variable.
            const std::string full_path{*paths_iterator + "/" + shortened_exe_name};
            std::memset(full_path_exe_name, 0, PATH_MAX);
            std::strncpy(full_path_exe_name, full_path.c_str(),
                full_path.size());
        }
        else
        {
            std::cerr << "The Device Server's executable \""
                << exe_name
                << "\" cannot be resolved to a full path name. This means "
                    "that the Device Server cannot be started. This usually "
                    "happens if the executable for the Device Server is in "
                    "one of the directories in ${PATH}. Unfortunately could "
                    "the executable not be found in any of the PATH "
                    "directories. This is what you can do now: Start the "
                    "Device Server again, but run it by providing the full "
                    "path name plus the executable, e.g. "
                    "/opt/foo/MyDeviceServer.\n";
                // Exit with the proper error code.
                exit(-ENOENT);
        }
        // If we reach this point, then full_path_exe_name should contain
        // the full path to the Device Server executable and hence the call
        // to dlopen further down should be successful.
    }

    void* mod{nullptr};
    if ((mod = dlopen(full_path_exe_name, RTLD_LAZY)) == nullptr)
    {
        std::cerr << "Error: Cannot find the file "
            << full_path_exe_name
            << ". Therefore the file cannot be checked for exported Device "
                "classes. This is an error and the Device Server cannot "
                "continue. The OS error is:\n\t"
            << dlerror()
            << "\nExiting.\n";
        exit(-ENOENT);
    }

    //
    // Use the mangled name to find the user DServer::class_factory method in
    // the executable.
    //
    void* proc{nullptr};
    const std::string darwin_class_factory_symbol
        {"_ZN5Tango7DServer13class_factoryEv"};
    if ((proc = dlsym(mod, darwin_class_factory_symbol.c_str())) == nullptr)
    {
        std::cerr << "Error: When inspecting the file \""
            << full_path_exe_name
            << "\" the symbol for the Tango Class Factory ("
            << darwin_class_factory_symbol
            << ") could not be found. This likely means that the inspected "
                "file does not contain a Tango Device Class. The OS error "
                "is:\n\t"
            << dlerror()
            << "\nCannot continue, because there is no class defined in this "
                "Device Server. Exiting.\n";
        exit(-ENOENT);
    }
    else
    {
        convertor conv{.d = &DServer::stop_polling};
        conv.s = proc;
        PTR tmp{conv.d};
        (this->*tmp)();
    }
#else
		std::cerr << "Oops, no class defined in this server. Exiting ..." << std::endl;
		exit(-1);
#endif

}

} // End of Tango namespace
