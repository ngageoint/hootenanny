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
#include "JosmMapValidatorAbstract.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/jni/JavaEnvironment.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/jni/JniConversion.h>
#include <hoot/core/util/MapProjector.h>

namespace hoot
{

JosmMapValidatorAbstract::JosmMapValidatorAbstract() :
_logMissingCertAsWarning(true),
_javaEnv(JavaEnvironment::getEnvironment()),
_josmInterfaceInitialized(false),
_numValidationErrors(0),
_numFailingValidators(0)
{
}

JosmMapValidatorAbstract::~JosmMapValidatorAbstract()
{
  //if (_javaEnv != 0 && _josmInterface != 0)
  //{
    // We have a memory leak if this doesn't happen.
    //LOG_DEBUG("Deleting josm interface...");
    //_javaEnv->DeleteGlobalRef(_josmInterface);
  //}
}

void JosmMapValidatorAbstract::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _josmValidatorsExclude = opts.getJosmValidatorsExclude();
  _josmValidatorsInclude = opts.getJosmValidatorsInclude();
  _josmCertificatePath = opts.getJosmCertificatePath();
  _josmCertificatePassword = opts.getJosmCertificatePassword();
}

void JosmMapValidatorAbstract::_initJosmImplementation()
{
  LOG_DEBUG("Initializing JOSM implementation...");

  if (_logMissingCertAsWarning &&
      (_josmCertificatePath.trimmed().isEmpty() || _josmCertificatePassword.trimmed().isEmpty()))
  {
    LOG_WARN("No JOSM user certificate specified. Some validators will be unavailable for use.");
  }

  // TODO: change back
  _josmInterfaceClass =
    _javaEnv->FindClass(/*_josmInterfaceName.toStdString().c_str()*/
                        "hoot/services/josm/JosmMapCleaner");
  // Using the same constructor signature for both validation and cleaning works since both Java
  // implementations use it. If the signatures change, then this logic will need to be moved down
  // to the child classes.
  _josmInterface =
    // Thought it would be best to grab this as a global ref, since its possible it could be garbage
    // collected at any time. Then it can be cleaned up in the destructor. However, that's causing
    // crashes during the tests, so need to think about it some more.
    /*_javaEnv->NewGlobalRef(*/
      _javaEnv->NewObject(
        _josmInterfaceClass,
        // Java sig: <ClassName>(String logLevel, String userCertPath, String userCertPassword)
        _javaEnv->GetMethodID(
          _josmInterfaceClass, "<init>",
          "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"),
        // logLevel
        JniConversion::toJavaString(_javaEnv, Log::getInstance().getLevelAsString()),
        // userCertPath
        JniConversion::toJavaString(_javaEnv, _josmCertificatePath),
        // userCertPassword
        JniConversion::toJavaString(_javaEnv, _josmCertificatePassword))/*)*/;
  JniConversion::checkForErrors(_javaEnv, _josmInterfaceName + " constructor");
  _josmInterfaceInitialized = true;

  LOG_DEBUG("JOSM implementation initialized.");
}

void JosmMapValidatorAbstract::_initJosmValidatorsList()
{
  LOG_DEBUG("Initializing validators...");

  _josmValidatorsInclude.sort();
  _josmValidatorsExclude.sort();

  if (_josmValidatorsInclude.size() > 0)
  {
    // If an include list was specified, let's start with that.
    _josmValidators = _josmValidatorsInclude;
  }
  else
  {
    // If no include list was specified, let's grab all the validators.
    _josmValidators = getAvailableValidators();
  }

  // The exclude list overrides the include list, so subtract it from our current list.
  StringUtils::removeAll(_josmValidators, _josmValidatorsExclude);

  // make sure the include/exclude lists didn't conflict
  if (!_josmValidatorsInclude.isEmpty() && _josmValidators.isEmpty())
  {
    throw IllegalArgumentException(
      "Cleaner include/exclude lists resulted in no JOSM cleaners specified.");
  }

  // If validators are empty by this point, then just use them all.
  if (_josmValidators.isEmpty())
  {
    _josmValidators = getAvailableValidators();
  }

  LOG_VARD(_josmValidators);
}

QMap<QString, QString> JosmMapValidatorAbstract::getAvailableValidatorsWithDescription()
{
  LOG_DEBUG("Retrieving available validators...");

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }

  // make the hoot-josm call to get the validators and convert the delimited validators string into
  // a map
  jobject validatorsJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: Map<String, String> getAvailableValidators()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getAvailableValidators", "()Ljava/util/Map;"));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    throw HootException("Error calling getAvailableValidators.");
  }

  return JniConversion::fromJavaStringMap(_javaEnv, validatorsJavaMap);
}

QStringList JosmMapValidatorAbstract::getAvailableValidators()
{
  return getAvailableValidatorsWithDescription().keys();
}

void JosmMapValidatorAbstract::apply(std::shared_ptr<OsmMap>& map)
{
  LOG_VARD(map->size());
  //LOG_TRACE("Input map: " << OsmXmlWriter::toString(map, true));

  _numAffected = map->size();
  _numValidationErrors = 0;
  _numFailingValidators = 0;

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }
  if (_josmValidators.isEmpty())
  {
    _initJosmValidatorsList();
  }

  // pass the map to JOSM for updating
  MapProjector::projectToWgs84(map);
  OsmMapPtr validatedMap = _getUpdatedMap(map);
  if (validatedMap)
  {
    LOG_VARD(validatedMap->size());
    map = validatedMap;

    // get statistics about the validation
    _getStats();
  }
  else
  {
    LOG_ERROR("No map returned from JOSM validation.");
  }

  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    throw HootException("Error calling getAvailableValidatorsWithDescription.");
  }

  // TODO: convert to debug?
  LOG_INFO(getCompletedStatusMessage());
}

void JosmMapValidatorAbstract::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into the hoot-josm validator to get the stats after validation

  // JNI sig format: (input params...)return type

  _numValidationErrors =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumValidationErrors()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumValidationErrors", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumValidationErrors");

  jobject validationErrorCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, Integer> getValidationErrorCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/util/Map;"));
  JniConversion::checkForErrors(_javaEnv, "getValidationErrorCountsByType");

  _numFailingValidators =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumFailingValidators()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumFailingValidators", "()I"));
  JniConversion::checkForErrors(_javaEnv, "getNumFailingValidators");

  _errorSummary =
    "Total JOSM validation errors: " + StringUtils::formatLargeNumber(_numValidationErrors) +
    " / " + StringUtils::formatLargeNumber(_numAffected) + " total features.\n";
  // convert the validation error info to a readable summary string
  _errorSummary +=
    _errorCountsByTypeToSummaryStr(
      JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorCountsByTypeJavaMap));
  _errorSummary +=
    "Total failing JOSM validators: " + QString::number(_numFailingValidators);
  _errorSummary = _errorSummary.trimmed();
  LOG_VART(_errorSummary);
}

QString JosmMapValidatorAbstract::_errorCountsByTypeToSummaryStr(
  const QMap<QString, int>& errorCountsByType) const
{
  QString summary = "";
  for (QMap<QString, int>::const_iterator errorItr = errorCountsByType.begin();
       errorItr != errorCountsByType.end(); ++errorItr)
  {
    summary += errorItr.key() + " errors: " + QString::number(errorItr.value()) + "\n";
  }
  return summary;
}

}
