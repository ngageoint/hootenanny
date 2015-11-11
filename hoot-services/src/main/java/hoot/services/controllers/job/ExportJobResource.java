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
package hoot.services.controllers.job;

import java.io.File;
import java.util.List;
import java.util.UUID;

import hoot.services.HootProperties;
import hoot.services.controllers.wfs.WfsManager;
import hoot.services.db.DataDefinitionManager;
import hoot.services.nativeInterfaces.NativeInterfaceException;
import hoot.services.utils.ResourceErrorHandler;

import javax.annotation.PreDestroy;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/export")
public class ExportJobResource extends JobControllerBase {
	private static final Logger log = LoggerFactory.getLogger(ExportJobResource.class);
	private static String tempOutputPath = null;
	private String delPath = null;
	private String wfsStoreDb = null;
	private String homeFolder = null;
	private String translationExtPath = null;


	public ExportJobResource()
	{
		try
		{
			if (processScriptName ==  null)
			{
				processScriptName = HootProperties.getProperty("ExportScript");
			}

			if(tempOutputPath ==  null){
				tempOutputPath = HootProperties.getProperty("tempOutputPath");
	    }

			wfsStoreDb =
	        HootProperties.getInstance().getProperty("wfsStoreDb");
			homeFolder = HootProperties.getProperty("homeFolder");
			translationExtPath = HootProperties.getProperty("translationExtPath");
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}

	}


	@PreDestroy
	public void preDestrory()
	{
		try
		{
			if(delPath != null)
			{
				File workfolder = new File(delPath);
				if(workfolder.exists() && workfolder.isDirectory())
				{
					FileUtils.deleteDirectory(workfolder);
				}
			}
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	/**
	 * <NAME>Export Service Job Execute</NAME>
	 * <DESCRIPTION>
	 * 	Asynchronous export service.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <translation>
	 * 	Translation script name.
	 * </translation>
	 * <inputtype>
	 *  [db | file] db means input from hoot db will be used. file mean a file path will be specified.
	 * </inputtype>
	 * <input>
	 * Input name. for inputtype = db then specify name from hoot db. For inputtype=file, specify full path to a file.
	 * </input>
	 * <outputtype>
	 * 	[gdb | shp | wfs]. gdb will produce file gdb, shp will output shapefile. if outputtype = wfs then a wfs front end will be created
	 * </outputtype>
	 * <USER_EMAIL>
	 * Email address of the user requesting the job.
	 * </USER_EMAIL>
	 * <append>
	 * Denotes if export should append to FGDB template
	 * </append>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Job ID
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/export/execute</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {
   * "translation":"MGCP.js",
   * "inputtype":"db",
   * "input":"ToyTestA",
   * "outputtype":"gdb",
   * "removereview" : "false"
   *
   * }
	 *	</INPUT>
	 * <OUTPUT>{"jobid":"24d1400e-434e-45e0-9afe-372215490be2"}</OUTPUT>
	 * </EXAMPLE>
	 * @param params
	 * @return
	 */
	@POST
	@Path("/execute")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.TEXT_PLAIN)
	public Response process(String params)
	{
		String jobId = UUID.randomUUID().toString();
		jobId = "ex_" + jobId.replace("-", "");
		try
		{
			JSONArray commandArgs = parseParams(params);

			JSONObject arg = new JSONObject();
			arg.put("outputfolder", tempOutputPath + "/" + jobId);
			commandArgs.add(arg);

			arg = new JSONObject();
			arg.put("output", jobId);
			commandArgs.add(arg);



			String type = getParameterValue("outputtype", commandArgs);
			if(type != null && type.equalsIgnoreCase("wfs"))
			{
				arg = new JSONObject();
				arg.put("outputname", jobId);
				commandArgs.add(arg);

				HootProperties.getProperty("dbName");
				String userid = HootProperties.getProperty("dbUserId");
				String pwd = HootProperties.getProperty("dbPassword");
				String host = HootProperties.getProperty("dbHost");
				String[] hostParts = host.split(":");

				String pgUrl = "host='"+ hostParts[0] + "' port='" + hostParts[1] + "' user='" + userid 
								+ "' password='" + pwd + "' dbname='" + wfsStoreDb + "'";
								
				arg = new JSONObject();
				arg.put("PG_URL", pgUrl);
				commandArgs.add(arg);

				JSONObject osm2orgCommand = _createPostBody(commandArgs);
				// this may need change in the future if we decided to use user defined ouputname..
				String outname = jobId;

				JSONArray wfsArgs = new JSONArray();
				JSONObject param = new JSONObject();
				param.put("value", outname);
				param.put("paramtype", String.class.getName());
				param.put("isprimitivetype", "false");
				wfsArgs.add(param);


				JSONObject prepareItemsForReviewCommand = _createReflectionSycJobReq(wfsArgs, "hoot.services.controllers.wfs.WfsManager",
						"createWfsResource");

				JSONArray jobArgs = new JSONArray();
				jobArgs.add(osm2orgCommand);
				jobArgs.add(prepareItemsForReviewCommand);


				postChainJobRquest( jobId,  jobArgs.toJSONString());
			}
			else
			{
			// replace with with getParameterValue
				boolean paramFound = false;
				for(int i=0; i<commandArgs.size(); i++)
				{
					JSONObject jo = (JSONObject)commandArgs.get(i);
					Object oo = jo.get("outputname");
					if(oo != null)
					{
						String strO = (String)oo;
						if(strO.length() > 0)
						{
							paramFound = true;
							break;
						}
					}
				}

				if(paramFound ==  false)
				{
					arg = new JSONObject();
					arg.put("outputname", jobId);
					commandArgs.add(arg);
				}

				String argStr = createPostBody(commandArgs);
				postJobRquest( jobId,  argStr);
			}
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error exporting data: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}
		JSONObject res = new JSONObject();
		res.put("jobid", jobId);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
	}


	/**
	 * <NAME>Export Service Get Job Output</NAME>
	 * <DESCRIPTION>
	 * To retrieve the output from job make Get request.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <outputname>
	 *  parameter overrides the output file name with the user defined name. If not specified then defaults to job id as name.
	 * </outputname>
	 * <removecache>
	 *  parameter controls if the output file from export job should be delete when Get request completes.
	 * </removecache>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Octet stream
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>"http://localhost:8080/hoot-services/job/export/[job id from export job]?outputname=[user defined name]&removecache=[true | false]"</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>Octet stream</OUTPUT>
	 * </EXAMPLE>
	 * @param id
	 * @param outputname
	 * @param remove
	 * @return
	 */
	@GET
	@Path("/{id}")
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response exportFile(@PathParam("id") String id, @QueryParam("outputname") final String outputname,
			@QueryParam("removecache") final String remove)
	{
		File out = null;
		try
		{
			String workingFolder = null ;

			File folder = hoot.services.utils.FileUtils.getSubFolderFromFolder(tempOutputPath, id);
			if(folder != null)
			{
				workingFolder = tempOutputPath + "/" + id ;
				
				if(remove.equalsIgnoreCase("true"))
				{
					delPath = workingFolder ;
				}

		    out = hoot.services.utils.FileUtils.getFileFromFolder(workingFolder, outputname , "zip"); 
		    if(out == null  || !out.exists())
		    {
		    	throw new NativeInterfaceException("Missing output file",
		          NativeInterfaceException.HttpCode.SERVER_ERROR);
		    }

			}

		}
		catch (NativeInterfaceException ne)
    {
      int nStat = ne.getExceptionCode().toInt();
      return Response.status(nStat).entity(ne.getMessage()).build();
    }
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error exporting data: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		String outFileName = id;
		if(outputname != null && outputname.length() > 0)
		{
			outFileName = outputname;
		}

		ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
		rBuild.header("Content-Disposition", "attachment; filename=" + outFileName + ".zip" );

		return rBuild.build();
	}


	/**
	 * <NAME>Export Service Remove WFS</NAME>
	 * <DESCRIPTION>
	 * Removes specified WFS resource.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Removed id
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/export/wfs/remove/ex_eed379c0b9f7469d80ab32c71550883b</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>{"id":"ex_eed379c0b9f7469d80ab32c71550883b"}</OUTPUT>
	 * </EXAMPLE>
	 * @param id
	 * @return
	 */
	@GET
	@Path("/wfs/remove/{id}")
	@Produces(MediaType.TEXT_PLAIN)
	public Response removeWfsResource(@PathParam("id") String id)
	{
		JSONObject ret = new JSONObject();
		try
		{
			WfsManager wfsMan = new WfsManager();
			wfsMan.removeWfsResource(id);

			DataDefinitionManager dbMan = new DataDefinitionManager();
			List<String> tbls = dbMan.getTablesList(wfsStoreDb, id);
			dbMan.deleteTables(tbls, wfsStoreDb);

		}
		catch(Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error removing WFS resource: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		ret.put("id", id);
		return Response.ok(ret.toString(), MediaType.TEXT_PLAIN).build();
	}

	/**
	 * <NAME>Export Service Get WFS List</NAME>
	 * <DESCRIPTION>
	 * 	Lists all wfs resources.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	List of wfs resources
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/export/wfs/resources</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>[{"id":"ex_eed379c0b9f7469d80ab32c71550883b"}]</OUTPUT>
	 * </EXAMPLE>
	 * @return
	 */
	@GET
	@Path("/wfs/resources")
	@Produces(MediaType.TEXT_PLAIN)
	public Response getWfsResources()
	{
		JSONArray srvList = new JSONArray();
		try
		{
			WfsManager wfsMan = new WfsManager();
			List<String>list = wfsMan.getAllWfsServices();

			for(int i=0; i<list.size(); i++)
			{
				String wfsResource = list.get(i);
				JSONObject o = new JSONObject();
				o.put("id", wfsResource);
				srvList.add(o);
			}

		}
		catch(Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error retrieving WFS resource list: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		return Response.ok(srvList.toString(), MediaType.TEXT_PLAIN).build();
	}


	/**
	 * <NAME>Export Service Get List of Translation Scripts For Export</NAME>
	 * <DESCRIPTION>
	 * 	Based on the existence of translation script extension, it will send the list of available translations script for export.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	List of translation script resources
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/export/resources</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>[{"description":"LTDS 4.0","name":"NFDD"},{"description":"MGCP","name":"MGCP"},{"description":"UTP","name":"UTP"}]</OUTPUT>
	 * </EXAMPLE>
	 * @return
	 */
	@GET
	@Path("/resources")
	@Produces(MediaType.TEXT_PLAIN)
	public Response getExportResources()
	{
		String transExtPath = homeFolder + "/" + "/plugins-local/script/utp";
		if(translationExtPath != null && translationExtPath.length() > 0)
		{
			transExtPath = translationExtPath;
		}
		JSONArray srvList = new JSONArray();
		try
		{
			JSONObject o = new JSONObject();
			o.put("name", "TDS");
			o.put("description", "LTDS 4.0");
			srvList.add(o);

			o = new JSONObject();
			o.put("name", "MGCP");
			o.put("description", "MGCP");
			srvList.add(o);

			File f = new File(transExtPath);
			if(f.exists() && f.isDirectory())
			{
				o = new JSONObject();
				o.put("name", "UTP");
				o.put("description", "UTP");
				srvList.add(o);
			}

		}
		catch(Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error retrieving exported resource list: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		return Response.ok(srvList.toString(), MediaType.APPLICATION_JSON).build();
	}
}