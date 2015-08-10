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
package hoot.services.controllers.ogr;

import hoot.services.HootProperties;
import hoot.services.nodeJs.ServerControllerBase;
import hoot.services.utils.ResourceErrorHandler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.CountDownLatch;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.commons.fileupload.util.Streams;
import org.apache.commons.httpclient.MultiThreadedHttpConnectionManager;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.StringRequestEntity;
import org.apache.commons.httpclient.params.HttpConnectionManagerParams;
import org.apache.commons.io.IOUtils;
import org.apache.http.entity.StringEntity;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;


@Path("")
public class TranslatorResource extends ServerControllerBase{



  private static final Logger log = LoggerFactory.getLogger(TranslatorResource.class);


  private static String homeFolder = null;
  private static String translationServerPort = null;
  private static String translationServerThreadCount = null;
  private static String translationServerScript = null;
  
  private static final Object procLock = new Object();
  private static final Object portLock = new Object();
  
  private static String currentPort = null;
  private static Process transProc = null;


  private static MultiThreadedHttpConnectionManager connectionManager = 
  		new MultiThreadedHttpConnectionManager();
	private static org.apache.commons.httpclient.HttpClient mclient = new org.apache.commons.httpclient.HttpClient(connectionManager);;
	
	
  public TranslatorResource()
  {

    if(homeFolder ==  null){
	    try {
	    	homeFolder = HootProperties.getProperty("homeFolder");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }

    if(translationServerPort ==  null){
	    try {
	    	translationServerPort = HootProperties.getProperty("translationServerPort");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }

    if(translationServerThreadCount ==  null){
	    try {
	    	translationServerThreadCount = HootProperties.getProperty("translationServerThreadCount");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }

    if(translationServerScript ==  null){
	    try {
	    	translationServerScript = HootProperties.getProperty("translationServerScript");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }
    
    // Use this with synchronized if needing configuration
    /*
    if(mclient == null)
    {
	    HttpConnectionManagerParams params = new HttpConnectionManagerParams();
	    params.setDefaultMaxConnectionsPerHost(2);
	    params.setMaxTotalConnections(3);
	
	    connectionManager.setParams(params);
	    mclient = new org.apache.commons.httpclient.HttpClient(connectionManager);
    }*/
  }


  /**
	 * <NAME>Translation Service Node Server Start</NAME>
	 * <DESCRIPTION>
	 * This rest end point start translation node js server. It first searches for process with "TranslationServer.js"
	 * and kills them to clean out previously running processes. It also combines stdout and stderr from node server output
	 * and then prints out to service stdout. 
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <port>
	 * 	The port node server should be listening. If none supplied then it will use default port specified in hoot-services.conf
	 * 	translationServerPort variable.
	 * </port>
	 * <threadcount>
	 * 	The number of processes used by translation server. If 0 then it will use numbers of CPU. If none specified then the default
	 * 	value in hoot-services.conf translationServerThreadCount gets used.
	 * </threadcount>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing port and threadcount
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogr/translationserver/start?port=8094&threadcount=0</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>{"port":"8094", "threadcount","0"}</OUTPUT>
	 * </EXAMPLE>
   * @param port
   * @param nThread
   * @return
   */
  @GET
  @Path("/translationserver/start")
  @Produces(MediaType.TEXT_PLAIN)
  public Response startTranslationService(@QueryParam("port") final String port, 
  		@QueryParam("threadcount") final String nThread) {

  	// set default default port and threadcount
  	String currPort = translationServerPort;
  	String currThreadCnt = translationServerThreadCount;
		try
		{
			// Make sure to wipe out previosuly running servers.
			stopServer(homeFolder + "/scripts/" + translationServerScript);
			
			// override with user specified values if available
			if(port != null)
			{
				try
				{
					Integer.parseInt(port);
					currPort = port;
				}
				catch (Exception pe)
				{
					log.warn("Supplied port number is invalid. Using " + currPort);
				}
				
			}
			
		// override with user specified values if available
			if(nThread != null)
			{
				try
				{
					Integer.parseInt(nThread);
					currThreadCnt = nThread;
				}
				catch (Exception pe)
				{
					log.warn("Supplied thread count is invalid. Using " + currThreadCnt);
				}
				
			}
			
			// Probably an overkill but just in-case using synch lock
			synchronized(portLock)
			{
				currentPort = currPort;
			}

			synchronized(procLock)
			{
				transProc = startServer(currPort, currThreadCnt, homeFolder + "/scripts/" + translationServerScript);
			}

		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error starting translation service request: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}
  	
  	JSONObject res = new JSONObject();
		res.put("port", currPort);
		res.put("threadcount", currThreadCnt);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
  }

 
  
  
  
  /**
	 * <NAME>Translation Service Node Server Stop</NAME>
	 * <DESCRIPTION>
	 *  Destroys all translation server process where it effectively shutting them down.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing state
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogr/translationserver/stop</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>{"isRunning":"false"}</OUTPUT>
	 * </EXAMPLE>
   * @return
   */
  @GET
  @Path("/translationserver/stop")
  @Produces(MediaType.TEXT_PLAIN)
  public Response stopTranslationService() {
  	// This also gets called automatically from HootServletContext when service exits but
  	// should not be reliable since there are many path where it will not be invoked.
		try
		{  
			stopServer(homeFolder + "/scripts/" + translationServerScript);
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error starting translation service request: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}
  	
  	JSONObject res = new JSONObject();
		res.put("isRunning", "false");
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
  }
  
  
  
  /**
	 * <NAME>Translation Service Node Server status</NAME>
	 * <DESCRIPTION>
	 *  Gets current status of translation server.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing state and port it is running
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogr/translationserver/status</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>{"isRunning":"true","port":"8094"}</OUTPUT>
	 * </EXAMPLE>
   * @return
   */  
  @GET
  @Path("/translationserver/status")
  @Produces(MediaType.TEXT_PLAIN)
  public Response isTranslationServiceRunning() {
  	boolean isRunning = false;
  	
  	
		try
		{
			isRunning = getStatus(transProc);
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error starting translation service request: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}
  	
  	JSONObject res = new JSONObject();
		res.put("isRunning", isRunning);
		res.put("port", currentPort);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
  }


 
}
