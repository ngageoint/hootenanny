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
#include "JosmValidator.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/rnd/util/JavaEnvironment.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmXmlReader.h>

namespace hoot
{

JosmValidator::JosmValidator(const bool fixFeatures) :
_fixFeatures(fixFeatures)
{
  JNIEnv* env = JavaEnvironment::getEnvironment();
  _validatorClass = env->FindClass("hoot/services/validation/JosmValidator");
  jmethodID constructorMethodId = env->GetMethodID(_validatorClass, "<init>", "()V");
  _validator = env->NewObject(_validatorClass, constructorMethodId);
}

void JosmValidator::setConfiguration(const Settings& conf)
{
  //validation.josm.validator.exclude.list
  //validation.josm.validator.include.list

  // TODO
}

void JosmValidator::apply(std::shared_ptr<OsmMap>& map)
{
  // convert map to xml string

  const QString mapXml = OsmXmlWriter::toString(map, false);

  // pass validators and xml to appropriate java method

  JNIEnv* env = JavaEnvironment::getEnvironment();

  jstring validatorsStr = env->NewStringUTF(_validatorsToUse.join(";"));
  jstring featuresXml = env->NewStringUTF(mapXml);
  jboolean fixFeatures = _fixFeatures;
  jmethodID getValidateMethodId =
    env->GetMethodID(_validatorClass, "validate", "()Ljava/lang/String;");
  jobject validationResult =
    env->CallObjectMethod(_validator, getValidateMethodId, validatorsStr, featuresXml, fixFeatures);
  //env->ReleaseStringUTFChars //??
  jboolean hasException = env->ExceptionCheck();
  if (hasException)
  {
    env->ExceptionDescribe();
    env->ExceptionClear();
    // TODO: get exception message from object and add here?
    throw HootException("Error calling JosmValidator::apply.");
  }

  // update map with fixed features and add validation info to validation results collection

  const char* str = env->GetStringUTFChars((jstring)validationResult, NULL);
  QString validationResultsStr(str);
  //env->ReleaseStringUTFChars //??
  assert(validationResultsStr.contains(";"));
  QStringList validationResultStrings = validationResultsStr.split(";");
  for (int i = 0; i < validationResultStrings.size(); i++)
  {
    const QString validationResultStr = validationResultStrings.at(i);
    assert(validationResultStr.contains(","));
    const QStringList validationResultParts = validationResultStr.split(",");
    if (!_fixFeatures)
    {
      assert(validationResultParts.size() == 2);
    }
    else
    {
      assert(validationResultParts.size() == 3);
    }
    const ElementId id = _idStrToElementId(validationResultParts[0]);
    _validationResults[id] = validationResultParts[1];
    ElementPtr fixedElement = OsmXmlReader::fromElementXml(validationResultParts[2]);
    map->replace(map->getElement(id), fixedElement);
  }
}

ElementId JosmValidator::_idStrToElementId(const QString& /*idStr*/) const
{
  // TODO
  return ElementId();
}

QMap<QString, QString> JosmValidator::getAvailableValidators() const
{
  QMap<QString, QString> validators;
  JNIEnv* env = JavaEnvironment::getEnvironment();

  jmethodID getAvailableValidatorsMethodId =
    env->GetMethodID(_validatorClass, "getAvailableValidators", "()Ljava/util/List;");
  jobject availableValidatorsResult =
    env->CallObjectMethod(_validator, getAvailableValidatorsMethodId);
  jboolean hasException = env->ExceptionCheck();
  if (hasException)
  {
    env->ExceptionDescribe();
    env->ExceptionClear();
    // TODO: get exception message from object and add here?
    throw HootException("Error calling getAvailableValidators.");
  }

  const char* str = env->GetStringUTFChars((jstring)availableValidatorsResult, NULL);
  QString validatorsStr(str);
  assert(validatorsStr.contains(";"));
  QStringList validatorStrings = validatorsStr.split(";");
  for (int i = 0; i < validatorStrings.size(); i++)
  {
    const QString validatorStr = validatorStrings.at(i);
    assert(validatorStr.contains(","));
    const QStringList validatorStrParts = validatorStr.split(",");
    assert(validatorStrParts.size() == 2);

    const QString validatorName = validatorStrParts[0];
    const QString validatorDescription = validatorStrParts[1];
    validators[validatorName] = validatorDescription;
  }
  //env->ReleaseStringUTFChars //??

  return validators;
}

}
