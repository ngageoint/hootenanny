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

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.Map;
import java.util.Properties;

/**
 * Services configuration file
 */
public class HootProperties
{
  private static Properties properties;
  private static Properties localProperties;

  private HootProperties()
  {
  }

  /**
   * Returns the properties read from the services configuration file
   *
   * @return a set of properties
   * @throws IOException if unable to read from the services configuration file
   */
  public synchronized static Properties getInstance() throws IOException
  {
    if (properties == null)
    {
      properties = new Properties();
      InputStream propsStrm = null;
      try
      {
      	propsStrm = 
      	  HootProperties.class.getClassLoader().getResourceAsStream("conf/hoot-services.conf");
        properties.load(propsStrm);
      }
      finally
      {
      	propsStrm.close();
      }
    }

    // This block of code checks for the local.conf and if there is one then it overrides the
    // properties.
    if (localProperties == null)
    {
    	localProperties = new Properties();
    	InputStream inRes = null;
    	try
    	{
    		inRes = HootProperties.class.getClassLoader().getResourceAsStream("conf/local.conf");
      	if (inRes != null)
      	{
  	    	localProperties.load(inRes);

  	    	Enumeration<?> enumeration = localProperties.propertyNames();
  	      while (enumeration.hasMoreElements()) {
  	          String key = (String) enumeration.nextElement();
  	          String value = localProperties.getProperty(key);
  	          properties.setProperty(key, value);
  	      }
      	}
    	}
    	finally
    	{
    		if (inRes != null)
    		{
    			inRes.close();
    		}
    	}
    }
    return properties;
  }


  /**
   * Helper function to add property reference.
   * It looks for property from the local properties and if it does not find it then uses environmental variable.
   *
   * @param prop
   * @return
   * @throws Exception
   */
  public synchronized static String getProperty(String prop) throws IOException
  {
  	String sFullProp = "";
  	String sProp = HootProperties.getInstance().getProperty(prop, "");
  	String[] parts = sProp.split("\\$");
  	if(parts.length > 1)
  	{
  		for(int i=0; i<parts.length; i++)
    	{
    		String part = parts[i];
    		if(part.startsWith("("))
    		{
    			int iClosure = part.indexOf(")");
    			String token = part.substring(1, iClosure);
    			// check to see if the local prop exists
    			String sToken = HootProperties.getInstance().getProperty(token, "");
    			// nested call of property so resolve it.
    			if(sToken.indexOf("$") > -1)
    			{
    				sToken = HootProperties.getProperty(token);
    			}

    			if(sToken == null  || sToken.length() == 0)
    			{
    				// if not get it from env
    				Map<String, String> env = System.getenv();
      			sToken = env.get(token);
    			}
    			if(sToken != null && sToken.length() > 0)
    			{
    				sFullProp += sToken;
    			}

    			if( iClosure + 1 < part.length())
    			{
    				sFullProp += part.substring(iClosure + 1);
    			}

    		}
    		else
    		{
    			sFullProp += part;
    		}
    	}
  	}
  	else
  	{
  		sFullProp = sProp;
  	}


    return sFullProp;
  }


  /**
   * Set the properties manually
   *
   * This is useful for testing.
   *
   * @param props a set of Hootenanny properties
   */
  public static void setProperties(Properties props) { properties = props; }

  /**
   * Determines reasonable default properties in case the server config file is missing
   *
   * @param key property key
   * @return a property value
   */
  public static String getDefault(String key)
  {
    if (key.equals("osmVersion"))
    {
      return "0.6";
    }
    else if (key.equals("generator"))
    {
      return "Hootenanny server";
    }
    else if (key.equals("copyright"))
    {
      return "?";
    }
    else if (key.equals("attribution"))
    {
      return "?";
    }
    else if (key.equals("license"))
    {
      return "?";
    }
    else if (key.equals("maxQueryAreaDegrees"))
    {
      return "0.25";
    }
    else if (key.equals("maxQueryNodes"))
    {
      return "50000";
    }
    else if (key.equals("mapQueryDimensions"))
    {
      return "2";
    }
    else if (key.equals("maximumChangesetElements"))
    {
      return "50000";
    }
    else if (key.equals("maximumWayNodes"))
    {
      return "2000";
    }
    else if (key.equals("changesetBoundsExpansionFactorDeegrees"))
    {
      return "0.1";
    }
    else if (key.equals("changesetIdleTimeoutMinutes"))
    {
      return "60";
    }
    else if (key.equals("changesetMaxOpenTimeHours"))
    {
      return "24";
    }
    else if (key.equals("debugSql"))
    {
      return "false";
    }
    else if (key.equals("testChangesetAutoClose"))
    {
      return "false";
    }
    else if (key.equals("hootCoreServicesDatabaseWriterCompatibility"))
    {
      return "false";
    }
    else if (key.equals("coreScriptPath"))
    {
      return "/project/hoot/scripts";
    }
    else if (key.equals("coreScriptOutputPath"))
    {
      return "";
    }
    else if (key.equals("coreJobServerUrl"))
    {
      return "http://localhost:8080";
    }
    else if (key.equals("ETLMakefile"))
    {
      return "makeetl";
    }
    else if (key.equals("grizzlyPort"))
    {
      return "9998";
    }
    else if (key.equals("maxRecordBatchSize"))
    {
      return "5000";
    }
    else if (key.equals("reviewGetMaxReviewSize"))
    {
      return "50000";
    }
    else if (key.equals("reviewPrepareOverwriteExistingDataDefault"))
    {
      return "false";
    }
    else if (key.equals("reviewGetReviewScoreThresholdMinimumDefault"))
    {
      return "0.5";
    }
    else if (key.equals("reviewGetGeospatialBoundsDefault"))
    {
      return "-180,-90,180,90";
    }
    else if (key.equals("testJobStatusPollerTimeout"))
    {
      return "250";
    }
    else if (key.equals("servicesTestClearEntireDb"))
    {
      return "false";
    }
    else if (key.equals("logPropsDynamicChangeScanInterval"))
    {
      return "1";
    }
    else if (key.equals("autoScanForLogPropsChanges"))
    {
      return "true";
    }
    else if (key.equals("reviewPrepareCleanup"))
    {
      return "true";
    }
    else if (key.equals("reviewTags"))
    {
      return "hoot:review:uuid;hoot:review:score;hoot:review:note;hoot:review:needs";
    }
    else if (key.equals("maxWarningsDisplayed"))
    {
      return "10";
    }
    return null;
  }
}
