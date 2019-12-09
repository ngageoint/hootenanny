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
 * Base class for classes interacting with JOSM for validation or cleaning purposes
 *
 * Helpful JNI reference:
 * https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html
 *
 * Note that in JNI interactions the choice has been made to pass in and return delimited strings
 * in place of Java collection classes. The passed in delimited strings are converted to Java
 * collection objects where necessary in the hoot-josm Java code and the returned strings are
 * converted to C++ collection objects in this and inheriting classes. This cuts down on the number
 * of overall JNI calls made for performance improvement reasons and also keeps the JNI client
 * code simpler arguably at some cost of readability. However, no specific comparison has been done
 * yet of performance using JNI to create collection classes vs passing delimited strings. So if
 * at some point we prove there isn't signficant overhead using JNI to create collection classes we
 * could convert this code over to do that instead.
 */
class JosmMapValidatorAbstract : public OsmMapOperation, public Configurable,
  public OperationStatusInfo
{

public:

  JosmMapValidatorAbstract();

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

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

  QString getSummary() const { return _errorSummary; }
  int getNumElementsProcessed() const { return _numAffected; }
  int getNumValidationErrors() const { return _numValidationErrors; }

  QStringList getJosmValidatorsUsed() const { return _josmValidators; }
  void setJosmValidatorsExclude(const QStringList& validators)
  { _josmValidatorsExclude = validators; }
  void setJosmValidatorsInclude(const QStringList& validators)
  { _josmValidatorsInclude = validators; }

protected:

  // path plus class name of the hoot-josm class used to interact with JOSM validation;
  // of the form: path/to/class/ClassName
  QString _josmInterfaceName;
  // list of Java class names with namespaces to use during validation
  QStringList _josmValidators;

  // JNI environment
  JNIEnv* _javaEnv;
  // JNI static class ref corresponding to class referenced in _josmInterfaceName
  jclass _josmInterfaceClass;
  // instantiated JNI object corresponding to class referenced in _josmInterfaceName
  jobject _josmInterface;
  bool _josmInterfaceInitialized; 

  // total number of validation errors found
  int _numValidationErrors;
  // a printable summer of validation errors found
  QString _errorSummary;

  /*
   * Passes a map to a JOSM operation and returns it with JOSM updates
   */
  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap) = 0;

  /*
   * Retrieves validation stats
   */
  virtual void _getStats();

  /*
   * Converts JOSM validation stats to a printable string
   */
  QString _errorCountsByTypeStrToSummaryStr(const QString& errorCountsByTypeStr) const;

private:

  friend class JosmMapValidatorTest;
  friend class JosmMapCleanerTest;

  // a list of explicit JOSM validator class names without prefixed namespace to be used as
  // validators; can be overridden by _josmValidatorsExclude
  QStringList _josmValidatorsInclude;
  // a list of explicit JOSM validator class name excludes without prefixed namespace to be excluded
  // as validators; overrides _josmValidatorsInclude
  QStringList _josmValidatorsExclude;

  void _initJosmValidatorsList();
  void _initJosmImplementation();
};

}

#endif // JOSM_MAP_VALIDATOR_ABSTRACT_H
