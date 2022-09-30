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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "JavaEnvironment.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/josm/jni/JniUtils.h>

namespace hoot
{

JavaEnvironment::JavaEnvironment()
  : _vm(nullptr),
    _env(nullptr)
{
  _initVm();
}

JavaEnvironment::~JavaEnvironment()
{
  if (_env != nullptr)
  {
    // This isn't absolutely necessary, as there may be nothing left to collect or the JVM will do
    // it at shutdown anyway. But maybe it will help us clean up and prevent a hangup.
    LOG_DEBUG("Calling garbage collector...");
    jclass systemClass = _env->FindClass("java/lang/System");
    jmethodID systemGCMethod = _env->GetStaticMethodID(systemClass, "gc", "()V");
    _env->CallStaticVoidMethod(systemClass, systemGCMethod);
    if (_env->ExceptionCheck())
    {
      _env->ExceptionDescribe();
      _env->ExceptionClear();
    }
    _env->DeleteLocalRef(systemClass);
  }

  if (_vm != nullptr)
  {
    jint status;

    LOG_DEBUG("Detaching current thread...");
    status = _vm->DetachCurrentThread();
    if (status != JNI_OK)
    {
      LOG_ERROR("Unable to detach JVM from current thread. Error code: " << QString::number(status));
    }

    // If this call hangs, its very likely there is a thread other than the one that launched this
    // JVM that is still running due to holding onto a reference, etc. Use kill -3 on the hoot or
    // HootTest process ID to determine what in JOSM is holding onto the memory.
    LOG_DEBUG("Destroying Java env...");
    status = _vm->DestroyJavaVM();
    if (status != JNI_OK)
    {
      LOG_ERROR("Unable to destroy JVM. Error code: " << QString::number(status));
    }
  }
}

JavaEnvironment& JavaEnvironment::getInstance()
{
  static JavaEnvironment instance;
  return instance;
}

void JavaEnvironment::_initVm()
{
  bool verbose = false;
  JavaVMInitArgs vm_args;
  int numOptions = 5;
  if (verbose)
    numOptions++;
  JavaVMOption options[numOptions];

  const QString classPathStr = "-Djava.class.path=" + ConfigOptions().getJniClassPath().join(":");
  QString classPathStrTemp = classPathStr;
  const QStringList jars = classPathStrTemp.remove("-Djava.class.path=").split(":");
  for (const auto& jar : qAsConst(jars))
  {
    QFile jarFile(jar);
    if (!jarFile.exists())
      throw IllegalArgumentException("JAR file does not exist: " + jar);
  }
  options[0].optionString = strdup(classPathStr.toStdString().c_str());
  LOG_VART(options[0].optionString);

  const QString minMemorySizeStr = "-Xms" + ConfigOptions().getJniInitialMemory();
  options[1].optionString = strdup(minMemorySizeStr.toStdString().c_str());
  LOG_VART(options[1].optionString);

  const QString maxMemorySizeStr = "-Xmx" + ConfigOptions().getJniMaxMemory();
  options[2].optionString = strdup(maxMemorySizeStr.toStdString().c_str());
  LOG_VART(options[2].optionString);

  const QString maxPermMemorySizeStr = "-XX:MaxPermSize=" + ConfigOptions().getJniInitialMemory();
  options[3].optionString = strdup(maxPermMemorySizeStr.toStdString().c_str());
  LOG_VART(options[3].optionString);

  const QString headlessStr = "-Djava.awt.headless=true";
  options[4].optionString = strdup(headlessStr.toStdString().c_str());
  LOG_VART(options[4].optionString);

  // for debugging only
  if (verbose)
  {
    const QString verboseStr = "-verbose:jni";
    options[5].optionString = strdup(verboseStr.toStdString().c_str());
    LOG_VART(options[5].optionString);
  }

  vm_args.version = JNI_VERSION_1_8;
  vm_args.nOptions = numOptions;
  vm_args.options = options;
  vm_args.ignoreUnrecognized = 1;

  jint status = JNI_CreateJavaVM(&_vm, (void**)&_env, &vm_args);
  if (status != JNI_OK)
    throw HootException("Unable to initialize JVM. Error code: " + QString::number(status));
}

}
