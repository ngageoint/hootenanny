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
  const int numOptions = 4;
  JavaVMOption options[numOptions];

  // Due to classpath loading issues on the Java end, now copying the josm jar to one to be used by
  // hoot, adding hoot validator classes to it, and then using that jar to call JNI against. This
  // isn't a great long term solution but good enough for testing now. Also, the fact that I can't
  // set the string from a config setting is INSANE...will figure that out too (string copying
  // issues likely).

  // TODO: read this from jni.class.path list
  // TODO: use jni.initial.memory and jni.max.memory
  //

  options[0].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/hoot-josm.jar";
  // If init mem is set too low, large file ingests will slow to a crawl.
  options[1].optionString = (char*)"-Xms256m";
  options[2].optionString = (char*)"-Xmx8g";
  options[3].optionString = (char*)"-Djava.awt.headless=true";
  //options[1].optionString = (char*)"-verbose:jni";
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
