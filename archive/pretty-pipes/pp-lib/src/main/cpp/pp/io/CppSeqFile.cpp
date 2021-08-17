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

#include "CppSeqFile.h"

using namespace std;

namespace pp
{

CppSeqFile::CppSeqFile()
{
}

////
// CppSeqFile::Reader
////

CppSeqFile::Reader::Reader(istream& in, long start, long length) :
  _in(in), _dis(in)
{
  _start = start;
  if (length >= 0)
  {
    _end = start + length;
  }
  else
  {
    long p = in.tellg();
    in.seekg(0, ios_base::end);
    _end = in.tellg();
    in.seekg(p, ios_base::beg);
  }
  _init();
}

void CppSeqFile::Reader::close()
{
}

void CppSeqFile::Reader::_init()
{
  _in.seekg(0, ios_base::beg);
  uint32_t header = _dis.readUnsignedInt();
  uint32_t magicHeader = header & 0xFFFFFF00;
  int version = header & 0xFF;
  if (magicHeader != MAGIC_HEADER || version != 1)
  {
    throw Exception("Not a sequence file. (Header is not the expected value.)");
  }

  _sync.resize(SYNC_HASH_SIZE);
  _syncBuf.resize(SYNC_HASH_SIZE);
  _dis.read(_sync.data(), _sync.length());
  if (_start != 0)
  {
    sync(_start);
  }
}

bool CppSeqFile::Reader::nextRaw(string& key, string& value)
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
    key.resize(keyLength);
    value.resize(valueLength);
    _dis.read((char*)key.data(), keyLength);
    _dis.read((char*)value.data(), valueLength);

    return true;
  }
}

void CppSeqFile::Reader::sync(long pos)
{
  if (pos + SYNC_SIZE > _end)
  {
    return;
  }

  _in.seekg(pos, ios_base::beg);
  QByteArray arr(SYNC_HASH_SIZE, 0);
  _in.read(arr.data(), SYNC_HASH_SIZE);
  char b;
  for (int i = 0; _in.tellg() < _end && _in.eof() == false; i++)
  {
    int j = 0;
    for (; j < SYNC_HASH_SIZE; j++)
    {
      if (_sync[j] != arr[(i + j) % SYNC_HASH_SIZE])
      {
        break;
      }
    }
    if (j == SYNC_HASH_SIZE)
    {
      _in.seekg(_in.tellg() - (istream::pos_type)SYNC_SIZE);
      return;
    }
    _in.read(&b, 1);
    arr[i % SYNC_HASH_SIZE] = b;
  }
}

void CppSeqFile::Reader::_verifySync()
{
  _dis.read(_syncBuf.data(), SYNC_HASH_SIZE);
  if (_syncBuf != _sync)
  {
    throw Exception("Sync does not match expected value.");
  }
}

////
// CppSeqFile::Writer
////


CppSeqFile::Writer::Writer(ostream& out) : _out(out), _dos(out)
{
  _init();
  initializeFileHeader();
  finalizeFileHeader();
}

void CppSeqFile::Writer::appendRaw(const string& key, const string& value)
{
  appendRaw(key.data(), key.size(), value.data(), value.size());
}

void CppSeqFile::Writer::appendRaw(const char* keyData, size_t keySize, const char* valueData,
  size_t valueSize)
{
  int recordLength = sizeof(int32_t) + sizeof(int32_t) + keySize + valueSize;
  _dos.writeInt(recordLength);
  _dos.writeInt(keySize);
  _dos.write(keyData, keySize);
  _dos.write(valueData, valueSize);

  if (_dos.bytesWritten() - _lastSync > SYNC_INTERVAL)
  {
    sync();
  }
}

void CppSeqFile::Writer::close()
{
  _out.flush();
}

void CppSeqFile::Writer::finalizeFileHeader()
{
  // create a seed. I would use currentMSecsSinceEpoch, but that is a relatively new method.
  QString seed = QUuid::createUuid().toString() + "@" + QDateTime::currentDateTime().toTime_t();
  _sync = QCryptographicHash::hash(seed.toUtf8(), QCryptographicHash::Md5);
  _dos.write(_sync.data(), _sync.length());
  _lastSync = _dos.bytesWritten();
}

void CppSeqFile::Writer::_init()
{
}

void CppSeqFile::Writer::initializeFileHeader()
{
  uint32_t version = 1;
  // header is just a magic value + the version.
  uint32_t header = MAGIC_HEADER | version;
  _dos.writeUnsignedInt(header);
}

void CppSeqFile::Writer::sync()
{
  if (_lastSync != (streampos)_dos.bytesWritten())
  {
    _lastSync = _dos.bytesWritten();
    _dos.writeInt(SYNC_ESCAPE);
    _dos.write(_sync.data(), _sync.length());
  }
}

}
