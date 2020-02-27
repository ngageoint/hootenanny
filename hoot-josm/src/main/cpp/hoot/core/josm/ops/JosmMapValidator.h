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
#ifndef JOSM_MAP_VALIDATOR_H
#define JOSM_MAP_VALIDATOR_H

// hoot
#include <hoot/core/josm/ops/JosmMapValidatorAbstract.h>

namespace hoot
{

/**
 * Calls a hoot-josm Java map validator that runs selected JOSM validation routines on a map and
 * marks elements that fail validation with custom tags
 *
 * See note in JosmMapValidatorAbstract about handling of collection objects via JNI.
 */
class JosmMapValidator : public JosmMapValidatorAbstract
{

public:

  static std::string className() { return "hoot::JosmMapValidator"; }

  JosmMapValidator();

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const { return "Validates a map using JOSM"; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getInitStatusMessage() const
  { return "Validating elements with JOSM..."; }

protected:

  /*
   * @see JosmMapValidatorAbstract
   */
  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap);

  virtual std::string getClassName() const { return className(); }

private:

  /*
   * validate the map and return it as an xml string
   */
  QString _validate(const QStringList& validators, const QString& map);

  /*
   * validate the map from a file and write the validated map out to another file
   */
  void _validate(
    const QStringList& validators, const QString& inputMapPath, const QString& outputMapPath);
};

}

#endif // JOSM_MAP_VALIDATOR_H
