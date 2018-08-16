
package hoot.services.language;

import java.io.IOException;
import java.nio.charset.Charset;

import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Arrays;

import hoot.services.controllers.language.LanguageTranslateRequest;
import hoot.services.language.ToEnglishTranslatorFactory;
import hoot.services.language.LanguageDetectionConsumer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
*/
public final class HootLanguageTranslator implements ToEnglishTranslator, LanguageDetectionConsumer, SupportedLanguageConsumer
{
  private static final Logger logger = LoggerFactory.getLogger(HootLanguageTranslator.class);

  private String[] detectors;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages;
  private boolean performExhaustiveTranslationSearchWithNoDetection;
  private String detectedLangCode;
  private String detectorUsed;
  private ToEnglishTranslator translator = null;
  
  public HootLanguageTranslator() throws Exception
  {
    //TODO: decouple this
    translator = ToEnglishTranslatorFactory.create("JoshuaLanguageTranslator");
  }

  public void setConfig(Object config)
  {
    LanguageTranslateRequest request = (LanguageTranslateRequest)config;
    this.detectors = request.getDetectors();
    this.detectedLanguageOverridesSpecifiedSourceLanguages = request.getDetectedLanguageOverridesSpecifiedSourceLanguages();
    this.performExhaustiveTranslationSearchWithNoDetection = request.getPerformExhaustiveTranslationSearchWithNoDetection();
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

  public String translate(String sourceLangCode, String text) throws Exception
  {
    String[] sourceLangCodes = new String[] { sourceLangCode };
    return translate(sourceLangCodes, text);
  }

  public String translate(String[] sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes.length == 0)
    {
      throw new Exception("No source language codes or detect mode specified.");
    }

    logger.error("text: " + text);
    List<String> specifiedSourceLangs = Arrays.asList(sourceLangCodes);
    logger.error("specifiedSourceLangs size: " + String.valueOf(specifiedSourceLangs.size()));
    String sourceLangCode = null;
    String translatedText = "";

    for (String langCode : specifiedSourceLangs)
    {
      if (!((SupportedLanguageConsumer)translator).isLanguageAvailable(langCode.toLowerCase()))
      {
        throw new Exception("Requested unavailable translation language: " + langCode);
      }
    }

    if (specifiedSourceLangs.contains("detect") || specifiedSourceLangs.size() == 1) //TODO: case sens
    {
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
            if (!translatedText.isEmpty() && !translatedText.equals(text))
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
               !specifiedSourceLangs.contains(sourceLangCode)) //TODO: case sens
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
            if (!translatedText.isEmpty() && !translatedText.equals(text))
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
        if (!specifiedSourceLangs.contains(sourceLangCode)) //TODO: case sens
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
