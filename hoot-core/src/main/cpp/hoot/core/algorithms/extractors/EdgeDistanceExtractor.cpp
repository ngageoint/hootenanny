/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EdgeDistanceExtractor.h"

// geos
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/Point.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/ValueAggregator.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/GeometryConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, EdgeDistanceExtractor)

class DiscretizeWaysVisitor : public ElementConstOsmMapVisitor
{
public:

  DiscretizeWaysVisitor(double spacing, vector<Coordinate>& v) : _spacing(spacing), _result(v) {}

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      WayDiscretizer wd(_map->shared_from_this(), std::dynamic_pointer_cast<const Way>(e));
      wd.discretize(_spacing, _result);
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

private:

  double _spacing;
  vector<Coordinate>& _result;
};

class LinesWaysVisitor : public ElementConstOsmMapVisitor
{
public:

  LinesWaysVisitor(vector<Geometry*>& lines) : _lines(lines) {}

  virtual void visit(const std::shared_ptr<const Element>& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      ConstWayPtr w(std::dynamic_pointer_cast<const Way>(e));

      Geometry* ls = ElementConverter(_map->shared_from_this()).convertToLineString(w)->clone();
      _lines.push_back(ls);
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

private:

  vector<Geometry*>& _lines;
};

EdgeDistanceExtractor::EdgeDistanceExtractor(ValueAggregatorPtr a, Meters spacing):
  _aggregator(a)
{
  if (!_aggregator)
    _aggregator.reset(new MeanAggregator());

  setSpacing(spacing);
}

EdgeDistanceExtractor::EdgeDistanceExtractor(Meters spacing)
{
  _aggregator.reset(new MeanAggregator());
  setSpacing(spacing);
}

vector<Coordinate> EdgeDistanceExtractor::_discretize(const OsmMap& map,
  const std::shared_ptr<const Element>& e) const
{
  vector<Coordinate> result;

  DiscretizeWaysVisitor v(_spacing, result);
  v.setOsmMap(&map);
  e->visitRo(map, v);

  return result;
}

double EdgeDistanceExtractor::distance(const OsmMap &map,
                                       const std::shared_ptr<const Element>& target,
                                       const std::shared_ptr<const Element> &candidate) const
{
  double d1 = _oneDistance(map, target, candidate);
  double d2 = _oneDistance(map, candidate, target);

  return min(d1, d2);
}

string EdgeDistanceExtractor::getName() const
{
  return (QString("EdgeDistance") + _aggregator->toString()).toStdString();
}

double EdgeDistanceExtractor::_oneDistance(const OsmMap& map,
                                           const std::shared_ptr<const Element>& e1,
                                           const std::shared_ptr<const Element>& e2) const
{
  vector<double> distances;
  vector<Coordinate> points = _discretize(map, e1);
  std::shared_ptr<Geometry> g = _toLines(map, e2);
  distances.reserve(points.size());

  for (size_t i = 0; i < points.size(); i++)
  {
    std::shared_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(points[i]));
    distances.push_back(g->distance(p.get()));
  }

  if (distances.size() == 0)
  {
    return -1;
  }

  return _aggregator->aggregate(distances);
}

std::shared_ptr<Geometry> EdgeDistanceExtractor::_toLines(const OsmMap& map,
  const std::shared_ptr<const Element>& e) const
{
  std::shared_ptr<Geometry> result;

  if (e->getElementType() != ElementType::Node)
  {
    //  Create a new vector to pass ownership to multilinestring
    vector<Geometry*>* lines = new vector<Geometry*>();
    LinesWaysVisitor v(*lines);
    v.setOsmMap(&map);
    e->visitRo(map, v);
    result.reset(GeometryFactory::getDefaultInstance()->createMultiLineString(lines));
  }
  else
  {
    result.reset(GeometryFactory::getDefaultInstance()->createEmptyGeometry());
  }

  return result;
}

void EdgeDistanceExtractor::setSpacing(const double spacing)
{
  _spacing = spacing;
}

void EdgeDistanceExtractor::setConfiguration(const Settings& conf)
{
  setSpacing(ConfigOptions(conf).getEdgeDistanceExtractorSpacing());
}

}
