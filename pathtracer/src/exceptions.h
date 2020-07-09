//
// Copyright (C) 2019, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
//

/// @file      exceptions.h
/// @author    Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
/// @copyright (C) 2019, Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
/// @date      2019-09-25
/// @brief     Definition of exception classes

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

/// @brief Base class for all exceptions. This file defines common exceptions. The defined exceptions are inspired by
///  the Python exceptions. At the moment, following exceptions are defined:
///       BaseException
///           +-- LookupError
///           +-- ParsingError
///           +-- InvalidFormatError
///           +-- OSError
///           |    +-- FileNotFoundError
///           +-- NotImplementedError
///           +-- RuntimeError
///           +-- ValueError
///           +-- GltfError
/// 
///  Exceptions can be thrown as in this example:
/// 
///      Throw FileNotFoundError("Cannot find file foo")
struct BaseException : public std::exception
{
    /// Description of what went wrong
    std::string m_what;

    /// Create a new BaseException exception
    BaseException(std::string what = "BaseException") { m_what = what; }

    /// Return string description of what went wrong
	const char *what() const throw () { return m_what.c_str(); }
};

/// The NotImplementedError exception is raised when a method or function has not been implemented yet.
struct NotImplementedError : public BaseException
{
    /// Create NotImplementedError exception
    NotImplementedError(std::string what="NotImplementedError") : BaseException(what) {}
};

/// The InvalidFormatError exception is raised when a file (for example a glTF file) was parsed correctly, but the content is in a wrong format.
struct InvalidFormatError : public BaseException
{
    /// Create InvalidFormatError exception
    InvalidFormatError(std::string what="InvalidFormatError") : BaseException(what) {}
};

/// The ParsingError exception is raised when a file could not be parsed.
struct ParsingError : public BaseException
{
    /// Create ParsingError exception
    ParsingError(std::string what="ParsingError") : BaseException(what) {}
};

/// The RuntimeError exception is raised when an error is detected that doesn't fall in any other category.
struct RuntimeError : public BaseException
{
    /// Create new ValueError exception
    RuntimeError(std::string what="RuntimeError") : BaseException(what) {}
};

/// The GltfError exception is raised when parsing an invalid gltf file.
struct GltfError : public BaseException
{
    /// Create new ValueError exception
    GltfError(std::string what="GltfError") : BaseException(what) {}
};

/// The ValueError exception is raised when a method or function receives an argument with an inappropriate value.
struct ValueError : public BaseException
{
    /// Create new ValueError exception
    ValueError(std::string what="ValueError") : BaseException(what) {}
};

/// The LookupError exception is raised when a value could not be found.
struct LookupError : public BaseException
{
    /// Create new LookupError exception
    LookupError(std::string what="LookupError") : BaseException(what) {}
};


/// The OSError exception is raised when a system function returns a system-related error
struct OSError : public BaseException
{
    /// Create new OSError exception
    OSError(std::string what="OSError") : BaseException(what) {}
};

/// The FileNotFoundError exception is raised when a file or directory is requested that doesn't exist.
struct FileNotFoundError : public OSError
{
    /// Create new FileNotFoundError exception
    FileNotFoundError(std::string what="FileNotFoundError") : OSError(what) {}
};

#endif // EXCEPTION_H

