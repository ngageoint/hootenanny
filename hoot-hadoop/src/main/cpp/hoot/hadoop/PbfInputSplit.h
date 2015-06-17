/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef INPUTSPLIT_H
#define INPUTSPLIT_H

// Standard Includes
#include <string>

// Pretty Pipes
#include <pp/InputSplit.h>

namespace hoot
{

using namespace std;

/**
 * Due to the complexities of hadoop I'm not testing this class directly. There are indirect
 * test in place that excersize the class. Writing a unit test specific to this class that actually
 * excersizes all the complexities in Hadoop is hard to jusitify.
 */
class PbfInputSplit : public pp::InputSplit
{
public:

  static std::string className() { return "hoot::PbfInputSplit"; }

  PbfInputSplit();

  PbfInputSplit(const PbfInputSplit& from);

  virtual ~PbfInputSplit() {}

  void addHeader(long pos, long length);

  virtual std::string getClassName() const { return className(); }

  const vector<long>& getHeaders() const { return _headers; }

  virtual long long getLength() const { return _length; }

  virtual const std::string& getLocations() const { return _locations; }

  virtual const std::string& getPath() const { return _path; }

  virtual long long getStart() const { return _start; }

  bool isValid() const { return _start != -1; }

  PbfInputSplit& operator=(const PbfInputSplit& from) { _copy(from); return *this; }

  virtual void readFields(char *byteArray, long len);

  void reset();

  void setLocations(const string& locations) { _locations = locations; }

  void setLength(long length) { _length = length; }

  void setPath(const string& path) { _path = path; }

  void setStart(long start) { _start = start; }

  virtual signed char* writeFields(size_t* len) const;

private:
  long _length;
  string _locations;
  string _path;
  long _start;
  vector<long> _headers;
  static string _className;


  void _copy(const PbfInputSplit& from);
};

}

#endif // INPUTSPLIT_H
