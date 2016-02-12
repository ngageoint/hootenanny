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
package hoot.services.controllers.job.custom.HGIS;

import java.util.UUID;

import hoot.services.HootProperties;
import hoot.services.exceptions.osm.InvalidResourceParamException;
import hoot.services.models.review.custom.HGIS.FilterNonHgisPoisRequest;
import hoot.services.models.review.custom.HGIS.FilterNonHgisPoisResponse;
import hoot.services.utils.ResourceErrorHandler;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/filter/custom/HGIS")
public class HGISFilterResource extends HGISResource {
	
	private static final Logger log = LoggerFactory.getLogger(HGISFilterResource.class);
	
	public HGISFilterResource() throws Exception
	{
		try
		{
			processScriptName = HootProperties.getProperty("hgisFilterScript");
		}
		catch (Exception e)
		{
			log.error("failed to retrieve parameter:" + e.getMessage());
		}
	}
	
  /**
  * <NAME>Filter Non HGIS POIs Service</NAME>
	 * <DESCRIPTION>This resource produces layer that filters Non HGIS POIs.</DESCRIPTION>
	 * <PARAMETERS>
	 * <source>
	 * 	Name of source layer
	 * </source>
	 * <output>
	 * 	Name of new outupt layer
	 * </output>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Job ID
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/filter/custom/HGIS/filternonhgispois</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {
	*		 "source":"AllDataTypesA",
	*		"output":"AllDataTypesAtest1"
	*		}
	 *	</INPUT>
	 * <OUTPUT>
	 * {"jobId":"91133065-ecb3-4476-9090-fb4bc3fabdf7"}
   * </OUTPUT>
	 * </EXAMPLE>
   * This resource produces layer that filters Non HGIS POIs.
   */
	@POST
  @Path("/filternonhgispois")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
	public FilterNonHgisPoisResponse filterNonHgisPois(FilterNonHgisPoisRequest request) throws Exception
	{
		FilterNonHgisPoisResponse resp = new FilterNonHgisPoisResponse();
		try
  	{
  		final String src = request.getSource();
  		final String output = request.getOutput();
  		if(src == null)
  		{
  			throw new InvalidResourceParamException("Invalid or empty sourceMap.");
  		}
  		
  		if(output == null)
  		{
  			throw new InvalidResourceParamException("Invalid or empty outputMap.");
  		}

  		if(!_mapExists(src))
  		{
  			throw new InvalidResourceParamException("sourceMap does not exist.");
  		}
  					

			String jobId = UUID.randomUUID().toString();
			String argStr = createBashPostBody(_createParamObj(src, output));
			postJobRquest( jobId,  argStr);

			
			resp.setJobId(jobId);
  		
  	}
  	catch (InvalidResourceParamException rpex)
  	{
  		ResourceErrorHandler.handleError(rpex.getMessage(), Status.BAD_REQUEST, log);
  	}
  	catch (Exception ex)
  	{
  		ResourceErrorHandler.handleError(ex.getMessage(), Status.INTERNAL_SERVER_ERROR, log);
  	}
		return resp;
	}
}
