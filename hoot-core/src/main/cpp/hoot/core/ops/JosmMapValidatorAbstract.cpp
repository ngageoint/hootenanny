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

namespace hoot
{

JosmMapValidatorAbstract::JosmMapValidatorAbstract() :
_javaEnv(JavaEnvironment::getEnvironment()),
_josmInterfaceInitialized(false),
_numValidationErrors(0),
_validatorsJosmNamespace("org.openstreetmap.josm.data.validation.tests")
{
}

void JosmMapValidatorAbstract::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _josmValidatorsExclude = opts.getJosmValidatorsExclude();
  _josmValidatorsInclude = opts.getJosmValidatorsInclude();
  _validatorsJosmNamespace = opts.getJosmValidatorsJavaNamespace();
  _josmInterfaceName = opts.getMapCleanerJosmJavaImplementation();
}

void JosmMapValidatorAbstract::_initJosmImplementation()
{
  LOG_DEBUG("Initializing JOSM implementation...");

  // TODO: change back
  _josmInterfaceClass =
    _javaEnv->FindClass(/*_josmInterfaceName.toStdString().c_str()*/
                        "hoot/services/josm/JosmMapCleaner");;
  jmethodID constructorMethodId =
    _javaEnv->GetMethodID(_josmInterfaceClass, "<init>", "(Ljava/lang/String;)V");
  // TODO: change back
  jstring logLevelStr =
    _javaEnv->NewStringUTF(Log::getInstance().getLevelAsString().toStdString().c_str());
  //jstring logLevelStr = _javaEnv->NewStringUTF(QString("TRACE").toStdString().c_str());
  _josmInterface = _javaEnv->NewObject(_josmInterfaceClass, constructorMethodId, logLevelStr);
  // TODO: _javaEnv->ReleaseStringUTFChars
  _josmInterfaceInitialized = true;
}

void JosmMapValidatorAbstract::_initJosmValidatorsList()
{
  LOG_DEBUG("Initializing validators...");

  _josmValidatorsInclude.sort();
  _josmValidatorsExclude.sort();

  // assuming class names only for validators passed in
  _updateJosmValidatorsWithNamepace(_josmValidatorsInclude);
  _updateJosmValidatorsWithNamepace(_josmValidatorsExclude);

  // If an include list was specified, let's start with that.
  _josmValidators = _josmValidatorsInclude;

  // The exclude list overrides the include list, so check it too.
  StringUtils::removeAll(_josmValidators, _josmValidatorsExclude);

  if (!_josmValidatorsInclude.isEmpty() && _josmValidators.isEmpty())
  {
    throw IllegalArgumentException(
      "Cleaner include/exclude lists resulted in no JOSM cleaners specified.");
  }

  // If validators are empty at this point, then use them all.
  if (_josmValidators.isEmpty())
  {
    _josmValidators = getAvailableValidators().keys();
    _updateJosmValidatorsWithNamepace(_josmValidators);
  }
}

void JosmMapValidatorAbstract::_updateJosmValidatorsWithNamepace(QStringList& validators)
{
  QStringList validatorsTemp;
  for (int i = 0; i < validators.size(); i++)
  {
    validatorsTemp.append(_validatorsJosmNamespace + "." + validators.at(i));
  }
  validators = validatorsTemp;
}

QMap<QString, QString> JosmMapValidatorAbstract::getAvailableValidators()
{
  LOG_DEBUG("Getting available validators...");

  QMap<QString, QString> validators;
  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }

  jobject availableValidatorsResult =
    _javaEnv->CallObjectMethod(
      _josmInterface,
      _javaEnv->GetMethodID(_josmInterfaceClass, "getAvailableValidators", "()Ljava/lang/String;"));
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    // TODO: get exception message from object and add here
    throw HootException("Error calling getAvailableValidators.");
  }

  const char* str = _javaEnv->GetStringUTFChars((jstring)availableValidatorsResult, NULL);
  QString validatorsStr(str);
  assert(validatorsStr.contains(";"));
  QStringList validatorStrings = validatorsStr.split(";");
  for (int i = 0; i < validatorStrings.size(); i++)
  {
    const QString validatorStr = validatorStrings.at(i);
    if (!validatorStr.isEmpty())
    {
      assert(validatorStr.contains(","));
      const QStringList validatorStrParts = validatorStr.split(",");
      assert(validatorStrParts.size() == 2);

      const QString validatorName = validatorStrParts[0];
      const QString validatorDescription = validatorStrParts[1];
      validators[validatorName] = validatorDescription;
    }
  }
  //TODO: env->ReleaseStringUTFChars //??

  return validators;
}

void JosmMapValidatorAbstract::apply(std::shared_ptr<OsmMap>& map)
{
  LOG_VARD(map->size());
  //LOG_TRACE("Input map: " << OsmXmlWriter::toString(map, true));

  _numValidationErrors = 0;

  if (!_josmInterfaceInitialized)
  {
    _initJosmImplementation();
  }
  if (_josmValidators.isEmpty())
  {
    _initJosmValidatorsList();
  }

  // update map with fixed features and add validation info to validation results collection
  OsmMapPtr validatedMap = _getUpdatedMap(map);
  LOG_VARD(validatedMap->size());
  map = validatedMap;
  _numAffected = map->size();

  _getStats();
}

void JosmMapValidatorAbstract::_getStats()
{
  LOG_DEBUG("Retrieving stats...");

  // call back into Java validator to get the validation stats
  _numValidationErrors =
    (int)_javaEnv->CallIntMethod(
      _josmInterface,
      _javaEnv->GetMethodID(_josmInterfaceClass, "getNumValidationErrors", "()I"));

  jstring validationErrorCountsByTypeResult =
    (jstring)_javaEnv->CallObjectMethod(
      _josmInterface,
      _javaEnv->GetMethodID(
        _josmInterfaceClass, "getValidationErrorCountsByType", "()Ljava/lang/String;"));
  LOG_VART(validationErrorCountsByTypeResult == 0);
  const char* validationErrorCountsByTypeTempStr =
    _javaEnv->GetStringUTFChars((jstring)validationErrorCountsByTypeResult, NULL);
  const QString validationErrorCountsByType(validationErrorCountsByTypeTempStr);
  // TODO: env->ReleaseStringUTFChars
  LOG_VART(validationErrorCountsByType);

  _errorSummary = "Total validation errors: " + QString::number(_numValidationErrors) + "\n";
  if (!validationErrorCountsByType.trimmed().isEmpty())
  {
    _errorSummary += _errorCountsByTypeStrToSummaryStr(validationErrorCountsByType);
  }
}

QString JosmMapValidatorAbstract::_errorCountsByTypeStrToSummaryStr(
  const QString& errorCountsByTypeStr) const
{
  // count numbers and types of validation errors

  QString summary = "";
  const QStringList countsByTypeParts = errorCountsByTypeStr.split(";");
  for (int i = 0; i < countsByTypeParts.size(); i++)
  {
    const QStringList countByTypeParts = countsByTypeParts.at(i).split(":");
    LOG_VART(countByTypeParts.size());
    countByTypeParts.at(0) + " errors: " + countByTypeParts.at(1) + "\n";
  }
  return summary;
}

}
