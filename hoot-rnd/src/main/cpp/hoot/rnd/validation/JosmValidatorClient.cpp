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
}

void JosmValidatorClient::setConfiguration(const Settings& /*conf*/)
{
}

long JosmValidatorClient::getBlankNodeIdTest() const
{
  JNIEnv* env = JavaEnvironment::getEnvironment();
  jclass validatorClass = env->FindClass("hoot/services/validation/JosmValidator");
  //LOG_VART(validatorClass == 0);
  jmethodID constructorMethodId = env->GetMethodID(validatorClass, "<init>", "()V");
  //LOG_VARW(constructorMethodId == 0);
  jobject validator = env->NewObject(validatorClass, constructorMethodId);
  //LOG_VARW(validator == 0);
  jmethodID getNodeIdMethodId = env->GetMethodID(validatorClass, "getBlankNodeIdTest", "()J");
  //LOG_VARW(getNodeIdMethodId == 0);
  jlong nodeIdResult = env->CallLongMethod(validator, getNodeIdMethodId);
  //LOG_VARW(nodeIdResult == 0);
  return (long)nodeIdResult;
}

QStringList JosmValidatorClient::getAvailableValidators() const
{
  return QStringList();
}

QMap<ElementId, QString> JosmValidatorClient::validate(const ConstOsmMapPtr& /*map*/)
{
  return QMap<ElementId, QString>();
}

void JosmValidatorClient::validateAndFix(OsmMapPtr& /*map*/)
{

}

}
