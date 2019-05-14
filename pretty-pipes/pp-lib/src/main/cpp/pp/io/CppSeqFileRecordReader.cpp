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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CppSeqFileRecordReader.h"

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/io/FileInputSplit.h>
#include <pp/Hdfs.h>

// Standard
#include <sstream>

namespace pp
{

PP_FACTORY_REGISTER(pp::RecordReader, CppSeqFileRecordReader)


CppSeqFileRecordReader::CppSeqFileRecordReader()
{
  _stream = 0;
  _reader = 0;
}

CppSeqFileRecordReader::~CppSeqFileRecordReader()
{
  delete _reader;
  delete _stream;
}

/**
 * The progress of the record reader through the split as a value between
 * 0.0 and 1.0.
 */
float CppSeqFileRecordReader::getProgress()
{
  long pos = _stream->tellg();
  return (float)(pos - _start) / (float)_length;
}


void CppSeqFileRecordReader::initialize(InputSplit* split, HadoopPipes::MapContext&)
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
  _reader = new CppSeqFile::Reader(*_stream, _start, _length);
}

bool CppSeqFileRecordReader::next(std::string& key, std::string& value)
{
  long pos = _reader->getPosition();
  bool more = _reader->nextRaw(key, value);
  if (pos >= _start + _length && _reader->syncSeen())
  {
    more = false;
    key.clear();
    value.clear();
  }
  return more;
}

}
