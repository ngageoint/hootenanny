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

#include "PbfRecordReader.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Log.h>

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/Factory.h>
#include <pp/Hdfs.h>

#include "PbfInputSplit.h"

namespace hoot
{
using namespace pp;

PP_FACTORY_REGISTER(pp::RecordReader, PbfRecordReader)

boost::shared_ptr<OsmMap> PbfRecordReader::getMap()
{
  boost::shared_ptr<OsmMap> result(new OsmMap());

  Hdfs fs;
  auto_ptr<istream> is(fs.open(_path));

  OsmPbfReader reader(true);


  reader.parseBlob(_headers[_currentHeader - 1], is.get(), result);

  return result;
}

/**
 * The progress of the record reader through the split as a value between
 * 0.0 and 1.0.
 */
float PbfRecordReader::getProgress()
{
  return (float)_currentHeader / (float)_headers.size();
}

void PbfRecordReader::initialize(InputSplit* split, HadoopPipes::MapContext&)
{
  // delete split safely.
  boost::shared_ptr<InputSplit> s(split);
  PbfInputSplit* pis = dynamic_cast<PbfInputSplit*>(split);
  if (pis == NULL)
  {
    throw Exception("Internal Error: Expected a split of type PbfInputSplit.");
  }
  _path = pis->getPath();
  _start = pis->getStart();
  _headers = pis->getHeaders();
  _currentHeader = 0;
  s.reset();
}

bool PbfRecordReader::next(std::string& key, std::string& value)
{
  bool result = false;
  if (_currentHeader < (int)_headers.size())
  {
    key = _path;
    value = QString("%1").arg(_headers[_currentHeader++]).toStdString();
    result = true;
  }
  return result;
}

}
