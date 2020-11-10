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

#ifndef DATAOUTPUTSTREAM_H
#define DATAOUTPUTSTREAM_H

// Qt
#include <QtEndian>
#include <QString>

// Standard
#include <ostream>
#include <stdint.h>

#include "Exception.h"

namespace pp
{

class DataOutputStream
{
public:
  DataOutputStream(std::ostream& out);

  /**
   * Write the specified data in its raw form. No addition information is stored before or after
   * the data written.
   */
  void write(const char* data, size_t length);

  void writeByte(char v);

  void writeDouble(double v);

  /**
   * Writes a signed integer to the stream in network encoding (big endian).
   */
  void writeInt(int v);

  /**
   * Writes a signed long to the stream in network encoding (big endian).
   */
  void writeLong(long v);

  /**
   * Similar to the java writeUTF() method, but no encoding conversion is attempted. The method
   * first writes an unsigned 16bit value and then uses that to determine the length of the string.
   * The string is then written to the stream. Null termination is not honored and no null
   * termination value is artificially appended to the stream.
   */
  void writeString(const std::string& v);

  void writeUnsignedInt(uint32_t v);

  void writeUnsignedShort(uint16_t v);

  void writeUTF(const QString& str);

  std::streamsize bytesWritten() { return _bytesWritten; }

private:
  std::ostream& _out;
  std::streamsize _bytesWritten;
};

inline void DataOutputStream::write(const char* data, size_t length)
{
  _out.write(data, length);
  _bytesWritten += length;
}

inline void DataOutputStream::writeByte(char v)
{
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeDouble(double v)
{
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeInt(int v)
{
  v = qToBigEndian((quint32)v);
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeLong(long v)
{
  v = qToBigEndian((quint64)v);
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeString(const std::string& v)
{
  if (v.size() > 65535)
  {
    throw Exception("Only strings shorter than 65536 are supported.");
  }
  writeUnsignedShort(v.size());
  _out.write((char*)v.data(), v.size());
  _bytesWritten += v.size();
}

inline void DataOutputStream::writeUnsignedInt(uint32_t v)
{
  v = qToBigEndian(v);
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeUnsignedShort(uint16_t v)
{
  v = qToBigEndian(v);
  _out.write((char*)&v, sizeof(v));
  _bytesWritten += sizeof(v);
}

inline void DataOutputStream::writeUTF(const QString& str)
{
  QByteArray arr = str.toUtf8();
  writeUnsignedShort(arr.size());
  _out.write(arr.constData(), arr.size());
  _bytesWritten += arr.size();
}


}

#endif // DATAOUTPUTSTREAM_H
