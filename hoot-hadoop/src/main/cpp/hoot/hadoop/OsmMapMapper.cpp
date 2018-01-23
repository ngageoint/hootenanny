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

#include "OsmMapMapper.h"

// Hadoop Utils
#include <hadoop/SerialUtils.hh>

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/OsmMap.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/Hdfs.h>

#include <sstream>

using namespace std;

namespace hoot
{

void OsmMapMapper::map(HadoopPipes::MapContext& context)
{
  _context = &context;
  _path = context.getInputKey();
  bool ok;
  _start = QString::fromStdString(context.getInputValue()).toLong(&ok);
  if (ok == false)
  {
    throw HootException("Error parsing start value.");
  }

  OsmMapPtr m(new OsmMap());

  _loadMap(m);

  _map(m, context);
}

void OsmMapMapper::emitRecord(HadoopPipes::MapContext& context, const std::string& k,
  const ConstOsmMapPtr& m)
{
  std::stringstream ss(std::stringstream::out);
  _OsmPbfWriter.writePb(m, &ss);
  context.emit(k, ss.str());
}

void OsmMapMapper::emitRecord(HadoopPipes::MapContext& context, const std::string& k, const ConstWayPtr& w)
{
  std::stringstream ss(std::stringstream::out);
  _OsmPbfWriter.writePb(w, &ss);
  context.emit(k, ss.str());
}

void OsmMapMapper::emitRecord(HadoopPipes::MapContext& context, const std::string& k,
  const ConstNodePtr& n)
{
  std::stringstream ss(std::stringstream::out);
  _OsmPbfWriter.writePb(n, &ss);
  context.emit(k, ss.str());
}

void OsmMapMapper::_loadMap(OsmMapPtr& m)
{
  OsmPbfReader reader(true);
  reader.setUseFileStatus(true);

  pp::Hdfs fs("default", 0);
  boost::shared_ptr<istream> is(fs.open(_path));

  // parse blob only needs the start location.
  OsmPbfReader::BlobLocation bl;
  bl.headerOffset = _start;
  reader.parseBlob(bl, is.get(), m);
}

}
