
package hoot.services.language;

import org.apache.tika.langdetect.OptimaizeLangDetector;
import org.apache.tika.language.detect.LanguageResult;
import org.apache.tika.exception.TikaException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

/*
http://tika.apache.org/
*/
public final class TikaLanguageDetector implements LanguageDetector
{
  private static final Logger logger = LoggerFactory.getLogger(TikaLanguageDetector.class);

  private final org.apache.tika.language.detect.LanguageDetector detector = new OptimaizeLangDetector();

  private static TikaLanguageDetector instance;

  private TikaLanguageDetector() throws RuntimeException
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
  }

  public synchronized static TikaLanguageDetector getInstance()
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

  public String detect(String text)
  {
    logger.debug("Detecting language with " + getClass().getName() + "; " + text + "...");
    String detectedLang = detector.detect(text).getLanguage();
    if (!detectedLang.isEmpty())
    {
      logger.debug(getClass().getName() + " detected language: " + detectedLang + " for text: " + text);
    }
    return detectedLang;
  }
}
