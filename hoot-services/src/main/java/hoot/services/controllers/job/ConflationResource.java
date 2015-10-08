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

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

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

@Path("/conflation")
public class ConflationResource extends JobControllerBase {
	private static final Logger log = LoggerFactory.getLogger(ConflationResource.class);
	protected static String _tileServerPath = null;
	protected static String _homeFolder = null;
	protected static String _confAdvOptsScript = null;

	public ConflationResource()
	{
		try
		{

			if (processScriptName ==  null)
			{
				processScriptName = HootProperties.getProperty("ConflateMakefilePath");
			}
			if(_tileServerPath == null)
			{
				_tileServerPath = HootProperties.getProperty("tileServerPath");
			}
			
			if(_homeFolder == null)
			{
				_homeFolder = HootProperties.getProperty("homeFolder");
			}
			
			if(_confAdvOptsScript == null)
			{
				_confAdvOptsScript = HootProperties.getProperty("confAdvOptsScript");
			}

		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
		}
	}
	/**
	 * <NAME>Conflate Service</NAME>
	 * <DESCRIPTION>Conflate service operates like a standard ETL service. The conflate service specifies the input files, conflation type, match threshold, miss threshold, and output file name. The conflation type can be specified as the average of the two input datasets or based on a single input file that is intended to be the reference dataset. It has two fronts, WPS and standard rest end point.</DESCRIPTION>
	 * <PARAMETERS>
	 * <INPUT1_TYPE>
	 * 	Conflation input type [OSM] | [OGR] | [DB]
	 * </INPUT1_TYPE>
	 * <INPUT1>
	 * 	Conlfation input 1
	 * </INPUT1>
	 * <INPUT2_TYPE>
	 * 	Conflation input type [OSM] | [OGR] | [DB]
	 * </INPUT2_TYPE>
	 * <INPUT2>
	 * 	 Conlfation input 2
	 * </INPUT2>
	 * <OUTPUT_NAME>
	 * 	 Conflation operation output name
	 * </OUTPUT_NAME>
	 * <CONFLATION_TYPE>
	 * 	 [average] | [reference]
	 * </CONFLATION_TYPE>
	 * <REFERENCE_LAYER>
	 * 	The reference layer which will be dominant tags. Default is 1 and if 2 selected, layer 2 tags will be dominant with layer 1 as geometry snap layer.
	 * </REFERENCE_LAYER>
	 * <AUTO_TUNNING>
	 * 	Not used. Always false
	 * </AUTO_TUNNING>
	 * <GENERATE_REPORT>
	 * 	true to generate conflation report
	 * </GENERATE_REPORT>
	 * <TIME_STAMP>
	 * 	Time stamp used in generated report if GENERATE_REPORT is true
	 * </TIME_STAMP>
	 * <USER_EMAIL>
	 * 	Email address of the user requesting the conflation job. 
	 * </USER_EMAIL>
	 * <ADV_OPTIONS>
	 * Advanced options list for hoot-core command
	 * </ADV_OPTIONS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Job ID
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/ogc</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
         * </OUTPUT>
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
		try
		{

			JSONParser pars = new JSONParser();
			JSONObject oParams = (JSONObject)pars.parse(params);
			oParams.put("IS_BIG", "false");
			String confOutputName = oParams.get("OUTPUT_NAME").toString();
			String input1Name = oParams.get("INPUT1").toString();
			String input2Name = oParams.get("INPUT2").toString();

			Object oTunn = oParams.get("AUTO_TUNNING");
			if(oTunn != null)
			{
				String autoTune = oTunn.toString();
				if(autoTune.equalsIgnoreCase("true"))
				{
					javax.management.MBeanServer mBeanServer = java.lang.management.ManagementFactory.getPlatformMBeanServer();
		    	Object attribute = mBeanServer.getAttribute(new javax.management.ObjectName("java.lang","type","OperatingSystem"), "TotalPhysicalMemorySize");
		    	long totalMemSize = Long.parseLong(attribute.toString());

		    	Long input1Size = Long.parseLong(oParams.get("INPUT1_ESTIMATE").toString());

		    	Long input2Size = Long.parseLong(oParams.get("INPUT2_ESTIMATE").toString());

		    	if((input1Size + input2Size) > (totalMemSize/2))
		    	{
		    		oParams.put("IS_BIG", "true");
		    		processScriptName = "makebigconflate";
		    	}

				}
			}
			
			
			
			

			JSONArray commandArgs = parseParams(oParams.toJSONString());
			JSONObject conflationCommand = _createMakeScriptJobReq(commandArgs);
			
			// add map tags
			// WILL BE DEPRECATED WHEN CORE IMPLEMENTS THIS
			Map<String, String> tags = new HashMap<String, String>();
			tags.put("input1", input1Name);
			tags.put("input2", input2Name);
			JSONArray mapTagsArgs = new JSONArray();
			JSONObject param = new JSONObject();
			param.put("value", tags);
			param.put("paramtype", Map.class.getName());
			param.put("isprimitivetype", "false");
			mapTagsArgs.add(param);

			param = new JSONObject();
			param.put("value", confOutputName);
			param.put("paramtype", String.class.getName());
			param.put("isprimitivetype", "false");
			mapTagsArgs.add(param);

			JSONObject updateMapsTagsCommand = _createReflectionJobReq(mapTagsArgs, "hoot.services.controllers.osm.MapResource",
					"updateTagsDirect");

			JSONArray reviewArgs = new JSONArray();
			param = new JSONObject();
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

			Object oUserEmail = oParams.get("USER_EMAIL");
			String userEmail = (oUserEmail ==  null)? null : oUserEmail.toString();
//	  Density Raster
			JSONArray rasterTilesArgs = new JSONArray();
			JSONObject rasterTilesparam = new JSONObject();
			rasterTilesparam.put("value", confOutputName);
			rasterTilesparam.put("paramtype", String.class.getName());
			rasterTilesparam.put("isprimitivetype", "false");
			rasterTilesArgs.add(rasterTilesparam);
			
			rasterTilesparam = new JSONObject();
			rasterTilesparam.put("value", userEmail);
			rasterTilesparam.put("paramtype", String.class.getName());
			rasterTilesparam.put("isprimitivetype", "false");
			rasterTilesArgs.add(rasterTilesparam);



			JSONObject ingestOSMResource = _createReflectionJobReq(rasterTilesArgs,
					"hoot.services.controllers.ingest.RasterToTilesService",
					"ingestOSMResourceDirect");

			JSONArray jobArgs = new JSONArray();
			jobArgs.add(conflationCommand);
			jobArgs.add(updateMapsTagsCommand);
			jobArgs.add(prepareItemsForReviewCommand);
			jobArgs.add(ingestOSMResource);


			postChainJobRquest( jobId,  jobArgs.toJSONString());

		}
		catch (Exception ex)
		{
		  ResourceErrorHandler.handleError(
			"Error running conflation job: " + ex.toString(),
		    Status.INTERNAL_SERVER_ERROR,
			log);
		}
		JSONObject res = new JSONObject();
		res.put("jobid", jobId);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
	}

	
}
