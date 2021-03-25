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
#ifndef HIGHWAYRFCLASSIFIER_H
#define HIGHWAYRFCLASSIFIER_H

#include <hoot/core/conflate/highway/HighwayClassifier.h>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

class FeatureExtractor;

class HighwayRfClassifier : public HighwayClassifier
{
public:

  static QString className() { return "hoot::HighwayRfClassifier"; }

  HighwayRfClassifier() = default;
  ~HighwayRfClassifier() = default;

  MatchClassification classify(const ConstOsmMapPtr& map,
    ElementId eid1, ElementId eid2, const WaySublineMatchString& match) override;

  std::map<QString, double> getFeatures(const ConstOsmMapPtr& m,
    ElementId eid1, ElementId eid2, const WaySublineMatchString& match) const override;

private:

  static int logWarnCount;
  // these are mutable because we do a lazy load.
  mutable std::shared_ptr<Tgs::RandomForest> _rf;
  mutable QStringList _rfFactorLabels;
  mutable std::vector<std::shared_ptr<const FeatureExtractor>> _extractors;

  void _createAllExtractors() const;
  void _createTestExtractors() const;

  const std::vector<std::shared_ptr<const FeatureExtractor>>& _getExtractors() const;

  /**
   * This provides a lazy load and should be called before any private members are accessed. This
   * can be called multiple times efficiently.
   */
  void _init() const;
};

}

#endif // HIGHWAYRFCLASSIFIER_H
