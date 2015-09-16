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
package hoot.services.controllers.info;

import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import hoot.services.HootProperties;
import hoot.services.info.BuildInfo;
import hoot.services.info.CoreDetail;
import hoot.services.info.ServicesDetail;
import hoot.services.info.VersionInfo;
import hoot.services.info.ServicesDetail.Property;
import hoot.services.info.ServicesDetail.ServicesResource;
import hoot.services.nativeInterfaces.JobExecutionManager;
import hoot.services.utils.ClassLoaderUtil;
import hoot.services.utils.ResourceErrorHandler;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

/**
 * Endpoint for returning information about Hootenanny core and services
 */
@Path("/about")
public class AboutResource
{
  private static final Logger log = LoggerFactory.getLogger(AboutResource.class);
  
  private static ClassPathXmlApplicationContext appContext = null;
  
  
  public AboutResource()
  {
    appContext = new ClassPathXmlApplicationContext("hoot/spring/CoreServiceContext.xml");
  }
  
  private Properties getBuildInfo()
  {
	Properties buildInfo = null;
	try
	{
      buildInfo = BuildInfo.getInstance();
	}
	catch (Exception e)
	{
	  log.warn(
		"About Resource unable to find the services build.info file.  Web Services version " +
		"information will be unavailable.");
	  buildInfo = new Properties();
	  buildInfo.setProperty("name", "unknown");
	  buildInfo.setProperty("version", "unknown");
	  buildInfo.setProperty("user", "unknown");
	}
	return buildInfo;
  }
  
  /**
   * <NAME>Version Information Service</NAME>
   * <DESCRIPTION>Service method endpoint for retrieving the Hootenanny services version.</DESCRIPTION>
   * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing Hoot service version information
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/info/about/servicesVersionInfo</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
   * <OUTPUT>
   * {
   *   "name": "Hootenanny Web Services",
   *   "version": "0.2.16-53-g7a74f64",
   *   "builtBy": "root"
   * }
   * </OUTPUT>
   * </EXAMPLE>
   * 
   * @return a version string
   * @throws Exception 
   */
  @GET
  @Path("/servicesVersionInfo")
  @Produces(MediaType.APPLICATION_JSON)
  public VersionInfo getServicesVersionInfo()
  {
	VersionInfo versionInfo = null;
    try
    {
      log.debug("Retrieving services version...");
      
      Properties buildInfo = getBuildInfo();
      versionInfo = new VersionInfo();
      versionInfo.setName(buildInfo.getProperty("name"));
      versionInfo.setVersion(buildInfo.getProperty("version"));
      versionInfo.setBuiltBy(buildInfo.getProperty("user"));
      
      log.debug("Returning response: " + versionInfo.toString() + " ...");
    }
    catch (Exception e) 
    {
      ResourceErrorHandler.handleError(
        "Error retrieving services version info: " + e.getMessage(), 
        Status.INTERNAL_SERVER_ERROR, 
        log);
    }
    
	return versionInfo;
  }
  

  /**
   * <NAME>Service Version Detail</NAME>
   * <DESCRIPTION>Service method endpoint for retrieving detailed information about the Hootenanny Services environment.</DESCRIPTION>
   * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	JSON Array containing Hoot service configuration detail
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/info/about/servicesVersionDetail</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
   * <OUTPUT>
	 *	{
	 *   "properties":
	 *   [
	 *	       {
	 *	           "name": "BasemapRasterExtensions",
	 *	           "value": "png,tif"
	 *	       },
	 *	       {
	 *	           "name": "ErrorLogPath",
	 *	           "value": "/var/log/tomcat6/catalina.out"
	 *	       },
	 *	       {
	 *	           "name": "wfsStoreDb",
	 *		           "value": "wfsstoredb"
	 *	       },  
	 *					.....
	 *   ] 
   * </OUTPUT>
   * </EXAMPLE>
   * 
   * @return a ServicesDetail object
   */

  @GET
  @Path("/servicesVersionDetail")
  @Produces(MediaType.APPLICATION_JSON)
  public ServicesDetail getServicesVersionDetail()
  {
	ServicesDetail servicesDetail = null;
    try
    {
      log.debug("Retrieving services version...");
      
      servicesDetail = new ServicesDetail();
      
      List<Property> properties = new ArrayList<Property>();
	  Properties props = HootProperties.getInstance();
	  @SuppressWarnings("rawtypes")
	  Iterator it = props.entrySet().iterator();
	  while (it.hasNext()) 
	  {
		Property prop = new Property();
	    @SuppressWarnings("rawtypes")
		Map.Entry parsedProp = (Map.Entry)it.next();
	    prop.setName((String)parsedProp.getKey());
	    prop.setValue((String)parsedProp.getValue());
	    properties.add(prop);
	  }
      servicesDetail.setProperties(properties.toArray(new Property[]{}));
      
      servicesDetail.setClassPath(System.getProperty("java.class.path", null));
      
      List<ServicesResource> resources = new ArrayList<ServicesResource>();
      
	  for (String url : ClassLoaderUtil.getMostJars())
	  {
		ServicesResource servicesResource = new ServicesResource();
		servicesResource.setType("jar");
		servicesResource.setUrl(url);
		resources.add(servicesResource);
	  }
	  
	  ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
	  Enumeration<URL> urls = classLoader.getResources("");
	  while (urls.hasMoreElements())
	  {
	    URL resource = urls.nextElement();
	    ServicesResource servicesResource = new ServicesResource();
	    servicesResource.setType(resource.getProtocol());
		servicesResource.setUrl(resource.toString());
		resources.add(servicesResource);
	  }
	  
	  servicesDetail.setResources(resources.toArray(new ServicesResource[]{}));
      
      log.debug("Returning response: " + " ...");
    }
    catch (Exception e) 
    {
      ResourceErrorHandler.handleError(
        "Error retrieving services version info: " + e.getMessage(), 
        Status.INTERNAL_SERVER_ERROR, 
        log);
    }
    
	return servicesDetail;
  }
  
  @SuppressWarnings("unchecked")
  private String getCoreInfo(boolean getDetailed) throws Exception
  {
	JSONObject command = new JSONObject();
	command.put("exectype", "hoot");
	command.put("exec", "version");
	JSONArray params = new JSONArray();
	if (getDetailed)
	{
	  JSONObject param = new JSONObject();
	  param.put("", "--debug");
	  params.add(param);
	}
	command.put("params", params);
	command.put("caller", this.getClass().getSimpleName());
	
	return 
	  ((JobExecutionManager)appContext.getBean(
	    "jobExecutionManagerNative")).execWithResult(command).get("stdout").toString();
  }
 

  /**
   * <NAME>Hootenanny Core Version Information</NAME>
   * <DESCRIPTION>Service method endpoint for retrieving the Hootenanny core (command line application) version.</DESCRIPTION>
   * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing Hoot core version information
	 * </OUTPUT>
	 * <EXAMPLE>
	 * <URL>http://localhost:8080/hoot-services/info/about/coreVersionInfo</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>	
   * <OUTPUT>
   * {
   * 	"name": "Hootenanny Core",
   * 	"version": "0.2.16-53-g7a74f64",
   * 	"builtBy": "root"
   * }
   * </OUTPUT>
   * </EXAMPLE>
   * 
   * @return a version info object
   */

  @GET
  @Path("/coreVersionInfo")
  @Produces(MediaType.APPLICATION_JSON)
  public VersionInfo getCoreVersionInfo()
  {
	VersionInfo versionInfo = null;
    try
    {
      log.debug("Retrieving services version...");
		  	
      final String versionStr = getCoreInfo(false);
      final String[] versionInfoParts = versionStr.split(" ");
      versionInfo = new VersionInfo();
      versionInfo.setName("Hootenanny Core");
      versionInfo.setVersion(versionInfoParts[1]);
      versionInfo.setBuiltBy(versionInfoParts[4]);
      
      log.debug("Returning response: " + versionInfo + " ...");
    }
    catch (Exception e) 
    {
      ResourceErrorHandler.handleError(
        "Error retrieving core version info: " + e.getMessage(), 
        Status.INTERNAL_SERVER_ERROR, 
        log);
    }
    
	return versionInfo;
  }
  


  /**
   * <NAME>Hootenanny Core Version Detail</NAME>
   * <DESCRIPTION>Service method endpoint for retrieving detailed environment information about the Hootenanny core (command line application).</DESCRIPTION>
   * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	JSON Array containing Hoot core version detail
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/info/about/coreVersionDetail</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
   * <OUTPUT>
   * {
   *    "environmentInfo":
   *    [
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(89) GEOS Version: 3.3.8",
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(90) GDAL Version: 1.10.1",
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(91) GLPK Version: 4.40",
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(92) Qt Version: 4.8.6",
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(96) Boost Version: 1.41.0",
   *        "15:07:33.857 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(101) CppUnit Version: 1.12.1",
   *        "15:07:33.859 DEBUG src/main/cpp/hoot/core/cmd/VersionCmd.cpp(104) Memory usage, vm: 348.8MB rss: 25.41MB"
   *    ]
   * }
   * </OUTPUT>
   * </EXAMPLE>
   * 
   * @return a version string
   */

  @GET
  @Path("/coreVersionDetail")
  @Produces(MediaType.APPLICATION_JSON)
  public CoreDetail getCoreVersionDetail()
  {
	CoreDetail coreDetail = null;
    try
    {
      log.debug("Retrieving services version...");
		  	
      String versionStr = getCoreInfo(true);
      //get rid of the first line that has the hoot core version info in it; call coreVersionInfo 
      //for that
      final String[] versionInfoParts = versionStr.split("\n");
      List<String> versionInfoPartsModified = new ArrayList<String>();
      versionStr = "";
      for (int i = 1; i < versionInfoParts.length; i++)
      {
    	versionInfoPartsModified.add(versionInfoParts[i]);
      }
      coreDetail = new CoreDetail();
      coreDetail.setEnvironmentInfo(versionInfoPartsModified.toArray(new String[]{}));
      
      log.debug("Returning response: " + coreDetail.toString() + " ...");
    }
    catch (Exception e) 
    {
      ResourceErrorHandler.handleError(
        "Error retrieving core version info: " + e.getMessage(), 
        Status.INTERNAL_SERVER_ERROR, 
        log);
    }
    
	return coreDetail;
  }
}
