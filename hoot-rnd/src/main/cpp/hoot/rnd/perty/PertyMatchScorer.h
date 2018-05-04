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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PERTY_MAP_SCORER_H
#define PERTY_MAP_SCORER_H

// Hoot
#include <hoot/core/scoring/MatchComparator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class OsmMap;

/**
 * Scores the accuracy of conflating an unmodified file with a PERTY'd version of that file
 */
class PertyMatchScorer : public Configurable
{
public:

  PertyMatchScorer();

  /**
    Takes a single file as input, runs PERTY on the file, and then conflates the unmodified data
    with the PERTY'd data

    @param referenceMapInputPath input file path for the reference data
    @param outputPath output directory
    @returns a match comparator from which the PERTY score can be retrieved
    */
  boost::shared_ptr<MatchComparator> scoreMatches(const QString referenceMapInputPath,
                                           const QString outputPath);

  /**
    @see Configurable
    */
  virtual void setConfiguration(const Settings &conf) { _settings = conf; }

  /**
    Returns the output path of the reference map
    */
  QString getReferenceMapOutput() { return _referenceMapOutput; }
  /**
    Returns the output path of the perturbed map
    */
  QString getPerturbedMapOutput() { return _perturbedMapOutput; }
  /**
    Returns the output path of the conflated map
    */
  QString getConflatedMapOutput() { return _conflatedMapOutput; }

  /**
    Returns the search distance used during conflation
    */
  double getSearchDistance() { return _searchDistance; }
  /**
    Sets the search distance used during conflation
    */
  void setSearchDistance(double searchDistance) { _searchDistance = searchDistance; }

  /**
    Returns whether rubbersheeting is applied to the input pre-conflation
    */
  bool getApplyRubberSheet() { return _applyRubberSheet; }
  /**
    Sets whether rubbersheeting is applied to the input pre-conflation
    */
  void setApplyRubberSheet(bool applyRubberSheet) { _applyRubberSheet = applyRubberSheet; }

  /**
    Returns a string representation of the object
    */
  QString toString();

private:

  //for testing purposes
  friend class PertyMatchScorerTest;

  Settings _settings;

  OsmMapPtr _loadReferenceMap(const QString referenceMapInputPath,
                                       const QString referenceMapOutputPath);
  void _loadPerturbedMap(const QString perturbedMapInputPath,
                         const QString perturbedMapOutputPath);
  OsmMapPtr _combineMapsAndPrepareForConflation(OsmMapPtr referenceMap,
                                                         const QString perturbedMapInputPath);
  boost::shared_ptr<MatchComparator> _conflateAndScoreMatches(OsmMapPtr combinedDataToConflate,
                                                       const QString conflatedMapOutputPath);

  /**
   * Prepares map for saving and saves the map. The map will be modified.
   */
  void _saveMap(OsmMapPtr map, QString path);

  QString _referenceMapOutput;
  QString _perturbedMapOutput;
  QString _conflatedMapOutput;

  double _searchDistance;
  bool _applyRubberSheet;

};

}

#endif // PERTY_MAP_SCORER_H
