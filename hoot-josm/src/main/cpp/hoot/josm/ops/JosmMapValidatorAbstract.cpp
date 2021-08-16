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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "JosmMapValidatorAbstract.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/josm/jni/JavaEnvironment.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/josm/jni/JniConversion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/josm/jni/JniUtils.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

JosmMapValidatorAbstract::JosmMapValidatorAbstract() :
_maxElementsForMapString(2000000),
_javaEnv(JavaEnvironment::getInstance().getEnvironment()),
_josmInterfaceInitialized(false),
_numValidationErrors(0),
_numFailingValidators(0)
{
  _josmInterfaceName = ConfigOptions().getJosmMapValidatorJavaImplementation();
}

JosmMapValidatorAbstract::~JosmMapValidatorAbstract()
{
  _josmInterface = nullptr;
  _josmInterfaceClass = nullptr;
}

void JosmMapValidatorAbstract::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  LOG_VARD(opts.getJosmValidatorsFile());
  _josmValidators = FileUtils::readFileToList(opts.getJosmValidatorsFile());
  _maxElementsForMapString = opts.getJosmMaxElementsForMapString();
}

void JosmMapValidatorAbstract::_initJosmImplementation()
{
  LOG_DEBUG("Initializing JOSM implementation...");

  LOG_VART(_josmInterfaceName);
  jstring interfaceJavaStr = JniConversion::toJavaString(_javaEnv, _josmInterfaceName);
  //LOG_VART(interfaceJavaStr);
  jboolean isCopy;
  const char* interfaceChars = _javaEnv->GetStringUTFChars(interfaceJavaStr, &isCopy);
  LOG_VART(interfaceChars);
  _josmInterfaceClass = _javaEnv->FindClass(interfaceChars);
  LOG_VART(_josmInterfaceClass == 0);
  _javaEnv->ReleaseStringUTFChars(interfaceJavaStr, interfaceChars);

  jstring logLevel = JniConversion::toJavaString(_javaEnv, Log::getInstance().getLevelAsString());
  _josmInterface =
    // Thought it would be best to wrap this in a global ref, since its possible it could be garbage
    // collected at any time. Then it could have been cleaned up in the destructor. However, that's
    // causing crashes during the tests. I've not observed any leaks w/o using the global ref when
    // running against large inputs so not using it.
    _javaEnv->NewObject(
      _josmInterfaceClass,
      // Java sig: <ClassName>(String logLevel)
      _javaEnv->GetMethodID(_josmInterfaceClass, "<init>", "(Ljava/lang/String;)V"),
      logLevel);
  LOG_VART(_josmInterface == 0);
  JniUtils::checkForErrors(_javaEnv, _josmInterfaceName + " constructor");
  _javaEnv->DeleteLocalRef(logLevel);
  _josmInterfaceInitialized = true;

  LOG_DEBUG("JOSM implementation initialized.");
}

QMap<QString, QString> JosmMapValidatorAbstract::getValidatorDetail()
{
  if (_josmValidators.isEmpty())
  {
    throw IllegalArgumentException("No validators configured.");
  }

  LOG_DEBUG("Retrieving available validators...");

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }

  jobject validatorsJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // JNI sig format: (input params...)return type
      // Java sig: Map<String, String> getValidatorDetail(List<String> validators)
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidatorDetail", "(Ljava/util/List;)Ljava/util/Map;"),
        JniConversion::toJavaStringList(_javaEnv, _josmValidators));
  JniUtils::checkForErrors(_javaEnv, "getValidatorDetail");
  return JniConversion::fromJavaStringMap(_javaEnv, validatorsJavaMap);
}

void JosmMapValidatorAbstract::apply(std::shared_ptr<OsmMap>& map)
{
  if (_josmValidators.isEmpty())
  {
    throw IllegalArgumentException("No validators configured.");
  }

  LOG_VARD(map->size());
  //LOG_TRACE("Input map: " << OsmXmlWriter::toString(map, true));

  _numAffected = map->size();
  _numValidationErrors = 0;
  _numFailingValidators = 0;

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }

  // Pass the map to JOSM for updating.
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

  JniUtils::checkForErrors(_javaEnv, "JosmMapValidatorAbstract::apply");

  if (Log::getInstance().getLevel() > Log::Debug)
  {
    LOG_INFO(getCompletedStatusMessage());
  }
}

void JosmMapValidatorAbstract::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // JNI sig format: (input params...)return type

  _numValidationErrors = _getNumValidationErrors();
  _numFailingValidators = _getNumFailingValidators();

  // TODO: Try to simplify this with SingleStat.
  _errorSummary =
    "Total JOSM validation errors: " + StringUtils::formatLargeNumber(_numValidationErrors) +
    " found in " + StringUtils::formatLargeNumber(_numAffected) + " total features.\n";
  // convert the validation error info to a readable summary string
  _errorSummary += _errorCountsByTypeToSummaryStr(_getValidationErrorCountsByType());
  _errorSummary +=
    "Total failing JOSM validators: " + QString::number(_numFailingValidators);
  _errorSummary = _errorSummary.trimmed();
  LOG_VART(_errorSummary);
}

int JosmMapValidatorAbstract::_getNumValidationErrors()
{
  const int numValidationErrors =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumValidationErrors()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumValidationErrors", "()I"));
  JniUtils::checkForErrors(_javaEnv, "getNumValidationErrors");
  return numValidationErrors;
}

int JosmMapValidatorAbstract::_getNumFailingValidators()
{
  const int numFailingValidators =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      // Java sig: int getNumFailingValidators()
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumFailingValidators", "()I"));
  JniUtils::checkForErrors(_javaEnv, "getNumFailingValidators");
  return numFailingValidators;
}

QMap<QString, int> JosmMapValidatorAbstract::_getValidationErrorCountsByType()
{
  jobject validationErrorCountsByTypeJavaMap =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      // Java sig: Map<String, Integer> getValidationErrorCountsByType()
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/util/Map;"));
  JniUtils::checkForErrors(_javaEnv, "getValidationErrorCountsByType");
  return JniConversion::fromJavaStringIntMap(_javaEnv, validationErrorCountsByTypeJavaMap);
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
