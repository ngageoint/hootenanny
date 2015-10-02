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
package hoot.services.controllers.ingest;

import java.io.File;
import java.io.FileFilter;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.UUID;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.filefilter.WildcardFileFilter;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.controllers.job.JobControllerBase;
import hoot.services.utils.ResourceErrorHandler;

@Path("/basemap")
public class BasemapResource extends JobControllerBase {
	private static final Logger log = LoggerFactory.getLogger(BasemapResource.class);
	protected static String _tileServerPath = null;
	protected static String _ingestStagingPath = null;
	private String homeFolder = null;

	protected static Map<String, String> _basemapRasterExt = null;

	public BasemapResource()
	{
		try
		{
			if (processScriptName ==  null)
			{
				processScriptName = HootProperties.getProperty("BasemapRasterToTiles");
			}

			if (_basemapRasterExt ==  null)
			{
				_basemapRasterExt = new HashMap<String, String>();
				String extStr = HootProperties.getProperty("BasemapRasterExtensions");
				String[] extList = extStr.toLowerCase().split(",");
				for(String ext : extList)
				{
					_basemapRasterExt.put(ext, ext);
				}
			}

			if(_tileServerPath == null)
			{
				_tileServerPath = HootProperties.getProperty("tileServerPath");

			}

			if(_ingestStagingPath == null)
			{
				_ingestStagingPath = HootProperties.getProperty("ingestStagingPath");
			}
			homeFolder = HootProperties.getProperty("homeFolder");


		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}



	/**
	 * <NAME>Basemap Upload Service</NAME>
	 * <DESCRIPTION>Upload file and create TMS tiles.</DESCRIPTION>
	 * <PARAMETERS>
	 * <INPUT_NAME>
	 * 	Name of basemap;
	 * </INPUT_NAME>
	 * <PROJECTION>
	 * 	Projection to apply. defaults to EPSG:4326
	 * </PROJECTION>
	 * </PARAMETERS>
	 * <INPUT>multipart data</INPUT>
	 * <OUTPUT>
	 * 	JSON Array containing JSON of job id
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ingest/basemap/upload?INPUT_NAME=MyBasemap</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
	 * <OUTPUT>[{"name":"MyBasemap","jobid":"1234567"}]</OUTPUT>
	 * </EXAMPLE>
	 * @param inputName
	 * @param projection
	 * @param request
	 * @return
	 */
	@POST
	@Path("/upload")
	@Produces(MediaType.TEXT_PLAIN)
	public Response processUpload(
			@QueryParam("INPUT_NAME") final String inputName,
			@QueryParam("PROJECTION") final String projection,
			@Context HttpServletRequest request)
	{
		String groupId = UUID.randomUUID().toString();
		JSONArray jobsArr = new JSONArray();
		try
		{
			File uploadDir = new File(homeFolder + "/upload/");
			uploadDir.mkdir();

			String repFolderPath = homeFolder + "/upload/" + groupId;
			File dir = new File(repFolderPath);
			dir.mkdir();

			if(!ServletFileUpload.isMultipartContent(request))
			{
				throw new ServletException("Content type is not multipart/form-data");
			}
			DiskFileItemFactory fileFactory = new DiskFileItemFactory();
			File filesDir = new File(repFolderPath);
			fileFactory.setRepository(filesDir);
			ServletFileUpload uploader = new ServletFileUpload(fileFactory);

			Map<String,String> uploadedFiles = new HashMap<String, String>();
			Map<String,String> uploadedFilesPaths = new HashMap<String, String>();
			List<FileItem> fileItemsList = uploader.parseRequest(request);
			Iterator<FileItem> fileItemsIterator = fileItemsList.iterator();

			while(fileItemsIterator.hasNext()){
				FileItem fileItem = fileItemsIterator.next();
				String fileName = fileItem.getName();

				String uploadedPath = repFolderPath + "/" + fileName;
				File file = new File(uploadedPath);
				fileItem.write(file);

				String[] nameParts = fileName.split("\\.");
				if(nameParts.length > 1){
					String extension = nameParts[nameParts.length-1].toLowerCase();
					String filename = nameParts[0];


					if(_basemapRasterExt.get(extension) != null)
					{
						uploadedFiles.put(filename, extension);
						uploadedFilesPaths.put(filename, fileName);
						log.debug("Saving uploaded:" + filename);
					}

				}

			}

			Iterator it = uploadedFiles.entrySet().iterator();
	    while (it.hasNext()) {
	    		String jobId = UUID.randomUUID().toString();
	        Map.Entry pairs = (Map.Entry)it.next();
	        String fName = pairs.getKey().toString();
	        pairs.getValue().toString();

	        log.debug("Preparing Basemap Ingest for :" + fName);
	        String inputFileName = "";
	        String bmName = inputName;
	        bmName = fName;


	  			inputFileName = uploadedFilesPaths.get(fName);

	  			try
	  			{
	  				JSONArray commandArgs = new JSONArray();
	  				JSONObject arg = new JSONObject();
	  				arg.put("INPUT", "upload/" + groupId + "/" + inputFileName);
	  				commandArgs.add(arg);


	  				arg = new JSONObject();
	  				arg.put("INPUT_NAME", bmName);
	  				commandArgs.add(arg);

	  				arg = new JSONObject();
	  				arg.put("RASTER_OUTPUT_DIR", _tileServerPath + "/BASEMAP");
	  				commandArgs.add(arg);

	  				arg = new JSONObject();
	  				if(projection != null && projection.length() > 0)
	  				{
		  				arg.put("PROJECTION", projection);
	  				}
	  				else
	  				{
	  					arg.put("PROJECTION", "auto");
	  				}
	  				commandArgs.add(arg);

	  				arg = new JSONObject();
	  				arg.put("JOB_PROCESSOR_DIR", _ingestStagingPath + "/BASEMAP");
	  				commandArgs.add(arg);

	  				String argStr = createBashPostBody(commandArgs);
	  				postJobRquest( jobId,  argStr);

	  				JSONObject res = new JSONObject();
	  				res.put("jobid", jobId);
	  				res.put("name", bmName);

	  				jobsArr.add(res);

	  			}
	  			catch (Exception ex)
	  			{
	  			  ResourceErrorHandler.handleError(
	  			    "Error processing upload: " + ex.getMessage(),
			        Status.INTERNAL_SERVER_ERROR,
			        log);
	  			}


	    }


		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	  		  "Error processing upload: " + ex.getMessage(),
			  Status.INTERNAL_SERVER_ERROR,
			  log);
		}

		return Response.ok(jobsArr.toJSONString(), MediaType.APPLICATION_JSON).build();
	}

	/**
  * <NAME>Get Basemaps List Service</NAME>
  * <DESCRIPTION>Service method endpoint for retrieving the uploaded basemaps list.</DESCRIPTION>
  * <PARAMETERS></PARAMETERS>
	* <OUTPUT>
	* 	JSON Array containing basemap information
	* </OUTPUT>
	* <EXAMPLE>
	* 	<URL>http://localhost:8093/hoot-services/ingest/basemap/getlist</URL>
	* 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	* <INPUT>None</INPUT>
  * <OUTPUT>
	* [{"status":"enabled","name":"TestMap","jobid":"123-456-789"},{"status":"enabled","name":"TestMap2","jobid":"123-456-789"}]
	* </OUTPUT>
  * </EXAMPLE>
  * 
  */
	@GET
	@Path("/getlist")
	@Produces(MediaType.TEXT_PLAIN)
	public Response getBasemapList()
	{
		JSONArray retList = new JSONArray();
		Map<String, JSONObject> sortedScripts = new TreeMap<String, JSONObject>();
		JSONArray filesList = new JSONArray();

		try
		{
			filesList = _getBasemapList();
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
  		    "Error getting base map list: " + ex.getMessage(),
		    Status.INTERNAL_SERVER_ERROR,
		    log);
		}

		 // sort the list
    for (Object o : filesList)
    {
    	JSONObject cO = (JSONObject)o;
    	String sName = cO.get("name").toString();
    	sortedScripts.put(sName.toUpperCase(), cO);
    }

    retList.addAll(sortedScripts.values());

		return Response.ok(retList.toString(), MediaType.TEXT_PLAIN).build();
	}

	protected JSONArray _getBasemapList() throws Exception
	{
		JSONArray filesList = new JSONArray();
		File basmapDir = new File(_ingestStagingPath + "/BASEMAP");
		if(basmapDir.exists())
		{

			String[] exts = new String[4];
			exts[0] = "processing";
			exts[1] = "enabled";
			exts[2] = "disabled";
			exts[3] = "failed";
			List<File> files = (List<File>) FileUtils.listFiles(basmapDir, exts, false);

			for(int i=0; i<files.size(); i++)
			{
				File f = files.get(i);
				if(f.isFile())
				{
					String basemapName = f.getName();
					String ext = FilenameUtils.getExtension(basemapName);
					String name = FilenameUtils.getBaseName(basemapName);

					String meta = FileUtils.readFileToString(f, Charset.defaultCharset());
					JSONParser parser = new JSONParser();
					JSONObject jsonMeta = (JSONObject) parser.parse(meta);
					String jobId = jsonMeta.get("jobid").toString();

					JSONObject oBaseMap = new JSONObject();
					oBaseMap.put("name", name);
					oBaseMap.put("status", ext);
					oBaseMap.put("jobid", jobId);
					filesList.add(oBaseMap);
				}

			}
		}
		return filesList;
	}


	protected void _toggleBaseMap(String bmName, boolean enable) throws Exception
	{
		// See ticket#6760
		// for file path manipulation
		String fileExt = "enabled";
		String targetExt = ".disabled";
		if(enable)
		{
			fileExt = "disabled";
			targetExt = ".enabled";
		}

		
		// We first verify that file exits in the folder first and then try to get the source file
		File sourceFile = hoot.services.utils.FileUtils.getFileFromFolder(_ingestStagingPath + "/BASEMAP/", bmName , fileExt); 
		
		if(sourceFile != null && sourceFile.exists())
		{
			// if the source file exist then just swap the extension
			boolean res = sourceFile.renameTo(new File(_ingestStagingPath + "/BASEMAP/", bmName + targetExt));
			if(res == false)
			{
				throw new Exception("Failed to rename file:" + bmName + fileExt + " to " + bmName + targetExt );
			}
		}
		else
		{
			throw new Exception("Can not enable file:" + bmName + targetExt + ". It does not exist.");
		}
	}

	/**
  * <NAME>Enable Basemap Service</NAME>
  * <DESCRIPTION>Service method endpoint for enabling a basemap.</DESCRIPTION>
  * <PARAMETERS>
  * <NAME>
  * Name of a basemap
  * </NAME>
  * <ENABLE>
  * true/false
  * </ENABLE>
  * </PARAMETERS>
	* <OUTPUT>
	* 	JSON containing enable state
	* </OUTPUT>
	* <EXAMPLE>
	* 	<URL>http://localhost:8093/hoot-services/ingest/basemap/enable?NAME=abc&ENABLE=true</URL>
	* 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	* <INPUT>{"name":"abc","isenabled":"true"}</INPUT>
  * <OUTPUT>
	* {"name":"abc","isenabled":"true"}
	* </OUTPUT>
  * </EXAMPLE>
  * 
  */
	@GET
	@Path("/enable")
	@Produces(MediaType.TEXT_PLAIN)
	public Response enableBasemap(@QueryParam("NAME") final String bmName,
			@QueryParam("ENABLE") final String enable)
	{

		boolean doEnable = true;
		try
		{

			if(enable != null && enable.length() > 0)
			{
				doEnable = Boolean.parseBoolean(enable);
			}

			_toggleBaseMap(bmName, doEnable);
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
	        "Error enabling base map: " + bmName + " Error: " + ex.getMessage(),
			Status.INTERNAL_SERVER_ERROR,
			log);
		}

		JSONObject resp = new JSONObject();
		resp.put("name", bmName);
		resp.put("isenabled", doEnable);
		return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
	}



	protected void _deleteBaseMap(String bmName) throws Exception
	{
		String controlFolder = _ingestStagingPath + "/BASEMAP/";

		File tileDir = hoot.services.utils.FileUtils.getSubFolderFromFolder(_tileServerPath + "/BASEMAP/", bmName);
		if(tileDir != null && tileDir.exists())
		{
			FileUtils.forceDelete(tileDir);
		}

		File dir = new File(controlFolder);
		FileFilter fileFilter = new WildcardFileFilter(bmName + ".*");
		File[] files = dir.listFiles(fileFilter);
		for (int i = 0; i < files.length; i++) {
			File curFile = files[i];
			FileUtils.forceDelete(curFile);
		}

	}

	/**
  * <NAME>Delete Basemap Service</NAME>
  * <DESCRIPTION>Service method endpoint for deleting a basemap.</DESCRIPTION>
  * <PARAMETERS>
  * <NAME>
  * Name of a basemap
  * </NAME>
  * </PARAMETERS>
	* <OUTPUT>
	* 	JSON containing enable state
	* </OUTPUT>
	* <EXAMPLE>
	* 	<URL>http://localhost:8093/hoot-services/ingest/basemap/delete?NAME=abc</URL>
	* 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	* <INPUT>
	* {"name":"abc"}
	* </INPUT>
  * <OUTPUT>
	* {"name":"abc"}
	* </OUTPUT>
  * </EXAMPLE>
  * 
  */
	@GET
	@Path("/delete")
	@Produces(MediaType.TEXT_PLAIN)
	public Response deleteBasemap(@QueryParam("NAME") final String bmName)
	{

		try
		{
			_deleteBaseMap(bmName);
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
	        "Error deleting base map: " + bmName + " Error: " + ex.getMessage(),
			Status.INTERNAL_SERVER_ERROR,
			log);
		}

		JSONObject resp = new JSONObject();
		resp.put("name", bmName);

		return Response.ok(resp.toString(), MediaType.TEXT_PLAIN).build();
	}


}
