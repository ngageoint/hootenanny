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

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;

//import org.json.simple.parser.JSONParser;


@Path("/clipdataset")
public class ClipDatasetResource extends JobControllerBase {

    private static final Logger log = LoggerFactory.getLogger(ClipDatasetResource.class);
    protected static String _tileServerPath = null;

    public ClipDatasetResource() {
        try {

            if (processScriptName == null) {
                processScriptName = HootProperties.getProperty("ClipDatasetMakefilePath");
            }

        }
        catch (Exception ex) {
            log.error(ex.getMessage());
        }
    }

    /**
     * This service will clip a dataset to a bounding box and create a new
     * output dataset within those dimensions.
     * 
     * POST hoot-services/job/clipdataset/execute
     * 
     * { "BBOX" :
     * "{"LR":[-77.04813267598544,38.89292259454727],"UL":[-77.04315011486628,38.89958152667718]}",
     * //The upper left and lower right of the bounding box to clip the dataset
     * "INPUT_NAME" : "DcRoads", //The name of the dataset to be clipped
     * "OUTPUT_NAME" : "DcRoads_Clip" //The output name of the new dataset. }
     * 
     * @param params
     *            JSON input params; see description
     * @return a job id
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response process(String params) {
        String jobId = UUID.randomUUID().toString();
        try {
            // JSONParser pars = new JSONParser();
            // JSONObject oParams = (JSONObject)pars.parse(params);
            // String clipOutputName = oParams.get("OUTPUT_NAME").toString();

            JSONArray commandArgs = parseParams(params);
            JSONObject clipCommand = _createMakeScriptJobReq(commandArgs);

            JSONArray jobArgs = new JSONArray();
            jobArgs.add(clipCommand);

            postChainJobRquest(jobId, jobArgs.toJSONString());
        }
        catch (Exception ex) {
            ResourceErrorHandler.handleError("Error processing cookie cutter request: " + ex.toString(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        JSONObject res = new JSONObject();
        res.put("jobid", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

}
