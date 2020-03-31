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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MANUAL_MATCH_VALIDATOR_H
#define MANUAL_MATCH_VALIDATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/visitors/ElementIdToTagValueMapper.h>

namespace hoot
{

/**
 * Finds errors in manual matches made by a human that are to be scored against hoot conflation
 * with the score-matches command
 *
 * A note on _requireRef1:
 *
 * If a regression test already exists where the input data has been cropped and the cropping
 * split any features, you will end up with missing ref1 tags. One remedy may be to re-create the
 * cropped input data with crop.keep.entire.features.crossing.bounds=true, but this might resut in
 * larger inputs than you want. If the manual matches are dense, it might not work anyway. If
 * re-creating the input data is not an option (input data created before this class was integrated
 * with score-matches, etc.), then you can change the validator to only log warnings, which allows
 * score-matches to proceed with missing ref1's by setting _requireRef1=false;
 */
class ManualMatchValidator : public ConstOsmMapOperation
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
   * Returns found warnings grouped by element ID (up to one for each element)
   *
   * @return a collection of warnings
   */
  QMap<ElementId, QString> getWarnings() const { return _warnings; }

  /**
   * Determines if any errors were found by validation
   *
   * @return true if any errors were found; false otherwise
   */
  bool hasErrors() const { return _errors.size() > 0; }

  /**
   * Determines if any warnings were found by validation
   *
   * @return true if any errors were found; false otherwise
   */
  bool hasWarnings() const { return _warnings.size() > 0; }

  void setRequireRef1(bool require) { _requireRef1 = require; }
  void setAllowUuidManualMatchIds(bool allow) { _allowUuidManualMatchIds = allow; }
  void setFullDebugOutput(bool fullDebugOutput) { _fullDebugOutput = fullDebugOutput; }

  virtual std::string getClassName() const { return className(); }

private:

  QMap<ElementId, QString> _errors;
  QMap<ElementId, QString> _warnings;
  ElementIdToTagValueMapper _ref1Mappings;

  // if true, every ref2/review id must have a correponding ref1 id in order to not trigger an error
  bool _requireRef1;

  // The original manual matching implementation used uuids as ids instead of the 6 char hex used
  // now, since manual matchers found the uuids a little unwieldy. There are still some regression
  // tests with the uuid ids, so to avoid the cost of redoing the matches we'll support them on a
  // case by case basis.
  bool _allowUuidManualMatchIds;
  QRegExp _uuidRegEx;

  // TODO
  bool _fullDebugOutput;

  void _validate(const ConstElementPtr& element);

  bool _isValidRef1Id(const QString& matchId) const;
  bool _isValidRef2OrReviewId(const QString& matchId) const;
  bool _isValidUniqueMatchId(const QString& matchId) const;
  bool _isValidNonUniqueMatchId(const QString& matchId) const;

  void _recordIssue(const ConstElementPtr& element, QString message, const bool isError = true);
};

}

#endif // MANUAL_MATCH_VALIDATOR_H
