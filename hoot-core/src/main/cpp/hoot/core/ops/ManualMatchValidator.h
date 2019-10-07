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
 * TODO
 */
class ManualMatchValidator : public ConstOsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::ManualMatchValidator"; }

  ManualMatchValidator();

  /**
   * TODO
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
   * TODO
   *
   * @param map
   * @return
   */
  QMap<ElementId, QStringList> getErrors() const { return _errors; }

private:

  QMap<ElementId, QStringList> _errors;
  ElementIdToTagValueMapper _ref1Mappings;

  void _validate(const ConstElementPtr& element);
  bool _isValidManualMatchId(const QString& matchId) const;
};

}

#endif // MANUAL_MATCH_VALIDATOR_H
