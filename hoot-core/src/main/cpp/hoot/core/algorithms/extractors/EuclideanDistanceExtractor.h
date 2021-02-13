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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef EUCLIDEANDISTANCEEXTRACTOR_H
#define EUCLIDEANDISTANCEEXTRACTOR_H

// Hoot
#include <hoot/core/algorithms/extractors/AbstractDistanceExtractor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class EuclideanDistanceExtractor : public AbstractDistanceExtractor, public Configurable
{
public:

  EuclideanDistanceExtractor();
  virtual ~EuclideanDistanceExtractor() = default;

  static QString className() { return "hoot::EuclideanDistanceExtractor"; }

  /**
   * Measure the Euclidean distance in meters between two elements in the same map
   *
   * @param map map containing the elements
   * @param target the first element of the pair to measure
   * @param candidate the second element of the pair to measure
   * @return the distance between the two elements in meters
   */
  virtual double distance(
    const OsmMap& map, const ConstElementPtr& target,
    const ConstElementPtr& candidate) const override;

  /**
   * Measure the Euclidean distance in meters between two elements in different maps
   *
   * @param map1 map containing the target element
   * @param map2 map containing the candidate element
   * @param target the first element of the pair to measure
   * @param candidate the second element of the pair to measure
   * @return the distance between the two elements in meters
   */
  double distance(
    const OsmMap& map1, const OsmMap& map2, const ConstElementPtr& target,
    const ConstElementPtr& candidate) const;

  virtual QString getClassName() const { return className(); }

  virtual QString getName() const { return className(); }

  virtual QString getDescription() const
  { return "Calculates the Euclidean distance between two features"; }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

private:

  bool _requireAreaForPolygonConversion;
};

}

#endif // EUCLIDEANDISTANCEEXTRACTOR_H
