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

#ifndef CONFLATEMAPPER_H
#define CONFLATEMAPPER_H

// Hoot
#include <hoot/hadoop/OsmMapMapper.h>

// Standard
#include <sstream>

namespace hoot
{

class OsmMap;

class ConflateMapper : public OsmMapMapper
{
public:

  static std::string className() { return "hoot::ConflateMapper"; }

  static std::string envelopesKey() { return "hoot.envelopes"; }

  static std::string bufferKey() { return "hoot.buffer"; }

  static std::string maxWaySizeKey() { return "hoot.max.way.size"; }

  static std::string replacementsKey() { return "hoot.node.replacements"; }

  ConflateMapper();

  virtual ~ConflateMapper() {}

  virtual void close() {}

  static std::vector<geos::geom::Envelope> parseEnvelopes(const std::string& envStr);

protected:

  QHash< int, boost::shared_ptr<OsmPbfWriter> > _writers;
  QHash< int, boost::shared_ptr<std::stringstream> > _buffers;
  double _tileBufferSize;

  std::vector<geos::geom::Envelope> _envelopes;

  bool _initialized;

  std::string _replacmentsPath;
  int _reduceTaskCount;

  void _addNode(const NodePtr& n);

  void _addWay(const ConstOsmMapPtr& map, const WayPtr& w);

  /**
   * After each input OsmMap, emit all the data thus far. This reduce memory usage.
   */
  void _flush();

  void _init(HadoopPipes::MapContext& context);

  virtual void _map(OsmMapPtr& m, HadoopPipes::MapContext& context);

  void _replaceNodes(OsmMapPtr& m);

  void _replaceNodes(OsmMapPtr& m, std::istream& is);

};

}

#endif // CONFLATEMAPPER_H
