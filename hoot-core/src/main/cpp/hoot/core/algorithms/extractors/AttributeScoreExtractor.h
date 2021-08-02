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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ATTRIBUTESCOREEXTRACTOR_H
#define ATTRIBUTESCOREEXTRACTOR_H

#include <hoot/core/algorithms/extractors/WayFeatureExtractor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * @brief The AttributeScoreExtractor class calculates "distance" between tags using TagComparator.
 */
class AttributeScoreExtractor : public WayFeatureExtractor, public Configurable
{
public:

  static QString className() { return "hoot::AttributeScoreExtractor"; }

  AttributeScoreExtractor(ValueAggregatorPtr wayAgg, bool useWeight = false);
  AttributeScoreExtractor(bool useWeight = false);
  ~AttributeScoreExtractor() = default;

  QString getDescription() const override
  { return "Calculates the similarity between two tag sets"; }
  QString getClassName() const override { return className(); }
  QString getName() const override;

  void setConfiguration(const Settings& conf) override;
  void setUseWeight(const bool useWeight);

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const override;

  bool _useWeight;
};

}

#endif // ATTRIBUTESCOREEXTRACTOR_H
