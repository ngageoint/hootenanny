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
#include "JniConversion.h"

// Hoot

#include <hoot/core/util/HootException.h>

namespace hoot
{

jstring JniConversion::toJavaString(JNIEnv* javaEnv, const QString& cppStr)
{
  LOG_TRACE("Converting to java string: " << cppStr << "...");
  return javaEnv->NewStringUTF(cppStr.toUtf8().data());
}

QString JniConversion::fromJavaString(JNIEnv* javaEnv, jstring javaStr)
{
  LOG_TRACE("Converting from java string...");

  if (javaStr == nullptr)
  {
    return "";
  }

  jboolean isCopy;
  const char* data = javaEnv->GetStringUTFChars(javaStr, &isCopy);
  QString result = QString::fromUtf8(data);
  // Do this to avoid a memory leak.
  javaEnv->ReleaseStringUTFChars(javaStr, data);
  javaEnv->DeleteLocalRef(javaStr);
  return result;
}

jobject JniConversion::toJavaStringList(JNIEnv* javaEnv, const QStringList& cppStrList)
{
  LOG_TRACE("Converting to java string list...");

  // see related note about method mappings in fromJavaStringMap
  jclass arrayListJavaClass = javaEnv->FindClass("java/util/ArrayList");
  jmethodID arrayListConstructorMethod =
    javaEnv->GetMethodID(arrayListJavaClass, "<init>", "(I)V");
  jmethodID arrayListAddMethod =
    javaEnv->GetMethodID(arrayListJavaClass, "add", "(Ljava/lang/Object;)Z");

  jobject result =
    javaEnv->NewObject(arrayListJavaClass, arrayListConstructorMethod, cppStrList.size());
  for (int i = 0; i < cppStrList.size(); i++)
  {
    jstring str = toJavaString(javaEnv, cppStrList.at(i));
    javaEnv->CallObjectMethod(result, arrayListAddMethod, str);
    javaEnv->DeleteLocalRef(str);
  }

  javaEnv->DeleteLocalRef(arrayListJavaClass);

  return result;
}

QStringList JniConversion::fromJavaStringList(JNIEnv* javaEnv, jobject javaStrList)
{
  LOG_TRACE("Converting from java string list...");

  if (javaStrList == nullptr)
  {
    return QStringList();
  }

  // see related note about method mappings in fromJavaStringMap
  jclass listClass = javaEnv->FindClass("java/util/List");
  jmethodID listSizeMethod = javaEnv->GetMethodID(listClass, "size", "()I");
  jmethodID listGetMethod = javaEnv->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

  QStringList result;

  jint len = javaEnv->CallIntMethod(javaStrList, listSizeMethod);
  result.clear();
  result.reserve(len);

  for (jint i = 0; i < len; i++)
  {
    jobject javaStrObj =
      javaEnv->CallObjectMethod(javaStrList, listGetMethod, i);
    result.append(fromJavaString(javaEnv, (jstring)javaStrObj));
    // If the list is large the strings won't get garbage collected until the end of the method,
    // so let's free them up as we go to take it easy on the JVM.
    javaEnv->DeleteLocalRef(javaStrObj);
  }

  javaEnv->DeleteLocalRef(listClass);

  return result;
}

QSet<QString> JniConversion::fromJavaStringSet(JNIEnv* javaEnv, jobject javaStrSet)
{
  QSet<QString> result;

  if (javaStrSet == nullptr)
  {
    return result;
  }

  // see related note about method mappings in fromJavaStringMap
  jclass setClass = javaEnv->GetObjectClass(javaStrSet);
  jmethodID iteratorMethod = javaEnv->GetMethodID(setClass, "iterator", "()Ljava/util/Iterator;");
  jclass iteratorClass = javaEnv->FindClass("java/util/Iterator");
  jmethodID hasNextMethod = javaEnv->GetMethodID(iteratorClass, "hasNext", "()Z");
  jmethodID nextMethod = javaEnv->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

  jobject iterator = javaEnv->CallObjectMethod(javaStrSet, iteratorMethod);
  bool hasNext = (bool)javaEnv->CallBooleanMethod(iterator, hasNextMethod);
  while (hasNext)
  {
    jobject javaStrObj = javaEnv->CallObjectMethod(iterator, nextMethod);
    result.insert(fromJavaString(javaEnv, (jstring)javaStrObj));
    // see related note in fromJavaStringList
    javaEnv->DeleteLocalRef(javaStrObj);

    hasNext = (bool)javaEnv->CallBooleanMethod(iterator, hasNextMethod);
  }

  javaEnv->DeleteLocalRef(setClass);
  javaEnv->DeleteLocalRef(iteratorClass);
  javaEnv->DeleteLocalRef(iterator);

  return result;
}

QMap<QString, QString> JniConversion::fromJavaStringMap(JNIEnv* javaEnv, jobject javaMap)
{
  LOG_TRACE("Converting from java string string map...");

  QMap<QString, QString> result;

  if (javaMap == nullptr)
  {
    return result;
  }

  // Creating these method mappings each time for now. If that proves to be a performance bottleneck
  // at some point, we can create some global refs.

  jclass mapClass = javaEnv->GetObjectClass(javaMap);
  jmethodID entrySetMethod = javaEnv->GetMethodID(mapClass, "entrySet", "()Ljava/util/Set;");

  jclass entrySetClass = javaEnv->FindClass("java/util/Set");
  jmethodID iteratorMethod =
    javaEnv->GetMethodID(entrySetClass, "iterator", "()Ljava/util/Iterator;");

  jclass iteratorClass = javaEnv->FindClass("java/util/Iterator");
  jmethodID hasNextMethod = javaEnv->GetMethodID(iteratorClass, "hasNext", "()Z");
  jmethodID nextMethod = javaEnv->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;");

  jclass entryClass = javaEnv->FindClass("java/util/Map$Entry");
  jmethodID getKeyMethod = javaEnv->GetMethodID(entryClass, "getKey", "()Ljava/lang/Object;");
  jmethodID getValueMethod = javaEnv->GetMethodID(entryClass, "getValue", "()Ljava/lang/Object;");

  jclass stringClass = javaEnv->FindClass("java/lang/String");
  jmethodID toStringMethod = javaEnv->GetMethodID(stringClass, "toString", "()Ljava/lang/String;");

  jobject entrySet = javaEnv->CallObjectMethod(javaMap, entrySetMethod);
  jobject iterator = javaEnv->CallObjectMethod(entrySet, iteratorMethod);
  bool hasNext = (bool)javaEnv->CallBooleanMethod(iterator, hasNextMethod);
  while (hasNext)
  {
    jobject entry = javaEnv->CallObjectMethod(iterator, nextMethod);

    jobject key = javaEnv->CallObjectMethod(entry, getKeyMethod);
    jobject value = javaEnv->CallObjectMethod(entry, getValueMethod);
    jstring jstrKey = (jstring)javaEnv->CallObjectMethod(key, toStringMethod);
    jstring jstrValue = (jstring)javaEnv->CallObjectMethod(value, toStringMethod);
    result[fromJavaString(javaEnv, jstrKey)] = fromJavaString(javaEnv, jstrValue);

    javaEnv->DeleteLocalRef(key);
    javaEnv->DeleteLocalRef(value);
    javaEnv->DeleteLocalRef(jstrKey);
    javaEnv->DeleteLocalRef(jstrValue);

    hasNext = (bool)javaEnv->CallBooleanMethod(iterator, hasNextMethod);
  }

  javaEnv->DeleteLocalRef(mapClass);
  javaEnv->DeleteLocalRef(entrySetClass);
  javaEnv->DeleteLocalRef(iteratorClass);
  javaEnv->DeleteLocalRef(entryClass);
  javaEnv->DeleteLocalRef(stringClass);
  javaEnv->DeleteLocalRef(entrySet);
  javaEnv->DeleteLocalRef(iterator);
  javaEnv->DeleteLocalRef(javaMap);

  return result;
}

QMap<QString, int> JniConversion::fromJavaStringIntMap(JNIEnv* javaEnv, jobject javaMap)
{
  LOG_TRACE("Converting from java string int map...");

  QMap<QString, int> result;

  if (javaMap == nullptr)
  {
    return result;
  }

  // yes, this is kind of kludgy...could time to come up with a templated version at some point
  QMap<QString, QString> tempResult = fromJavaStringMap(javaEnv, javaMap);
  for (QMap<QString, QString>::const_iterator mapItr = tempResult.begin();
       mapItr != tempResult.end(); ++mapItr)
  {
    bool ok = false;
    const int val = mapItr.value().toInt(&ok);
    if (ok)
    {
      result[mapItr.key()] = val;
    }
  }
  javaEnv->DeleteLocalRef(javaMap);

  return result;
}

}
