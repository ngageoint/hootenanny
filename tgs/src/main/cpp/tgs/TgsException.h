/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __SA_URGENT_EXCEPTION_H__
#define __SA_URGENT_EXCEPTION_H__

#include <tgs/TgsExport.h>

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
    Exception(const std::string& error) : _errorStr(error) { }

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

