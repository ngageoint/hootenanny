
package hoot.services.language;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.net.Socket;
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
public final class JoshuaLanguageTranslator implements ToEnglishTranslator, SupportedLanguageConsumer
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  private SupportedLanguagesConfigReader langsConfigReader = new SupportedLanguagesConfigReader();
  private SupportedLanguage[] supportedLangs = null;
  private Map<String, JoshuaServer> servers = null;
  public static final Charset ENCODING = Charset.forName("UTF-8");

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws Exception
  {
    servers = JoshuaServersInitializer.init();

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
    return servers.containsKey(langCode.toLowerCase());
  }

  public String getLanguageName(String langCode)
  {
    return langsConfigReader.getLanguageName(langCode);
  }

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

    //TODO: implement socket pooling
    String translatedText = "";
    BufferedReader reader = null;
    Socket clientSocket = null;
    DataOutputStream writer = null;
    try
    {
      logger.error("port: " + servers.get(sourceLangCode).getPort());
      clientSocket = new Socket("localhost", servers.get(sourceLangCode).getPort());
      writer = new DataOutputStream(clientSocket.getOutputStream());
      reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream(), ENCODING));
      byte[] bytes = textToSend.getBytes("UTF-8");
      writer.write(bytes, 0, bytes.length);
      writer.flush();
      translatedText = reader.readLine().trim();
    }
    finally
    {
      if (reader != null)
      {
        reader.close();
      }
      if (writer != null)
      {
        writer.close();
      }
      if (clientSocket != null)
      {
        clientSocket.close();
      }
    }

    logger.error(getClass().getName() + " translated: " + text + " to: " + translatedText);
    logger.error("Translation took {} seconds", (System.currentTimeMillis() - startTime) / 1000);

    return translatedText;
  }
}
