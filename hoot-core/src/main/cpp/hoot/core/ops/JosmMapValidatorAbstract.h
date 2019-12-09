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
#ifndef JOSM_MAP_VALIDATOR_ABSTRACT_H
#define JOSM_MAP_VALIDATOR_ABSTRACT_H

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
 * TODO
 */
class JosmMapValidatorAbstract : public OsmMapOperation, public Configurable,
  public OperationStatusInfo
{

public:

  JosmMapValidatorAbstract();

  virtual void setConfiguration(const Settings& conf);

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  QString getSummary() const { return _errorSummary; }

  QStringList getAvailableValidators();
  QMap<QString, QString> getAvailableValidatorsWithDescription();

  int getNumElementsProcessed() const { return _numAffected; }
  int getNumValidationErrors() const { return _numValidationErrors; }

  QStringList getJosmValidatorsUsed() const { return _josmValidators; }
  QString getValidatorsJosmNamespace() const { return _validatorsJosmNamespace; }
  void setValidatorsJosmNamespace(const QString& validatorsNamespace)
  { _validatorsJosmNamespace = validatorsNamespace; }
  void setJosmInterfaceName(const QString& name) { _josmInterfaceName = name; }
  void setJosmValidatorsExclude(const QStringList& validators)
  { _josmValidatorsExclude = validators; }
  void setJosmValidatorsInclude(const QStringList& validators)
  { _josmValidatorsInclude = validators; }

protected:

  // TODO
  QString _josmInterfaceName;
  // TODO
  QStringList _josmValidators;

  // TODO
  JNIEnv* _javaEnv;
  // TODO
  jclass _josmInterfaceClass;
  // TODO
  jobject _josmInterface;
  // TODO
  bool _josmInterfaceInitialized; 

  // TODO
  int _numValidationErrors;
  QString _errorSummary;

  // can get away with this in the base class b/c all the josm java classes used by all children
  // classes have the same signature...this may not be true forever
  virtual void _initJosmImplementation();

  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap) = 0;

  virtual void _getStats();

  QString _errorCountsByTypeStrToSummaryStr(const QString& errorCountsByTypeStr) const;

private:

  friend class JosmMapCleanerTest;

  // TODO - assuming a single one for now
  QString _validatorsJosmNamespace;
  // TODO
  QStringList _josmValidatorsInclude;
  // TODO
  QStringList _josmValidatorsExclude;

  void _initJosmValidatorsList();
  void _updateJosmValidatorsWithNamepace(QStringList& validators);
};

}

#endif // JOSM_MAP_VALIDATOR_ABSTRACT_H
