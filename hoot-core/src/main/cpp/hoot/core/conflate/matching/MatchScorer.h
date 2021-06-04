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
#ifndef MATCH_SCORER_H
#define MATCH_SCORER_H

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>

// Tgs
#include <tgs/Optimization/NelderMead.h>

namespace hoot
{

/**
 * TODO
 */
class MatchScorer
{
public:

  MatchScorer();
  ~MatchScorer() = default;

  /**
   * TODO
   *
   * @param ref1Inputs
   * @param ref2Inputs
   * @param output
   */
  void score(
    const QStringList& ref1Inputs, const QStringList& ref2Inputs, const QString output = "");

  /**
   * TODO
   *
   * @param maps
   * @param output
   * @param mt
   * @param showConfusion
   * @param score
   * @return
   */
  QString evaluateThreshold(
    const std::vector<OsmMapPtr>& maps, const QString& output,
    const std::shared_ptr<MatchThreshold>& mt, const bool showConfusion, double& score) const;

  void setPrintConfusion(bool print) { _printConfusion = print; }
  void setOptimizeThresholds(bool optimize) { _optimizeThresholds = optimize; }
  void setValidateManualMatches(bool validate) { _validateManualMatches = validate; }

private:

  bool _printConfusion;
  bool _optimizeThresholds;
  bool _validateManualMatches;

  void _optimize(const std::vector<OsmMapPtr>& maps, const bool showConfusion);

  bool _validateMatches(
    const OsmMapPtr& map, const QString& map1Path, const QString& map2Path) const;
  void _printIssues(
    const QMap<ElementId, QString>& issues, const QString& type, const QString& map1Path,
    const QString& map2Path) const;
};

}

#endif // MATCH_SCORER_H
