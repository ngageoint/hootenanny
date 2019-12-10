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
#include "JniConversion.h"

// Hoot
#include <hoot/core/util/Log.h>

namespace hoot
{

jstring JniConversion::toJavaString(JNIEnv* javaEnvironment, const QString& cppStr)
{
  LOG_TRACE("Converting to java string: " << cppStr << "...");
  return javaEnvironment->NewStringUTF(cppStr.toUtf8().data());
}

QString JniConversion::fromJavaString(JNIEnv* javaEnvironment, jstring javaStr)
{
  LOG_TRACE("Converting from java string...");
  jboolean isCopy;
  const char* data = javaEnvironment->GetStringUTFChars(javaStr, &isCopy);
  QString result = QString::fromUtf8(data);
  // Do this to avoid a memory leak.
  javaEnvironment->ReleaseStringUTFChars(javaStr, data);
  return result;
}

jobject JniConversion::toJavaStringList(JNIEnv* javaEnvironment, const QStringList& cppStrList)
{
  LOG_TRACE("Converting to java string list...");

  // TODO: cache some of this with static
  jclass arrayListJavaClass = javaEnvironment->FindClass("java/util/ArrayList");
  jmethodID arrayListConstructorMethodId =
    javaEnvironment->GetMethodID(arrayListJavaClass, "<init>", "(I)V");
  jmethodID arrayListAddMethodId =
    javaEnvironment->GetMethodID(arrayListJavaClass, "add", "(Ljava/lang/Object;)Z");

  jobject result =
    javaEnvironment->NewObject(arrayListJavaClass, arrayListConstructorMethodId, cppStrList.size());
  for (int i = 0; i < cppStrList.size(); i++)
  {
    javaEnvironment->CallObjectMethod(
      result, arrayListAddMethodId, toJavaString(javaEnvironment, cppStrList.at(i)));
  }
  return result;
}

QStringList JniConversion::fromJavaStringList(JNIEnv* javaEnvironment, jobject javaStrList)
{
  LOG_TRACE("Converting from java string list...");

  // TODO: cache some of this with static
  jclass arrayListJavaClass = javaEnvironment->FindClass("java/util/ArrayList");
  jmethodID arrayListSizeMethodId = javaEnvironment->GetMethodID(arrayListJavaClass, "size", "()I");
  jmethodID arrayListGetMethodId =
    javaEnvironment->GetMethodID(arrayListJavaClass, "get", "(I)Ljava/lang/Object;");

  QStringList result;

  jint len = javaEnvironment->CallIntMethod(javaStrList, arrayListSizeMethodId);
  result.clear();
  result.reserve(len);

  for (jint i = 0; i < len; i++)
  {
    jstring javaStr =
      (jstring)javaEnvironment->CallObjectMethod(javaStrList, arrayListGetMethodId, i);
    result.append(fromJavaString(javaEnvironment, javaStr));
  }

  return result;
}

QMap<QString, QString> JniConversion::fromJavaStringStringMap(JNIEnv* javaEnvironment,
                                                              jobject javaMap)
{
  LOG_TRACE("Converting from java string string map...");

  QMap<QString, QString> result;

  // TODO: cache some of this with static

  jclass c_map = javaEnvironment->GetObjectClass(javaMap);
  jmethodID id_entrySet = javaEnvironment->GetMethodID(c_map, "entrySet", "()Ljava/util/Set;");

  jclass c_entryset = javaEnvironment->FindClass("java/util/Set");
  jmethodID id_iterator = javaEnvironment->GetMethodID(c_entryset, "iterator", "()Ljava/util/Iterator;");

  jclass c_iterator = javaEnvironment->FindClass("java/util/Iterator");
  jmethodID id_hasNext = javaEnvironment->GetMethodID(c_iterator, "hasNext", "()Z");
  jmethodID id_next = javaEnvironment->GetMethodID(c_iterator, "next", "()Ljava/lang/Object;");

  jclass c_entry = javaEnvironment->FindClass("java/util/Map$Entry");
  jmethodID id_getKey = javaEnvironment->GetMethodID(c_entry, "getKey", "()Ljava/lang/Object;");
  jmethodID id_getValue = javaEnvironment->GetMethodID(c_entry, "getValue", "()Ljava/lang/Object;");

  jclass c_string = javaEnvironment->FindClass("java/lang/String");
  jmethodID id_toString = javaEnvironment->GetMethodID(c_string, "toString", "()Ljava/lang/String;");

  jobject obj_entrySet = javaEnvironment->CallObjectMethod(javaMap, id_entrySet);

  jobject obj_iterator = javaEnvironment->CallObjectMethod(obj_entrySet, id_iterator);

  bool hasNext = (bool) javaEnvironment->CallBooleanMethod(obj_iterator, id_hasNext);
  while (hasNext)
  {
    jobject entry = javaEnvironment->CallObjectMethod(obj_iterator, id_next);

    jobject key = javaEnvironment->CallObjectMethod(entry, id_getKey);
    jobject value = javaEnvironment->CallObjectMethod(entry, id_getValue);

    jstring jstrKey = (jstring) javaEnvironment->CallObjectMethod(key, id_toString);
    jstring jstrValue = (jstring) javaEnvironment->CallObjectMethod(value, id_toString);

    const char *strKey = javaEnvironment->GetStringUTFChars(jstrKey, 0);
    const char *strValue = javaEnvironment->GetStringUTFChars(jstrValue, 0);

    result[QString(strKey)] = QString(strValue);

    javaEnvironment->ReleaseStringUTFChars(jstrKey, strKey);
    javaEnvironment->ReleaseStringUTFChars(jstrValue, strValue);

    hasNext = (bool)javaEnvironment->CallBooleanMethod(obj_iterator, id_hasNext);
  }

  return result;
}

QMap<QString, int> JniConversion::fromJavaStringIntMap(JNIEnv* javaEnvironment, jobject javaMap)
{
  LOG_TRACE("Converting from java string int map...");

  QMap<QString, int> result;

  // yes, kind of kludgy...
  QMap<QString, QString> tempResult = fromJavaStringStringMap(javaEnvironment, javaMap);
  for (QMap<QString, QString>::const_iterator mapItr = tempResult.begin();
       mapItr != tempResult.end(); ++mapItr)
  {
    bool ok = false;
    result[mapItr.key()] = mapItr.value().toInt(&ok);
  }

  return result;
}

}
