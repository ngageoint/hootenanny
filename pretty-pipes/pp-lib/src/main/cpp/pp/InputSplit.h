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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef INPUTFORMATSPLIT_H
#define INPUTFORMATSPLIT_H

// Standard Includes
#include <string>
#include <vector>

namespace pp
{

class InputSplit
{
public:
  static std::string className() { return "pp::InputSplit"; }

  InputSplit() {}

  virtual ~InputSplit() {}

#ifndef SWIG
  /**
   * Creates a new InputSplit of the appropriate type and copies this into the new object. Caller
   * owns the result.
   * This method uses getClassName() and create() to construct the new object and readFields/
   * writeFields to copy the data.
   */
  virtual InputSplit* copy() const;
#endif

  static InputSplit* create(const std::string& className);

  virtual std::string getClassName() const = 0;

  virtual long long getLength() const = 0;

  virtual const std::string& getLocations() const = 0;

  virtual const std::string& getPath() const = 0;

  virtual long long getStart() const = 0;

#ifndef SWIG
  void readFields(const std::string& bytes) { readFields((char*)bytes.data(), bytes.size()); }
#endif

  virtual void readFields(char* byteArray, long len) = 0;

  /**
   * Return a newly allocated array, caller owns it. The size of the array should be put in
   * len. The strange type "signed char*" is to help SWIG out with determining how to treat the
   * return type.
   */
  virtual signed char* writeFields(size_t* len) const = 0;

  void writeFields(std::string& bytes) const;
};

}

#endif // INPUTFORMATSPLIT_H
