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
#include "AngleHistogramExtractor.h"

// geos
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/extractors/Histogram.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMap.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, AngleHistogramExtractor)

class HistogramVisitor : public ConstElementVisitor
{
public:

  static std::string className() { return "hoot::HistogramVisitor"; }

  HistogramVisitor(Histogram& h, const OsmMap& map) : _h(h), _map(map) {}

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Way)
    {
      const ConstWayPtr& w = std::dynamic_pointer_cast<const Way>(e);

      vector<long> nodes = w->getNodeIds();
      if (nodes[0] != nodes[nodes.size() - 1])
      {
        nodes.push_back(nodes[0]);
      }

      Coordinate last = _map.getNode(nodes[0])->toCoordinate();
      for (size_t i = 1; i < nodes.size(); i++)
      {
        Coordinate c = _map.getNode(nodes[i])->toCoordinate();
        double distance = c.distance(last);
        double theta = atan2(c.y - last.y, c.x - last.x);
        _h.addAngle(theta, distance);
        last = c;
      }
    }
  }

  virtual QString getDescription() const { return ""; }
  virtual std::string getClassName() const { return ""; }

private:

  Histogram& _h;
  const OsmMap& _map;
};

AngleHistogramExtractor::AngleHistogramExtractor()
{
  setConfiguration(conf());
}

AngleHistogramExtractor::AngleHistogramExtractor(Radians smoothing, unsigned int bins) :
_smoothing(smoothing),
_bins(bins)
{
  LOG_VART(_smoothing);
  LOG_VART(_bins);
}

void AngleHistogramExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _smoothing = options.getAngleHistogramExtractorSmoothing();
  _bins = options.getAngleHistogramExtractorBins();
  LOG_VART(_smoothing);
  LOG_VART(_bins);
}

Histogram* AngleHistogramExtractor::_createHistogram(const OsmMap& map, const ConstElementPtr& e)
  const
{
  Histogram* result = new Histogram(_bins);
  HistogramVisitor v(*result, map);
  e->visitRo(map, v);
  return result;
}

double AngleHistogramExtractor::extract(const OsmMap& map, const ConstElementPtr& target,
  const ConstElementPtr& candidate) const
{
  std::shared_ptr<Histogram> h1(_createHistogram(map, target));
  std::shared_ptr<Histogram> h2(_createHistogram(map, candidate));
  if (_smoothing > 0.0)
  {
    h1->smooth(_smoothing);
    h2->smooth(_smoothing);
  }
  h1->normalize();
  h2->normalize();

  const double diff = max(0.0, h1->diff(*h2));
  return 1.0 - diff;
}

string AngleHistogramExtractor::getName() const
{
  string result = getClassName();
  if (_smoothing > 0.0)
  {
    result += QString(" %2").arg(_smoothing, 0, 'g', 4).toStdString();
  }
  if (_bins > 16)
  {
    result += QString(" %2").arg(_bins, 0, 10, QChar('_')).toStdString();
  }
  return result;
}

}
