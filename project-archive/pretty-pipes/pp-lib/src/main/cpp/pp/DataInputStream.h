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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */

#ifndef DATAINPUTSTREAM_H
#define DATAINPUTSTREAM_H

// Qt
#include <QtEndian>
#include <QString>

// Standard
#include <assert.h>
#include <istream>
#include <stdint.h>

#include "Exception.h"

namespace pp
{

class DataInputStream
{
public:
  DataInputStream(std::istream& in) : _in(in) {}

  template<class ValueClass>
  int read(ValueClass& value);

  int read(char* data, int length);

  char readByte();

  double readDouble();

  /**
   * Reads an signed integer off the stream in network encoding and converts it to the host
   * encoding.
   */
  int readInt();

  /**
   * Reads an signed long off the stream in network encoding and converts it to the host
   * encoding.
   */
  long readLong();

  /**
   * Similar to the java readUTF() method, but no encoding conversion is attempted. The method first
   * reads an unsigned 16bit value and then uses that to determine the length of the string. The
   * string is then read into a std::string object and returned.
   */
  std::string readString();

  uint32_t readUnsignedInt();

  uint16_t readUnsignedShort();

  QString readUTF();

private:
  std::istream& _in;

  void _checkEof(int s)
  {
    if (_in.gcount() == 0 && _in.eof() == true)
    {
      // no problem
    }
    else if (_in.gcount() != s)
    {
      throw Exception("Unable to read requested data.");
    }
  }
};

inline int DataInputStream::read(char* data, int length)
{
  _in.read(data, length);
  _checkEof(length);
  return _in.gcount();
}

template<class ValueClass>
inline int DataInputStream::read(ValueClass& value)
{
  _in.read((char*)&value, sizeof(value));
  _checkEof(sizeof(value));
  return _in.gcount();
}

inline char DataInputStream::readByte()
{
  char v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return v;
}

inline double DataInputStream::readDouble()
{
  double v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return v;
}

inline int DataInputStream::readInt()
{
  int v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return qFromBigEndian((quint32)v);
}

inline long DataInputStream::readLong()
{
  long v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return qFromBigEndian((quint64)v);
}

inline std::string DataInputStream::readString()
{
  unsigned short v = readUnsignedShort();

  std::string result;
  result.resize(v);
  _in.read((char*)result.data(), v);
  _checkEof(v);
  return result;
}

inline uint32_t DataInputStream::readUnsignedInt()
{
  int v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return (uint32_t)qFromBigEndian((quint32)v);
}

inline uint16_t DataInputStream::readUnsignedShort()
{
  unsigned short v;
  _in.read((char*)&v, sizeof(v));
  _checkEof(sizeof(v));
  return qFromBigEndian((quint16)v);
}

inline QString DataInputStream::readUTF()
{
  unsigned short v = readUnsignedShort();

  std::string result;
  result.resize(v);
  _in.read((char*)result.data(), v);
  _checkEof(v);
  return QString::fromUtf8(result.data(), v);
}


}

#endif // DATAINPUTSTREAM_H
