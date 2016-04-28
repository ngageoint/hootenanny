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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/mapeditexport")
public class ExportToMapEditResource  extends JobControllerBase {
	private static final Logger log = LoggerFactory.getLogger(ExportToMapEditResource.class);

	public ExportToMapEditResource()
	{
		try
		{
			if (processScriptName ==  null)
			{
				processScriptName = HootProperties.getProperty("HootToMapEdit");
			}

			throw new UnsupportedOperationException("OSM API DB support not yet enabled.");
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

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
			String tmpLoc = HootProperties.getProperty("osmTempOutputPath");
			String dbname = HootProperties.getProperty("MapEditDbName");
	  	String userid = HootProperties.getProperty("MapEditDbUserId");
	  	String pwd = HootProperties.getProperty("MapEditDbPassword");
	  	String host = HootProperties.getProperty("MapEditDbHostAddr");
	  	String port = HootProperties.getProperty("MapEditDbHostPort");
	  	String dbUrl = "hootapidb://" + userid + ":" + pwd + "@" + host + ":" + port + "/" + dbname;

	  	JSONObject arg = new JSONObject();
			arg.put("MAPEDIT_DB_URL", dbUrl);
			commandArgs.add(arg);

			arg = new JSONObject();
			arg.put("OUTPUT_DIR", tmpLoc);
			commandArgs.add(arg);



			String argStr = createPostBody(commandArgs);
			postJobRquest( jobId,  argStr);
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
}
