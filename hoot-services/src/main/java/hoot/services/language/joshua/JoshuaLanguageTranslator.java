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

import static hoot.services.HootProperties.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.net.Socket;
import java.io.InputStream;
import java.io.DataOutputStream;

import java.nio.charset.Charset;

import java.util.Map;
import java.util.HashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public final class JoshuaLanguageTranslator implements ToEnglishTranslator, SupportedLanguageConsumer, LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  private SupportedLanguagesConfigReader langsConfigReader = new SupportedLanguagesConfigReader();
  private SupportedLanguage[] supportedLangs = null;
  private Map<String, JoshuaServiceInfo> services = null;
  private JoshuaConnectionPool connectionPool = null;
  public static final Charset ENCODING = Charset.forName("UTF-8");

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws Exception
  {
    services = JoshuaServicesInitializer.init();

    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.error("Reading JoshuaLanguageTranslator languages config...");
      supportedLangsConfigStrm = 
        JoshuaLanguageTranslator.class.getClassLoader().getResourceAsStream("language-translation/joshuaLanguages");
      supportedLangs = langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.error("Read " + supportedLangs.length + " languages from config for JoshuaLanguageTranslator.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }

    connectionPool = new JoshuaConnectionPool(services, Integer.parseInt(JOSHUA_CONNECTION_POOL_MAX_SIZE));
  }

  public void setConfig(Object config) {}

  public synchronized static JoshuaLanguageTranslator getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new JoshuaLanguageTranslator();
    }
    return instance;
  }

  //this may end up being faster
  /*private static class StaticHolder 
  {
    static final JoshuaLanguageTranslator INSTANCE = new JoshuaLanguageTranslator();
  }
 
  public static JoshuaLanguageTranslator getInstance() 
  {
    return StaticHolder.INSTANCE;
  }*/

  public SupportedLanguage[] getSupportedLanguages()
  {
    return supportedLangs;
  }

  public boolean isLanguageAvailable(String langCode)
  {
    return services.containsKey(langCode.toLowerCase());
  }

  public String getLanguageName(String langCode)
  {
    return langsConfigReader.getLanguageName(langCode);
  }

  //Not really expecting these to change often...but if so, could move them to the props config.
  public String getUrl() { return "https://cwiki.apache.org/confluence/display/JOSHUA"; }
  public String getDescription() 
  { return "A statistical machine translation decoder for phrase-based, hierarchical, and syntax-based machine translation"; }

  public String translate(String[] sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes.length > 1)
    {
      throw new Exception("Only one source language may be passed to this translator.");
    }
    return translate(sourceLangCodes[0], text);
  }

  public String translate(String sourceLangCode, String text) throws Exception
  {
    long startTime = System.currentTimeMillis();

    logger.error("text: " + text);
    //joshua expects a newline at the end of what's passed in
    String textToSend = null;
    if (!text.endsWith("\n"))
    {
      textToSend = text + "\n";
    }
    logger.error("textToSend: " + textToSend);
    sourceLangCode = sourceLangCode.toLowerCase();
    logger.error("sourceLangCode: " + sourceLangCode);
    logger.error("language is available: " + isLanguageAvailable(sourceLangCode));
    if (!isLanguageAvailable(sourceLangCode))
    {
      throw new Exception("No language translator available for language code: " + sourceLangCode);
    }

    String translatedText = "";
    JoshuaConnection connection = null;
    try
    {
      logger.trace("port: " + services.get(sourceLangCode).getPort());
      connection = connectionPool.borrowObject(sourceLangCode);
      byte[] bytes = textToSend.getBytes("UTF-8");
      DataOutputStream writer = (DataOutputStream)connection.getWriter();
      writer.write(bytes, 0, bytes.length);
      writer.flush();
      BufferedReader reader = (BufferedReader)connection.getReader();
      translatedText = reader.readLine().trim();
    }
    finally
    {
      connectionPool.returnObject(sourceLangCode, connection);
    }

    logger.error(getClass().getName() + " translated: " + text + " to: " + translatedText);
    logger.error("Translation took {} seconds", (System.currentTimeMillis() - startTime) / 1000);

    return translatedText;
  }

  public void close() throws Exception
  {
    if (connectionPool != null)
    {
      connectionPool.close();
    }
  }
}
