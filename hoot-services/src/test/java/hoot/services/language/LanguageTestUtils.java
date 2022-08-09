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

import static org.mockito.Mockito.*;

import org.powermock.api.mockito.PowerMockito;

import java.util.Set;
import java.util.TreeSet;

import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.LanguageDetectorFactory;
import hoot.services.language.joshua.JoshuaLanguageTranslator;
import hoot.services.language.tika.TikaLanguageDetector;
import hoot.services.language.SupportedLanguage;
import hoot.services.language.SupportedLanguageConsumer;

/**
 * Various general test utilities for language detection/translation
 */
public class LanguageTestUtils 
{
  public static SupportedLanguage[] getSupportedLanguages()
  {
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

    return langs;
  }

  public static ToEnglishTranslator mockJoshua() throws Exception
  {
    //assuming two Joshua services are running, one for German and one for Spanish

    JoshuaLanguageTranslator translator = PowerMockito.mock(JoshuaLanguageTranslator.class);

    PowerMockito.when(translator.getSupportedLanguages()).thenReturn(getSupportedLanguages());

    PowerMockito.when(translator.isLanguageAvailable("de")).thenReturn(true);
    PowerMockito.when(translator.isLanguageAvailable("es")).thenReturn(true);
    PowerMockito.when(translator.isLanguageAvailable("fr")).thenReturn(false);

    PowerMockito.when(translator.getLanguageName("de")).thenReturn("German");
    PowerMockito.when(translator.getLanguageName("es")).thenReturn("Spanish");
    PowerMockito.when(translator.getLanguageName("fr")).thenReturn(null);

    PowerMockito
      .when(translator.translate("de", "DB Reisezentrum"))
      .thenReturn("DB Tickets");
    PowerMockito
      .when(translator.translate("es", "Buenos días"))
      .thenReturn("Good morning");
    PowerMockito
      .when(translator.translate(new String[] { "es" }, "Buenos días"))
      .thenReturn("Good morning");
    //batch translate
    PowerMockito
      .when(translator.translate(new String[] { "es" }, "Buenos días\nBuenos noches"))
      .thenReturn("Good morning\nGood night");

    PowerMockito
      .whenNew(JoshuaLanguageTranslator.class).withAnyArguments()
      .thenReturn(translator);

    PowerMockito.mockStatic(ToEnglishTranslatorFactory.class);
    PowerMockito
      .when(ToEnglishTranslatorFactory.create("JoshuaLanguageTranslator"))
      .thenReturn(translator); 
    Set<String> classNames = new TreeSet<String>();
    classNames.add("JoshuaLanguageTranslator");
    PowerMockito
      .when(ToEnglishTranslatorFactory.getSimpleClassNames())
      .thenReturn(classNames);

    return translator;
  }
 
  public static LanguageDetector mockTika() throws Exception
  {
    TikaLanguageDetector detector = PowerMockito.mock(TikaLanguageDetector.class);

    PowerMockito.when(detector.getSupportedLanguages()).thenReturn(getSupportedLanguages());

    PowerMockito.when(detector.isLanguageAvailable("de")).thenReturn(true);
    PowerMockito.when(detector.isLanguageAvailable("es")).thenReturn(true);
    PowerMockito.when(detector.isLanguageAvailable("fr")).thenReturn(false);

    PowerMockito.when(detector.getLanguageName("de")).thenReturn("German");
    PowerMockito.when(detector.getLanguageName("es")).thenReturn("Spanish");
    PowerMockito.when(detector.getLanguageName("fr")).thenReturn(null);

    PowerMockito.when(detector.detect("DB Reisezentrum")).thenReturn("de");
    PowerMockito.when(detector.detect("Buenos días")).thenReturn("es");

    PowerMockito.when(
      detector.getConfidence()).thenReturn(LanguageUtils.DetectionConfidence.HIGH);

    PowerMockito.mockStatic(LanguageDetectorFactory.class);
    PowerMockito
      .when(LanguageDetectorFactory.create("TikaLanguageDetector"))
      .thenReturn(detector);
    Set<String> classNames = new TreeSet<String>();
    classNames.add("TikaLanguageDetector");
    PowerMockito
      .when(LanguageDetectorFactory.getSimpleClassNames())
      .thenReturn(classNames);

    return detector;
  }
}
