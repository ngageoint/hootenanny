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
#include <hoot/core/OsmMap.h>
#include <hoot/hadoop/OsmMapMapper.h>

// Standard
#include <sstream>

namespace hoot
{

class ConflateMapper : public OsmMapMapper
{
public:

  static std::string className() { return "hoot::ConflateMapper"; }

  static string envelopesKey() { return "hoot.envelopes"; }

  static string bufferKey() { return "hoot.buffer"; }

  static string maxWaySizeKey() { return "hoot.max.way.size"; }

  static string replacementsKey() { return "hoot.node.replacements"; }

  ConflateMapper();

  virtual ~ConflateMapper() {}

  virtual void close() {}

  static vector<Envelope> parseEnvelopes(const string& envStr);

protected:

  QHash< int, shared_ptr<PbfWriter> > _writers;
  QHash< int, shared_ptr<stringstream> > _buffers;
  double _tileBufferSize;

  vector<Envelope> _envelopes;

  bool _initialized;

  string _replacmentsPath;
  int _reduceTaskCount;

  void _addNode(const shared_ptr<Node>& n);

  void _addWay(ConstOsmMapPtrR map, const shared_ptr<Way>& w);

  /**
   * After each input OsmMap, emit all the data thus far. This reduce memory usage.
   */
  void _flush();

  void _init(HadoopPipes::MapContext& context);

  virtual void _map(shared_ptr<OsmMap>& m, HadoopPipes::MapContext& context);

  void _replaceNodes(shared_ptr<OsmMap>& m);

  void _replaceNodes(shared_ptr<OsmMap>& m, istream& is);

};

}

#endif // CONFLATEMAPPER_H
