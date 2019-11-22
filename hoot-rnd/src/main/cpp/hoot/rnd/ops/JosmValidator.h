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
#ifndef JOSM_VALIDATOR_H
#define JOSM_VALIDATOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

// JNI
#include <jni.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * TODO
 */
class JosmValidator : public OsmMapOperation, public Configurable
{

public:

  static std::string className() { return "hoot::JosmValidator"; }

  /**
   * TODO
   *
   * @param fixFeatures
   */
  JosmValidator(const bool fixFeatures = false);

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * TODO
   *
   * @return
   */
  QMap<QString, QString> getAvailableValidators() const;
  void setValidatorsToUse(const QStringList& validators) { _validatorsToUse = validators; }

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const { return "TODO"; }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

private:

  // TODO
  QStringList _validatorsToUse;
  // TODO
  bool _fixFeatures;

  // TODO
  jclass _validatorClass;
  // TODO
  jobject _validator;
};

}

#endif // JOSM_VALIDATOR_H
