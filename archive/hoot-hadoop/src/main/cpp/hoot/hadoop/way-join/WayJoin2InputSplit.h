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

#ifndef WAYJOIN2INPUTSPLIT_H
#define WAYJOIN2INPUTSPLIT_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/InputSplit.h>

// Standard Includes
#include <string>
#include <vector>

namespace hoot
{

class WayJoin2InputSplit : public pp::InputSplit
{
public:

  enum
  {
    PbfInputSplitType = 0,
    FileInputSplitType = 1
  };

  static std::string className() { return "hoot::WayJoin2InputSplit"; }

  WayJoin2InputSplit();

  virtual ~WayJoin2InputSplit() {}

  virtual std::string getClassName() const { return className(); }

  virtual long long getLength() const;

  virtual const std::string& getLocations() const;

  virtual const std::string& getPath() const;

  boost::shared_ptr<const InputSplit> getChildSplit() const { return _childSplit; }

  virtual long long getStart() const;

  virtual void readFields(char* byteArray, long len);

  /**
   * Sets the child split. This object takes ownership of the split.
   */
  void setSplit(boost::shared_ptr<InputSplit> split);

  /**
   * Return a newly allocated array, caller owns it. The size of the array should be put in
   * len. The strange type "signed char*" is to help SWIG out with determining how to treat the
   * return type.
   */
  virtual signed char* writeFields(size_t* len) const;

private:
  /**
   * Contains the "real" split data.
   */
  boost::shared_ptr<InputSplit> _childSplit;
  char _type;
};

}

#endif // WAYJOIN2INPUTSPLIT_H
