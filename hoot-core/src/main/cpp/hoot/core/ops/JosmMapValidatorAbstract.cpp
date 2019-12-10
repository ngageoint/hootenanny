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
#include <hoot/core/util/JavaEnvironment.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/JniConversion.h>

namespace hoot
{

JosmMapValidatorAbstract::JosmMapValidatorAbstract() :
_javaEnv(JavaEnvironment::getEnvironment()),
_josmInterfaceInitialized(false),
_numValidationErrors(0)
{
}

JosmMapValidatorAbstract::~JosmMapValidatorAbstract()
{
  if (_javaEnv != 0 && _josmInterface != 0)
  {
    _javaEnv->DeleteGlobalRef(_josmInterface);
  }
}

void JosmMapValidatorAbstract::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _josmValidatorsExclude = opts.getJosmValidatorsExclude();
  _josmValidatorsInclude = opts.getJosmValidatorsInclude();
}

void JosmMapValidatorAbstract::_initJosmImplementation()
{
  LOG_DEBUG("Initializing JOSM implementation...");

  // TODO: change back
  _josmInterfaceClass =
    _javaEnv->FindClass(/*_josmInterfaceName.toStdString().c_str()*/
                        "hoot/services/josm/JosmMapCleaner");
  jmethodID constructorMethodId =
    _javaEnv->GetMethodID(_josmInterfaceClass, "<init>", "(Ljava/lang/String;)V");
  jstring logLevelStr =
    _javaEnv->NewStringUTF(Log::getInstance().getLevelAsString().toStdString().c_str());
  // Grab this as a global ref, since its possible it could be garbage collected at any time. We'll
  // clean it up in the destructor.
  _josmInterface =
    _javaEnv->NewGlobalRef(_javaEnv->NewObject(_josmInterfaceClass, constructorMethodId, logLevelStr));
  _josmInterfaceInitialized = true;

  LOG_DEBUG("JOSM implementation initialized.");
}

void JosmMapValidatorAbstract::_initJosmValidatorsList()
{
  LOG_DEBUG("Initializing validators...");

  _josmValidatorsInclude.sort();
  _josmValidatorsExclude.sort();

  // If an include list was specified, let's start with that.
  _josmValidators = _josmValidatorsInclude;

  // The exclude list overrides the include list, so check it.
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

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }
  if (_josmValidators.isEmpty())
  {
    _initJosmValidatorsList();
  }

  // pass the map into JOSM and update it
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
}

void JosmMapValidatorAbstract::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into the hoot-josm validator to get the stats after validation

  _numValidationErrors =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: int getNumValidationErrors()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumValidationErrors", "()I"));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    throw HootException("Error calling getNumValidationErrors.");
  }

  jobject validationErrorCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: Map<String, Integer> getValidationErrorCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/util/Map;"));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    throw HootException("Error calling getValidationErrorCountsByType.");
  }

  _errorSummary = "Total validation errors: " + QString::number(_numValidationErrors) + "\n";
  // convert the validation error info to a readable summary string
  _errorSummary +=
    _errorCountsByTypeToSummaryStr(
      JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorCountsByTypeJavaMap));
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
