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
#ifndef JNI_CONVERSION_H
#define JNI_CONVERSION_H

// JNI
#include <jni.h>

// Qt
#include <QString>
#include <QMap>
#include <QSet>

namespace hoot
{

/**
 * Common conversion utilities to go between Java and C++ using JNI.
 *
 * Helpful JNI references:
 *
 * https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/functions.html
 * http://journals.ecs.soton.ac.uk/java/tutorial/native1.1/implementing/method.html
 */
class JniConversion
{

public:

  /**
   * Converts a Qt string to a Java string
   *
   * @param javaEnv the associated JNI environment
   * @param cppStr the string to convert
   * @return a Java string
   */
  static jstring toJavaString(JNIEnv* javaEnv, const QString& cppStr);

  /**
   * Converts a Java string to a Qt string
   *
   * @param javaEnv the associated JNI environment
   * @param javaStr the string to convert
   * @return a Qt string
   */
  static QString fromJavaString(JNIEnv* javaEnv, jstring javaStr);

  /**
   * Converts a Qt string list to a Java string list
   *
   * @param javaEnv the associated JNI environment
   * @param cppStrList the list to convert
   * @return a Java string list
   */
  static jobject toJavaStringList(JNIEnv* javaEnv, const QStringList& cppStrList);

  /**
   * Converts a Java string list to a Qt string list
   *
   * @param javaEnv the associated JNI environment
   * @param javaStrList the list to convert
   * @return a Qt string list
   */
  static QStringList fromJavaStringList(JNIEnv* javaEnv, jobject javaStrList);

  /**
   * Converts a Java string set to a Qt string set
   *
   * @param javaEnv the associated JNI environment
   * @param javaStrSet the set to convert
   * @return a Qt string set
   */
  static QSet<QString> fromJavaStringSet(JNIEnv* javaEnv, jobject javaStrSet);

  /**
   * Converts a Java map with strings as keys and values to a Qt map with strings as keys and values
   *
   * @param javaEnv the associated JNI environment
   * @param javaMap the map to convert
   * @return a Qt map
   */
  static QMap<QString, QString> fromJavaStringMap(JNIEnv* javaEnv, jobject javaMap);

  /**
   * Converts a Java map with strings as keys and integers as values to a Qt map with strings as
   * keys and integers as values
   *
   * @param javaEnv the associated JNI environment
   * @param javaMap the map to convert
   * @return a Qt map
   */
  static QMap<QString, int> fromJavaStringIntMap(JNIEnv* javaEnv, jobject javaMap);

  /**
   * Check a JNI call for errors
   *
   * @param javaEnv Java environment invoking the call
   * @param operationName descriptive name of the call for logging purposes
   * @todo This probably belongs in its own class, like JniUtils.
   */
  static void checkForErrors(JNIEnv* javaEnv, const QString& operationName);
};

}

#endif // JNI_CONVERSION_H
