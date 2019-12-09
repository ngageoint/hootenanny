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
#ifndef JAVA_ENVIRONMENT_H
#define JAVA_ENVIRONMENT_H

// JNI
#include <jni.h>

// Qt
#include <QString>

namespace hoot
{

class JavaEnvironment;

typedef std::shared_ptr<JavaEnvironment> JavaEnvironmentPtr;

/**
 * Creates a Java Virtual Machine for use by Hootenanny C++ code that calls Java code in the
 * hoot-josm library via JNI (Singleton)
 */
class JavaEnvironment
{

public:

  static std::string className() { return "hoot::JavaEnvironment"; }

  ~JavaEnvironment();

  /**
   * Creates a single instance of a JNI environment if none yet exists and returns it.
   *
   * @return a pointer to a JNI environment
   */
  static JNIEnv* getEnvironment() { return getInstance()->_env; }

  static const JavaEnvironmentPtr& getInstance();

  /**
   * TODO
   *
   * @param from
   * @return
   */
  jstring toJavaString(const QString& from);

  /**
   * TODO
   *
   * @param from
   * @return
   */
  QString fromJavaString(jstring from);

private:

  JavaEnvironment();

  static JavaEnvironmentPtr _theInstance;
  // A Java VM; is destroyed by this class in the destructor
  JavaVM* _vm;
  // A JNI environment
  JNIEnv* _env;


  void _initVm();
};

}

#endif // JAVA_ENVIRONMENT_H
