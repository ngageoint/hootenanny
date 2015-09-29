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
   * @param request
   * @return
   * @throws Exception
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
