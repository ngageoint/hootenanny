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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "SampledAngleHistogramExtractor.h"

// GEOS
#include <geos/geom/LineString.h>
using namespace geos::geom;

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/extractors/Histogram.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

// Qt
#include <qnumeric.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, SampledAngleHistogramExtractor)

class SampledAngleHistogramVisitor : public ElementConstOsmMapVisitor
{
public:

  static std::string className() { return "hoot::SampledAngleHistogramVisitor"; }

  SampledAngleHistogramVisitor(Histogram& histogram, const double sampleDistance,
    const double headingDelta) :
  _angleHistogram(histogram),
  _sampleDistance(sampleDistance),
  _headingDelta(headingDelta)
  {

  }

  virtual void visit(const std::shared_ptr<const Element>& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      const ConstWayPtr& way = _map->getWay(e->getElementId());

      vector<long> wayNodes = way->getNodeIds();
      if (wayNodes[0] != wayNodes[wayNodes.size() - 1])
      {
        wayNodes.push_back(wayNodes[0]);
      }

      vector<WayLocation> discretizedLocs;
      WayDiscretizer wayDiscretizer(_map->shared_from_this(), way);
      wayDiscretizer.discretize(_sampleDistance, discretizedLocs);

      WayLocation lastLoc = discretizedLocs.at(0);
      for (size_t i = 1; i < discretizedLocs.size(); i++)
      {
        //select a loc sampledDistance meters along the way
        WayLocation currentLoc = discretizedLocs.at(i);
        const double distance = currentLoc.getCoordinate().distance(lastLoc.getCoordinate());
        //calculate the heading using some distance around the way
        const double theta = WayHeading::calculateHeading(currentLoc, _headingDelta);
        if (! ::qIsNaN(theta))
        {
          _angleHistogram.addAngle(theta, distance);
        }
        lastLoc = currentLoc;
      }
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

private:

  Histogram& _angleHistogram;
  double _sampleDistance;
  double _headingDelta;
};

SampledAngleHistogramExtractor::SampledAngleHistogramExtractor()
{
}

void SampledAngleHistogramExtractor::setConfiguration(const Settings& conf)
{
  AngleHistogramExtractor::setConfiguration(conf);
  ConfigOptions config = ConfigOptions(conf);
  setSampleDistance(config.getWayAngleSampleDistance());
  setHeadingDelta(config.getWayMatcherHeadingDelta());
}

Histogram* SampledAngleHistogramExtractor::_createHistogram(const OsmMap& map,
                                                            const ConstElementPtr& e) const
{
  Histogram* result = new Histogram(8);
  SampledAngleHistogramVisitor v(*result, _sampleDistance, _headingDelta);
  v.setOsmMap(&map);
  e->visitRo(map, v);
  return result;
}

}
