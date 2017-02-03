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
#include "PbfRecordWriter.h"

#include <pp/HadoopPipesUtils.h>

namespace hoot
{

class OsmMapIterator : public pp::Iterator< shared_ptr<OsmMap> >
{
public:

  OsmMapIterator(HadoopPipes::ReduceContext* context, shared_ptr<OsmMap>& map,
                 PbfReader& reader) :
    _context(context),
    _map(map),
    _reader(reader)
  {
    _hasNext = _context->nextValue();
  }

  virtual bool hasNext() { return _hasNext; }

  virtual const shared_ptr<OsmMap>& next()
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
  shared_ptr<OsmMap>& _map;
  PbfReader& _reader;
};

OsmMapReducer::OsmMapReducer() : _reader(true)
{
  _map.reset(new OsmMap());
  _context = NULL;
  _pbfWriter = NULL;
}

PbfRecordWriter* OsmMapReducer::getPbfRecordWriter()
{
  if (_pbfWriter == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(_context);
    _pbfWriter = dynamic_cast<PbfRecordWriter*>(writer);
    if (_pbfWriter == NULL)
    {
      throw HootException("Unable to convert to PbfRecordWriter. Did you set the right record "
                            "writer in the Job?");
    }
    if (_context == NULL)
    {
      throw HootException("Unexpected error: _context is NULL");
    }
    _pbfWriter->setReduceContext(*_context);
  }
  return _pbfWriter;
}

void OsmMapReducer::reduce(HadoopPipes::ReduceContext& context)
{
  _context = &context;
  const string& k = context.getInputKey();
  OsmMapIterator it(_context, _map, _reader);
  reduce(k, it, context);
}

}
