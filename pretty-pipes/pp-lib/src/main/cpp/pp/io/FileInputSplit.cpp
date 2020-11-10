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

#include "FileInputSplit.h"

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Factory.h>

// Standard
#include <iostream>
#include <sstream>

using namespace std;

namespace pp
{

PP_FACTORY_REGISTER(InputSplit, FileInputSplit)

FileInputSplit::FileInputSplit()
{
}

void FileInputSplit::readFields(char *byteArray, long len)
{
  string s(byteArray, (size_t)len);
  stringstream sin(s, stringstream::in);
  DataInputStream dis(sin);

  _start = dis.readLong();
  _length = dis.readLong();
  _path = dis.readString();
  _locations = dis.readString();
}

signed char* FileInputSplit::writeFields(size_t* len) const
{
  stringstream sout(stringstream::out);
  DataOutputStream dos(sout);

  dos.writeLong(_start);
  dos.writeLong(_length);
  dos.writeString(_path);
  dos.writeString(_locations);
  sout.flush();

  *len = sout.str().length();
  signed char* result = new signed char[*len];
  memcpy(result, sout.str().data(), *len);
  return result;
}

}
