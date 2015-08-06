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
import hoot.services.nativeInterfaces.JobExecutionManager;
import hoot.services.nativeInterfaces.NativeInterfaceException;
import hoot.services.utils.ResourceErrorHandler;

import java.io.File;
import java.io.IOException;
import java.util.UUID;

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

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.apache.commons.io.FileUtils;




@Path("/export")
public class ExportResource {
	private static final Logger log = LoggerFactory.getLogger(ExportResource.class);
	private static ClassPathXmlApplicationContext appContext = null;
	private static String homeFolder = null;
	private static String exportScript = null;
	private static String tempOutputPath = null;

	private String outputFolderPath = null;
	private String tempOsmFilePath = null;

	public ExportResource()
	{
		if (appContext == null)
    {
      appContext = new ClassPathXmlApplicationContext("hoot/spring/CoreServiceContext.xml");
    }

		if(homeFolder ==  null){
	    try {
	    	homeFolder = HootProperties.getProperty("homeFolder");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }

		if(exportScript ==  null){
	    try {
	    	exportScript = HootProperties.getProperty("ExportScript");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }

		if(tempOutputPath ==  null){
	    try {
	    	tempOutputPath = HootProperties.getProperty("tempOutputPath");
			} catch (IOException e) {
				log.error(e.getMessage());
			}
    }
	}

	@PreDestroy
	public void preDestrory()
	{
		try
		{
			if(tempOsmFilePath != null && tempOsmFilePath.length() > 0)
			{
				File tempOsm = new File(tempOsmFilePath);
				if(tempOsm.exists())
				{
					tempOsm.delete();
				}
			}
			File workfolder = new File(outputFolderPath);
			if(workfolder.exists() && workfolder.isDirectory())
			{
				FileUtils.deleteDirectory(workfolder);
			}
		}
		catch (Exception ex)
		{
			//TODO: should this error be thrown?
			log.error(ex.getMessage());
		}
	}

	/**
	 * <NAME>Export Service Synchronous</NAME>
	 * <DESCRIPTION>
	 * THIS REST END POINT IS DEPRECATED.
	 * 	Synchronous export service.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <outputtype>
	 * 	[gdb | shp | wfs]. gdb will produce file gdb, shp will output shapefile. if outputtype = wfs then a wfs front end will be created
	 * </outputtype>
	 * <translation>
	 * 	Translation script name.
	 * </translation>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Octet stream
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL> http://localhost:8080/hoot-services/job/export/[job id]?outputtype=gdb&translation=MGCP.js</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>Octet stream</OUTPUT>
	 * </EXAMPLE>
	 * @param id
	 * @param outputType
	 * @param translation
	 * @return
	 */
	@GET
	@Path("/{id}")
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response exportFile(@PathParam("id") String id, @QueryParam("outputtype") final String outputType,
			@QueryParam("translation") final String translation)
	{
		File out = null;
		try
		{
			String outputPath = execOsm2Ogr(id, outputType, translation, id, "db");
			out = new File(outputPath);

		}
		catch (NativeInterfaceException ne)
    {
      int nStat = ne.getExceptionCode().toInt();
      return Response.status(nStat).entity(ne.getMessage()).build();
    }
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error exporting file: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
		rBuild.header("Content-Disposition", "attachment; filename=" + id + ".zip" );

		return rBuild.build();
	}

	@POST
	@Path("/osm")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response exportOsmFile(@QueryParam("outputname") final String outputName,
			@QueryParam("outputtype") final String outputType,
			@QueryParam("translation") final String translation, String osmXml)
	{
		File out = null;
		try
		{

			File dir = new File(tempOutputPath);
 	    if(!dir.exists())
 	    {
 	    	dir.mkdir();
 	    }
			String outputname = UUID.randomUUID().toString();
 	    String tempInputPath = tempOutputPath + "/" + outputname + ".osm";
 	    tempOsmFilePath = tempInputPath;
	    File inputFile = new File(tempInputPath);
	    // create temp input osm
	    FileUtils.writeStringToFile(inputFile, osmXml);

			String outputPath = execOsm2Ogr(outputname, outputType, translation, tempInputPath, "file");
			out = new File(outputPath);

		}
		catch (NativeInterfaceException ne)
    {
      int nStat = ne.getExceptionCode().toInt();
      return Response.status(nStat).entity(ne.getMessage()).build();
    }
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error exporting OSM file: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
		rBuild.header("Content-Disposition", "attachment; filename=" + outputName + ".zip" );

		return rBuild.build();
	}


	/**
	 * Generates command params
	 *
	 * @param outputType
	 * @param translation
	 * @param input
	 * @return
	 * @throws Exception
	 */
	private JSONObject createCommand(String output, String outputType, String translation, String input,
			String inputType) throws Exception
	{

  	String outputtype = "";
  	String jobid = UUID.randomUUID().toString();

  	if(outputType.equalsIgnoreCase("FGDB"))
  	{
  		outputtype = "gdb";
  	}
  	else if(outputType.equalsIgnoreCase("SHP"))
  	{
  		outputtype = "shp";
  	}
  	else
  	{
  		throw new Exception("Invalid file type");
  	}

  	outputFolderPath = tempOutputPath + "/" + jobid;

  	JSONArray params = new JSONArray();
  	JSONObject param = new JSONObject();


  	param = new JSONObject();
  	param.put("translation", translation);
  	params.add(param);

  	param = new JSONObject();
  	param.put("input", input);
  	params.add(param);

  	param = new JSONObject();
  	param.put("inputtype", inputType);
  	params.add(param);

  	param = new JSONObject();
  	param.put("output", output);
  	params.add(param);

  	param = new JSONObject();
  	param.put("outputtype", outputtype);
  	params.add(param);

  	param = new JSONObject();
  	param.put("outputfolder", outputFolderPath);
  	params.add(param);

  	JSONObject command = new JSONObject();
  	command.put("exectype", "make");
  	command.put("exec", exportScript);
  	command.put("jobId", jobid);
  	command.put("params", params);

  	return command;
	}


	/**
	 * Executes osm2ogr to produce zip of fgdb folder
	 *
	 * @param outputType
	 * @param translation
	 * @param input
	 * @return
	 * @throws NativeInterfaceException
	 */
	private String execOsm2Ogr(String output, String outputType, String translation, String input, String inputType) throws NativeInterfaceException
	{

  	String outputFilePath = null;
  	try
  	{
  		JSONObject command = createCommand(output, outputType, translation, input, inputType);
  		JobExecutionManager jobExecManager = (JobExecutionManager)appContext.getBean("jobExecutionManagerNative");
  		jobExecManager.exec(command);
  	}
  	catch (Exception ex)
  	{
  		throw new NativeInterfaceException(ex.getMessage(),
          NativeInterfaceException.HttpCode.SERVER_ERROR);
  	}

    outputFilePath = outputFolderPath + "/" + output + ".zip";
    File outFile = new File(outputFilePath);
    if(!outFile.exists())
    {
    	throw new NativeInterfaceException("Missing output file",
          NativeInterfaceException.HttpCode.SERVER_ERROR);
    }

    return outputFilePath;
	}
}
