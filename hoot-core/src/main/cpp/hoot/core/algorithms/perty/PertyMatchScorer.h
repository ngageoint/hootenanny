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
 * @brief The PertyMatchScorer class scores the accuracy of conflating an unmodified file with a
 * PERTY perturbed version of that file.
 */
class PertyMatchScorer : public Configurable
{
public:

  static QString className() { return "hoot::PertyMatchScorer"; }

  PertyMatchScorer();
  ~PertyMatchScorer() = default;

  /**
    @brief scoreMatches takes a single file as input, runs PERTY on the file, and then conflates the
    unmodified data with the PERTY'd data
    @param referenceMapInputPath input file path for the reference data
    @param outputPath output directory
    @returns a match comparator from which the PERTY score can be retrieved
    */
  std::shared_ptr<MatchComparator> scoreMatches(const QString& referenceMapInputPath,
                                                const QString& outputPath);

  /**
    @see Configurable
    */
  void setConfiguration(const Settings &conf) override { _settings = conf; }

  /**
    @brief getReferenceMapOutput returns the output path of the reference map.
    */
  QString getReferenceMapOutput() const { return _referenceMapOutput; }
  /**
    @brief getPerturbedMapOutput returns the output path of the perturbed map.
    */
  QString getPerturbedMapOutput() const { return _perturbedMapOutput; }
  /**
    @brief getConflatedMapOutput returns the output path of the conflated map.
    */
  QString getConflatedMapOutput() const { return _conflatedMapOutput; }
  /**
    @brief getSearchDistance returns the search distance used during conflation.
    */
  double getSearchDistance() const { return _searchDistance; }

  /**
    @brief setSearchDistance sets the search distance used during conflation.
    */
  void setSearchDistance(double searchDistance) { _searchDistance = searchDistance; }

  /**
    @brief toString returns a string representation of the object.
    */
  QString toString() const;

private:

  //for testing purposes
  friend class PertyMatchScorerTest;

  Settings _settings;

  OsmMapPtr _loadReferenceMap(const QString& referenceMapInputPath,
                              const QString& referenceMapOutputPath) const;
  void _loadPerturbedMap(const QString& perturbedMapInputPath,
                         const QString& perturbedMapOutputPath) const;
  OsmMapPtr _combineMapsAndPrepareForConflation(const OsmMapPtr& referenceMap,
                                                const QString& perturbedMapInputPath) const;
  std::shared_ptr<MatchComparator> _conflateAndScoreMatches(const OsmMapPtr& combinedDataToConflate,
                                                            const QString& conflatedMapOutputPath) const;

  /**
   * Prepares map for saving and saves the map. The map will be modified.
   */
  void _saveMap(OsmMapPtr& map, const QString& path) const;

  QString _referenceMapOutput;
  QString _perturbedMapOutput;
  QString _conflatedMapOutput;

  double _searchDistance;
};

}

#endif // PERTY_MAP_SCORER_H
