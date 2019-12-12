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

#ifndef STRING_CRYPTO_H
#define STRING_CRYPTO_H

// JNI
#include <jni.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * TODO
 *
 * Calling into a Java crypto library b/c it was easier to set up. A future alternative might be
 * using something like Crypto++ as a C lib (https://github.com/weidai11/cryptopp).
 */
class StringCrypto
{

public:

  /**
   * @brief encrypt
   * @param plainText
   * @return
   */
  static QString encrypt(QString plainText);

  /**
   * @brief decrypt
   * @param encryptedText
   * @return
   */
  static QString decrypt(QString encryptedText);

private:

  static JNIEnv* _javaEnv;
  static jclass _cryptoJavaClass;
  static jmethodID _encryptMethod;
  static jmethodID _decryptMethod;

  static void _initJavaImplementation();
};

}

#endif // STRING_CRYPTO_H
