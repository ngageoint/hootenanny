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

import java.io.IOException;
import java.nio.charset.Charset;

import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

import hoot.services.controllers.language.LanguageTranslateRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
*/
public final class HootLanguageTranslator implements ToEnglishTranslator, LanguageDetectionConsumer, SupportedLanguageConsumer, 
  LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(HootLanguageTranslator.class);

  private String[] detectors;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages = false;
  private boolean performExhaustiveTranslationSearchWithNoDetection = false;
  private String detectedLangCode = "";
  private String detectorUsed = "";
  private ToEnglishTranslator translator;
  
  public HootLanguageTranslator() throws Exception
  {
    //TODO: put in config
    translator = ToEnglishTranslatorFactory.create("JoshuaLanguageTranslator");
  }

  public void setConfig(Object config)
  {
    LanguageTranslateRequest request = (LanguageTranslateRequest)config;
    detectors = request.getDetectors();
    if (detectors == null || detectors.length == 0)
    {
      detectors = LanguageDetectorFactory.getSimpleClassNames().toArray(new String[]{});
    }
    detectedLanguageOverridesSpecifiedSourceLanguages = request.getDetectedLanguageOverridesSpecifiedSourceLanguages();
    performExhaustiveTranslationSearchWithNoDetection = request.getPerformExhaustiveTranslationSearchWithNoDetection();
  }

  public String getDetectedLangCode() { return detectedLangCode; }

  public String getDetectorUsed() { return detectorUsed; }

  public boolean isLanguageAvailable(String langCode)
  {
    return ((SupportedLanguageConsumer)translator).isLanguageAvailable(langCode);
  }

  public SupportedLanguage[] getSupportedLanguages()
  {
    return ((SupportedLanguageConsumer)translator).getSupportedLanguages();
  }

  public String getLanguageName(String langCode)
  {
    return ((SupportedLanguageConsumer)translator).getLanguageName(langCode);
  }

  public String getUrl() { return "N/A"; }
  public String getDescription() 
  { return "A custom translator that combines language detection into its workflow.  See the Hootenanny User Documentation for more details."; }

  public String translate(String sourceLangCode, String text) throws Exception
  {
    String[] sourceLangCodes = new String[] { sourceLangCode };
    return translate(sourceLangCodes, text);
  }

  public String translate(String[] sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes == null || sourceLangCodes.length == 0)
    {
      throw new Exception("No source language codes or detect mode specified.");
    }

    logger.error("text: " + text);
    List<String> specifiedSourceLangs = Arrays.asList(sourceLangCodes);;
    logger.error("specifiedSourceLangs size: " + String.valueOf(specifiedSourceLangs.size()));
    logger.error("performExhaustiveTranslationSearchWithNoDetection: " + performExhaustiveTranslationSearchWithNoDetection);
    logger.error("detectedLanguageOverridesSpecifiedSourceLanguages: " + detectedLanguageOverridesSpecifiedSourceLanguages);

    String sourceLangCode = "";
    String translatedText = "";

    for (String langCode : specifiedSourceLangs)
    {
      if (!langCode.toLowerCase().equals("detect") && 
          !((SupportedLanguageConsumer)translator).isLanguageAvailable(langCode.toLowerCase()))
      {
        throw new Exception("Requested unavailable translation language: " + langCode);
      }
    }

    final boolean containsDetect = specifiedSourceLangs.stream().anyMatch("detect"::equalsIgnoreCase);
    if (containsDetect || specifiedSourceLangs.size() > 1)
    {
      logger.error("detectors: " + String.join(",", detectors));
      for (int i = 0; i < detectors.length; i++)
      {
        String detectorName = detectors[i];
        logger.error("detectorName: " + detectorName);
        LanguageDetector detector = LanguageDetectorFactory.create(detectorName);
        sourceLangCode = detector.detect(text);
        logger.error("sourceLangCode: " + sourceLangCode);
        if (!sourceLangCode.isEmpty())
        {
          detectedLangCode = sourceLangCode;
          logger.error("detectedLangCode: " + detectedLangCode);
          detectorUsed = detector.getClass().getSimpleName();
          logger.error("specifiedSourceLangs.contains(sourceLangCode): " + specifiedSourceLangs.contains(sourceLangCode));

          if (!((SupportedLanguageConsumer)translator).isLanguageAvailable(sourceLangCode))
          {
            logger.error(
              "Language detector detected language code: " + sourceLangCode + ", but that language is not available for " +
              "translation by the selected translator: " + translator.getClass().getName() + ". Disregarding the detected language.");
            sourceLangCode = "";
          }

          break;
        }
      }

      if (sourceLangCode.isEmpty())
      {
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          logger.error("Unable to detect language.  Performing translation against each specified " +
                       "language until a translation is found...");
          for (String langCode : specifiedSourceLangs)
          {
            translatedText = translator.translate(langCode, text);
            logger.error("translatedText: " + translatedText);
            if (!translatedText.isEmpty() && !translatedText.toLowerCase().equals(text.toLowerCase()))
            {
              return translatedText;
            }
          }
        }
        else
        {
          logger.error("Unable to detect language.  Skipping translation for: " + text);
          return "";
        }
      }
      else if (!detectedLanguageOverridesSpecifiedSourceLanguages && specifiedSourceLangs.size() > 1 &&
               !specifiedSourceLangs.contains(sourceLangCode.toLowerCase()))
      {
        String msg =
          "Detected language code: " + sourceLangCode + " not in specified source languages: " + 
          String.join(",", sourceLangCodes) + ".  ";
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          msg += "Performing translation against each specified language until a translation is found...";
          logger.error(msg);
          for (String langCode : specifiedSourceLangs)
          {
            translatedText = translator.translate(langCode, text);
            logger.error("translatedText: " + translatedText);
            if (!translatedText.isEmpty() && !translatedText.toLowerCase().equals(text.toLowerCase()))
            {
              return translatedText;
            }
          }
        }
        else
        {
          logger.error(
            "Detected language code: " + sourceLangCode + " not in specified source languages: " + 
            String.join(",", sourceLangCodes) + ".  Skipping translation; text: " + text);
          return "";
        }
      }
      else
      {
        if (!specifiedSourceLangs.contains(sourceLangCode.toLowerCase()))
        {
          assert(detectedLanguageOverridesSpecifiedSourceLanguages);
          logger.error("Detected language code: " + sourceLangCode + " overrides specified language(s) for text: " + text);
        }
        else
        {
          logger.error("Detected language code: " + sourceLangCode + " for text: " + text);
        }

        translatedText = translator.translate(sourceLangCode, text);
        logger.error("translatedText: " + translatedText);
      }
    }
    else
    {
      sourceLangCode = specifiedSourceLangs.get(0);
      logger.error("sourceLangCode: " + sourceLangCode);
      logger.error("Using specified language code: " + sourceLangCode);
      translatedText = translator.translate(sourceLangCode, text);
      logger.error("translatedText: " + translatedText);
    }

    return translatedText;
  }
}
