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
#include <hoot/core/ops/ElementReplacer.h>

namespace hoot
{

JosmValidator::JosmValidator(const bool fixElements) :
_fixElements(fixElements),
_numValidationErrors(0),
_numElementsFixed(0)
{
  JNIEnv* env = JavaEnvironment::getEnvironment();
  _validatorClass = env->FindClass("hoot/services/validation/JosmValidator");
  jmethodID constructorMethodId =
    env->GetMethodID(_validatorClass, "<init>", "(Ljava/lang/String;)V");
  // TODO: change back
  //jstring logLevelStr =
    //env->NewStringUTF(Log::getInstance().getLevelAsString().toStdString().c_str());
  jstring logLevelStr = env->NewStringUTF(QString("TRACE").toStdString().c_str());
  _validator = env->NewObject(_validatorClass, constructorMethodId, logLevelStr);
  // TODO: env->ReleaseStringUTFChars
}

void JosmValidator::setConfiguration(const Settings& /*conf*/)
{
  //validation.josm.validator.exclude.list
  //validation.josm.validator.include.list

  // TODO
}

QMap<QString, QString> JosmValidator::getAvailableValidators() const
{
  QMap<QString, QString> validators;
  JNIEnv* env = JavaEnvironment::getEnvironment();

  jobject availableValidatorsResult =
    env->CallObjectMethod(
      _validator,
      env->GetMethodID(_validatorClass, "getAvailableValidators", "()Ljava/lang/String;"));
  if (env->ExceptionCheck())
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
  //env->ReleaseStringUTFChars //??

  return validators;
}

void JosmValidator::apply(std::shared_ptr<OsmMap>& map)
{
  LOG_VARD(map->size());
  LOG_VARD(_fixElements);
  _numAffected = 0;
  _numValidationErrors = 0;
  _numElementsFixed = 0;

  JNIEnv* env = JavaEnvironment::getEnvironment();

  // pass in the validators to use, the features to be examined, and whether to just validate or to
  // validate and fix them

  // validators delimited by ';'
  jstring validatorsStr = env->NewStringUTF(_validatorsToUse.join(";").toStdString().c_str());
  // convert map to xml string
  jstring mapXml = env->NewStringUTF(OsmXmlWriter::toString(map, false).toStdString().c_str());
  jobject validationResult =
    env->CallObjectMethod(
    _validator,
    // JNI sig format: (input params...)return type
    // Java sig: String validate(String validatorsStr, String featuresXml, boolean fixFeatures)
    env->GetMethodID(
      _validatorClass, "validate", "(Ljava/lang/String;Ljava/lang/String;Z)Ljava/lang/String;"),
    validatorsStr,
    mapXml,
    _fixElements);
  // TODO: env->ReleaseStringUTFChars
  if (env->ExceptionCheck())
  {
    env->ExceptionDescribe();
    env->ExceptionClear();
    // TODO: get exception message from object and add here?
    throw HootException("Error calling JosmValidator::apply.");
  }

  // update map with fixed features and add validation info to validation results collection

  const char* xml = env->GetStringUTFChars((jstring)validationResult, NULL);
  const QString validatedMapXml(xml);
  LOG_VART(validatedMapXml);
  // TODO: env->ReleaseStringUTFChars

  // call back in to Java to get validation stats

  _numAffected = map->size();
  _numValidationErrors =
    (int)env->CallIntMethod(
      _validator, env->GetMethodID(_validatorClass, "getNumValidationErrors", "()I"));
  if (_fixElements)
  {
    _numElementsFixed =
      (int)env->CallIntMethod(
        _validator, env->GetMethodID(_validatorClass, "getNumElementsFixed", "()I"));
  }

  // empty string returned means no features had validation issues
  if (!validatedMapXml.trimmed().isEmpty())
  {
    // replace any of the validated and/or fixed elements with what's in the original map; validated
    // only elements will just have tags indicating which validations they failed; elements also
    // attempted to be fixed after validation will have both the failed validation tag and whether
    // the fix was successful or not

    ElementReplacer replacer(OsmXmlReader::fromXml(validatedMapXml.trimmed(), true, true, true));
    LOG_INFO(replacer.getInitStatusMessage());
    replacer.apply(map);
    LOG_INFO(replacer.getCompletedStatusMessage());
  }
}

}
