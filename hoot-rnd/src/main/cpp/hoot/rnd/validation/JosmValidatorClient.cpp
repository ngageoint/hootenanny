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
#include "JosmValidatorClient.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/rnd/validation/JavaEnvironment.h>

namespace hoot
{

JosmValidatorClient::JosmValidatorClient()
{
  JNIEnv* env = JavaEnvironment::getEnvironment();
  _validatorClass = env->FindClass("hoot/services/validation/JosmValidator");
  jmethodID constructorMethodId = env->GetMethodID(_validatorClass, "<init>", "()V");
  _validator = env->NewObject(_validatorClass, constructorMethodId);
}

QMap<QString, QString> JosmValidatorClient::getAvailableValidators() const
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
    // get exception message from object and add here?
    throw HootException("Error calling getAvailableValidators.");
  }

  jclass listClass = env->FindClass("java/util/List");
  jmethodID getListSizeMethodId = env->GetMethodID(listClass, "size", "()I");
  const int validatorsSize =
    (int)env->CallIntMethod(availableValidatorsResult, getListSizeMethodId);
  jmethodID listGetMethodId = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
  for (int i = 0; i < validatorsSize; i++)
  {
    jint index = i;
    jobject validatorNameResult =
      env->CallObjectMethod(availableValidatorsResult, listGetMethodId, index);
    const QString testResultStr =
      QString(env->GetStringUTFChars((jstring)validatorNameResult, NULL));
    LOG_VART(testResultStr);
    assert(testResultStr.contains(";"));
    const QStringList testResultParts = testResultStr.split(";");
    //env->ReleaseStringUTFChars //??
    const QString testName = testResultParts[0];
    const QString testDescription = testResultParts[1];
    validators[testName] = testDescription;
  }

  return validators;
}

QMap<ElementId, QString> JosmValidatorClient::validate(const ConstOsmMapPtr& /*map*/)
{
  // convert map to xml string

  // pass validators and xml to java method

  // convert returned info into map

  return QMap<ElementId, QString>();
}

QMap<ElementId, QString> JosmValidatorClient::validateAndFix(OsmMapPtr& /*map*/)
{
  // convert map to xml string

  // pass validators and xml to java method

  // convert returned info into map

  return QMap<ElementId, QString>();
}

}
