/****************************************************************************
* Copyright (c) 2005-2008 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  
* All rights reserved.
*****************************************************************************/

#ifndef __SA_URGENT_EXCEPTION_H__
#define __SA_URGENT_EXCEPTION_H__

#include "TgsExport.h"

//STD Includes
#include <exception>
#include <sstream>
#include <string>


namespace Tgs
{
  /**
  *  An Exception class extending the stl exceptions to add extra
  *  description for the error
  */
  class Exception : public std::exception
  {
  public:
    /** 
    * A constructor
    *
    * @param error - a string describing the error
    */
    Exception(const std::string& error)
    {
      _errorStr = error;
    }

    /**
     * @brief Exception an exception object
     *
     * @param lineNum the line number throwing the exception
     * @param errorMessage the error message
     */
    Exception(unsigned int lineNum, const std::string & errorMessage)
    {
      std::stringstream ss;
      ss <<  "Line( " << lineNum << "): -->" << errorMessage << std::endl;
      _errorStr = ss.str();
    }

    /**
     * @brief Exception an exception object
     *
     * Use to propagate a previous exception
     *
     * @param className the name of the class throwing the exception
     * @param functionName the name of the function throwning the exception
     * @param lineNum the line number throwing the exception
     * @param error the previous error in the exception chain
     */
    Exception(std::string className, std::string functionName, unsigned int lineNum,
      const Exception & error)
    {
      std::stringstream ss;
      ss <<  className << "::" << functionName << " Line( " << lineNum << "): -->" << error.what() <<
        std::endl << std::endl;
      _errorStr = ss.str();
    }

    /**
     * @brief Exception an exception object
     *
     * Use to propagate a previous exception
     *
     * @param className the name of the class throwing the exception
     * @param functionName the name of the function throwning the exception
     * @param lineNum the line number throwing the exception
     * @param errorMessage the description of the error
     */
    Exception(std::string className, std::string functionName, unsigned int lineNum,
      const std::string & errorMessage)
    {
      std::stringstream ss;
      ss <<  className << "::" << functionName << " Line( " << lineNum << "): -->" <<
        errorMessage << std::endl << std::endl;
      _errorStr = ss.str();
    }

    /**
     * A destructor
     */
    ~Exception() throw ()
    {

    }

    /** 
    * @returns the error string provided in the constructor
    */
    virtual const char* what() const throw()
    {
      return _errorStr.data();
    }

  protected:

    std::string _errorStr;
  };
}

#endif

