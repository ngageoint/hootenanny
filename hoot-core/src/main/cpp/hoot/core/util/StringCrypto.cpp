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

#include "StringCrypto.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/jni/JavaEnvironment.h>
#include <hoot/core/jni/JniConversion.h>

namespace hoot
{

JNIEnv* StringCrypto::_javaEnv;
jclass StringCrypto::_cryptoJavaClass;
jmethodID StringCrypto::_encryptMethod;
jmethodID StringCrypto::_decryptMethod;

QString StringCrypto::encrypt(QString plainText)
{
  if (_javaEnv == 0)
  {
    _initJavaImplementation();
  }

  jstring encryptedJavaStr =
    (jstring)_javaEnv->CallStaticObjectMethod(
      _cryptoJavaClass,
      _encryptMethod,
      // text to encrypt
      JniConversion::toJavaString(_javaEnv, plainText));
  JniConversion::checkForErrors(_javaEnv, "encrypt");

  return JniConversion::fromJavaString(_javaEnv, encryptedJavaStr);
}

QString StringCrypto::decrypt(QString encryptedText)
{
  if (_javaEnv == 0)
  {
    _initJavaImplementation();
  }

  jstring decryptedJavaStr =
    (jstring)_javaEnv->CallStaticObjectMethod(
      _cryptoJavaClass,
      _decryptMethod,
      // text to decrypt
      JniConversion::toJavaString(_javaEnv, encryptedText));
  JniConversion::checkForErrors(_javaEnv, "decrypt");

  return JniConversion::fromJavaString(_javaEnv, decryptedJavaStr);
}

void StringCrypto::_initJavaImplementation()
{
  _javaEnv = JavaEnvironment::getEnvironment();
  _cryptoJavaClass = _javaEnv->FindClass("hoot/services/josm/StringCrypto");
  // JNI sig format: (input params...)return type
  // Java sig: String encrypt(String plainText)
  _encryptMethod =
    _javaEnv->GetStaticMethodID(
      _cryptoJavaClass, "encrypt", "(Ljava/lang/String;)Ljava/lang/String;");
  // Java sig: String decrypt(String encryptedText)
  _decryptMethod =
    _javaEnv->GetStaticMethodID(
      _cryptoJavaClass, "decrypt", "(Ljava/lang/String;)Ljava/lang/String;");
}

}
