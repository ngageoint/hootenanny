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

#include "OsmMapReducer.h"

// Hadoop Utils
#include <hadoop/SerialUtils.hh>

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/OsmMap.h>
#include <hoot/hadoop/pbf/PbfRecordWriter.h>

#include <pp/HadoopPipesUtils.h>

#include <sstream>

using namespace std;

namespace hoot
{

class OsmMapIterator : public pp::Iterator<OsmMapPtr >
{
public:

  OsmMapIterator(HadoopPipes::ReduceContext* context, OsmMapPtr& map,
                 OsmPbfReader& reader) :
    _context(context),
    _map(map),
    _reader(reader)
  {
    _hasNext = _context->nextValue();
  }

  virtual bool hasNext() { return _hasNext; }

  virtual const OsmMapPtr& next()
  {
    const string& v = _context->getInputValue();
    stringstream ss(v, stringstream::in);
    _map->clear();
    _reader.parseElements(&ss, _map);
    _hasNext = _context->nextValue();
    return _map;
  }

private:
  HadoopPipes::ReduceContext* _context;
  bool _hasNext;
  OsmMapPtr& _map;
  OsmPbfReader& _reader;
};

OsmMapReducer::OsmMapReducer() : _reader(true)
{
  _map.reset(new OsmMap());
  _context = NULL;
  _OsmPbfWriter = NULL;
}

PbfRecordWriter* OsmMapReducer::getPbfRecordWriter()
{
  if (_OsmPbfWriter == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(_context);
    _OsmPbfWriter = dynamic_cast<PbfRecordWriter*>(writer);
    if (_OsmPbfWriter == NULL)
    {
      throw HootException("Unable to convert to PbfRecordWriter. Did you set the right record "
                            "writer in the Job?");
    }
    if (_context == NULL)
    {
      throw HootException("Unexpected error: _context is NULL");
    }
    _OsmPbfWriter->setReduceContext(*_context);
  }
  return _OsmPbfWriter;
}

void OsmMapReducer::reduce(HadoopPipes::ReduceContext& context)
{
  _context = &context;
  const string& k = context.getInputKey();
  OsmMapIterator it(_context, _map, _reader);
  reduce(k, it, context);
}

}
