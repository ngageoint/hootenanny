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

import static hoot.services.HootProperties.*;

import java.io.IOException;
import java.nio.charset.Charset;

import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

import java.lang.CloneNotSupportedException;

import hoot.services.controllers.language.LanguageTranslateRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.apache.commons.lang3.StringUtils;

/**
 * A translator using a customized translation workflow that makes use of language detection and 
   other options
 */
public class HootLanguageTranslator implements ToEnglishTranslator, LanguageDetectionConsumer, 
  SupportedLanguageConsumer, LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(HootLanguageTranslator.class);

  //detectors to use for language detection; default behavior is to continue using them, in order, 
  //until a detection is made
  private String[] detectors;

  //if true and a detected lang doesn't match a specified lang, then the detected lang takes priority
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages = false;

  //if true and no lang can be detected, an exhaustive search for the correct translation is made
  //against all available translatable languages (expensive)
  private boolean performExhaustiveTranslationSearchWithNoDetection = false;

  private String detectedLangCode = "";
  private String detectorUsed = "";

  //wrapped translator app
  private ToEnglishTranslator translator;

  public boolean getDetectedLanguageOverridesSpecifiedSourceLanguages() 
  { return detectedLanguageOverridesSpecifiedSourceLanguages; }

  public boolean getPerformExhaustiveTranslationSearchWithNoDetection() 
  { return performExhaustiveTranslationSearchWithNoDetection; }
  
  public HootLanguageTranslator() throws Exception
  {
    try
    {
      translator = ToEnglishTranslatorFactory.create(HOOT_LANGUAGE_TRANSLATOR_APP);
    }
    catch (Exception e)
    {
      throw new IllegalArgumentException(
        "Error creating translator: " + HOOT_LANGUAGE_TRANSLATOR_APP + "; error: " + e.getMessage());
    }
  }

  public void setConfig(Object config)
  {
    //don't like giving this translator class knowledge about LanguageTranslateRequest, a web class, but 
    //it will do for now
    LanguageTranslateRequest request = (LanguageTranslateRequest)config;
    detectors = request.getDetectors();
    if (detectors == null || detectors.length == 0)
    {
      detectors = LanguageDetectorFactory.getSimpleClassNames().toArray(new String[]{});
    }
    detectedLanguageOverridesSpecifiedSourceLanguages = 
      request.getDetectedLanguageOverridesSpecifiedSourceLanguages();
    performExhaustiveTranslationSearchWithNoDetection = 
      request.getPerformExhaustiveTranslationSearchWithNoDetection();
  }

  /**
   * @see LanguageDetectionConsumer
   */
  public String getDetectedLangCode() { return detectedLangCode; }

  /**
   * @see LanguageDetectionConsumer
   */
  public String getDetectorUsed() { return detectorUsed; }

  /**
   * @see SupportedLanguageConsumer
   */
  public boolean isLanguageAvailable(String langCode)
  {
    return ((SupportedLanguageConsumer)translator).isLanguageAvailable(langCode);
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public SupportedLanguage[] getSupportedLanguages() throws CloneNotSupportedException
  {
    return ((SupportedLanguageConsumer)translator).getSupportedLanguages();
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public String getLanguageName(String langCode)
  {
    return ((SupportedLanguageConsumer)translator).getLanguageName(langCode);
  }

  /**
   * @see LanguageAppInfo
   */
  public String getUrl() { return "N/A"; }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting this to change often...but if so, could move it to the props config.
   */
  public String getDescription() 
  { 
    return 
      "A custom translator that combines language detection into its workflow.  See the Hootenanny User " +
      "Documentation for more details.";
  }

  private String detectLanguage(String text, List<String> specifiedSourceLangs) throws Exception
  {
    String detLangCode = "";
    logger.trace("detectors: " + String.join(",", detectors));
    for (int i = 0; i < detectors.length; i++)
    {
      String detectorName = detectors[i];
      logger.trace("detectorName: " + detectorName);
      LanguageDetector detector = null;
      try
      {
        detector = LanguageDetectorFactory.create(detectorName);
      }
      catch (Exception e)
      {
        throw new IllegalArgumentException(
          "Error creating detector: " + detectorName + "; error: " + e.getMessage());
      }
      detLangCode = detector.detect(text);
      logger.trace("detLangCode: " + detLangCode);
      if (!detLangCode.isEmpty())
      {
        //successful detection

        detectorUsed = detector.getClass().getSimpleName();
        logger.trace("detectorUsed: " + detectorUsed);
        return detLangCode;
      }
    }

    return "";
  }

  /*
   * Attempts to use all available translators for the specified language to get a translation
   */
  private String performExhaustiveTranslation(String text, List<String> specifiedSourceLangs) 
    throws Exception
  {
    String translatedText = "";
    for (String langCode : specifiedSourceLangs)
    {
      translatedText = translator.translate(langCode, text);
      logger.trace("langCode: " + langCode);
      logger.trace("translatedText: " + translatedText);
      if (!translatedText.isEmpty() && !translatedText.toLowerCase().equals(text.toLowerCase()))
      {
        //successful translation
        return translatedText;
      }
    }
    return "";
  }

  /**
   * @see ToEnglishTranslator
   */
  public String translate(String sourceLangCode, String text) throws Exception
  {
    return translate(new String[] { sourceLangCode }, text);
  }

  /**
   * @see ToEnglishTranslator
   * @note sourceLangCodes must contain at least one lang code or the text, "detect"
   */
  public String translate(String[] sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes == null || sourceLangCodes.length == 0)
    {
      throw new IllegalArgumentException("No source language codes or detect mode specified.");
    }

    logger.trace("text: " + text);
    List<String> specifiedSourceLangs = Arrays.asList(sourceLangCodes);
    logger.trace("specifiedSourceLangs size: " + String.valueOf(specifiedSourceLangs.size()));
    logger.trace(
      "performExhaustiveTranslationSearchWithNoDetection: " + 
      performExhaustiveTranslationSearchWithNoDetection);
    logger.trace(
      "detectedLanguageOverridesSpecifiedSourceLanguages: " + 
      detectedLanguageOverridesSpecifiedSourceLanguages);

    String sourceLangCode = "";
    String translatedText = "";

    //make sure we have translators available for each of the requested languages
    for (String langCode : specifiedSourceLangs)
    {
      logger.trace("langCode: " + langCode);
      if (!langCode.toLowerCase().equals("detect") && 
          !((SupportedLanguageConsumer)translator).isLanguageAvailable(langCode.toLowerCase()))
      {
        throw new IllegalArgumentException("Requested unavailable translation language: " + langCode);
      }
    }

    final boolean containsDetect = specifiedSourceLangs.stream().anyMatch("detect"::equalsIgnoreCase);
    if (!containsDetect && specifiedSourceLangs.size() == 1)
    {
      //This is a straight up translation from a single specified language with no need for detection.

      sourceLangCode = specifiedSourceLangs.get(0);
      logger.debug("Using specified language code: " + sourceLangCode + " for translation...");
      translatedText = translator.translate(sourceLangCode, text);
    }
    else
    {
      //In this workflow, we'll try to detect the language before translating since either no source 
      //languages or more than one source language was specified, and we don't know for sure what the 
      //source language should be.

      //If "detect" mode was specified or more than one language was specified, then we'll have to try 
      //to detect the language that needs to be used for translation.
      detectedLangCode = detectLanguage(text, specifiedSourceLangs);
      logger.trace(
        "specifiedSourceLangs.contains(detectedLangCode): " + 
        specifiedSourceLangs.contains(detectedLangCode));
      //If the language detected isn't one we can translate from, we'll just have to ignore it. 
      if (!((SupportedLanguageConsumer)translator).isLanguageAvailable(detectedLangCode))
      {
        //Technically, if we detected an unsupported lang we could assume maybe the detected lang 
        //was incorrect and keep trying other detectors to see if one of them came up with something 
        //correct that we supported...but I don't think that's necessary.
        logger.debug(
          "Language detector detected language code: " + detectedLangCode + 
          ", but that language is not available for translation by the selected translator: " + 
          translator.getClass().getName() + ". Disregarding the detected language.");
      }
      else
      {
        sourceLangCode = detectedLangCode;
      }

      //If we still have no lang code by this point, then we weren't able to succesfully detect it.
      if (sourceLangCode.isEmpty())
      {
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          //Here we'll brute force try translations against all available languages (expensive).
          logger.debug("Unable to detect language.  Performing translation against each specified " +
                       "language until a translation is found...");
          translatedText = performExhaustiveTranslation(text, specifiedSourceLangs);
        }
        else
        {
          logger.debug("Unable to detect language.  Skipping translation for: " + text);
          translatedText = "";
        }
      }
      else if (!detectedLanguageOverridesSpecifiedSourceLanguages && specifiedSourceLangs.size() > 1 &&
               !specifiedSourceLangs.contains(sourceLangCode.toLowerCase()))
      {
        //If we've gotten to here, we have a detected lang but it doesn't match any of the langs we 
        //specified.  Since we're being strict that it must match one of them, we'll have to either try an 
        //exhaustive search or give up.

        String msg =
          "Detected language code: " + sourceLangCode + " not in specified source languages: " + 
          String.join(",", sourceLangCodes) + ".  ";
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          //brute force translation (expensive)
          msg += "Performing translation against each specified language until a translation is found...";
          logger.debug(msg);
          translatedText = performExhaustiveTranslation(text, specifiedSourceLangs);
        }
        else
        {
          logger.debug(
            "Detected language code: " + sourceLangCode + " not in specified source languages: " + 
            String.join(",", sourceLangCodes) + ".  Skipping translation; text: " + text);
          translatedText = "";
        }
      }
      else
      {
        //We've detected a language, so let's use it to translate.
        
        if (!specifiedSourceLangs.contains(sourceLangCode.toLowerCase()))
        {
          //Even though the lang we detected wasn't one that was specified, we're allowing the detected 
          //lang to be used since we support translating from it.
          assert(detectedLanguageOverridesSpecifiedSourceLanguages);
          logger.debug(
            "Detected language code: " + sourceLangCode + " overrides specified language(s) for text: " + 
            text);
        }
        else
        {
          logger.debug("Detected language code: " + sourceLangCode + " for text: " + text);
        }

        translatedText = translator.translate(sourceLangCode, text);
      }
    }
   
    if (!translatedText.isEmpty() && !translatedText.equals(text))
    {
      logger.debug(
        "Translated: " + StringUtils.left(text, 25) + " to: " + StringUtils.left(translatedText, 25) + 
        " for source language: " + sourceLangCode);
    }
    else if (translatedText.equals(text))
    {
      logger.debug(
        "Translated text matches source text: " + StringUtils.left(text, 25) + " for source language: " + 
        sourceLangCode);
    }
    else
    {
      logger.debug("No translation found for text: " + StringUtils.left(text, 25));
    }
    return translatedText;
  }
}
