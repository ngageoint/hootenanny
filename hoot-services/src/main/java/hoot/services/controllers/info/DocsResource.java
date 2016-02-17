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

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import java.io.File;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;
import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/document")
public class DocsResource
{
	private static final Logger log = LoggerFactory.getLogger(DocsResource.class);

	protected static String _docName = null;
	protected static String _homeFolder = null;

	public DocsResource()
	{
		try
		{
			if (_docName == null)
			{
				_docName = HootProperties.getProperty("documentName");
			}

			if (_homeFolder == null)
			{
				_homeFolder = HootProperties.getProperty("homeFolder");
			}

		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	@GET
	@Path("/export")
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response exportDoc()
	{
		File out = null;
		try
		{
			String documentPath = _homeFolder + "/" + "docs" + "/" + _docName;
			out = new File(documentPath);
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
			    "Error exporting document file: " + ex.toString(),
			    Status.INTERNAL_SERVER_ERROR, log);
		}
		ResponseBuilder rBuild = Response.ok(out, "application/pdf");
		rBuild.header("Content-Disposition", "attachment; filename=" + _docName);

		return rBuild.build();
	}
}
