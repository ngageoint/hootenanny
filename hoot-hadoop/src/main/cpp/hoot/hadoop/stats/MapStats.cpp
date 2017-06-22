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

#include "MapStats.h"

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Node.h>

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Hdfs.h>

// Standard
#include <limits>
#include <memory>

using namespace boost;
using namespace geos::geom;
using namespace std;

namespace hoot
{

MapStats::MapStats()
{
  clear();
}

MapStats::MapStats(const MapStats& from)
{
  *this = from;
}

void MapStats::clear()
{
  _minNodeId = numeric_limits<long>::max();
  _maxNodeId = -numeric_limits<long>::max();
  _minWayId = numeric_limits<long>::max();
  _maxWayId = -numeric_limits<long>::max();
  _minRelationId = numeric_limits<long>::max();
  _maxRelationId = -numeric_limits<long>::max();
  _envelope.setToNull();
}

void MapStats::combine(const MapStats& other)
{
  _minNodeId = std::min(_minNodeId, other._minNodeId);
  _maxNodeId = std::max(_maxNodeId, other._maxNodeId);
  _minWayId = std::min(_minWayId, other._minWayId);
  _maxWayId = std::max(_maxWayId, other._maxWayId);
  _minRelationId = std::min(_minRelationId, other._minRelationId);
  _maxRelationId = std::max(_maxRelationId, other._maxRelationId);
  if (other.getEnvelope().isNull() == false)
  {
    if (_envelope.isNull() == true)
    {
      _envelope = other.getEnvelope();
    }
    else
    {
      _envelope.expandToInclude(&other.getEnvelope());
    }
  }
}

void MapStats::expandEnvelope(double x, double y)
{
  if (_envelope.isNull())
  {
    _envelope = Envelope(x, x, y, y);
  }
  else
  {
    _envelope.expandToInclude(x, y);
  }
}

void MapStats::expandEnvelope(const Envelope& e)
{
  if (_envelope.isNull())
  {
    _envelope = e;
  }
  else
  {
    _envelope.expandToInclude(&e);
  }
}

void MapStats::expandNodeRange(const boost::shared_ptr<const Node> &n)
{
  expandNodeRange(n->getId());
  expandEnvelope(n->getX(), n->getY());
}

void MapStats::expandNodeRange(long nid)
{
  _minNodeId = std::min(nid, _minNodeId);
  _maxNodeId = std::max(nid, _maxNodeId);
}

void MapStats::expandWayRange(long wid)
{
  _minWayId = std::min(wid, _minWayId);
  _maxWayId = std::max(wid, _maxWayId);
}

void MapStats::expandRelationRange(long rid)
{
  _minRelationId = std::min(rid, _minRelationId);
  _maxRelationId = std::max(rid, _maxRelationId);
}

MapStats& MapStats::operator=(const MapStats& from)
{
  _minNodeId = from._minNodeId;
  _maxNodeId = from._maxNodeId;
  _minWayId = from._minWayId;
  _maxWayId = from._maxWayId;
  _minRelationId = from._minRelationId;
  _maxRelationId = from._maxRelationId;
  _envelope = from._envelope;
  return *this;
}

Envelope MapStats::_parseEnvelope(const std::string& str)
{
  // The string should be in the format:
  // Env[7.2:2.3,7.1:8.2]

  // extract out the values between the [ and ] characters
  string::size_type index = str.find("[");
  string coordString = str.substr(index + 1, str.size() - 1 - 1);

  Envelope result;

  // now split apart the string on : and , characters
  vector<string> values = _split(coordString, ":,");

  double minX = ::atof(values[0].c_str());
  double maxX = ::atof(values[1].c_str());
  double minY = ::atof(values[2].c_str());
  double maxY = ::atof(values[3].c_str());

  if (maxX < minX || maxY < minY)
  {
    result.setToNull();
  }
  else
  {
    result.init(minX, maxX, minY, maxY);
  }

  return result;
}

void MapStats::read(const pp::Configuration &c)
{
  _minNodeId = c.getLong("hoot.min.node.id");
  _maxNodeId = c.getLong("hoot.max.node.id");
  _minWayId = c.getLong("hoot.min.way.id");
  _maxWayId = c.getLong("hoot.max.way.id");
  _minRelationId = c.getLong("hoot.min.relation.id");
  _maxRelationId = c.getLong("hoot.max.relation.id");
  _envelope = _parseEnvelope(c.get("hoot.envelope"));
}

void MapStats::read(istream& is)
{
  pp::DataInputStream dis(is);

  _minNodeId = dis.readLong();
  _maxNodeId = dis.readLong();
  _minWayId = dis.readLong();
  _maxWayId = dis.readLong();
  _minRelationId = dis.readLong();
  _maxRelationId = dis.readLong();
  double minX = dis.readDouble();
  double maxX = dis.readDouble();
  double minY = dis.readDouble();
  double maxY = dis.readDouble();
  if (maxX < minX || maxY < minY)
  {
    _envelope.setToNull();
  }
  else
  {
    _envelope = Envelope(minX, maxX, minY, maxY);
  }
}

void MapStats::readDir(QString dir)
{
  pp::Hdfs fs;

  bool foundOne = false;

  QStringList dirs = dir.split(",");
  for (int d = 0; d < dirs.size(); d++)
  {
    vector<pp::FileStatus> status = fs.listStatus(dirs[d].toStdString(), true);
    for (size_t i = 0; i < status.size(); i++)
    {
      QString fn = QString::fromStdString(status[i].getPath());
      if (fn.endsWith(".stats"))
      {
        MapStats s;
        boost::shared_ptr<istream> is(fs.open(fn.toStdString()));
        s.read(*is);
        combine(s);
        is.reset();
        foundOne = true;
      }
    }
  }

  if (foundOne == false)
  {
    throw HootException("Error reading stats, no stats files found in path: " + dir);
  }
}

vector<string> MapStats::_split(const string &str, const string &delimiters)
{
  vector<string> tokens;

  // Find first "non-delimiter".
  string::size_type lastPos = 0;
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);

    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }

  return tokens;
}

QString MapStats::toString() const
{
  return QString("minNode: %1 maxNode: %2 minWay: %3 maxWay: %4 minRelation: %5 maxRelation: %6 "
                 "env: %7")
      .arg(_minNodeId).arg(_maxNodeId)
      .arg(_minWayId).arg(_maxWayId)
      .arg(_minRelationId).arg(_maxRelationId)
      .arg(QString::fromStdString(toString(_envelope)));
}

string MapStats::toString(const Envelope& e)
{
  ostringstream s;
  s.precision(16);
  s << "Env[" << e.getMinX() << ":" << e.getMaxX() << "," <<
       e.getMinY() << ":" << e.getMaxY() << "]";
  return s.str();
}

void MapStats::write(ostream& os) const
{
  pp::DataOutputStream dos(os);

  dos.writeLong(_minNodeId);
  dos.writeLong(_maxNodeId);
  dos.writeLong(_minWayId);
  dos.writeLong(_maxWayId);
  dos.writeLong(_minRelationId);
  dos.writeLong(_maxRelationId);
  dos.writeDouble(!_envelope.isNull() ? _envelope.getMinX() : numeric_limits<double>::max());
  dos.writeDouble(!_envelope.isNull() ? _envelope.getMaxX() : -numeric_limits<double>::max());
  dos.writeDouble(!_envelope.isNull() ? _envelope.getMinY() : numeric_limits<double>::max());
  dos.writeDouble(!_envelope.isNull() ? _envelope.getMaxY() : -numeric_limits<double>::max());
}

void MapStats::write(pp::Configuration &c) const
{
  c.setLong("hoot.min.node.id", _minNodeId);
  c.setLong("hoot.max.node.id", _maxNodeId);
  c.setLong("hoot.min.way.id", _minWayId);
  c.setLong("hoot.max.way.id", _maxWayId);
  c.setLong("hoot.min.relation.id", _minRelationId);
  c.setLong("hoot.max.relation.id", _maxRelationId);
  c.set("hoot.envelope", toString(_envelope));
}

}
