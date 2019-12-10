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
   * TODO
   *
   * @param javaEnvironment
   * @param cppStr
   * @return
   */
  static jstring toJavaString(JNIEnv* javaEnvironment, const QString& cppStr);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param javaStr
   * @return
   */
  static QString fromJavaString(JNIEnv* javaEnvironment, jstring javaStr);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param cppStrList
   * @return
   */
  static jobject toJavaStringList(JNIEnv* javaEnvironment, const QStringList& cppStrList);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param javaStrList
   * @return
   */
  static QStringList fromJavaStringList(JNIEnv* javaEnvironment, jobject javaStrList);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param javaStrSet
   * @return
   */
  static QSet<QString> fromJavaStringSet(JNIEnv* javaEnvironment, jobject javaStrSet);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param javaMap
   * @return
   */
  static QMap<QString, QString> fromJavaStringStringMap(JNIEnv* javaEnvironment, jobject javaMap);

  /**
   * TODO
   *
   * @param javaEnvironment
   * @param javaMap
   * @return
   */
  static QMap<QString, int> fromJavaStringIntMap(JNIEnv* javaEnvironment, jobject javaMap);
};

}

#endif // JNI_CONVERSION_H
