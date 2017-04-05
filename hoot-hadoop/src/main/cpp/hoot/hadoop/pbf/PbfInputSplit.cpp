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

#include "PbfInputSplit.h"

// Hoot
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Factory.h>

// Standard
#include <sstream>
#include <stdio.h>
#include <string.h>

namespace hoot
{
using namespace pp;

PP_FACTORY_REGISTER(pp::InputSplit, PbfInputSplit)

PbfInputSplit::PbfInputSplit()
{
  reset();
}

PbfInputSplit::PbfInputSplit(const PbfInputSplit& from)
{
  _copy(from);
}

void PbfInputSplit::_copy(const PbfInputSplit& from)
{
  _start = from._start;
  _length = from._length;
  _headers = from._headers;
  _locations = from._locations;
  _path = from._path;
}

void PbfInputSplit::addHeader(long pos, long length)
{
  if (_start == -1)
  {
    _start = pos;
    _length = 0;
  }
  _length = std::max(_length, (pos + length) - _start);
  _headers.push_back(pos);
}

void PbfInputSplit::readFields(char *byteArray, long len)
{
  string s(byteArray, (size_t)len);

  stringstream sin(s, stringstream::in);
  DataInputStream dis(sin);

  _start = dis.readLong();
  _length = dis.readLong();
  _path = dis.readString();
  _locations = dis.readString();
  int headerCount = dis.readInt();
  for (int i = 0; i < headerCount; i++)
  {
    long h = dis.readLong();
    _headers.push_back(h);
  }
}

void PbfInputSplit::reset()
{
  _start = -1;
  _length = 0;
  _headers.clear();
  _path.clear();
  _locations.clear();
}

signed char* PbfInputSplit::writeFields(size_t* len) const
{
  stringstream sout(stringstream::out);
  DataOutputStream dos(sout);

  dos.writeLong(_start);
  dos.writeLong(_length);
  dos.writeString(_path);
  dos.writeString(_locations);
  dos.writeInt(_headers.size());
  for (size_t i = 0; i < _headers.size(); i++)
  {
    dos.writeLong(_headers.at(i));
  }
  sout.flush();

  *len = sout.str().length();
  signed char* result = new signed char[*len];
  memcpy(result, sout.str().data(), *len);
  return result;
}


}
