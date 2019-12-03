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
#include "JosmMapValidator.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/JavaEnvironment.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, JosmMapValidator)

JosmMapValidator::JosmMapValidator() :
JosmMapValidatorAbstract()
{
  _josmInterfaceName = "hoot/services/josm/JosmMapValidator";
}

void JosmMapValidator::setConfiguration(const Settings& conf)
{
  JosmMapValidatorAbstract::setConfiguration(conf);

  ConfigOptions opts(conf);
  _josmInterfaceName = opts.getMapValidatorJosmJavaImplementation();
  _modifyMap = opts.getMapValidatorJosmModifyMap();
}

QMap<QString, QString> JosmMapValidator::getAvailableValidators()
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

OsmMapPtr JosmMapValidator::_getUpdatedMap(OsmMapPtr& inputMap)
{
  // pass in the validators to use, the features to be examined, and whether to just validate or to
  // validate and fix them

  // validators delimited by ';'
  jstring validatorsStr = _javaEnv->NewStringUTF(_josmValidators.join(";").toStdString().c_str());
  // convert input map to xml string
  jstring mapXml =
    _javaEnv->NewStringUTF(OsmXmlWriter::toString(inputMap, false).toStdString().c_str());
  jobject validateResult = NULL;

  if (_modifyMap)
  {
    validateResult =
      _javaEnv->CallObjectMethod(
        _josmInterface,
        // JNI sig format: (input params...)return type
        // Java sig: String validate(String validatorsStr, String elementsXml)
        _javaEnv->GetMethodID(
          _josmInterfaceClass, "validate",
          "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"),
        validatorsStr,
        mapXml);
  }
  else
  {
    validateResult =
      _javaEnv->CallObjectMethod(
        _josmInterface,
        // JNI sig format: (input params...)return type
        // Java sig: String getValidateStats(String validatorsStr, String elementsXml)
        _javaEnv->GetMethodID(
          _josmInterfaceClass, "getValidateStats",
          "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"),
        validatorsStr,
        mapXml);
  }

  // TODO: env->ReleaseStringUTFChars
  if (_javaEnv->ExceptionCheck())
  {
    _javaEnv->ExceptionDescribe();
    _javaEnv->ExceptionClear();
    // TODO: get exception message from object and add here
    throw HootException("Error calling JosmMapValidator::validate.");
  }

  if (_modifyMap)
  {
    const char* xml = _javaEnv->GetStringUTFChars((jstring)validateResult, NULL);
    OsmMapPtr validatedMap =
      OsmXmlReader::fromXml(QString(xml).trimmed(), true, true, false, true);
    // TODO: env->ReleaseStringUTFChars

    // return a null map if no features had validation issues
    //LOG_VART(OsmXmlWriter::toString(validatedMap, true));
    return validatedMap;
  }
  else
  {
    return OsmMapPtr();
  }
}

}
