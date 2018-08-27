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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

package hoot.services.language;

import static org.junit.Assert.assertEquals;
import static hoot.services.HootProperties.*;
import static org.mockito.Mockito.*;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.BeforeClass;

import hoot.services.UnitTest;
import hoot.services.language.joshua.JoshuaLanguageTranslator;
import hoot.services.language.tika.TikaLanguageDetector;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.language.HootLanguageTranslator;
import hoot.services.controllers.language.LanguageTranslateRequest;

public class HootLanguageTranslatorTest  
{
  @BeforeClass
  public static void beforeClass()
  {
    //assuming two Joshua services are running, one for German and one for Spanish
    /*JoshuaLanguageTranslator wrappedTranslator = mock(JoshuaLanguageTranslator.class);
    when(wrappedTranslator.translate(anyString(), "DB Reisezentrum").thenReturn("DB Tickets"));
    when(wrappedTranslator.translate(anyString(), "Buenos días").thenReturn("Good morning"));
    when(wrappedTranslator.translate(anyString(), "Fahrschule Weiß").thenReturn("Driving School Weiss"));
    when(wrappedTranslator.translate(anyString(), "Carte de crédit").thenReturn(""));
    when(wrappedTranslator.translate(anyString(), "TC IT Service").thenReturn("TC IT Service"));

    TikaLanguageDetector detector = mock(TikaLanguageDetector.class);
    when(detector.detect("DB Reisezentrum").thenReturn("de"));
    when(detector.detect("Buenos días").thenReturn("es"));
    when(detector.detect("Fahrschule Weiß").thenReturn("de"));
    when(detector.detect("Carte de crédit").thenReturn("fr"));
    when(detector.detect("TC IT Service").thenReturn("en"));*/
  }

  @Test
  @Category(UnitTest.class)
  public void testTranslate() throws Exception
  {
    HootCustomPropertiesSetter.setProperty(HOOT_LANGUAGE_TRANSLATOR_APP, "JoshuaLanguageTranslator");
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[]{ "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    Assert.assertEquals("DB Tickets", translator.translate(new String[]{ "de"}, "DB Reisezentrum"));
    Assert.assertEquals("Good morning", translator.translate(new String[]{ "es"}, "Buenos días"));
  }

  @Test
  @Category(UnitTest.class)
  public void testTranslateWithDetect()
  {
    
  }

  @Test
  @Category(UnitTest.class)
  public void testExhaustiveSearch()
  {
    
  }

  @Test
  @Category(UnitTest.class)
  public void testDetectedLangOverrides()
  {
    
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testLanguageNotAvailable()
  {
    
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testNoSourceLangs()
  {
    
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testInvalidTranslator()
  {
    
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testInvalidDetector()
  {
    
  }
}
