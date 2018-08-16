
package hoot.services.language;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.io.File;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.ByteArrayOutputStream;

import java.util.Map;
import java.util.HashMap;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.commons.exec.ExecuteStreamHandler;
import org.apache.commons.exec.Executor;
import org.apache.commons.exec.DaemonExecutor;
import org.apache.commons.exec.ShutdownHookProcessDestroyer;
import org.apache.commons.exec.DefaultExecuteResultHandler;

import org.apache.commons.io.FileUtils;

import hoot.services.language.SupportedLanguages;
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

  private Map<String, JoshuaServer> servers = new HashMap<String, JoshuaServer>();
  private static final Charset FILE_ENCODING = Charset.forName("UTF-8");

  private static JoshuaLanguageTranslator instance;

  private JoshuaLanguageTranslator() throws Exception
  {
    launchTranslationServices();
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

  private void launchTranslationServices() throws Exception
  {
    logger.error("Initializing Joshua...");

    //TODO: read all of this from hoot services config
    int serverPort = 5764;
   
    JoshuaServer server1 = new JoshuaServer();
    server1.setLanguageCode("de");
    server1.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-de-en-2016-11-18");
    servers.put(server1.getLanguageCode(), server1);

    JoshuaServer server2 = new JoshuaServer();
    server2.setLanguageCode("es");
    server2.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-es-en-2016-11-18");
    servers.put(server2.getLanguageCode(), server2);

    logger.error("servers size: " + servers.size());
    int ctr = 0;
    for (Map.Entry<String, JoshuaServer> serverEntry : servers.entrySet()) 
    {
      long startTime = System.currentTimeMillis();

      ctr++;
      JoshuaServer server = serverEntry.getValue();  
      server.setPort(serverPort); 
      String langName = SupportedLanguages.getInstance().getLanguageName(server.getLanguageCode());
      logger.error(
        "Launching language translation service " + ctr + " / " + servers.size() + " for lang: " + langName + " from path: " + 
        server.getLanguagePackPath() + " to port: " + server.getPort() + "...");  
      
      String configPath = server.getLanguagePackPath() + "/joshua.config";
      convertConfigFileModelPathsToAbsolute(configPath, server.getLanguagePackPath());
      //TODO: read from config
      //not sure why a wildcard won't work here...
      //String classPath = server.getLanguagePackPath() + "/target/joshua-*-jar-with-dependencies.jar";
      String classPath = server.getLanguagePackPath() + "/target/joshua-6.2-SNAPSHOT-jar-with-dependencies.jar";
      //TODO: read from config
      //Under light use, I've seen the memory consumption for the largest language packs hit 9-10GB so this is probably a safe value
      //for the larger language packs.  Smaller ones may not need so much memory.
      final int memoryGigs = 16;

      OutputStream stdout = new ByteArrayOutputStream();
      OutputStream stderr = new ByteArrayOutputStream();

      String line = 
        "java -mx" + memoryGigs + "g -Dfile.encoding=utf8 -Djava.library.path=./lib -cp " + classPath + 
        " org.apache.joshua.decoder.JoshuaDecoder -c " + configPath + " -v 1 -server-port " + server.getPort() + " -server-type tcp";
      logger.error("command: " + line);
      CommandLine cmdLine = CommandLine.parse(line);

      ExecuteStreamHandler executeStreamHandler = new PumpStreamHandler(stdout, stderr);
      Executor executor = new DaemonExecutor();
      ExecuteWatchdog watchDog = new ExecuteWatchdog(ExecuteWatchdog.INFINITE_TIMEOUT);
      executor.setWatchdog(watchDog);
      executor.setStreamHandler(executeStreamHandler);
      executor.setProcessDestroyer(new ShutdownHookProcessDestroyer());
      executor.execute(cmdLine, new DefaultExecuteResultHandler());
      logger.error("stdout: " + stdout.toString());
      logger.error("stderr: " + stderr.toString());

      serverPort++;
    }
    logger.error(
      "Finished launching Joshua translation services for " + ctr + " language packs.  The services may take a few minutes to " +
      "finish initializing.");
  }

  private void convertConfigFileModelPathsToAbsolute(String configPath, String langPackPath) throws IOException
  {
    File configFile = new File(configPath);
    String configContent = FileUtils.readFileToString(configFile, FILE_ENCODING);
    //change all model relative paths to absolute in the config
    configContent = configContent.replaceAll(" model/", " " + langPackPath + "/model/");
    FileUtils.writeStringToFile(configFile, configContent, FILE_ENCODING);
  }

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
