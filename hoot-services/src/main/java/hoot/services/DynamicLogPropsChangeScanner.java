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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services;

import java.net.URL;

import javax.servlet.http.HttpServlet;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.apache.log4j.xml.DOMConfigurator;

/**
 * Allows for dynamic log level changes while the server is still running.  This is meant to work 
 * with log4j 1.x (I think this can be accomplished in log4j 2.x with a simple configuration change 
 * instead).  Also, it possibly shouldn't be run in an actual J2EE container for stability reasons 
 * (see configureAndWatch documentation for details).
 */
@SuppressWarnings("serial")
public class DynamicLogPropsChangeScanner extends HttpServlet
{
	private static final Logger log = Logger.getLogger ( DynamicLogPropsChangeScanner.class );
	
	public void init()
  {
		try
		{
			if (Boolean.parseBoolean(
	          HootProperties.getInstance().getProperty(
		          "autoScanForLogPropsChanges",
		          HootProperties.getDefault("autoScanForLogPropsChanges"))))
			{
				configureLogging(ConfigFileType.xml);
			}
		}
		catch (Exception e)
		{
			log.error("Error configuring logging properties change scanning: " + e.getMessage());
		}
  }
  
  private static enum ConfigFileType 
  {
    xml, 
    properties
  }

	private void configureLogging(ConfigFileType configFileType) throws Exception
	{
		log.info(
		  "Configuring dynamic log properties for file type: " + configFileType.toString() + " ...");
		final URL configFile = getClass().getResource("/log4j." + configFileType);
		if (configFile != null && "file".equals(configFile.getProtocol()))
		{
			final int scanDelayMinutes = 
				Integer.parseInt(
	        HootProperties.getInstance().getProperty(
	          "logPropsDynamicChangeScanInterval",
	          HootProperties.getDefault("logPropsDynamicChangeScanInterval")));
			if (scanDelayMinutes >= 1)
			{
				final long scanDelayMillis = scanDelayMinutes * 60 * 1000;
				switch (configFileType)
				{
					case xml:
						
						DOMConfigurator.configureAndWatch(configFile.getPath(), scanDelayMillis);
						
						break;
						
					case properties:
						
						PropertyConfigurator.configureAndWatch(configFile.getPath(), scanDelayMillis);
						
						break;
						
					default:
						
						throw new Exception("Invalid file type.");
				}
				
				log.info("Configured dynamic log properties for file type: " + configFileType.toString());
			}
			else
			{
				log.error(
				  "Invalid log properties file change scan interval: " + String.valueOf(scanDelayMinutes));
			}
		}
	}
}
