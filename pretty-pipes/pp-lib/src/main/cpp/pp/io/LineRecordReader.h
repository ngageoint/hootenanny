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

#ifndef LINERECORDREADER_H
#define LINERECORDREADER_H

// Pretty Pipes
#include <pp/RecordReader.h>

namespace pp
{

/**
 * Mimics org.apache.hadoop.mapreduce.lib.input.LineRecordReader used by TextInputFormat
 */
class LineRecordReader : public pp::RecordReader
{
public:

  static std::string className() { return "pp::LineRecordReader"; }

  LineRecordReader();

  virtual ~LineRecordReader();

  float getProgress();

  virtual void initialize(InputSplit* split, HadoopPipes::MapContext&) { initialize(split); }

  void initialize(InputSplit* split);

  virtual bool next(std::string& key, std::string& value);

private:
  std::string _path;
  long _start, _length;
  std::istream* _stream;
  std::string _buffer;
};

}

#endif // LINERECORDREADER_H
