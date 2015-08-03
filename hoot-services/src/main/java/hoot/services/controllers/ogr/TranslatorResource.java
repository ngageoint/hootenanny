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


  
	/**
	 * <NAME>OGR-LTDS Translation Service</NAME>
	 * <DESCRIPTION>
	 * 	WARNING: THIS END POINT WILL BE DEPRECATED SOON
	 * To translate osm element into OGR/LTDS format 2 services are available
	 *  http://localhost:8080/hoot-services/ogr/ltds/translate/{OSM element id}?translation={translation script}
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <translation>
	 * 	relative path of translation script
	 * </translation>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	TDS output
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogr/ltds/translate/-1669795?translation=MGCP.js</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {some OSM XML}
	 *	</INPUT>
	 * <OUTPUT>{"attrs":{"FCODE":"AP030","HCT":"0","UID":"d9c4b1df-066c-4ece-a583-76fec0056b58"},"tableName":"LAP030"}</OUTPUT>
	 * </EXAMPLE>
   * @param id
   * @param translation
   * @param osmXml
   * @return
   */
  @POST
  @Path("/ltds/translate/{id}")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)  
  public Response translateOsm(@PathParam("id") String id, @QueryParam("translation") final String translation,
  		String osmXml)
  {
    String outStr = "unknown";
    
    try
    {
    	PostMethod mpost = new PostMethod("http://localhost:" + currentPort + "/osmtotds");
  		try{

  	    
  			
  	    osmXml = osmXml.replace('"', '\'');
  	    JSONObject requestParams = new JSONObject();
  	    requestParams.put("command","translate");
  	    requestParams.put("uid",id);
  	    requestParams.put("input",osmXml);
  	    requestParams.put("script",homeFolder + "/translations" + translation);
  	    requestParams.put("direction","toogr");

  	    StringRequestEntity requestEntity = new StringRequestEntity(
  	    		requestParams.toJSONString(),
  	        "text/plain",
  	        "UTF-8");
  	    mpost.setRequestEntity(requestEntity);
  	    
  	    mclient.executeMethod(mpost);
  	    String response = mpost.getResponseBodyAsString();
  	    //r = client.execute(post);
  	    
  	    
  	    
  	   // String response = EntityUtils.toString(r.getEntity());
  	    JSONParser par = new JSONParser();
  	    JSONObject transRes = (JSONObject) par.parse(response);
  	    String tdsOSM = transRes.get("output").toString();
  	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
  	    DocumentBuilder builder = factory.newDocumentBuilder();
  	    StringReader strReader = new StringReader(tdsOSM);
  	    InputSource is = new InputSource(strReader);
  	    
  	    Document document = builder.parse(is);
  	    strReader.close();
  	    
  	    String sFCode = null;
  	    JSONObject attrib = new JSONObject();
  	    NodeList nodeList = document.getDocumentElement().getChildNodes();
  	    for (int i = 0; i < nodeList.getLength(); i++) {
  	    	Node node = nodeList.item(i);
  	    	if (node instanceof Element) {
  	    		NodeList childNodes = node.getChildNodes();
  	    		for (int j = 0; j < childNodes.getLength(); j++) {
  	    			Node cNode = childNodes.item(j);
  	    			if (cNode instanceof Element) {
  	    				String k = ((Element) cNode).getAttribute("k");
  	    				String v = ((Element) cNode).getAttribute("v");
  	    				attrib.put(k, v);
  	    				if(k.equalsIgnoreCase("Feature Code"))
  	    				{
  	    					String[] parts = v.split(":");
  	    					sFCode = parts[0].trim();
  	    				}
  	    			}
  	    		}
  	    	}
  	    }
  	    String sFields = getFields(sFCode);
  	    JSONObject ret = new JSONObject();
  	    ret.put("tablenName", "");
  	    ret.put("attrs", attrib);
  	    ret.put("fields", sFields);
  	    outStr = ret.toJSONString();


  		}
  		catch (Exception ee)
  		{
  		  ResourceErrorHandler.handleError(
  			"Failed upload: " + ee.toString(),
  		    Status.INTERNAL_SERVER_ERROR,
  			log);
  		}
  		finally
  		{
  			mpost.releaseConnection();
  		
  		}
    }
    catch (Exception e)
    {
	  ResourceErrorHandler.handleError(
		"Translation error: " + e.toString(),
	    Status.INTERNAL_SERVER_ERROR,
		log);
    }


    return Response.ok(outStr, MediaType.APPLICATION_JSON).build();
  }
 

  //WARNING: THIS END POINT WILL BE DEPRECATED SOON
  protected String getFields(String sFCode) throws Exception
  {
  	String fields = "";
  	GetMethod get = new GetMethod("http://localhost:" + currentPort + "/osmtotds?fcode=" + sFCode);
    try {
        mclient.executeMethod(get);
        // print response to stdout
        fields = get.getResponseBodyAsString();
    } finally {
        // be sure the connection is released back to the connection 
        // manager
        get.releaseConnection();
    }
    return fields;
  }



	/**
	 * <NAME>OGR-LTDS Translation Service Translate TDS to OSM</NAME>
	 * <DESCRIPTION>
	 * WARNING: THIS END POINT WILL BE DEPRECATED SOON
	 * Translate TDS to OSM
	 *  http://localhost:8080/hoot-services/ogr/ltds/translate/tds/{OSM element id}?translation={translation script}
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <translation>
	 * 	relative path of translation script
	 * </translation>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	TDS output
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogr/ltds/translate/-1669795?translation=MGCP.js</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {"attrs":{"FCODE":"AP030","HCT":"0","UID":"d9c4b1df-066c-4ece-a583-76fec0056b58"},"tableName":"LAP030"}
	 *	</INPUT>
	 * <OUTPUT>OSM XML output</OUTPUT>
	 * </EXAMPLE>
   * @param id
   * @param translation
   * @param osmXml
   * @return
   */
  @POST
  @Path("/ltds/translate/tds/{id}")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public Response translateTdsToOsm(@PathParam("id") String id, @QueryParam("translation") final String translation,
  		String osmXml)
  {
    String outStr = "unknown";
    PostMethod mpost = new PostMethod("http://localhost:" + currentPort + "/tdstoosm");
    try
    {

  		try{
  			
  			String ogrxml = osmXml.replace('"', '\'');
  	    
	    	JSONObject requestParams = new JSONObject();
  	    requestParams.put("command","translate");
  	    requestParams.put("uid",id);
  	    requestParams.put("input",ogrxml);
  	    requestParams.put("script",homeFolder + "/translations" + translation);
  	    requestParams.put("direction","toogr");
  	    String postData = requestParams.toJSONString();
  	    StringEntity se = new StringEntity( requestParams.toJSONString());
  	    StringRequestEntity requestEntity = new StringRequestEntity(
  	    		requestParams.toJSONString(),
  	        "text/plain",
  	        "UTF-8");
  	    mpost.setRequestEntity(requestEntity);
  	    
  	    mclient.executeMethod(mpost);
  	    String response = mpost.getResponseBodyAsString();
	      
	      JSONParser par = new JSONParser();
	 	    JSONObject transRes = (JSONObject) par.parse(response);
	 	    String tdsOSM = transRes.get("output").toString();
	 	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	 	    DocumentBuilder builder = factory.newDocumentBuilder();
	 	    StringReader strReader = new StringReader(tdsOSM);
	 	    InputSource is = new InputSource(strReader);
	 	    
	 	    Document document = builder.parse(is);
	 	    strReader.close();
	 	    
	 	    JSONObject attrib = new JSONObject();
	 	    NodeList nodeList = document.getDocumentElement().getChildNodes();
	 	    for (int i = 0; i < nodeList.getLength(); i++) {
	 	    	Node node = nodeList.item(i);
	 	    	if (node instanceof Element) {
	 	    		NodeList childNodes = node.getChildNodes();
	 	    		for (int j = 0; j < childNodes.getLength(); j++) {
	 	    			Node cNode = childNodes.item(j);
	 	    			if (cNode instanceof Element) {
	 	    				String k = ((Element) cNode).getAttribute("k");
	 	    				String v = ((Element) cNode).getAttribute("v");
	 	    				attrib.put(k, v);
	 	    				
	 	    			}
	 	    		}
	 	    	}
	 	    }
	
	 	    JSONObject ret = new JSONObject();
	 	    ret.put("tablenName", "");
	 	    ret.put("attrs", attrib);
	 	    outStr = ret.toJSONString();

  		}
  		catch (Exception ee)
  		{
  		  ResourceErrorHandler.handleError(
  			"Failed upload: " + ee.toString(),
  		    Status.INTERNAL_SERVER_ERROR,
  			log);
  		}
  		finally
  		{
  			log.debug("postJobRequest Closing");
  			mpost.releaseConnection();
  		}
    
    }
    catch (Exception e)
    {
	  ResourceErrorHandler.handleError(
		"Translation error: " + e.toString(),
	    Status.INTERNAL_SERVER_ERROR,
		log);
    }


    return Response.ok(outStr, MediaType.APPLICATION_JSON).build();
  }



}
