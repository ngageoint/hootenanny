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
#include <hoot/core/util/ConfigOptions.h>
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

void JosmValidatorClient::setConfiguration(const Settings& /*conf*/)
{
}

long JosmValidatorClient::getBlankNodeIdTest() const
{
  JNIEnv* env = JavaEnvironment::getEnvironment();;
  jmethodID getNodeIdMethodId = env->GetMethodID(_validatorClass, "getBlankNodeIdTest", "()J");
  jlong nodeIdResult = env->CallLongMethod(_validator, getNodeIdMethodId);
  return (long)nodeIdResult;
}

QStringList JosmValidatorClient::getAvailableValidators() const
{
  QStringList validators;

  JNIEnv* env = JavaEnvironment::getEnvironment();

  jmethodID getAvailableValidatorsMethodId =
    env->GetMethodID(_validatorClass, "getAvailableValidators", "()Ljava/util/List;");
  jobject availableValidatorsResult =
    env->CallObjectMethod(_validator, getAvailableValidatorsMethodId);

  jclass listClass = env->FindClass("java/util/List");
  jmethodID getListSizeMethodId = env->GetMethodID(listClass, "size", "()I");
  const int validatorsSize =
    (int)env->CallIntMethod(availableValidatorsResult, getListSizeMethodId);
  LOG_VART(validatorsSize);
  jmethodID listGetMethodId = env->GetMethodID(listClass, "get", "(I)V");
  for (int i = 0; i < validatorsSize; i++)
  {
    jint index = i;
    jobject validatorNameResult =
      env->CallObjectMethod(availableValidatorsResult, listGetMethodId, index);
    const QString testName = QString(env->GetStringUTFChars((jstring)validatorNameResult, NULL));
    LOG_VART(testName);
    validators.append(testName);
  }

  return validators;
}

QMap<ElementId, QString> JosmValidatorClient::validate(const ConstOsmMapPtr& /*map*/)
{
  return QMap<ElementId, QString>();
}

void JosmValidatorClient::validateAndFix(OsmMapPtr& /*map*/)
{

}

}
