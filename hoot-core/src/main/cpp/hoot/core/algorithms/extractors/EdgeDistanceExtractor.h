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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef EDGEDISTANCEEXTRACTOR_H
#define EDGEDISTANCEEXTRACTOR_H

#include <hoot/core/algorithms/aggregator/ValueAggregatorConsumer.h>
#include <hoot/core/algorithms/extractors/AbstractDistanceExtractor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
class ValueAggregator;

/**
 * Calculates the distance from one set of lines to the other set of lines. There are several
 * options for how to aggregate the distances. See subclasses of ValueAggregator.
 *
 * The distance will be calculated from target to candidate and candidate to target. The lower
 * of the two values will be returned.
 *
 * The distances are determined by sampling one of the geometries into points every samples meters
 * and then calculating the distance from each sample to the lines that make up the other
 * geometry.
 */
class EdgeDistanceExtractor : public AbstractDistanceExtractor, public ValueAggregatorConsumer,
  public Configurable
{
public:

  EdgeDistanceExtractor(ValueAggregatorPtr, Meters spacing = 5.0);
  EdgeDistanceExtractor(Meters spacing = 5.0);
  virtual ~EdgeDistanceExtractor() = default;

  static QString className() { return "hoot::EdgeDistanceExtractor"; }

  double distance(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  virtual QString getClassName() const override { return className(); }

  virtual QString getName() const override;

  void setValueAggregator(const ValueAggregatorPtr& va) override { _aggregator = va; }

  void setConfiguration(const Settings& conf) override;

  void setSpacing(const double spacing) { _spacing = spacing; }

  QString getDescription() const override
  { return "Calculates the distance from one set of lines to another"; }

private:

  ValueAggregatorPtr _aggregator;
  double _spacing;

  double _oneDistance(const OsmMap& map, const std::shared_ptr<const Element>& e1,
    const std::shared_ptr<const Element>& e2) const;

  std::vector<geos::geom::Coordinate> _discretize(const OsmMap& map,
                                                  const std::shared_ptr<const Element>& e) const;

  std::shared_ptr<geos::geom::Geometry> _toLines(const OsmMap& map,
                                                 const std::shared_ptr<const Element>& e) const;
};

}

#endif // EDGEDISTANCEEXTRACTOR_H
