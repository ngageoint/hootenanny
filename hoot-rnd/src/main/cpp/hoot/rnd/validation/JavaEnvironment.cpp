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

const JavaEnvironmentPtr& JavaEnvironment::_getInstance()
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
  const int numOptions = 2;
  JavaVMOption options[numOptions];

  const QString cpStart = "-Djava.class.path=";
  ConfigOptions opts(conf());
  const QString josmJarPath = cpStart + opts.getValidationJosmLibraryPath();
  LOG_VART(josmJarPath);
  //options[0].optionString = (char*)josmJarPath.toStdString().c_str();
  // THIS IS INSANE that only this works...will figure this out at some point...
  options[0].optionString =
    (char*)"-Djava.class.path=/home/vagrant/.m2/repository/org/openstreetmap/josm/me-josm.4.4.4.jar";
  //options[0].optionString = josmJarPath.toUtf8().data();
  //options[0].optionString = &josmJarPath.toStdString()[0];
  LOG_VART(options[0].optionString);
  const QString hootJosmJarPath = cpStart + opts.getValidationHootJosmLibraryPath();
  LOG_VART(hootJosmJarPath);
  //options[1].optionString = (char*)hootJosmJarPath.toStdString().c_str();
  options[1].optionString = (char*)"-Djava.class.path=/home/vagrant/hoot/tmp/hoot-josm.jar";
  //options[1].optionString = hootJosmJarPath.toUtf8().data();
  //options[1].optionString = &hootJosmJarPath.toStdString()[0];
  LOG_VART(options[1].optionString);
  //options[2].optionString = (char*)"-verbose:jni";
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
