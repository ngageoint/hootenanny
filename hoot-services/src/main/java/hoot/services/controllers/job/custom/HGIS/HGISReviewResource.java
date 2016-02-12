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

import java.sql.Connection;
import java.sql.SQLException;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.exceptions.osm.InvalidResourceParamException;
import hoot.services.exceptions.review.custom.HGIS.ReviewMapTagUpdateException;
import hoot.services.job.JobStatusManager;
import hoot.services.models.review.custom.HGIS.PrepareForValidationRequest;
import hoot.services.models.review.custom.HGIS.PrepareForValidationResponse;
import hoot.services.review.custom.HGIS.HGISValidationMarker;
import hoot.services.utils.ResourceErrorHandler;

@Path("/review/custom/HGIS")
public class HGISReviewResource extends HGISResource {

	private static final Logger log = LoggerFactory.getLogger(HGISReviewResource.class);
	public HGISReviewResource() throws Exception
	{
		try
		{
			processScriptName = HootProperties.getProperty("hgisPrepareForValidationScript");
		}
		catch (Exception e)
		{
			log.error("failed to retrieve parameter:" + e.getMessage());
		}
	}
	
	
  /**
  * <NAME>HGIS Prepare Validation Review Service</NAME>
	 * <DESCRIPTION>This resource prepares existing map for 30% of random HGIS specific validation.</DESCRIPTION>
	 * <PARAMETERS>
	 * <sourceMap>
	 * 	Name of source layer
	 * </sourceMap>
	 * <outputMap>
	 * 	Name of new outupt layer with reviewables
	 * </outputMap>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Job ID
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/custom/HGIS/preparevalidation</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {
	*		 "sourceMap":"AllDataTypesA",
	*		"outputMap":"AllDataTypesAtest1"
	*		}
	 *	</INPUT>
	 * <OUTPUT>
	 * {"jobId":"91133065-ecb3-4476-9090-fb4bc3fabdf7"}
   * </OUTPUT>
	 * </EXAMPLE>
   * This resource prepares existing map for 30% of random HGIS specific validation
   */
  @POST
  @Path("/preparevalidation")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public PrepareForValidationResponse prepareItemsForValidationReview(
  		PrepareForValidationRequest request
  ) throws Exception
  {
  	PrepareForValidationResponse res = new PrepareForValidationResponse();
  	try
  	{
  		final String src = request.getSourceMap();
  		final String output = request.getOutputMap();
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
			JSONObject validationCommand = _createBashPostBody(_createParamObj(src, output));
			//postJobRquest( jobId,  argStr);		
		
		
					
			JSONObject updateMapTagCommand = _createUpdateMapTagCommand(output);
			
			// with new relation based review process
			// we will no longer need to run prepare review
			// Instead core will take care of generation relation review for
			// POI validation
			JSONArray jobArgs = new JSONArray();
			jobArgs.add(validationCommand);			
			jobArgs.add(updateMapTagCommand);



			postChainJobRquest( jobId,  jobArgs.toJSONString());
			
			res.setJobId(jobId);
  		
  	}
  	catch (InvalidResourceParamException rpex)
  	{
  		ResourceErrorHandler.handleError(rpex.getMessage(), Status.BAD_REQUEST, log);
  	}
  	catch (Exception ex)
  	{
  		ResourceErrorHandler.handleError(ex.getMessage(), Status.INTERNAL_SERVER_ERROR, log);
  	}
  	return res;
  }

  private JSONObject _createUpdateMapTagCommand(final String mapName) throws Exception
  {
  	JSONArray reviewArgs = new JSONArray();
		JSONObject param = new JSONObject();
		param.put("value", mapName);
		param.put("paramtype", String.class.getName());
		param.put("isprimitivetype", "false");
		reviewArgs.add(param);

		return _createReflectionJobReq(reviewArgs, "hoot.services.controllers.job.custom.HGIS.HGISReviewResource",
				"updateMapsTag");
		
  }
  public String updateMapsTag(final String mapName) throws SQLException, ReviewMapTagUpdateException, Exception
  {
  	String jobId = UUID.randomUUID().toString();
  	JobStatusManager jobStatusManager = null;
  	try(Connection conn = DbUtils.createConnection())
  	{
  		jobStatusManager = new JobStatusManager(conn);
			jobStatusManager.addJob(jobId);
			
  		HGISValidationMarker marker = new HGISValidationMarker(conn, mapName);
  		marker.updateValidationMapTag();
  		jobStatusManager.setComplete(jobId);
  	}
  	catch(SQLException | ReviewMapTagUpdateException se)
  	{
  		jobStatusManager.setFailed(jobId);
  		log.error(se.getMessage());
  		throw se;
  	}
  	catch(Exception e)
  	{
  		jobStatusManager.setFailed(jobId);
  		log.error(e.getMessage());
  		throw e;
  	}
  	
  	return jobId;
  	
  }

}
