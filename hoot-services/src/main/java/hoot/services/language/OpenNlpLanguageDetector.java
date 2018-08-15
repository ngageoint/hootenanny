
package hoot.services.language;

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetectorME;
import opennlp.tools.langdetect.Language;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

/*
http://opennlp.apache.org/
*/
public final class OpenNlpLanguageDetector implements LanguageDetector
{
  private static final Logger logger = LoggerFactory.getLogger(OpenNlpLanguageDetector.class);

  private /*final*/ LanguageDetectorME detector = null;
  //TODO: move to config
  private static final String OPEN_NLP_MODEL = "language-translation/langdetect-183.bin";

  private static OpenNlpLanguageDetector instance;

  private OpenNlpLanguageDetector() throws RuntimeException, IOException
  {
    InputStream modelStrm = null;
    try 
    {
      logger.debug("loading opennlp model...");
      modelStrm = OpenNlpLanguageDetector.class.getClassLoader().getResourceAsStream(OPEN_NLP_MODEL); 
      detector = new LanguageDetectorME(new LanguageDetectorModel(modelStrm));
    }
    catch (IOException ioe) 
    {
      throw new RuntimeException("Error reading OpenNLP model as resource stream", ioe);
    }
    finally
    { 
      if (modelStrm != null)
      {
        modelStrm.close();
      }
    }
  }

  public synchronized static OpenNlpLanguageDetector getInstance() throws IOException
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
