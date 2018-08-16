
package hoot.services.language;

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetectorME;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

/*
http://opennlp.apache.org/
*/
public final class OpenNlpLanguageDetector implements LanguageDetector, SupportedLanguageConsumer
{
  private static final Logger logger = LoggerFactory.getLogger(OpenNlpLanguageDetector.class);

  private SupportedLanguagesConfigReader langsConfigReader = new SupportedLanguagesConfigReader();
  private SupportedLanguage[] supportedLangs = null;

  private /*final*/ LanguageDetectorME detector = null;
  //TODO: move to config
  private static final String OPEN_NLP_MODEL = "language-translation/langdetect-183.bin";

  private static OpenNlpLanguageDetector instance;

  private OpenNlpLanguageDetector() throws Exception
  {
    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.error("Reading OpenNlpLanguageDetector languages config...");
      supportedLangsConfigStrm = 
        OpenNlpLanguageDetector.class.getClassLoader().getResourceAsStream("language-translation/openNlpLanguages");
      supportedLangs = langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.error("Read " + supportedLangs.length + " languages from config for OpenNlpLanguageDetector.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }
  }

  public synchronized static OpenNlpLanguageDetector getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new OpenNlpLanguageDetector();
    }
    return instance;
  }

  //this may end up being faster
  /*private static class StaticHolder 
  {
    static final OpenNlpLanguageDetector INSTANCE = new OpenNlpLanguageDetector();
  }
 
  public static OpenNlpLanguageDetector getInstance() 
  {
    return StaticHolder.INSTANCE;
  }*/

  public boolean isLanguageAvailable(String langCode)
  {
    return true;
  }

  public SupportedLanguage[] getSupportedLanguages()
  {
    return supportedLangs;
  }

  public String getLanguageName(String langCode)
  {
    return langsConfigReader.getLanguageName(langCode);
  }

  public String detect(String text)
  {
    long startTime = System.currentTimeMillis();

    logger.debug("Detecting language with " + getClass().getName() + "; " + text + "...");
    String detectedLang = detector.predictLanguage(text).getLang();
    if (!detectedLang.isEmpty())
    {
      logger.debug(getClass().getName() + " detected language: " + detectedLang + " for text: " + text);
    }

    logger.error("Detection took {} seconds", (System.currentTimeMillis() - startTime) / 1000); 

    return detectedLang;
  }
}
