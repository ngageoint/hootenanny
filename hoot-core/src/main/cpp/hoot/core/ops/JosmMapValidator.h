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
#ifndef JOSM_MAP_VALIDATOR_H
#define JOSM_MAP_VALIDATOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/info/OperationStatusInfo.h>

// JNI
#include <jni.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * TODO - finish
 */
class JosmMapValidator : public OsmMapOperation, public Configurable, public OperationStatusInfo
{

public:

  static std::string className() { return "hoot::JosmMapValidator"; }

  /**
   * TODO
   */
  JosmMapValidator();

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const { return "TODO"; }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "Validating elements..."; }

  virtual QString getCompletedStatusMessage() const
  {
//    return
//      QString::number(_numValidationErrors) + " / " + QString::number(_numAffected) +
//      " elements had validation errors.";
    return "";
  }

private:

};

}

#endif // JOSM_MAP_VALIDATOR_H
