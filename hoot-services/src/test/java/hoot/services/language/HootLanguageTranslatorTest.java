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

import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.core.classloader.annotations.PowerMockIgnore;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.BeforeClass;
import org.junit.runner.RunWith;
import org.junit.AfterClass;
import org.junit.Before;

import hoot.services.UnitTest;
import hoot.services.language.joshua.JoshuaLanguageTranslator;
import hoot.services.language.tika.TikaLanguageDetector;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.language.HootLanguageTranslator;
import hoot.services.controllers.language.LanguageTranslateRequest;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.LanguageDetectorFactory;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@RunWith(PowerMockRunner.class)
@PrepareForTest(
  { JoshuaLanguageTranslator.class, TikaLanguageDetector.class, 
    ToEnglishTranslatorFactory.class, LanguageDetectorFactory.class })
@PowerMockIgnore("javax.management.*")
public class HootLanguageTranslatorTest  
{
  //private static String langAppBefore;

  private static final Logger logger = LoggerFactory.getLogger(HootLanguageTranslatorTest.class);

  @BeforeClass
  public static void beforeClass() throws Exception
  {
    //setting this property isn't working and not sure why; if the default translator changes, 
    //then it will need to be set
    //langAppBefore = HOOT_LANGUAGE_TRANSLATOR_APP;
    //HootCustomPropertiesSetter.setProperty("hootLanguageTranslatorApp", "JoshuaLanguageTranslator");
  }

  private void mockJoshua() throws Exception
  {
    //assuming two Joshua services are running, one for German and one for Spanish

    JoshuaLanguageTranslator wrappedTranslator = PowerMockito.mock(JoshuaLanguageTranslator.class);

    SupportedLanguage[] langs = new SupportedLanguage[2];

    SupportedLanguage lang1 = new SupportedLanguage();
    lang1.setIso6391Code("de");
    lang1.setIso6392Code("deu");
    lang1.setName("German");
    lang1.setAvailable(true);
    langs[0] = lang1;

    SupportedLanguage lang2 = new SupportedLanguage();
    lang2.setIso6391Code("es");
    lang2.setIso6392Code("spa");
    lang2.setName("Spanish");
    lang2.setAvailable(true);
    langs[1] = lang2;

    PowerMockito.when(wrappedTranslator.getSupportedLanguages()).thenReturn(langs);

    PowerMockito.when(wrappedTranslator.isLanguageAvailable("de")).thenReturn(true);
    PowerMockito.when(wrappedTranslator.isLanguageAvailable("es")).thenReturn(true);
    PowerMockito.when(wrappedTranslator.isLanguageAvailable("fr")).thenReturn(false);
    PowerMockito.when(wrappedTranslator.isLanguageAvailable("detect")).thenReturn(true);

    PowerMockito.when(wrappedTranslator.getLanguageName("de")).thenReturn("German");
    PowerMockito.when(wrappedTranslator.getLanguageName("es")).thenReturn("Spanish");
    PowerMockito.when(wrappedTranslator.getLanguageName("fr")).thenReturn(null);

    PowerMockito
      .when(wrappedTranslator.translate("de", "DB Reisezentrum"))
      .thenReturn("DB Tickets");
    PowerMockito
      .when(wrappedTranslator.translate("es", "Buenos días"))
      .thenReturn("Good morning");
    PowerMockito
      .when(wrappedTranslator.translate("de", "Fahrschule Weiß"))
      .thenReturn("Driving School Weiss");
    //no French service, so return empty string
    PowerMockito.when(wrappedTranslator.translate("de", "Carte de crédit")).thenReturn(""); 
    PowerMockito.when(wrappedTranslator.translate("es", "Carte de crédit")).thenReturn(""); 
    //This is already in English, so return the same as input.
    PowerMockito
      .when(wrappedTranslator.translate("de", "TC IT Service"))
      .thenReturn("TC IT Service");
    PowerMockito
      .when(wrappedTranslator.translate("es", "TC IT Service"))
      .thenReturn("TC IT Service");

    PowerMockito
      .whenNew(JoshuaLanguageTranslator.class).withAnyArguments()
      .thenReturn(wrappedTranslator);

    PowerMockito.mockStatic(ToEnglishTranslatorFactory.class);
    PowerMockito
      .when(ToEnglishTranslatorFactory
      .create("JoshuaLanguageTranslator"))
      .thenReturn(wrappedTranslator); 
  }
 
  private void mockTika() throws Exception
  {
    TikaLanguageDetector detector = PowerMockito.mock(TikaLanguageDetector.class);

    PowerMockito.when(detector.detect("DB Reisezentrum")).thenReturn("de");
    PowerMockito.when(detector.detect("Buenos días")).thenReturn("es");
    PowerMockito.when(detector.detect("Fahrschule Weiß")).thenReturn("de");
    PowerMockito.when(detector.detect("Carte de crédit")).thenReturn("fr");
    PowerMockito.when(detector.detect("TC IT Service")).thenReturn("en");

    PowerMockito.mockStatic(LanguageDetectorFactory.class);
    PowerMockito
      .when(LanguageDetectorFactory
      .create("TikaLanguageDetector"))
      .thenReturn(detector);
  }

  @Before
  public void beforeTest() throws Exception
  {
    //mocking Joshua b/c its too expensive to bring up during tests with any of the default lang packs; in the future we
    //could maybe create small test lang pack; Tika isn't that expensive to instantiate, but we'll mock it for
    //consistency's sake
    mockJoshua();
    mockTika();
  }

  @AfterClass
  public static void afterClass() throws Exception
  {
    //HootCustomPropertiesSetter.setProperty("hootLanguageTranslatorApp", langAppBefore);
  }

  @Test
  @Category(UnitTest.class)
  public void testLanguages() throws Exception
  {
    //these methods are just a pass through to JoshuaLanguageTranslator
    HootLanguageTranslator translator = new HootLanguageTranslator();
    Assert.assertEquals(2, translator.getSupportedLanguages().length);
    Assert.assertTrue(translator.isLanguageAvailable("de"));
    Assert.assertTrue(translator.isLanguageAvailable("es"));
    Assert.assertFalse(translator.isLanguageAvailable("fr"));
    Assert.assertEquals("German", translator.getLanguageName("de"));
    Assert.assertEquals("Spanish", translator.getLanguageName("es"));
    Assert.assertEquals(null, translator.getLanguageName("fr"));
  }

  @Test
  @Category(UnitTest.class)
  public void testTranslate() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    String translatedText = translator.translate("de", "DB Reisezentrum");
    Assert.assertEquals("DB Tickets", translatedText);
    Assert.assertTrue(translator.getDetectorUsed().isEmpty());
    Assert.assertTrue(translator.getDetectedLangCode().isEmpty());
    Assert.assertEquals(
      false, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      false, translator.getPerformExhaustiveTranslationSearchWithNoDetection());

    translatedText = translator.translate("es", "Buenos días");
    Assert.assertEquals("Good morning", translatedText);
    Assert.assertTrue(translator.getDetectorUsed().isEmpty());
    Assert.assertTrue(translator.getDetectedLangCode().isEmpty());
    Assert.assertEquals(
      false, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      false, translator.getPerformExhaustiveTranslationSearchWithNoDetection());
  }

  @Test
  @Category(UnitTest.class)
  public void testTranslateWithDetect() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    String translatedText = translator.translate(new String[] { "de", "es" }, "Buenos días");
    Assert.assertEquals("Good morning", translatedText);
    Assert.assertTrue(translator.getDetectorUsed().startsWith("TikaLanguageDetector"));
    Assert.assertEquals("es", translator.getDetectedLangCode());
    Assert.assertEquals(
      false, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      false, translator.getPerformExhaustiveTranslationSearchWithNoDetection());

    translatedText = translator.translate(new String[] { "detect" }, "Buenos días");
    Assert.assertEquals("Good morning", translator.translate(new String[] { "detect" }, "Buenos días"));
    Assert.assertTrue(translator.getDetectorUsed().startsWith("TikaLanguageDetector"));
    Assert.assertEquals("es", translator.getDetectedLangCode());
    Assert.assertEquals(
      false, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      false, translator.getPerformExhaustiveTranslationSearchWithNoDetection());
  }

  @Test
  @Category(UnitTest.class)
  public void testExhaustiveSearch() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(true);
    translator.setConfig(config);

    String translatedText = translator.translate("detect", "Buenos días");
    Assert.assertEquals("Good morning", translatedText);
    Assert.assertTrue(translator.getDetectorUsed().startsWith("TikaLanguageDetector"));
    Assert.assertEquals("es", translator.getDetectedLangCode());
    Assert.assertEquals(
      false, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      true, translator.getPerformExhaustiveTranslationSearchWithNoDetection());
  }

  @Test
  @Category(UnitTest.class)
  public void testDetectedLangOverrides() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(true);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    String translatedText = translator.translate("detect", "Buenos días");
    Assert.assertEquals("Good morning", translatedText);
    Assert.assertTrue(translator.getDetectorUsed().startsWith("TikaLanguageDetector"));
    Assert.assertEquals("es", translator.getDetectedLangCode());
    Assert.assertEquals(
      true, translator.getDetectedLanguageOverridesSpecifiedSourceLanguages());
    Assert.assertEquals(
      false, translator.getPerformExhaustiveTranslationSearchWithNoDetection());
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testLanguageNotAvailable() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    translator.translate("fr", "Buenos días");
  }

  @Test(expected = IllegalArgumentException.class)
  @Category(UnitTest.class)
  public void testNoSourceLangs() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "TikaLanguageDetector" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    translator.translate(new String[] {}, "Buenos días");
  }

  /*
   * @todo should be IllegalArgumentException
   */
  @Test(expected = /*IllegalArgument*/Exception.class)
  @Category(UnitTest.class)
  public void testInvalidDetector() throws Exception
  {
    HootLanguageTranslator translator = new HootLanguageTranslator();

    LanguageTranslateRequest config = new LanguageTranslateRequest();
    config.setDetectors(new String[] { "blah" });
    config.setDetectedLanguageOverridesSpecifiedSourceLanguages(false);
    config.setPerformExhaustiveTranslationSearchWithNoDetection(false);
    translator.setConfig(config);

    translator.translate(new String[] { "de", "es" }, "Buenos días");
  }
}
