package hoot.services.controllers.job.custom.HGIS;

import java.io.IOException;
import java.sql.Connection;
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
import hoot.services.controllers.job.JobControllerBase;
import hoot.services.controllers.job.JobResource;
import hoot.services.db.DbUtils;
import hoot.services.exceptions.osm.InvalidResourceParamException;
import hoot.services.models.review.custom.HGIS.PrepareForValidationRequest;
import hoot.services.models.review.custom.HGIS.PrepareForValidationResponse;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.validators.osm.MapValidator;

@Path("/review/custom/HGIS")
public class HGISReviewResource extends JobControllerBase {
	static private final String _dbName;
	static private final String _dbUserId;
	static private final String _dbPassword;
	static private final String _dbHost;
	
	private static final Logger log = LoggerFactory.getLogger(JobResource.class);

	// Load just once during class load
	static
	{
		String dbName = null;
		String dbUserId = null;
		String dbPassword = null;
		String dbHost = null;
		try {
			dbName = HootProperties.getProperty("dbName");
			dbUserId = HootProperties.getProperty("dbName");
			dbPassword = HootProperties.getProperty("dbPassword");
			dbHost = HootProperties.getProperty("dbHost");
    } catch (IOException e) {
    	log.error("failed to retrieve parameter:" + e.getMessage());
    }
		
		_dbName = dbName;
		_dbUserId = dbUserId;
		_dbPassword = dbPassword;
		_dbHost = dbHost;
	}
	
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
   * @param request
   * @return
   * @throws Exception
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
  		
  		
  		

			JSONArray commandArgs = new JSONArray();
			JSONObject arg = new JSONObject();
			arg.put("SOURCE", _generateDbMapParam(src));
			commandArgs.add(arg);


			arg = new JSONObject();
			arg.put("OUTPUT", _generateDbMapParam(output));
			commandArgs.add(arg);

			String jobId = UUID.randomUUID().toString();
			String argStr = createBashPostBody(commandArgs);
			postJobRquest( jobId,  argStr);

			
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
  
  
  protected boolean _mapExists(final String mapName) throws Exception
  {
  	boolean exists = false;
  	try(Connection conn = DbUtils.createConnection())
  	{
  		MapValidator validator = new MapValidator(conn);
  		exists = (validator.verifyMapExists(mapName) > -1);
  		
  	}
  	catch(Exception ex)
  	{
  		throw ex;
  	}
  	
  	
  	return exists;
  }
  
//postgresql://hoot:hoottest@localhost:5432/hoot1/BrazilOsmPois
  protected final String _generateDbMapParam(final String mapName)
  {
  	return "postgresql://" + _dbUserId + ":" + _dbPassword + "@" + _dbHost + "/" + _dbName + "/" + mapName;
  }
}
