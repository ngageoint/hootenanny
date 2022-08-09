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
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.language;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static hoot.services.HootProperties.*;

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.language.SupportedLanguagesConfigReader;
import hoot.services.language.SupportedLanguage;

public class SupportedLanguagesConfigReaderTest  
{
  @Test
  @Category(UnitTest.class)
  public void testRead() throws Exception
  {
    SupportedLanguagesConfigReader configReader = new SupportedLanguagesConfigReader();
    configReader.readConfig(
      SupportedLanguagesConfigReaderTest.class.getClassLoader().getResourceAsStream(
        "hoot/services/language/supportedLanguages"));
    SupportedLanguage[] supportedLangs = configReader.getSupportedLanguages();
    
    Assert.assertEquals(2, supportedLangs.length);
    
    Assert.assertEquals("de", supportedLangs[0].getIso6391Code());
    Assert.assertEquals("deu", supportedLangs[0].getIso6392Code());
    Assert.assertEquals("German", supportedLangs[0].getName());
    Assert.assertFalse(supportedLangs[0].getAvailable());
    Assert.assertTrue(configReader.isSupportedLanguage(supportedLangs[0].getIso6391Code()));
    Assert.assertEquals("de", configReader.getIso6391Code("de"));
    Assert.assertEquals("German", configReader.getLanguageName("de"));

    Assert.assertEquals("es", supportedLangs[1].getIso6391Code());
    Assert.assertEquals("spa", supportedLangs[1].getIso6392Code());
    Assert.assertEquals("Spanish", supportedLangs[1].getName());
    Assert.assertFalse(supportedLangs[1].getAvailable());
    Assert.assertTrue(configReader.isSupportedLanguage(supportedLangs[0].getIso6391Code()));
    Assert.assertEquals("es", configReader.getIso6391Code("es"));
    Assert.assertEquals("Spanish", configReader.getLanguageName("es"));

    Assert.assertFalse(configReader.isSupportedLanguage("en"));
  }
}
