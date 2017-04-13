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

#include "WayJoin2RecordReader.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/OsmMap.h>
#include <hoot/hadoop/pbf/PbfInputSplit.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/Factory.h>
#include <pp/Hdfs.h>

#include "WayJoin2Mapper.h"

namespace hoot
{
using namespace pp;

PP_FACTORY_REGISTER(pp::RecordReader, WayJoin2RecordReader)

boost::shared_ptr<OsmMap> WayJoin2RecordReader::getMap()
{
  return _OsmPbfReader->getMap();
}

/**
 * The progress of the record reader through the split as a value between
 * 0.0 and 1.0.
 */
float WayJoin2RecordReader::getProgress()
{
  return _reader->getProgress();
}

char WayJoin2RecordReader::getRecordType()
{
  if (_pbfSplit != NULL)
  {
    return WayJoin2InputSplit::PbfInputSplitType;
  }
  else if (_csqSplit != NULL)
  {
    return WayJoin2InputSplit::FileInputSplitType;
  }
  else
  {
    throw InternalErrorException("Internal Error: Shouldn't get here.");
  }
}

bool WayJoin2RecordReader::next(std::string& key, std::string& value)
{
  return _reader->next(key, value);
}

void WayJoin2RecordReader::initialize(InputSplit* split, HadoopPipes::MapContext& context)
{
  WayJoin2InputSplit* is = dynamic_cast<WayJoin2InputSplit*>(split);
  if (is == NULL)
  {
    delete split;
    throw InternalErrorException("Internal Error: Expected an input split type of "
      "WayJoin2InputSplit");
  }
  _split.reset(is);

  _pbfSplit = dynamic_cast<const PbfInputSplit*>(is->getChildSplit().get());
  _csqSplit = dynamic_cast<const FileInputSplit*>(is->getChildSplit().get());
  if (_pbfSplit != NULL)
  {
    _OsmPbfReader.reset(new PbfRecordReader());
    _OsmPbfReader->initialize(_pbfSplit->copy(), context);
    _reader = _OsmPbfReader;
  }
  else if (_csqSplit != NULL)
  {
    _csqReader.reset(new CppSeqFileRecordReader());
    _csqReader->initialize(_csqSplit->copy(), context);
    _reader = _csqReader;
  }
  else
  {
    throw InternalErrorException("Internal Error: Expected PBF or CSQ Split.");
  }
}

}
