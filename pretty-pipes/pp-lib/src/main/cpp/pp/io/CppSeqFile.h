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

#ifndef CPPSEQFILE_H
#define CPPSEQFILE_H

#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <hadoop/Pipes.hh>
#pragma GCC diagnostic warning "-Wunused-parameter"

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Exception.h>

// Qt
#include <QCryptographicHash>
#include <QDateTime>
#include <QUuid>

// Standard
#include <stdio.h>

namespace pp
{

/**
 * Implements a simplified version of Hadoop's SequenceFile class. No attempt has been made to be
 * backwards compatible, but, while time consuming it shouldn't be too difficult to make it
 * compatible.
 */
class CppSeqFile
{
public:
  const static unsigned int MAGIC_HEADER = 0x41de5d00;
  const static int SYNC_HASH_SIZE = 16;
  const static int SYNC_SIZE = 4 + SYNC_HASH_SIZE;
  const static int SYNC_ESCAPE = -1;
  const static int SYNC_INTERVAL = SYNC_SIZE * 100;

  class Reader : public HadoopPipes::Closable
  {
  public:

    Reader(std::istream& in, long start = 0, long length = -1);

    long getPosition() { return _in.tellg(); }

    template<class KeyClass, class ValueClass>
    bool next(KeyClass& key, ValueClass& value);

    bool nextRaw(std::string& key, std::string& value);

    template<class KeyClass, class ValueClass>
    bool nextFixed(KeyClass& key, ValueClass& value);

    virtual void close();

    void sync(long pos);

    bool syncSeen() { return _syncSeen; }

  private:
    std::istream& _in;
    DataInputStream _dis;
    QByteArray _sync;
    QByteArray _syncBuf;
    bool _syncSeen;
    long _start;
    long _end;

    void _init();

    void _verifySync();
  };

  class Writer : public HadoopPipes::Closable
  {
  public:

    Writer(std::ostream& out);

    template<class KeyClass, class ValueClass>
    void append(const KeyClass& key, const ValueClass& value);

    void appendRaw(const std::string& key, const std::string& value);

    /**
     * Append fixed sized objects. This works well for simple object types such as elementary data
     * types (e.g. int, double) and structs that contain elementary data types. Variable sized data
     * types such as vectors or sets require something more complex like append().
     */
    template<class KeyClass, class ValueClass>
    void appendFixed(const KeyClass& key, const ValueClass& value);

    void appendRaw(const char* keyData, size_t keySize, const char* valueData, size_t valueSize);

    void finalizeFileHeader();

    void initializeFileHeader();

    virtual void close();

    virtual void sync();

  private:
    std::ostream& _out;
    DataOutputStream _dos;
    QByteArray _sync;
    std::streampos _lastSync;

    void _init();
  };

  CppSeqFile();

};

template<class KeyClass, class ValueClass>
bool CppSeqFile::Reader::next(KeyClass& key, ValueClass& value)
{
  std::string keyBytes, valueBytes;
  if (nextRaw(keyBytes, valueBytes))
  {
    key.readFields((char*)keyBytes.data(), keyBytes.size());
    value.readFields((char*)valueBytes.data(), valueBytes.size());
    return true;
  }
  else
  {
    return false;
  }
}

template<class KeyClass, class ValueClass>
bool CppSeqFile::Reader::nextFixed(KeyClass& key, ValueClass& value)
{
  if (_in.eof())
  {
    return false;
  }
  else
  {
    int recordLength = _dis.readInt();
    if (_in.eof())
    {
      return false;
    }
    if (recordLength == -1)
    {
      _verifySync();
      recordLength = _dis.readInt();
      _syncSeen = true;
      if (_in.eof() || _in.gcount() == 0)
      {
        return false;
      }
    }
    else
    {
      _syncSeen = false;
    }
    if (recordLength <= 0)
    {
      throw Exception(QString("Record length was not positive. (recordLength: %1 position: %2)").
        arg(recordLength).arg(_in.tellg()).toStdString());
    }

    int keyLength = _dis.readInt();
    if (_in.gcount() == 0 || _in.eof())
    {
      return false;
    }
    int valueLength = recordLength - sizeof(int32_t) * 2 - keyLength;
    if (keyLength != sizeof(key))
    {
      throw Exception("Key data size does not match Key class size.");
    }
    if (valueLength != sizeof(value))
    {
      throw Exception("Value data size does not match value class size.");
    }
    _dis.read(key);
    _dis.read(value);

    return true;
  }
}


template<class KeyClass, class ValueClass>
void CppSeqFile::Writer::appendFixed(const KeyClass& key, const ValueClass& value)
{
  _dos.writeInt(sizeof(key) + sizeof(value) + sizeof(int32_t) * 2);
  _dos.writeInt(sizeof(key));
  _dos.write((char*)&key, sizeof(key));
  _dos.write((char*)&value, sizeof(value));

  if (_dos.bytesWritten() - _lastSync > SYNC_INTERVAL)
  {
    sync();
  }
}

template<class KeyClass, class ValueClass>
void CppSeqFile::Writer::append(const KeyClass& key, const ValueClass& value)
{
  size_t keyLength;
  signed char* keyBytes = key.writeFields(&keyLength);
  size_t valueLength;
  signed char* valueBytes = value.writeFields(&valueLength);

  int recordLength = sizeof(int32_t) + sizeof(int32_t) + keyLength + valueLength;
  _dos.writeInt(recordLength);
  _dos.writeInt(keyLength);
  _dos.write((char*)keyBytes, keyLength);
  _dos.write((char*)valueBytes, valueLength);

  if (_dos.bytesWritten() - _lastSync > SYNC_INTERVAL)
  {
    sync();
  }

  delete [] keyBytes;
  delete [] valueBytes;
}


}

#endif // CPPSEQFILE_H
