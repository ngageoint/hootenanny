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

#ifndef MAPSTATS_H
#define MAPSTATS_H

// GEOS
#include <geos/geom/Envelope.h>

// Pretty Pipes
#include <pp/conf/Configuration.h>

// Standard
#include <istream>
#include <ostream>

// Boost
#include <boost/shared_ptr.hpp>

namespace hoot
{
class Node;

using namespace boost;
using namespace geos::geom;
using namespace std;

class MapStats
{
public:
  MapStats();

  MapStats(const MapStats& from);

  void clear();

  void combine(const MapStats& other);

  void expandEnvelope(double x, double y);

  void expandEnvelope(const Envelope& e);

  void expandNodeRange(const boost::shared_ptr<const Node>& n);

  void expandNodeRange(long nid);

  void expandRelationRange(long rid);

  void expandWayRange(long wid);

  const Envelope& getEnvelope() const { return _envelope; }

  long getMaxNodeId() const { return _maxNodeId; }

  long getMinNodeId() const { return _minNodeId; }

  long getMaxWayId() const { return _maxWayId; }

  long getMinWayId() const { return _minWayId; }

  long getMaxRelationId() const { return _maxRelationId; }

  long getMinRelationId() const { return _minRelationId; }

  bool isValid() const
  {
    return _minNodeId < _maxNodeId ||
        _minWayId < _maxWayId ||
        _minRelationId < _maxRelationId; }

  MapStats& operator=(const MapStats& from);

  void read(const pp::Configuration& c);

  void read(istream& is);

  void readDir(QString dir);

  QString toString() const;

  /**
   * Mimics Envelope::toString(), but with a higher precision.
   */
  static string toString(const Envelope& e);

  void write(pp::Configuration& c) const;

  void write(ostream& os) const;

private:

  long _minNodeId, _maxNodeId;
  long _minWayId, _maxWayId;
  long _minRelationId, _maxRelationId;
  Envelope _envelope;

  Envelope _parseEnvelope(const std::string& str);

  vector<string> _split(const string& str, const string& delimiters);
};

}

#endif // MAPSTATS_H
