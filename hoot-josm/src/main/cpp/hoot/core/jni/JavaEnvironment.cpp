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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "JavaEnvironment.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

// Std
#include <string.h>

namespace hoot
{

JavaEnvironment::JavaEnvironment() :
_vm(0),
_env(0)
{
  _initVm();
}

JavaEnvironment::~JavaEnvironment()
{
  if (_vm != 0)
  {
    _vm->DetachCurrentThread();
    _vm->DestroyJavaVM();
  }
}

JavaEnvironment& JavaEnvironment::getInstance()
{
  static JavaEnvironment instance;
  return instance;
}

void JavaEnvironment::_initVm()
{
  JavaVMInitArgs vm_args;
  const int numOptions = 5;
  JavaVMOption options[numOptions];

  const QString classPathStr = "-Djava.class.path=" + ConfigOptions().getJniClassPath().join(":");
  options[0].optionString = strdup(classPathStr.toStdString().c_str());
  LOG_VART(options[0].optionString);

  const QString minMemorySizeStr = "-Xms" + ConfigOptions().getJniInitialMemory();
  options[1].optionString = strdup(minMemorySizeStr.toStdString().c_str());
  LOG_VART(options[1].optionString);

  const QString maxMemorySizeStr = "-Xms" + ConfigOptions().getJniMaxMemory();
  options[2].optionString = strdup(maxMemorySizeStr.toStdString().c_str());
  LOG_VART(options[2].optionString);

  const QString maxPermMemorySizeStr = "-XX:MaxPermSize=" + ConfigOptions().getJniInitialMemory();
  options[3].optionString = strdup(maxPermMemorySizeStr.toStdString().c_str());
  LOG_VART(options[3].optionString);

  const QString headlessStr = "-Djava.awt.headless=true";
  options[4].optionString = strdup(headlessStr.toStdString().c_str());
  LOG_VART(options[4].optionString);

  // for debugging only
//  const QString verboseStr = "-verbose:jni";
//  options[5].optionString = strdup(verboseStr.toStdString().c_str());
//  LOG_VART(options[5].optionString);

  vm_args.version = JNI_VERSION_1_8;
  vm_args.nOptions = numOptions;
  vm_args.options = options;
  vm_args.ignoreUnrecognized = 1;

  jint res = JNI_CreateJavaVM(&_vm, (void**)&_env, &vm_args);
  if (res != 0)
  {
    throw HootException("Unable to initialize JVM. Error code: " + QString::number(res));
  }
}

}
