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
#ifndef JOSM_MAP_VALIDATOR_ABSTRACT_H
#define JOSM_MAP_VALIDATOR_ABSTRACT_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

// JNI
#include <jni.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Base class for classes interacting with JOSM for validation or cleaning purposes
 *
 * Helpful JNI references:
 *
 * https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html
 * http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
 *
 * Note that we're passing maps as XML to JOSM vs trying to build a JOSM map dataset to avoid
 * complexity in the code.
 */
class JosmMapValidatorAbstract : public OsmMapOperation, public Configurable
{

public:

  JosmMapValidatorAbstract();

  virtual ~JosmMapValidatorAbstract();

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * Returns a collection of available JOSM validators
   *
   * @return collection of validator Java class names
   */
  QStringList getAvailableValidators();

  /**
   * Returns a collection of available JOSM validators with descriptions
   *
   * @return collection of validator Java class names with descriptions
   */
  QMap<QString, QString> getAvailableValidatorsWithDescription();

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see OperationStatusInfo
   */
  virtual QString getCompletedStatusMessage() const { return _errorSummary; }

  QString getSummary() const { return _errorSummary; }
  int getNumElementsProcessed() const { return _numAffected; }
  int getNumValidationErrors() const { return _numValidationErrors; }
  int getNumFailingValidators() const { return _numFailingValidators; }

  QStringList getJosmValidatorsUsed() const { return _josmValidators; }
  void setJosmValidatorsExclude(const QStringList& validators)
  { _josmValidatorsExclude = validators; }
  void setJosmValidatorsInclude(const QStringList& validators)
  { _josmValidatorsInclude = validators; }
  void setMaxElementsForMapString(int max) { _maxElementsForMapString = max; }

protected:

  // path plus class name of the hoot-josm class used to interact with JOSM validation;
  // of the form: path/to/class/ClassName
  QString _josmInterfaceName;
  // list of Java class names with namespaces to use during validation
  QStringList _josmValidators;
  // Since JOSM doesn't have a streaming interface for loading maps, we need a cutoff point above
  // which we'll serialize all maps passed to/from JOSM to file rather than passing them as a
  // string. If this value is set too high, bad string allocs will occur.
  int _maxElementsForMapString;

  // JNI environment
  JNIEnv* _javaEnv;
  // JNI static class ref corresponding to class referenced in _josmInterfaceName
  jclass _josmInterfaceClass;
  // instantiated JNI object corresponding to class referenced in _josmInterfaceName
  jobject _josmInterface;
  bool _josmInterfaceInitialized; 

  // total number of validation errors found
  int _numValidationErrors;
  // the number of validators that threw an error during validation
  int _numFailingValidators;
  // a printable summer of validation errors found
  QString _errorSummary;

  void _checkForErrors(const QString& operationName);

  /*
   * Passes a map to a JOSM operation and returns it with JOSM updates
   */
  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap) = 0;

  /*
   * Retrieves validation stats
   */
  virtual void _getStats();

  QMap<QString, int> _getValidationErrorCountsByType();
  int _getNumFailingValidators();
  int _getNumValidationErrors();

  /*
   * Converts JOSM validation stats to a printable string
   */
  QString _errorCountsByTypeToSummaryStr(const QMap<QString, int>& errorCountsByType) const;

private:

  friend class JosmMapValidatorTest;
  friend class JosmMapCleanerTest;

  // a list of explicit JOSM validator class names without prefixed namespace to be used as
  // validators; can be overridden by _josmValidatorsExclude
  QStringList _josmValidatorsInclude;
  // a list of explicit JOSM validator class name excludes without prefixed namespace to be excluded
  // as validators; overrides _josmValidatorsInclude
  QStringList _josmValidatorsExclude;
  // a known list of validators that need a user cert to operate; will skip them
  QStringList _josmValidatorsRequiringUserCert;

  void _initJosmValidatorsList();
  void _initJosmImplementation();
};

}

#endif // JOSM_MAP_VALIDATOR_ABSTRACT_H
