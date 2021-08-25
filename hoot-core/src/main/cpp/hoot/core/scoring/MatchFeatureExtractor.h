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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef MANIPULATORFEATUREEXTRACTOR_H
#define MANIPULATORFEATUREEXTRACTOR_H

// hoot
#include <hoot/core/scoring/DataSamples.h>

// Qt
#include <QVariant>

// Standard
#include <list>

namespace hoot
{

class MatchCreator;
class MatchFactory;
class OsmMap;
class ElementId;
class MatchType;

/**
 * Given an OsmMap and a set of matches the MatchFeatureExtractor will pull out all
 * the scores relevant to determining a match and dump them out for further analysis. This is
 * primarily useful when experimenting with supervised machine learning.
 *
 * The input map _must_ have a REF attribute on both maps that matches to determine whether or
 * not the match is correct. All Unknown1 features must have one unique value in REF. Unknown2
 * features may have multiple comma delimited values in REF. If there are multiple values then
 * a match against any of them will be registered as a valid match.
 */
class MatchFeatureExtractor
{
public:

  static QString className() { return "MatchFeatureExtractor"; }

  static int logWarnCount;

  MatchFeatureExtractor(bool evenClasses=false);

  /**
   * Adds a match creator to the list of matches that will be evaluated. There must be at least
   * one. The match creator's matches must implement MatchDetails.
   */
  void addMatchCreator(const std::shared_ptr<MatchCreator>& m);

  /**
   * Returns the results as an ARFF file.
   * http://www.cs.waikato.ac.nz/ml/weka/arff.html
   */
  QString getResults(bool useNulls = true);

  const DataSamples& getSamples() const { return _samples; }

  /**
   * Evaluates all the features in the given map.
   */
  void processMap(const std::shared_ptr<const OsmMap>& map);

private:

  DataSamples _samples;
  std::vector<std::shared_ptr<MatchCreator>> _creators;
  // if true then make sure there is an even representation from each class and only fully populated
  // records are represented.
  bool _evenClasses;
  bool _useNulls;
  const MatchFactory* _matchFactory;

  MatchType _getActualMatchType(const std::set<ElementId> &eids,
                                const std::shared_ptr<const OsmMap>& map) const;

  /**
   * Returns a vector of unique factor labels. Not efficient.
   */
  std::vector<std::string> _getUniqueLabels() const;

  void _resampleClasses();
};

}

#endif // MANIPULATORFEATUREEXTRACTOR_H
