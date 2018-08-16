
package hoot.services.language;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.io.DataOutputStream;

import java.util.Map;
import java.util.HashMap;

import hoot.services.language.JoshuaServersInitializer;
import hoot.services.language.JoshuaServer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public final class JoshuaLanguageTranslator implements ToEnglishTranslator
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaLanguageTranslator.class);

  private Map<String, JoshuaServer> servers = null;

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws Exception
  {
    servers = JoshuaServersInitializer.init();
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

  public boolean isLanguageAvailable(String langCode)
  {
    return servers.containsKey(langCode.toLowerCase());
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

    String translatedText = "";
    BufferedReader reader = null;
    Socket clientSocket = null;
    DataOutputStream writer = null;
    try
    {
      logger.error("port: " + servers.get(sourceLangCode).getPort());
      clientSocket = new Socket("localhost", servers.get(sourceLangCode).getPort());
      writer = new DataOutputStream(clientSocket.getOutputStream());
      reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
      writer.writeBytes(textToSend);
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
