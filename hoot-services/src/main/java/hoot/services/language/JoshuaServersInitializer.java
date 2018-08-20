
package hoot.services.language;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.ByteArrayOutputStream;

import java.util.Map;
import java.util.HashMap;

import java.nio.charset.Charset;

import org.apache.commons.io.FileUtils;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.commons.exec.ExecuteStreamHandler;
import org.apache.commons.exec.Executor;
import org.apache.commons.exec.DaemonExecutor;
import org.apache.commons.exec.ShutdownHookProcessDestroyer;
import org.apache.commons.exec.DefaultExecuteResultHandler;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/*
@article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
*/
public class JoshuaServersInitializer 
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaServersInitializer.class);

  public JoshuaServersInitializer() {}

  public static Map<String, JoshuaServer> init() throws Exception
  {
    logger.error("Initializing Joshua servers...");

    Map<String, JoshuaServer> servers = new HashMap<String, JoshuaServer>();

    //TODO: read all of this from hoot services config
   
    JoshuaServer server1 = new JoshuaServer();
    server1.setLanguageCode("de");
    server1.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-de-en-2016-11-18");
    server1.setPort(5764);
    servers.put(server1.getLanguageCode(), server1);

    JoshuaServer server2 = new JoshuaServer();
    server2.setLanguageCode("es");
    server2.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-es-en-2016-11-18");
    server2.setPort(5765);
    servers.put(server2.getLanguageCode(), server2);

    logger.error("servers size: " + servers.size());
    int ctr = 0;
    for (Map.Entry<String, JoshuaServer> serverEntry : servers.entrySet()) 
    {
      long startTime = System.currentTimeMillis();

      ctr++;
      JoshuaServer server = serverEntry.getValue();  
      logger.error(
        "Launching language translation service " + ctr + " / " + servers.size() + " for lang code: " + 
        server.getLanguageCode() + " from path: " + server.getLanguagePackPath() + " to port: " + server.getPort() + "...");  
      
      String configPath = server.getLanguagePackPath() + "/joshua.config";
      convertConfigFileModelPathsToAbsolute(configPath, server.getLanguagePackPath());
      //TODO: read from config
      //not sure why a wildcard won't work here...
      //String classPath = server.getLanguagePackPath() + "/target/joshua-*-jar-with-dependencies.jar";
      String classPath = server.getLanguagePackPath() + "/target/joshua-6.2-SNAPSHOT-jar-with-dependencies.jar";
      //TODO: read from config
      //Under light use, I've seen the memory consumption for the largest language packs hit 9-10GB, so 16 is probably a safe value
      //for them.  Smaller ones may not need as much memory.
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
    }
    logger.error(
      "Finished launching Joshua translation services for " + ctr + " language packs.  The services may take a few minutes to " +
      "finish initializing.");

    return servers;
  }

  private static void convertConfigFileModelPathsToAbsolute(String configPath, String langPackPath) throws IOException
  {
    File configFile = new File(configPath);
    String configContent = FileUtils.readFileToString(configFile, JoshuaLanguageTranslator.ENCODING);
    //change all model relative paths to absolute in the config
    configContent = configContent.replaceAll(" model/", " " + langPackPath + "/model/");
    FileUtils.writeStringToFile(configFile, configContent, JoshuaLanguageTranslator.ENCODING);
  }
}
