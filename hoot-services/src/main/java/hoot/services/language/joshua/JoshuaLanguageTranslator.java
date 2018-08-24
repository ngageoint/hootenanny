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

import org.apache.commons.lang3.StringUtils;

/*
 * Performs to English translations using a Joshua service

  @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public final class JoshuaLanguageTranslator implements ToEnglishTranslator, 
  SupportedLanguageConsumer, LanguageAppInfo
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  //this reads a hoot managed config that determines what languages Joshua supports
  private SupportedLanguagesConfigReader langsConfigReader = 
    new SupportedLanguagesConfigReader();
  private SupportedLanguage[] supportedLangs = null;
  //which services we have running; one for each language
  private Map<String, JoshuaServiceInfo> services = null;
  private JoshuaConnectionPool connectionPool = null;
  public static final Charset ENCODING = Charset.forName("UTF-8");

  //startup costs are costly, so running as Singleton
  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws Exception
  {
    //launch all of our services
    services = JoshuaServicesInitializer.init();

    //determine which languages we support (can be a superset of the languages made available by 
    //the running services)
    readSupportedLangsConfig();

    //init our connection pool to the services
    connectionPool = 
      new JoshuaConnectionPool(services, Integer.parseInt(JOSHUA_CONNECTION_POOL_MAX_SIZE));
  }

  private void readSupportedLangsConfig() throws Exception
  {
    InputStream supportedLangsConfigStrm = null;
    try
    {
      logger.debug("Reading JoshuaLanguageTranslator languages config...");
      supportedLangsConfigStrm = 
        JoshuaLanguageTranslator.class.getClassLoader().getResourceAsStream(
          "language-translation/joshuaLanguages");
      supportedLangs = langsConfigReader.readConfig(supportedLangsConfigStrm);
      logger.debug(
        "Read " + supportedLangs.length + " languages from config for JoshuaLanguageTranslator.");
    }
    finally 
    {  
      if (supportedLangsConfigStrm != null)
      {
        supportedLangsConfigStrm.close();
      }
    }
  }

  public void setConfig(Object config) {}

  //see singleton comment in TikaLanguageDetector::getInstance.
  public synchronized static JoshuaLanguageTranslator getInstance() throws Exception
  {
    if (instance == null)
    { 
      instance = new JoshuaLanguageTranslator();
    }
    return instance;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public SupportedLanguage[] getSupportedLanguages()
  {
    return supportedLangs;
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public boolean isLanguageAvailable(String langCode)
  {
    return services.containsKey(langCode.toLowerCase());
  }

  /**
   * @see SupportedLanguageConsumer
   */
  public String getLanguageName(String langCode)
  {
    return langsConfigReader.getLanguageName(langCode);
  }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting this to change often...but if so, could move it to the props config.
   */
  public String getUrl() { return "https://cwiki.apache.org/confluence/display/JOSHUA"; }

  /**
   * @see LanguageAppInfo
   *
   * Not really expecting these to change often...but if so, could move it to the props config.
   */
  public String getDescription() 
  { 
    return 
      "A statistical machine translation decoder for phrase-based, hierarchical, and " +
      "syntax-based machine translation"; 
  }

  /**
   * @see ToEnglishTranslator
   * @note sourceLangCodes must contain at least one code
   */
  public String translate(String[] sourceLangCodes, String text) throws Exception
  {
    if (sourceLangCodes.length > 1)
    {
      throw new IllegalArgumentException(
        "Only one source language may be passed to this translator.");
    }
    return translate(sourceLangCodes[0], text);
  }

  /**
   * @see ToEnglishTranslator
   */
  public String translate(String sourceLangCode, String text) throws Exception
  {
    long startTime = System.currentTimeMillis();

    logger.trace("text: " + text);
    //joshua expects a newline at the end of what's passed in
    String textToSend = text;
    if (!textToSend.endsWith("\n"))
    {
      textToSend += "\n";
    }
    logger.trace("textToSend: " + textToSend);
    final int numTextLines = textToSend.split("\n").length;
    logger.trace("numTextLines: " + numTextLines);

    sourceLangCode = sourceLangCode.toLowerCase();
    logger.trace("sourceLangCode: " + sourceLangCode);
    logger.trace("language is available: " + isLanguageAvailable(sourceLangCode));
    if (!isLanguageAvailable(sourceLangCode))
    {
      throw new IllegalArgumentException(
        "No language translator available for language code: " + sourceLangCode);
    }
    logger.trace("port: " + services.get(sourceLangCode).getPort());

    String translatedText = "";
    JoshuaConnection connection = null;
    try
    {
      connection = sendRequest(textToSend, sourceLangCode);
      translatedText = parseResponse(connection, numTextLines);
    }
    finally
    {
      connectionPool.returnObject(sourceLangCode, connection);
    }

    logger.trace(getClass().getSimpleName() + " translated: " + text + " to: " + translatedText);
    logger.trace("Translation took {} seconds", (System.currentTimeMillis() - startTime) / 1000);

    return translatedText;
  }

  private String parseResponse(JoshuaConnection connection, final int numTextLines) throws IOException
  {
    String translatedText = "";
    BufferedReader reader = (BufferedReader)connection.getReader();

    String line = "";
    //Joshua isn't newline terminating the last piece of text when translating multiple 
    //lines, so a while loop checking for a null line won't work here as the reader will block 
    //forever waiting for the last newline.
    for (int i = 0; i < numTextLines; i++) 
    {
      line = StringUtils.trimToNull(reader.readLine());
      logger.trace("line: " + line);
      if (line != null)
      {
        translatedText += line + "\n";
        logger.trace("translatedText: " + translatedText);
      }
    }

    return translatedText.trim();
  }

  private JoshuaConnection sendRequest(String textToSend, String sourceLangCode) throws Exception
  {
    JoshuaConnection connection = connectionPool.borrowObject(sourceLangCode);
    byte[] bytes = textToSend.getBytes("UTF-8");
    DataOutputStream writer = (DataOutputStream)connection.getWriter();
    writer.write(bytes, 0, bytes.length);
    writer.flush();
    return connection;
  }

  /**
   * Closes the connection pool
   */
  public void close() throws Exception
  {
    if (connectionPool != null)
    {
      connectionPool.close();
    }
  }
}
