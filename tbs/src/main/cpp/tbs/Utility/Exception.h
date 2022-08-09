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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H


//STD Includes
#include <exception>
#include <sstream>
#include <string>

namespace tbs
{

/**
*  An Exception class extending the stl exceptions to add extra
*  description for the error
*/
class Exception : public std::exception
{
public:
  /**
   * @brief Constructor
   * @param error a message describing the error
   */
  Exception(const std::string& error)
  {
    _errorStr = error;
  }

  /**
   * @brief Constructor
   * @param lineNum the line number the exception was generated at
   * @param error a message describing the error
   */
  Exception(int lineNum, const std::string& error)
  {
    std::stringstream ss;
    ss << "Line(" << lineNum << "): " << error;
    _errorStr = ss.str();
  }

  /**
   * @brief Destructor
   */
  ~Exception() throw ()
  {

  }

  /**
   * @brief provides erro message for the exception
   * @return the error string passed in during construction
   */
  virtual const char* what() const throw()
  {
    return _errorStr.data();
  }

protected:

  std::string _errorStr;
};

}

#endif // EXCEPTION_H
