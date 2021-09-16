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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MEDIAN_TO_DIVIDED_ROAD_CLASSIFIER_H
#define MEDIAN_TO_DIVIDED_ROAD_CLASSIFIER_H

// Hoot
#include <hoot/core/conflate/highway/HighwayClassifier.h>

namespace hoot
{

class FeatureExtractor;

/**
 * @brief The MedianToDividedRoadClassifier class is used to classify road median to divided road
 * (dual carriageway) matches.
 *
 * Its feature extractors have been derived directly from HighwayRfClassifer and may need tweaking
 * over time. No actual RF model file has been generated to be used by this classifier.
 */
class MedianToDividedRoadClassifier : public HighwayClassifier
{
public:

  static QString className() { return "MedianToDividedRoadClassifier"; }

  static const QString MEDIAN_MATCHED_SUBROUTINE_NAME;
  static const QString MEDIAN_MATCHED_DESCRIPTION;

  MedianToDividedRoadClassifier();
  ~MedianToDividedRoadClassifier() override = default;

  /**
   * @see HighwayClassifier
   */
  MatchClassification classify(
    const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
    const WaySublineMatchString& match) override;

  /**
   * @see HighwayClassifier
   */
  std::map<QString, double> getFeatures(
    const ConstOsmMapPtr& m, const ElementId& eid1, const ElementId& eid2,
    const WaySublineMatchString& match) const override;

private:

  // feature extractors used to create the classifier; extractors mapped to their minimum allowable
  // score for the classifier to classify a match
  QMap<std::shared_ptr<const FeatureExtractor>, double> _extractors;

  void _createExtractors();
};

}

#endif // MEDIAN_TO_DIVIDED_ROAD_CLASSIFIER_H
