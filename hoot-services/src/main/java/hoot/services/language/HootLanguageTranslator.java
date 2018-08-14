
package hoot.services.language;

import java.io.IOException;
import java.nio.charset.Charset;

import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;

import hoot.services.language.SupportedLanguages;
import hoot.services.controllers.language.LanguageTranslateRequest;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
*/
public final class HootLanguageTranslator implements ToEnglishTranslator
{
  private static final Logger logger = LoggerFactory.getLogger(HootLanguageTranslator.class);

  private String[] detectors;
  private boolean detectedLanguageOverridesSpecifiedSourceLanguages;
  private boolean performExhaustiveTranslationSearchWithNoDetection;
  private String detectedLanguage;
  
  public HootLanguageTranslator()
  {
  }

  public void setConfig(Object config)
  {
    LanguageTranslateRequest request = (LanguageTranslateRequest)config;
    this.detectors = request.getDetectors();
    this.detectedLanguageOverridesSpecifiedSourceLanguages = request.getDetectedLanguageOverridesSpecifiedSourceLanguages();
    this.performExhaustiveTranslationSearchWithNoDetection = request.getPerformExhaustiveTranslationSearchWithNoDetection();
  }

  public String getDetectedLanguage() { return detectedLanguage; }

  public String translate(StringList sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes.length() == 0)
    {
      throw new Exception("No source language codes or detect mode specified.");
    }

    List<String> specifiedSourceLangs = Arrays.asList(sourceLangCodes);
    logger.error(specifiedSourceLangs.size());
    String sourceLangCode = null;
    String translatedText = "";

    for (String sourceLangCode : specifiedSourceLangs)
    {
      if (!SupportedLanguages.getInstance().isSupportedLanguage(sourceLangCode.toLowerCase()))
      {
        throw new Exception("Requested unsupported translation language: " + request.getSourceLangCode());
      }
    }

    if (specifiedSourceLangs.contains("detect") || specifiedSourceLangs.size() == 1) //TODO: case sens
    {
      //for (String detector : detectors)
      for (int i = 0; i < detectors.length(); i++)
      {
        String detector = detectors[i];
        logger.error(detector);
        sourceLangCode = LanguageDetectorFactory.create(detector).detect(text);
        if (!sourceLangCode.isEmpty())
        {
          detectedLanguage = SupportedLanguages.getInstance().getCountryName(sourceLangCode);
          break;
        }
      }

      if (sourceLangCode.isEmpty())
      {
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          logger.error("Unable to detect language.  Performing translation against each specified " +
                       "language until a translation is found...");
          for (String sourceLangCode : specifiedSourceLangs)
          {
            translatedText = translator.translate(sourceLangCode, text);
            if (!translatedText.isEmpty() && !translatedText.equals(text)
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
          "Detected language: " + SupportedLanguages.getInstance().getCountryName(sourceLangCode) +
          " not in specified source languages: " + String.join(",", request.getSourceLangCodes()) + ".  ";
        if (performExhaustiveTranslationSearchWithNoDetection)
        {
          msg +=
            "Performing translation against each specified language until a translation is found...";
          logger.error(msg);
          for (String sourceLangCode : specifiedSourceLangs)
          {
            translatedText = translator.translate(sourceLangCode, text);
            if (!translatedText.isEmpty() && !translatedText.equals(text)
            {
              return translatedText;
            }
          }
        }
        else
        {
          logger.error(
            "Detected language: " +
            SupportedLanguages.getInstance().getCountryName(sourceLangCode) +
            " not in specified source languages: " + String.join(",", request.getSourceLangCodes()) +
            ".  Skipping translation; text: " + request.getText());
          return "";
        }
      }
      else
      {
        if (!specifiedSourceLangs.contains(sourceLangCode)) //TODO: case sens
        {
          assert(detectedLanguageOverridesSpecifiedSourceLanguages);
          logger.error(
            "Detected language: " +
            SupportedLanguages.getInstance().getCountryName(sourceLangCode) +
            " overrides specified language(s) for text: " + text);
        }
        else
        {
          logger.error(
            "Detected language: " +
            SupportedLanguages.getInstance().getCountryName(sourceLangCode) + " for text: " + text);
        }

        translatedText = translator.translate(sourceLangCode, text);
      }
    }
    else
    {
      sourceLangCode = specifiedSourceLangs.at(0);
      logger.error(
        "Using specified language: " +
        SupportedLanguages.getInstance().getCountryName(sourceLangCode));
      translatedText = translator.translate(sourceLangCode, text);
    }

    return translatedText;
  }
}
