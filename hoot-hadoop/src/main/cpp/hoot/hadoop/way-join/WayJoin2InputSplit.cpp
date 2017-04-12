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

#include "WayJoin2InputSplit.h"

// Hoot
#include <hoot/hadoop/pbf/PbfInputSplit.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Factory.h>
#include <pp/io/FileInputSplit.h>

// Standard
#include <sstream>

namespace hoot
{

PP_FACTORY_REGISTER(pp::InputSplit, WayJoin2InputSplit)

WayJoin2InputSplit::WayJoin2InputSplit()
{
  _type = -1;
}

long long WayJoin2InputSplit::getLength() const
{
  return _childSplit->getLength();
}

const std::string& WayJoin2InputSplit::getLocations() const
{
  return _childSplit->getLocations();
}

const std::string& WayJoin2InputSplit::getPath() const
{
  return _childSplit->getPath();
}

long long WayJoin2InputSplit::getStart() const
{
  return _childSplit->getStart();
}

void WayJoin2InputSplit::readFields(char* byteArray, long len)
{
  std::string s(byteArray, len);
  stringstream ss(s, stringstream::in);
  pp::DataInputStream dis(ss);

  _type = dis.readByte();

  if (_type == PbfInputSplitType)
  {
    _childSplit.reset(new PbfInputSplit());
  }
  else if (_type == FileInputSplitType)
  {
    _childSplit.reset(new pp::FileInputSplit());
  }
  else
  {
    throw HootException("Unexpected input split type.");
  }

  int size = dis.readInt();
  LOG_TRACE("read length: " << len);
  vector<char> childData(size);
  dis.read(childData.data(), size);
  _childSplit->readFields(childData.data(), size);
}

void WayJoin2InputSplit::setSplit(boost::shared_ptr<InputSplit> split)
{
  if (dynamic_cast<PbfInputSplit*>(split.get()) != NULL)
  {
    _type = PbfInputSplitType;
  }
  else if (dynamic_cast<pp::FileInputSplit*>(split.get()) != NULL)
  {
    _type = FileInputSplitType;
  }
  else
  {
    throw InternalErrorException("Expected either PbfInputSplit or pp::FileInputSplit.");
  }
  _childSplit = split;
}

/**
 * Return a newly allocated array, caller owns it. The size of the array should be put in
 * len. The strange type "signed char*" is to help SWIG out with determining how to treat the
 * return type.
 */
signed char* WayJoin2InputSplit::writeFields(size_t* len) const
{
  size_t childLen;
  signed char* childData = _childSplit->writeFields(&childLen);
  stringstream ss(stringstream::out);
  pp::DataOutputStream dos(ss);

  dos.writeByte(_type);
  dos.writeInt(childLen);
  dos.write((const char*)childData, childLen);
  ss.flush();
  *len = ss.str().length();
  signed char* result = new signed char[ss.str().length()];
  memcpy(result, ss.str().data(), ss.str().length());
  LOG_TRACE("write length: " << ss.str().length());

  return result;
}

}
