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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "AngleHistogramExtractor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/algorithms/extractors/Histogram.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, AngleHistogramExtractor)

class HistogramVisitor : public ElementConstOsmMapVisitor
{
public:

  static QString className() { return "hoot::HistogramVisitor"; }

  HistogramVisitor(Histogram& h) : _h(h) { }
  ~HistogramVisitor() = default;

  void visit(const ConstElementPtr& e) override
  {
    if (e->getElementType() == ElementType::Way)
      _addWay(std::dynamic_pointer_cast<const Way>(e));
    else if (e->getElementType() == ElementType::Relation)
    {
      const ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(e);
      const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
      for (const auto& member : relationMembers)
      {
        if (member.getElementId().getType() == ElementType::Way)
          _addWay(_map->getWay(member.getElementId()));
      }
    }
  }

  QString getDescription() const override { return ""; }
  QString getClassName() const override { return ""; }
  QString toString() const override { return ""; }
  QString getName() const override { return ""; }

private:

  Histogram& _h;

  void _addWay(const ConstWayPtr& way)
  {
    if (way)
    {
      vector<long> nodes = way->getNodeIds();
      if (nodes[0] != nodes[nodes.size() - 1])
        nodes.push_back(nodes[0]);

      Coordinate last = _map->getNode(nodes[0])->toCoordinate();
      for (auto node_id : nodes)
      {
        Coordinate c = _map->getNode(node_id)->toCoordinate();
        double distance = c.distance(last);
        double theta = atan2(c.y - last.y, c.x - last.x);
        _h.addAngle(theta, distance);
        last = c;
      }
    }
  }
};

AngleHistogramExtractor::AngleHistogramExtractor()
{
  setConfiguration(conf());
}

AngleHistogramExtractor::AngleHistogramExtractor(Radians smoothing, unsigned int bins)
  : _bins(bins),
    _smoothing(smoothing)
{
}

void AngleHistogramExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _smoothing = options.getAngleHistogramExtractorSmoothing();
  _bins = options.getAngleHistogramExtractorBins();
}

shared_ptr<Histogram> AngleHistogramExtractor::_createHistogram(const OsmMap& map, const ConstElementPtr& e) const
{
  shared_ptr<Histogram> result = std::make_shared<Histogram>(_bins);
  HistogramVisitor v(*result);
  v.setOsmMap(&map);
  e->visitRo(map, v);
  LOG_VART(result->numBins());
  return result;
}

std::shared_ptr<Histogram> AngleHistogramExtractor::getNormalizedHistogram(const OsmMap& map, const ConstElementPtr& element) const
{
  std::shared_ptr<Histogram> hist = _createHistogram(map, element);
  if (_smoothing > 0.0)
    hist->smooth(_smoothing);
  hist->normalize();
  return hist;
}

double AngleHistogramExtractor::extract(const OsmMap& map, const ConstElementPtr& target, const ConstElementPtr& candidate) const
{
  std::shared_ptr<Histogram> h1 = getNormalizedHistogram(map, target);
  std::shared_ptr<Histogram> h2 = getNormalizedHistogram(map, candidate);
  const double diff = max(0.0, h1->diff(*h2));
  return 1.0 - diff;
}

QString AngleHistogramExtractor::getName() const
{
  QString result = getClassName();
  if (_smoothing > 0.0)
    result += QString(" %2").arg(_smoothing, 0, 'g', 4);
  if (_bins > 16)
    result += QString(" %2").arg(_bins, 0, 10, QChar('_'));
  return result;
}

}
