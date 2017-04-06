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

#ifndef PAINTNODESMAPPER_H
#define PAINTNODESMAPPER_H

#include "../NodeDensity.h"
#include "../OsmMapMapper.h"

namespace hoot
{
using namespace boost;

class PaintNodesMapper : public OsmMapMapper
{
public:

  static std::string className() { return "hoot::PaintNodesMapper"; }

  virtual void close();

  void flush();

  PaintNodesMapper();

protected:

  virtual void _init(HadoopPipes::MapContext& context);

  virtual void _map(boost::shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context);

  NodeDensity _nd;
  Envelope _envelope;
  double _pixelSize;
  int _width, _height;

  HadoopPipes::MapContext* _context;
};

}

#endif // PAINTNODESMAPPER_H
