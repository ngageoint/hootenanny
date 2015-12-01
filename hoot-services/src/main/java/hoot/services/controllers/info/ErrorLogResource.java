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

import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.annotation.PreDestroy;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.info.ErrorLog;
import hoot.services.utils.ResourceErrorHandler;

@Path("/logging")
public class ErrorLogResource {
	private static final Logger log = LoggerFactory.getLogger(ErrorLogResource.class);

	private String _exportLogPath = null;
	public ErrorLogResource()
	{

	}

	@PreDestroy
	public void PreDestroy()
	{
		try
		{
			if(_exportLogPath != null && _exportLogPath.length() > 0)
			{
				FileUtils.forceDelete(new File(_exportLogPath));
			}
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}
	
	
	 /**
   * <NAME>Logging Debug Log Service</NAME>
   * <DESCRIPTION>Service method endpoint for retrieving the Hootenanny tomcat log.</DESCRIPTION>
   * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	JSON containing debug log
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/info/logging/debuglog</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
   * <OUTPUT>
 	 * {
   * "log": " Reprojecting 2000 / 22601 Reprojecting 3000 / 22601 ..."
   * } 
   * </OUTPUT>
   * </EXAMPLE>
   * 
   */
	@GET
  @Path("/debuglog")
  @Produces(MediaType.TEXT_PLAIN)
  public Response getDebugLog()
	{
		String logStr = null;

		try
		{
			ErrorLog logging = new ErrorLog();
			// 50k Length
			logStr = logging.getErrorlog(50000);
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
					"Error getting error log: " + ex.toString(),
				    Status.INTERNAL_SERVER_ERROR,
					log);
		}
		JSONObject res = new JSONObject();
		res.put("log", logStr);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
	}

	 /**
  * <NAME>Logging Export Service</NAME>
  * <DESCRIPTION>Service method endpoint for exporting logging information.</DESCRIPTION>
  * <PARAMETERS></PARAMETERS>
	 * <OUTPUT>
	 * 	Octet stream
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/info/logging/export</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * <INPUT>None</INPUT>
  * <OUTPUT>
	* 	Binary octet stream
  * </OUTPUT>
  * </EXAMPLE>
  * 
  */
	@GET
  @Path("/export")
  @Produces(MediaType.APPLICATION_OCTET_STREAM)
  public Response exportLog()
	{
		ErrorLog logging = new ErrorLog();
		File out = null;

		try
		{
			String outputPath = logging.generateExportLog();
			out = new File(outputPath);
			_exportLogPath = outputPath;
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
					"Error exporting log file: " + ex.toString(),
				    Status.INTERNAL_SERVER_ERROR,
					log);
		}
		DateFormat dateFormat = new SimpleDateFormat("yyyyMMddHHmmss");
		Date dd = new Date();
		String dtStr = dateFormat.format(dd);
		ResponseBuilder rBuild = Response.ok(out, MediaType.APPLICATION_OCTET_STREAM);
		rBuild.header("Content-Disposition", "attachment; filename=hootlog_" + dtStr + ".log" );

		return rBuild.build();
	}
}
