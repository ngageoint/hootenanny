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

import static hoot.services.HootProperties.CLIP_DATASET_MAKEFILE_PATH;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;

import hoot.services.controllers.ingest.RasterToTilesCommandFactory;


@Controller
@Path("/clipdataset")
public class ClipDatasetResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(ClipDatasetResource.class);

    @Autowired
    private RasterToTilesCommandFactory rasterToTilesCommandFactory;


    public ClipDatasetResource() {
        super(CLIP_DATASET_MAKEFILE_PATH);
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
    @Produces(MediaType.APPLICATION_JSON)
    public JobId process(String params) {
        try {
            String jobId = UUID.randomUUID().toString();

            JSONParser parser = new JSONParser();
            JSONObject arguments = (JSONObject) parser.parse(params);
            String clipOutputName = arguments.get("OUTPUT_NAME").toString();

            JSONArray commandArgs = super.parseParams(params);
            JSONObject clipCommand = super.createMakeScriptJobReq(commandArgs);

            Command[] commands = {
                    // Clip to a bounding box
                    () -> { return externalCommandInterface.exec(jobId, clipCommand); },
                    // Ingest
                    () -> {
                        JSONObject rasterToTilesCommand = rasterToTilesCommandFactory.createExternalCommand(clipOutputName, null);
                        return externalCommandInterface.exec(jobId, rasterToTilesCommand);
                    }
            };

            super.processChainJob(jobId, commands);

            return new JobId(jobId);
        }
        catch (Exception e) {
            String msg = "Error processing cookie cutter request! Params: " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }
    }
}
