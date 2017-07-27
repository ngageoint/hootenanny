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

#ifndef WAYJOIN2RECORDREADER_H
#define WAYJOIN2RECORDREADER_H

// Boost
#include <boost/shared_ptr.hpp>

// Pretty Pipes
#include <pp/io/CppSeqFileRecordReader.h>
#include <pp/io/FileInputSplit.h>
#include <pp/MapContextConsumer.h>
#include <pp/RecordReader.h>

#include "WayJoin1Reducer.h"
#include "WayJoin2InputSplit.h"

namespace hoot
{
class PbfInputSplit;
class PbfRecordReader;
class OsmMap;

class WayJoin2RecordReader : public pp::RecordReader
{
public:
  static std::string className() { return "hoot::WayJoin2RecordReader"; }

  WayJoin2RecordReader() {}

  virtual ~WayJoin2RecordReader() {}

  char getRecordType();

  boost::shared_ptr<OsmMap> getMap();

  bool next(int64_t& key, WayJoin1Reducer::Value& value);

  virtual bool next(std::string& key, std::string& value);

  /**
   * The progress of the record reader through the split as a value between
   * 0.0 and 1.0.
   */
  virtual float getProgress();

  virtual void initialize(pp::InputSplit* split, HadoopPipes::MapContext& context);

private:
  boost::shared_ptr<WayJoin2InputSplit> _split;
  boost::shared_ptr<RecordReader> _reader;

  const pp::FileInputSplit* _csqSplit;
  const PbfInputSplit* _pbfSplit;
  boost::shared_ptr<pp::CppSeqFileRecordReader> _csqReader;
  boost::shared_ptr<PbfRecordReader> _OsmPbfReader;
};


}

#endif // WAYJOIN2RECORDREADER_H
