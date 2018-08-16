
package hoot.services.language;

import org.apache.tika.langdetect.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageResult;
import org.apache.tika.exception.TikaException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

/*
http://tika.apache.org/
*/
public final class TikaLanguageDetector implements LanguageDetector, SupportedLanguageConsumer
{
  private static final Logger logger = LoggerFactory.getLogger(TikaLanguageDetector.class);

  private SupportedLanguagesConfigReader langsConfigReader = new SupportedLanguagesConfigReader();
  private SupportedLanguage[] supportedLangs = null;

  private final org.apache.tika.language.detect.LanguageDetector detector = new OptimaizeLangDetector();

  private static TikaLanguageDetector instance;

  private TikaLanguageDetector() throws Exception
  {
    try
    {
      logger.debug("loading tika model...");
      detector.loadModels();
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("Error reading Tika model", ioe);
    }

    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.error("Reading TikaLanguageDetector languages config...");
      supportedLangsConfigStrm = 
        TikaLanguageDetector.class.getClassLoader().getResourceAsStream("language-translation/tikaLanguages");
      supportedLangs = langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.error("Read " + supportedLangs.length + " languages from config for TikaLanguageDetector.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }
  }

  public synchronized static TikaLanguageDetector getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new TikaLanguageDetector();
    }
    return instance;
  }

  //this may end up being faster
  /*private static class StaticHolder 
  {
    static final TikaLanguageDetector INSTANCE = new TikaLanguageDetector();
  }
 
  public static TikaLanguageDetector getInstance() 
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
    String detectedLang = detector.detect(text).getLanguage();
    if (!detectedLang.isEmpty())
    {
      logger.debug(getClass().getName() + " detected language: " + detectedLang + " for text: " + text);
    }

    logger.error("Detection took {} seconds", (System.currentTimeMillis() - startTime) / 1000);    

    return detectedLang;
  }
}
