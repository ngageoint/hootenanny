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
package hoot.services.josm;

import com.google.crypto.tink.Aead;
import com.google.crypto.tink.KeysetHandle;
import com.google.crypto.tink.aead.AeadKeyTemplates;
import com.google.crypto.tink.config.TinkConfig;
import com.google.crypto.tink.config.AeadConfig;

/**
 * TODO
 */
public class StringCrypto
{
  /**
   * TODO
   */
  public static String encrypt(String plainText) throws Exception
  {
    TinkConfig.register();
    //AeadConfig.register();
    // 1. Generate the key material.
    KeysetHandle keysetHandle = KeysetHandle.generateNew(AeadKeyTemplates.AES128_GCM);
    // 2. Get the primitive.
    Aead aead = keysetHandle.getPrimitive(Aead.class);
    // 3. Use the primitive to encrypt a plaintext,
    byte[] encryptedText = aead.encrypt(plainText.getBytes(), null);
    return new String(encryptedText);
  }

  /**
   * TODO
   */
  public static String decrypt(String encryptedText) throws Exception
  {
    TinkConfig.register();
    //AeadConfig.register();
    // 1. Generate the key material.
    KeysetHandle keysetHandle = KeysetHandle.generateNew(AeadKeyTemplates.AES128_GCM);
    // 2. Get the primitive.
    Aead aead = keysetHandle.getPrimitive(Aead.class);
    // ... or to decrypt a ciphertext.
    byte[] decryptedText = aead.decrypt(encryptedText.getBytes(), null);
    return new String(decryptedText);
  }
}
