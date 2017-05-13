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

#ifndef CPPSEQFILERECORDREADER_H
#define CPPSEQFILERECORDREADER_H

// Pretty Pipes
#include <pp/MapContextConsumer.h>
#include <pp/io/CppSeqFile.h>
#include <pp/RecordReader.h>

namespace pp
{

class CppSeqFileRecordReader : public pp::RecordReader
{
public:
  static std::string className() { return "pp::CppSeqFileRecordReader"; }

  CppSeqFileRecordReader();

  virtual ~CppSeqFileRecordReader();

  virtual void initialize(InputSplit* split, HadoopPipes::MapContext& context);

  virtual bool next(std::string& key, std::string& value);

  /**
  * The progress of the record reader through the split as a value between
  * 0.0 and 1.0.
  */
  virtual float getProgress();

private:
  std::string _path;
  long _start, _length;
  CppSeqFile::Reader* _reader;
  std::istream* _stream;
};

}

#endif // CPPSEQFILERECORDREADER_H
