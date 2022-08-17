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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "BufferedOverlapExtractor.h"

// geos
#include <geos/geom/Geometry.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, BufferedOverlapExtractor)

BufferedOverlapExtractor::BufferedOverlapExtractor()
{
  setConfiguration(conf());
}

BufferedOverlapExtractor::BufferedOverlapExtractor(double bufferPortion)
  : _bufferPortion(bufferPortion)
{
}

void BufferedOverlapExtractor::setConfiguration(const Settings& conf)
{
  _bufferPortion = ConfigOptions(conf).getBufferedOverlapExtractorPortion();
}

double BufferedOverlapExtractor::extract(const OsmMap& map, const ConstElementPtr& target,
                                         const ConstElementPtr& candidate) const
{
  ElementToGeometryConverter ec(map.shared_from_this());
  std::shared_ptr<Geometry> g1 = ec.convertToGeometry(target);
  if (g1->isEmpty())
    return nullValue();
  std::shared_ptr<Geometry> g2 = ec.convertToGeometry(candidate);
  if (g2->isEmpty())
    return nullValue();

  double a1, a2;
  try
  {
    a1 = g1->getArea();
    a2 = g2->getArea();
  }
  catch (const geos::util::TopologyException&)
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    a1 = g1->getArea();
    a2 = g2->getArea();
  }

  double buffer = sqrt(max(a1, a2)) * _bufferPortion;

  std::shared_ptr<Geometry> overlap;
  try
  {
    // This buffer calc is a performance bottleneck for Area Conflation. Attempts to reduce the quad
    // segments option passed to the buffer operation have had no effect on performance so far.
    g1 = g1->buffer(buffer);
    g2 = g2->buffer(buffer);
    overlap = g1->intersection(g2.get());
  }
  catch (const geos::util::TopologyException&)
  {
    g1.reset(GeometryUtils::validateGeometry(g1.get()));
    g2.reset(GeometryUtils::validateGeometry(g2.get()));
    g1 = g1->buffer(buffer);
    g2 = g2->buffer(buffer);
    overlap = g2->intersection(g1.get());
  }

  double bufferedA1 = g1->getArea();
  double bufferedA2 = g2->getArea();
  double overlapArea = overlap->getArea();

  return std::min(1.0, (2 * overlapArea) / (bufferedA1 + bufferedA2));
}

QString BufferedOverlapExtractor::getName() const
{
  return QString("BufferedOverlapExtractor %1").arg(_bufferPortion);
}

}
