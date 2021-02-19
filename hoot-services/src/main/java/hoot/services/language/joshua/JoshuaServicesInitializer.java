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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2021 Maxar (http://www.maxar.com/)
 */

package hoot.services.language.joshua;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;

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
import org.apache.commons.exec.ExecuteException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Launches specified Joshua services.  The language packs for the services are assumed to have 
   already been downloaded and extracted.  Joshua services are launched as external server processes, 
   one per language pack.  This is necessary due to the fact that Joshua keeps some configuration 
   information global, which can't be shared across multiple languages, requiring separate processes.  
   Joshua services are quite memory hungry and take awhile to initialize.
 * 
 * @article{post2015joshua,
    Author = {Post, Matt and Cao, Yuan and Kumar, Gaurav},
    Journal = {The Prague Bulletin of Mathematical Linguistics},
    Title = {Joshua 6: A phrase-based and hierarchical statistical machine translation system},
    Year = {2015} }
 */
public class JoshuaServicesInitializer 
{
  private static final Logger logger = LoggerFactory.getLogger(JoshuaServicesInitializer.class);

  //which services we have running; one for each language
  private Map<String, JoshuaServiceInfo> services = null;

  public JoshuaServicesInitializer() {}

  public Map<String, JoshuaServiceInfo> getServices() { return services; }

  public void init() throws Exception
  {
    logger.info("Initializing Joshua services...");

    //read the services configuration file
    services = readServicesConfig();

    //launch the services asynchronously as external processes that listen on a port, one by one, 
    //one for each language
    int ctr = 0;
    for (Map.Entry<String, JoshuaServiceInfo> serverEntry : services.entrySet()) 
    {
      JoshuaServiceInfo serviceInfo = serverEntry.getValue();
      ctr++;
      logger.info(
        "Launching language translation service " + ctr + " / " + services.size() + " for lang code: " + 
        serviceInfo.getLanguageCode() + " from path: " + serviceInfo.getLanguagePackPath() + 
        " to port: " + serviceInfo.getPort() + "...");
      String configPath = serviceInfo.getLanguagePackPath() + "/joshua.config";
      convertConfigFileModelPathsToAbsolute(configPath, serviceInfo.getLanguagePackPath());
      launchService(serviceInfo, configPath);
    }
    logger.info(
      "Finished launching Joshua translation services for " + ctr + " language packs.  The services " + 
      "may take up to several minutes to finish initializing.");
  }

  private static CommandLine getProcessExecCommand(JoshuaServiceInfo serviceInfo, String configPath) 
    throws IOException
  {
    String classPath = serviceInfo.getLanguagePackPath() + "/target/" + JOSHUA_LIBRARY;
    String line = 
      "java -mx" + JOSHUA_MAX_MEMORY + "g -Dfile.encoding=utf8 -Djava.library.path=./lib -cp " + 
      classPath + " org.apache.joshua.decoder.JoshuaDecoder -c " + configPath + 
      " -v 1 -server-port " + serviceInfo.getPort() + " -server-type tcp";
    logger.trace("command: " + line);
    return CommandLine.parse(line);
  }

  private static void launchService(JoshuaServiceInfo serviceInfo, String configPath) throws IOException, 
    ExecuteException
  {   
    //The service is set up to write stdout/stderr and will auto destroy itself when the web 
    //server shuts down.
    OutputStream stdout = new ByteArrayOutputStream();
    OutputStream stderr = new ByteArrayOutputStream();
    Executor executor = new DaemonExecutor();
    executor.setWatchdog(new ExecuteWatchdog(ExecuteWatchdog.INFINITE_TIMEOUT));
    executor.setStreamHandler(new PumpStreamHandler(stdout, stderr));
    executor.setProcessDestroyer(new ShutdownHookProcessDestroyer());

    executor.execute(getProcessExecCommand(serviceInfo, configPath), new DefaultExecuteResultHandler());

    String stdOutStr = stdout.toString();
    String stdErrStr = stderr.toString();
    if (!stdOutStr.isEmpty())
    {
      logger.debug("JoshuaServicesInitializer stdout: " + stdOutStr);
    }
    if (!stdErrStr.isEmpty())
    {
      logger.error("JoshuaServicesInitializer stderr: " + stdErrStr);
    }
  }

  private static Map<String, JoshuaServiceInfo> readServicesConfig() throws Exception
  {
    JoshuaServiceInfo[] servicesTmp = null;
    InputStream configStrm = null;
    try
    {
      logger.debug("Reading Joshua services config...");
      configStrm = 
        JoshuaServicesInitializer.class.getClassLoader().getResourceAsStream(
          "language-translation/joshuaServices");
      servicesTmp = JoshuaServicesConfigReader.readConfig(configStrm);
      logger.debug("Read " + servicesTmp.length + " services from config.");
    }
    finally 
    {  
      if (configStrm != null)
      {
        configStrm.close();
      }
    }
    Map<String, JoshuaServiceInfo> services = new HashMap<String, JoshuaServiceInfo>();
    for (JoshuaServiceInfo service : servicesTmp)
    {
      services.put(service.getLanguageCode(), service);
    }
    logger.trace("services size: " + services.size());

    return services;
  }

  private static void convertConfigFileModelPathsToAbsolute(String configPath, String langPackPath) 
    throws IOException
  {
    File configFile = new File(configPath);
    String configContent = 
      FileUtils.readFileToString(configFile, JoshuaLanguageTranslator.ENCODING);
    //change all model relative paths to absolute in the config
    configContent = configContent.replaceAll(" model/", " " + langPackPath + "/model/");
    FileUtils.writeStringToFile(configFile, configContent, JoshuaLanguageTranslator.ENCODING);
  }
}
