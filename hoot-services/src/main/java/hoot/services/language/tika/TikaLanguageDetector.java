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

package hoot.services.language.tika;

import org.apache.tika.langdetect.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageResult;
import org.apache.tika.exception.TikaException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

import hoot.services.language.LanguageDetector;
import hoot.services.language.SupportedLanguageConsumer;
import hoot.services.language.LanguageAppInfo;
import hoot.services.language.SupportedLanguagesConfigReader;
import hoot.services.language.SupportedLanguage;

/**
 * Detects languages using Tika
 *
 * http://tika.apache.org/

 * Keep this class immutable for thread safety purposes, since its a Singleton.
 */
public final class TikaLanguageDetector implements LanguageDetector, SupportedLanguageConsumer, 
  LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(TikaLanguageDetector.class);

  //reads the langs that Tika supports
  private final SupportedLanguagesConfigReader langsConfigReader = 
    new SupportedLanguagesConfigReader();

  private final org.apache.tika.language.detect.LanguageDetector detector = 
    new OptimaizeLangDetector();

  private static TikaLanguageDetector instance;

  private TikaLanguageDetector() throws Exception
  {
    //which languages are supported are read from a hoot managed config file
    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.debug("Reading TikaLanguageDetector languages config...");
      supportedLangsConfigStrm = 
        TikaLanguageDetector.class.getClassLoader().getResourceAsStream(
          "language-translation/tikaLanguages");
      langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.debug(
        "Read " + langsConfigReader.getSupportedLanguages().length + 
        " languages from config for TikaLanguageDetector.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }

    //load the language detection model
    try
    {
      logger.debug("loading tika model...");
      detector.loadModels();
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("Error reading Tika model", ioe);
    }
  }

  //There's apparently a trick in recent Java where you can get a more performant singleton 
  //by using a static inner class instead of a synchronized method.  I haven't been able to 
  //figure out yet how to get that to work when the constructor throws an exception, though.
  public synchronized static TikaLanguageDetector getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new TikaLanguageDetector();
    }
    return instance;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public boolean isLanguageAvailable(String langCode)
  {
    return true;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public SupportedLanguage[] getSupportedLanguages()
  {
    return langsConfigReader.getSupportedLanguages().clone();
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public String getLanguageName(String langCode)
  {
    return langsConfigReader.getLanguageName(langCode);
  }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting this to change often...but if so, could move it to the props config.
   */
  public String getUrl() { return "https://tika.apache.org"; }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting this to change often...but if so, could move it to the props config.
   */
  public String getDescription() 
  { 
    return 
      "The language detection portion of a library which detects and extracts metadata and text " +
      "from many different file types"; 
  }

  /**
   * Detects the language of the provided text
   *
   * @param text text for which to detect a language
   */
  public String detect(String text)
  {
    long startTime = System.currentTimeMillis();

    logger.debug("Detecting language with " + getClass().getName() + "; " + text + "...");
    String detectedLang = detector.detect(text).getLanguage();
    if (!detectedLang.isEmpty())
    {
      logger.debug(
        getClass().getName() + " detected language: " + detectedLang + " for text: " + text);
    }

    logger.trace("Detection took {} seconds", (System.currentTimeMillis() - startTime) / 1000);    

    return detectedLang;
  }
}
