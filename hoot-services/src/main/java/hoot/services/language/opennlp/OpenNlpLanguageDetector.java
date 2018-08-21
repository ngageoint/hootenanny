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

import opennlp.tools.langdetect.LanguageDetectorModel;
import opennlp.tools.langdetect.LanguageDetectorME;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;

/*
http://opennlp.apache.org/
*/
public final class OpenNlpLanguageDetector implements LanguageDetector, SupportedLanguageConsumer, LanguageAppInfo
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

  //Not really expecting these to change often...but if so, could move them to the props config.
  public String getUrl() { return "https://opennlp.apache.org/"; }
  public String getDescription() 
  { return "The language detector portion of a machine learning based toolkit for the processing of natural language text"; }

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
