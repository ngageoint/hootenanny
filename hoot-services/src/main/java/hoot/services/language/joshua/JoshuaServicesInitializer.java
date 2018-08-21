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
*/
public class JoshuaServicesInitializer 
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaServicesInitializer.class);

  public JoshuaServicesInitializer() {}

  public static Map<String, JoshuaServiceInfo> init() throws Exception
  {
    logger.error("Initializing Joshua servers...");

    Map<String, JoshuaServiceInfo> services = new HashMap<String, JoshuaServiceInfo>();

    //TODO: read all of this from hoot services config
   
    JoshuaServiceInfo service1 = new JoshuaServiceInfo();
    service1.setLanguageCode("de");
    service1.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-de-en-2016-11-18");
    service1.setPort(5764);
    services.put(service1.getLanguageCode(), service1);

    JoshuaServiceInfo service2 = new JoshuaServiceInfo();
    service2.setLanguageCode("es");
    service2.setLanguagePackPath("/home/vagrant/joshua-language-packs/apache-joshua-es-en-2016-11-18");
    service2.setPort(5765);
    services.put(service2.getLanguageCode(), service2);

    logger.error("servers size: " + services.size());
    int ctr = 0;
    for (Map.Entry<String, JoshuaServiceInfo> serverEntry : services.entrySet()) 
    {
      long startTime = System.currentTimeMillis();

      ctr++;
      JoshuaServiceInfo service = serverEntry.getValue();  
      logger.error(
        "Launching language translation service " + ctr + " / " + services.size() + " for lang code: " + 
        service.getLanguageCode() + " from path: " + service.getLanguagePackPath() + " to port: " + service.getPort() + "...");  
      
      String configPath = service.getLanguagePackPath() + "/joshua.config";
      convertConfigFileModelPathsToAbsolute(configPath, service.getLanguagePackPath());
      //TODO: read from config
      //not sure why a wildcard won't work here...
      //String classPath = server.getLanguagePackPath() + "/target/joshua-*-jar-with-dependencies.jar";
      String classPath = service.getLanguagePackPath() + "/target/joshua-6.2-SNAPSHOT-jar-with-dependencies.jar";
      //TODO: read from config
      //Under light use, I've seen the memory consumption for the largest language packs hit 9-10GB, so 16 is probably a safe value
      //for them.  Smaller ones may not need as much memory.
      final int memoryGigs = 16;

      OutputStream stdout = new ByteArrayOutputStream();
      OutputStream stderr = new ByteArrayOutputStream();

      String line = 
        "java -mx" + memoryGigs + "g -Dfile.encoding=utf8 -Djava.library.path=./lib -cp " + classPath + 
        " org.apache.joshua.decoder.JoshuaDecoder -c " + configPath + " -v 1 -server-port " + service.getPort() + " -server-type tcp";
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

    return services;
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
