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

#include "LineRecordReader.h"

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/io/FileInputSplit.h>
#include <pp/Hdfs.h>

using namespace std;

namespace pp
{

PP_FACTORY_REGISTER(pp::RecordReader, LineRecordReader)

LineRecordReader::LineRecordReader()
{
  _stream = 0;
  // handle a 1MB line at most.
  _buffer.resize(1048576);
}

LineRecordReader::~LineRecordReader()
{
  delete _stream;
}

/**
 * The progress of the record reader through the split as a value between
 * 0.0 and 1.0.
 */
float LineRecordReader::getProgress()
{
  long pos = _stream->tellg();
  return (float)(pos - _start) / (float)_length;
}


void LineRecordReader::initialize(InputSplit* split)
{
  // delete split safely.
  boost::shared_ptr<InputSplit> s(split);
  if (dynamic_cast<FileInputSplit*>(split) == NULL)
  {
    throw Exception("Internal Error: Expected a split of type FileInputSplit.");
  }
  _path = split->getPath();
  _start = split->getStart();
  _length = split->getLength();
  s.reset();

  Hdfs fs;

  _stream = fs.open(_path);

  if (_start != 0)
  {
    _stream->seekg(_start, ios_base::beg);
    _stream->getline((char*)_buffer.data(), _buffer.size());
  }
}

bool LineRecordReader::next(std::string& key, std::string& value)
{
  long pos = _stream->tellg();
  bool more = true;
  if (pos > _start + _length)
  {
    more = false;
    key.clear();
    value.clear();
  }
  else
  {
    _stream->getline((char*)_buffer.data(), _buffer.size());
    if (_stream->gcount() == 0)
    {
      more = false;
      key.clear();
      value.clear();
    }
    else
    {
      if (key.size() != sizeof(pos))
      {
        key.resize(sizeof(pos));
      }
      long* keyLong = (long*)key.data();
      *keyLong = pos;
      value = _buffer.data();
    }
  }
  return more;
}

}
