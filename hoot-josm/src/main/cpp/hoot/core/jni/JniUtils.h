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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef JNI_UTILS_H
#define JNI_UTILS_H

// JNI
#include <jni.h>

namespace hoot
{

/**
 * Common utilities used with JNI.
 *
 * Helpful JNI references:
 *
 * https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html
 * http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
 */
class JniUtils
{

public:

  /**
   * Check a JNI call for errors
   *
   * @param javaEnv Java environment invoking the call
   * @param operationName descriptive name of the call for logging purposes
   */
  static void checkForErrors(JNIEnv* javaEnv, const QString& operationName);
};

}

#endif // JNI_UTILS_H
