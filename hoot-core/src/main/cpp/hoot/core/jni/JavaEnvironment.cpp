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
#include "JavaEnvironment.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/HootException.h>

namespace hoot
{

JavaEnvironmentPtr JavaEnvironment::_theInstance;

JavaEnvironment::JavaEnvironment() :
_vm(0),
_env(0)
{
}

JavaEnvironment::~JavaEnvironment()
{
  if (_vm != 0)
  {
    _vm->DestroyJavaVM();
  }
}

const JavaEnvironmentPtr& JavaEnvironment::getInstance()
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new JavaEnvironment());
    _theInstance->_initVm();
  }
  return _theInstance;
}

void JavaEnvironment::_initVm()
{
  JavaVMInitArgs vm_args;
  const int numOptions = 5;
  JavaVMOption options[numOptions];

  options[0].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/hoot-josm.jar";
//  const QString classPathStr = "-Djava.class.path=" + ConfigOptions().getJniClassPath().join(",");
//  options[0].optionString = (char*)classPathStr.toStdString().c_str();
//  LOG_VART(options[0].optionString);

  options[1].optionString = (char*)"-Xms2g";
//  const QString minMemorySizeStr = "-Xms" + ConfigOptions().getJniInitialMemory();
//  options[1].optionString = (char*)minMemorySizeStr.toStdString().c_str();
//  LOG_VART(options[1].optionString);

  options[2].optionString = (char*)"-Xmx8g";
//  const QString maxMemorySizeStr = "-Xms" + ConfigOptions().getJniMaxMemory();
//  options[2].optionString = (char*)maxMemorySizeStr.toStdString().c_str();
//  LOG_VART(options[2].optionString);

  options[3].optionString = (char*)"-XX:MaxPermSize=2g";
//  const QString maxPermMemorySizeStr = "-XX:MaxPermSize=" + ConfigOptions().getJniInitialMemory();
//  options[3].optionString = (char*)maxPermMemorySizeStr.toStdString().c_str();
//  LOG_VART(options[3].optionString);

  options[4].optionString = (char*)"-Djava.awt.headless=true";

  //options[5].optionString = (char*)"-verbose:jni";

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
