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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MANUAL_MATCH_VALIDATOR_H
#define MANUAL_MATCH_VALIDATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/visitors/ElementIdToTagValueMapper.h>

namespace hoot
{

/**
 * Finds errors in manual matches made by a human that are to be scored against hoot conflation
 * with the score-matches command
 */
class ManualMatchValidator : public ConstOsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::ManualMatchValidator"; }

  ManualMatchValidator();

  /**
   * Validates all manual matches in the map and records the first error found for each element
   *
   * @param map
   */
  virtual void apply(const OsmMapPtr& map);

  virtual QString getDescription() const { return "Validates manual matches"; }

  virtual QString getInitStatusMessage() const
  { return "Validating manual matches..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Validated " + QString::number(_numAffected) + " manual matches"; }

  /**
   * Returns found errors grouped by element ID (up to one for each element)
   *
   * @return a collection of errors
   */
  QMap<ElementId, QString> getErrors() const { return _errors; }

  /**
   * Determines if any errors were found by validation
   *
   * @return true if any errors were found; false otherwise
   */
  bool hasErrors() const { return _errors.size() > 0; }

  void setRequireRef1(bool require) { _requireRef1 = require; }

private:

  QMap<ElementId, QString> _errors;
  ElementIdToTagValueMapper _ref1Mappings;
  // TODO
  /*
   * There was one regression test where the crop bounds for some of the data caused some of
   * the manual matches to drop out b/c they were right on the crop boundary. This wasn't a
   * problem in that situation b/c they were simply ignored. You could make the argument that
   * ignoring the problem in that situation is ok (and add a configuration option for it), or
   * you could update the crop settings to not split features. Since this only occurred with
   * one dataset, leaving the behavior as error in order to have the utmost awareness during
   * match scoring. All of this would apply to the correspong logic in the next loop for
   * reviews as well.
   */

  bool _requireRef1;

  void _validate(const ConstElementPtr& element);
  bool _isValidRef1Id(const QString& matchId) const;
  bool _isValidRef2OrReviewId(const QString& matchId) const;
  bool _isValidUniqueMatchId(const QString& matchId) const;
  bool _isValidNonUniqueMatchId(const QString& matchId) const;
  void _recordError(const ConstElementPtr& element, QString errorMessage);
};

}

#endif // MANUAL_MATCH_VALIDATOR_H
