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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef RFEXTRACTORCLASSIFIER_H
#define RFEXTRACTORCLASSIFIER_H

// hoot
#include <hoot/core/elements/OsmMap.h>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

class FeatureExtractor;
class MatchClassification;

class RfExtractorClassifier
{
public:

  static QString className() { return "RfExtractorClassifier"; }

  RfExtractorClassifier() = default;
  virtual ~RfExtractorClassifier() = default;

  /**
   * @brief classify classifies the match type of a building pair and returns the results.
   */
  virtual MatchClassification classify(
    const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2) const;

  /**
   * See note in MatchDetails::getFeatures about the return type of this method.
   */
  virtual std::map<QString, double> getFeatures(
    const ConstOsmMapPtr& m, const ElementId& eid1, const ElementId& eid2) const;

  void importTree(const QDomElement& docRoot);

protected:

  static int logWarnCount;

  mutable std::vector<std::shared_ptr<const FeatureExtractor>> _extractors;
  QStringList _rfFactorLabels;
  std::shared_ptr<Tgs::RandomForest> _rf;

  /**
   * Creates all the possible extractors for this classifier. Only the ones that are also listed
   * in the trained RandomForest will be used. The _extractors member variable should be populated
   * upon completion.
   */
  virtual void _createExtractors() const = 0;

  const std::vector<std::shared_ptr<const FeatureExtractor>>& _getExtractors() const;
};

}

#endif // RFEXTRACTORCLASSIFIER_H
