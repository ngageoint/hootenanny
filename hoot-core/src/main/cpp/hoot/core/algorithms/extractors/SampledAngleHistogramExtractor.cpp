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
#include "SampledAngleHistogramExtractor.h"

// GEOS
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/extractors/Histogram.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>

// Qt
#include <qnumeric.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, SampledAngleHistogramExtractor)

class SampledAngleHistogramVisitor : public ElementConstOsmMapVisitor
{
public:

  static QString className() { return "hoot::SampledAngleHistogramVisitor"; }

  SampledAngleHistogramVisitor(Histogram& histogram, const double sampleDistance, const double headingDelta)
    : _angleHistogram(histogram),
      _sampleDistance(sampleDistance),
      _headingDelta(headingDelta)
  {
  }

  ~SampledAngleHistogramVisitor() = default;

  void visit(const std::shared_ptr<const Element>& e) override
  {
    if (!e)
      return;
    LOG_VART(e->getElementId());
    if (e->getElementType() == ElementType::Way)
      _addWay(_map->getWay(e->getElementId()));
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
  QString getName() const override { return ""; }
  QString getClassName() const override { return className(); }

private:

  Histogram& _angleHistogram;
  double _sampleDistance;
  double _headingDelta;

  void _addWay(const ConstWayPtr& way)
  {
    if (way)
    {
      vector<long> wayNodes = way->getNodeIds();
      LOG_VART(wayNodes.size());
      if (wayNodes[0] != wayNodes[wayNodes.size() - 1])
        wayNodes.push_back(wayNodes[0]);
      LOG_VART(wayNodes.size());

      LOG_VART(_sampleDistance);
      LOG_VART(_headingDelta);

      vector<WayLocation> discretizedLocs;
      WayDiscretizer wayDiscretizer(_map->shared_from_this(), way);
      wayDiscretizer.discretize(_sampleDistance, discretizedLocs);

      WayLocation lastLoc = discretizedLocs.at(0);
      LOG_VART(lastLoc);
      for (size_t i = 1; i < discretizedLocs.size(); i++)
      {
        // select a loc sampledDistance meters along the way
        WayLocation currentLoc = discretizedLocs.at(i);
        LOG_VART(currentLoc);
        const double distance = currentLoc.getCoordinate().distance(lastLoc.getCoordinate());
        LOG_VART(distance);
        // calculate the heading using some distance around the way
        const double theta = WayHeading::calculateHeading(currentLoc, _headingDelta);
        LOG_VART(theta);
        if (! ::qIsNaN(theta))
          _angleHistogram.addAngle(theta, distance);
        lastLoc = currentLoc;
      }
      LOG_VART(_angleHistogram.numBins());
    }
  }
};

SampledAngleHistogramExtractor::SampledAngleHistogramExtractor()
  : AngleHistogramExtractor()
{
  setConfiguration(conf());
}

SampledAngleHistogramExtractor::SampledAngleHistogramExtractor(Radians smoothing, unsigned int bins)
  : AngleHistogramExtractor(smoothing, bins)
{
}

void SampledAngleHistogramExtractor::setConfiguration(const Settings& conf)
{
  AngleHistogramExtractor::setConfiguration(conf);
  ConfigOptions config = ConfigOptions(conf);
  setSampleDistance(config.getWayAngleSampleDistance());
  setHeadingDelta(config.getWayMatcherHeadingDelta());
}

std::shared_ptr<Histogram> SampledAngleHistogramExtractor::_createHistogram(const OsmMap& map, const ConstElementPtr& e) const
{
  shared_ptr<Histogram> result = std::make_shared<Histogram>(_bins);
  SampledAngleHistogramVisitor v(*result, _sampleDistance, _headingDelta);
  v.setOsmMap(&map);
  e->visitRo(map, v);
  return result;
}

}
