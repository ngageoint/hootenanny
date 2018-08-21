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
public final class TikaLanguageDetector implements LanguageDetector, SupportedLanguageConsumer, LanguageAppInfo
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

  //Not really expecting these to change often...but if so, could move them to the props config.
  public String getUrl() { return "https://tika.apache.org/"; }
  public String getDescription() 
  { return "The language detection portion of a library which detects and extracts metadata and text from many different file types"; }

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
