// File: CheckOpenGLError.h
// Author: Rowan Clark
//
// This file provides the macro GL_CHECK which when place around 
// an OpenGL function, calls the function and then calls glGetError()
// to query the hardware for errors and throws an exception if
// an error has occured. Note the GL_CHECK macro is only enabled in the
// debug build of this library
//
// ------ Macro Usage ------
//
//   GL_CHECK(glUseProgram(shaderProgram));
//
// ------ GlException Usage Example -------
//
//   try
//   {
//		Code containing GLW Functions macros...
//   }
//   catch (GLW::GlException& e)
//   {
//   	std::cerr << "OpenGL Error at line " << e.GetLine()
//		   << " in file " << e.GetFilename() << std::endl
//		   << "Statement: " << e.GetStatement() << std::endl
//		   << "Error: " << e.GetError() << std::endl;
//   }

#ifndef _CHECK_OPENGL_ERROR_H_
#define _CHECK_OPENGL_ERROR_H_

#include <exception>
#include <iostream>
#include <string>

#include "glad/glad.h"

namespace GLW
{
	class GlException : public std::exception
	{
	public:
		GlException(const std::string& _error, const std::string& _statement,
			const std::string& _filename, const unsigned int _line)
			: error(_error), statement(_statement), filename(_filename), line(_line) {}
		~GlException() {}

		virtual const char* what() const throw()
		{
			return error.c_str();
		}

		const std::string GetError() { return error; }
		const std::string GetStatement() { return statement; }
		const std::string GetFilename() { return filename; }
		const unsigned int GetLine() { return line; }

	private:
		const std::string error;
		const std::string statement;
		const std::string filename;
		const int line;
	};

	void CheckOpenGLError(const std::string& _statement, const std::string& _filename, const int _line);

#ifdef _DEBUG
	#define GL_CHECK(stmt) stmt; GLW::CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
	#define GL_CHECK(stmt) stmt; stmt;
#endif

}

#endif // _CHECK_OPENGL_ERROR_H_