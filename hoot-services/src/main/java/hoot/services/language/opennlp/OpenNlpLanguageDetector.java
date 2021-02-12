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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

package hoot.services.language.opennlp;

import static hoot.services.HootProperties.*;

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetectorME;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

import java.lang.CloneNotSupportedException;

import hoot.services.language.LanguageDetector;
import hoot.services.language.SupportedLanguageConsumer;
import hoot.services.language.LanguageAppInfo;
import hoot.services.language.SupportedLanguagesConfigReader;
import hoot.services.language.SupportedLanguage;
import hoot.services.language.LanguageUtils;

/**
 * Detects languages using OpenNLP
 *
 * http://opennlp.apache.org/

 * Keep this class immutable for thread safety purposes, since its a Singleton.
 */
public final class OpenNlpLanguageDetector implements LanguageDetector, SupportedLanguageConsumer, 
  LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(OpenNlpLanguageDetector.class);

  //reads the langs that OpenNLP supports
  private final SupportedLanguagesConfigReader langsConfigReader =
    new SupportedLanguagesConfigReader();

  private final LanguageDetectorME detector;

  //init is expensive, so running as singleton
  private static OpenNlpLanguageDetector instance;

  private OpenNlpLanguageDetector() throws Exception
  {
    //which langs are supported is read from a hoot managed config file
    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.debug("Reading OpenNlpLanguageDetector languages config...");
      supportedLangsConfigStrm = 
        OpenNlpLanguageDetector.class.getClassLoader().getResourceAsStream(
          "language-translation/openNlpLanguages");
      langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.debug(
        "Read " + langsConfigReader.getSupportedLanguages().length + 
      " languages from config for OpenNlpLanguageDetector.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }
    
    //load the detection model
    InputStream modelConfigStrm = null;
    try
    {
      logger.debug("Loading OpenNlpLanguageDetector model...");
      modelConfigStrm = 
        OpenNlpLanguageDetector.class.getClassLoader().getResourceAsStream(
          OPEN_NLP_LANGUAGE_DETECTION_MODEL);
      if (modelConfigStrm == null)
      {
        logger.warn(
          "No OpenNLP language detection model available at: " + OPEN_NLP_LANGUAGE_DETECTION_MODEL + 
          ".  Disabling OpenNLP language detection.");
        detector = null;
      }
      else
      {
        detector = new LanguageDetectorME(new LanguageDetectorModel(modelConfigStrm));
      }
    }
    finally 
    {  
      if (modelConfigStrm != null)
      {
        modelConfigStrm.close();
      }
    }
  }

  //see singleton comment in TikaLanguageDetector::getInstance.
  public synchronized static OpenNlpLanguageDetector getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new OpenNlpLanguageDetector();
    }
    return instance;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public boolean isLanguageAvailable(String langCode)
  {
    if (detector == null)
    {
      return false;
    }
    return true;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public SupportedLanguage[] getSupportedLanguages() throws CloneNotSupportedException
  {
    return SupportedLanguage.cloneArray(langsConfigReader.getSupportedLanguages());
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
  public String getUrl() { return "https://opennlp.apache.org"; }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting this to change often...but if so, could move it to the props config.
   */
  public String getDescription() 
  { 
    return 
      "The language detector portion of a machine learning based toolkit for the processing of " +
      "natural language text"; 
  }

  /**
   * @see LanguageDetector
   */
  public LanguageUtils.DetectionConfidence getConfidence() 
  { return LanguageUtils.DetectionConfidence.NONE_AVAILABLE; }

  /**
   * Detects the language of the provided text
   *
   * @param text text for which to detect a language
   */
  public String detect(String text)
  {
    long startTime = System.currentTimeMillis();

    logger.trace("Detecting language with " + getClass().getName() + "; " + text + "...");
    String detectedLang = detector.predictLanguage(text).getLang();
    if (!detectedLang.isEmpty())
    {
      logger.trace(
        getClass().getName() + " detected language: " + detectedLang + " for text: " + text);
    }
    else
    {
      logger.trace("No language detection found for: " + text + " with TikaLanguageDetector.");
    }

    logger.trace("Detection took {} seconds", (System.currentTimeMillis() - startTime) / 1000); 

    return detectedLang;
  }
}
