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

import java.util.UUID;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


@Path("/cookiecutter")
public class CookieCutterResource extends JobControllerBase {

	private static final Logger log = LoggerFactory.getLogger(CookieCutterResource.class);
	protected static String _tileServerPath = null;

	public CookieCutterResource()
	{
		try
		{

			if (processScriptName ==  null)
			{
				processScriptName = HootProperties.getProperty("CookieCutterMakefilePath");
			}

			if(_tileServerPath == null)
			{
				_tileServerPath = HootProperties.getProperty("tileServerPath");
			}

		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

	/**
	 * <NAME>Cookie Cutter Service</NAME>
	 * <DESCRIPTION>
	 * DEPRICATED
	 *  Cookie cutter service contains 2 end points, WPS and rest. Both use post request to execute job.
	 *  When successfully executed, the service willreturn Job Id where it is used to track the progress of job and result.
	 *  For valid parameters, the following are accepted.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * 	<INPUT_TYPE>
	 * 	Conflation input type [OSM] | [OGR] | [DB]
	 * 	</INPUT_TYPE>
	 * 	<INPUT>
	 * 	Conlfation input (during cookie cutter conflation, output from cookie cutter op will be used as INPUT2)
	 * 	</INPUT>
	 * 	<OUTPUT_NAME>
	 * 	Conflation operation output name
	 * 	</OUTPUT_NAME>
	 * 	<CONFLATION_TYPE>
	 * 	[average] | [reference]
	 * 	</CONFLATION_TYPE>
	 *  <MATCH_THRESHOLD>
	 *  The threshold for calling a relationship a match. Defaults to 0.6.
	 *  The higher the value the lower the TPR, but likely also the lower the FPR.
	 *  </MATCH_THRESHOLD>
	 *  <MISS_THRESHOLD>
	 *  The threshold for calling a relationship a miss.
	 *  Defaults to 0.6. The higher the value the lower the TNR, but likely also the lower the FNR.
	 *  </MISS_THRESHOLD>
	 *  <cuttershapetype>
	 *  [OSM] | [OGR] | [DB]
	 *  </cuttershapetype>
	 *  <cuttershape>
	 *  identifier of the cutter shape where for DB it is MapId and for others relative path of the file.
	 *  </cuttershape>
	 *  <alpha>
	 *  alpha value for --alpha-shape operation
	 *  </alpha>
	 *  <buffer>
	 *  buffer value for --alpha-shape operation
	 *  </buffer>
	 *  <inputtype>
	 *  [OSM] | [OGR] | [DB]
	 *  </inputtype>
	 *  <input>
	 *  identifier of the input (dough) where for DB it is MapId and for others relative path of the file.
	 *  </input>
	 *  <outputbuffer>
	 *  buffer value of --cookie-cutter operation
	 *  </outputbuffer>
	 *  <crop>
	 *  [true] | [false]
	 *  </crop>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * Job Id
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/cookiecutter/execute</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>{
   * "INPUT1_TYPE" : "DB",
   * "INPUT1" : "dough",
   * "OUTPUT_NAME" : "Conflation_out",
   * "CONFLATION_TYPE" : "reference",
   * "MATCH_THRESHOLD" : "0.6",
   * "MISS_THRESHOLD" : "0.6",
   * "cuttershapetype":"DB",
   * "cuttershape":"cutter",
   * "alpha":"5000",
   * "buffer":"0",
   * "inputtype":"DB",
   * "input":"dough",
   * "outputbuffer":"-500",
   * "crop":"false",
   * "outputname":"test3"
	 * }</INPUT>
	 * <OUTPUT>{"jobid":"0df81145-5f3b-46f4-9937-64ade48fd604"}</OUTPUT>
	 * </EXAMPLE>
	 * @param params
	 * @return
	 */
	@POST
	@Path("/execute")
	@Consumes(MediaType.TEXT_PLAIN)
	@Produces(MediaType.TEXT_PLAIN)
	public Response process(String params)
	{		String jobId = UUID.randomUUID().toString();
		try
		{

			JSONParser pars = new JSONParser();
			JSONObject oParams = (JSONObject)pars.parse(params);
			String confOutputName = oParams.get("OUTPUT_NAME").toString();



			JSONArray commandArgs = parseParams(params);
			JSONObject conflationCommand = _createMakeScriptJobReq(commandArgs);

			JSONArray reviewArgs = new JSONArray();
			JSONObject param = new JSONObject();
			param.put("value", confOutputName);
			param.put("paramtype", String.class.getName());
			param.put("isprimitivetype", "false");
			reviewArgs.add(param);

			param = new JSONObject();
			param.put("value", false);
			param.put("paramtype", Boolean.class.getName());
			param.put("isprimitivetype", "true");
			reviewArgs.add(param);

			JSONObject prepareItemsForReviewCommand = _createReflectionJobReq(reviewArgs, "hoot.services.controllers.job.ReviewResource",
					"prepareItemsForReview");


//	  Density Raster
			JSONArray rasterTilesArgs = new JSONArray();
			JSONObject rasterTilesparam = new JSONObject();
			rasterTilesparam.put("value", confOutputName);
			rasterTilesparam.put("paramtype", String.class.getName());
			rasterTilesparam.put("isprimitivetype", "false");
			rasterTilesArgs.add(rasterTilesparam);



			JSONObject ingestOSMResource = _createReflectionJobReq(rasterTilesArgs,
					"hoot.services.controllers.ingest.RasterToTilesService",
					"ingestOSMResourceDirect");

			JSONArray jobArgs = new JSONArray();
			jobArgs.add(conflationCommand);
			jobArgs.add(prepareItemsForReviewCommand);
			jobArgs.add(ingestOSMResource);


			postChainJobRquest( jobId,  jobArgs.toJSONString());
		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error processing cookie cutter request: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}

		JSONObject res = new JSONObject();
		res.put("jobid", jobId);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
	}



}
