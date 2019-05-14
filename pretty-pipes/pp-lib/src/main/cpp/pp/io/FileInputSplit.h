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

#ifndef FILEINPUTSPLIT_H
#define FILEINPUTSPLIT_H

// Pretty Pipes
#include <pp/InputSplit.h>

// Standard
#include <string>

namespace pp
{

class FileInputSplit : public InputSplit
{
public:

  static std::string className() { return "pp::FileInputSplit"; }

  FileInputSplit();

  virtual ~FileInputSplit() {}

  virtual std::string getClassName() const { return className(); }

  virtual long long getLength() const { return _length; }

  virtual const std::string& getLocations() const { return _locations; }

  virtual const std::string& getPath() const { return _path; }

  virtual long long getStart() const { return _start; }

  void setLength(long length) { _length = length; }

  void setLocations(const std::string& locations) { _locations = locations; }

  void setPath(const std::string& path) { _path = path; }

  void setStart(long start) { _start = start; }

  virtual void readFields(char* byteArray, long len);

  /**
   * Return a newly allocated array, caller owns it. The size of the array should be put in
   * len. The strange type "signed char*" is to help SWIG out with determining how to treat the
   * return type.
   */
  virtual signed char* writeFields(size_t* len) const;

private:

  std::string _path;
  long _start;
  long _length;
  std::string _locations;
};

}

#endif // FILEINPUTSPLIT_H
