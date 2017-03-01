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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.clipping;

import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.controllers.ExportRenderDBCommandFactory;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;


@Controller
@Path("/clipdataset")
public class ClipDatasetResource {
    private static final Logger logger = LoggerFactory.getLogger(ClipDatasetResource.class);

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private ExternalCommandManager externalCommandManager;

    @Autowired
    private ClipDatasetCommandFactory clipDatasetCommandFactory;

    @Autowired
    private ExportRenderDBCommandFactory exportRenderDBCommandFactory;


    /**
     * This service will clip a dataset to a bounding box and create a new output dataset within those dimensions.
     *
     * POST hoot-services/job/clipdataset/execute
     *
     * {
     *   //The upper left (UL) and lower right (LR) of the bounding box to clip the dataset
     *   "BBOX" : "{"LR":[-77.04813267598544,38.89292259454q727],"UL":[-77.04315011486628,38.89958152667718]}",
     *
     *   //The name of the dataset to be clipped
     *   "INPUT_NAME" : "DcRoads",
     *
     *   //The output name of the new dataset.
     *   "OUTPUT_NAME" : "DcRoads_Clip"
     * }
     *
     * @param params
     *            JSON input params; see description above

     * @return a job id
     */
    @POST
    @Path("/execute")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response clipDataset(String params) {
        String jobId = UUID.randomUUID().toString();

        try {
            JSONParser parser = new JSONParser();
            JSONObject arguments = (JSONObject) parser.parse(params);
            String newDatasetOutputName = arguments.get("OUTPUT_NAME").toString();

            Command[] commands = {

                // Clip to a bounding box
                () -> {
                    ExternalCommand clipCommand = clipDatasetCommandFactory.build(params, this.getClass());
                    return externalCommandManager.exec(jobId, clipCommand);
                },

                () -> {
                    ExternalCommand exportRenderDBCommand = exportRenderDBCommandFactory.build(newDatasetOutputName, this.getClass());
                    return externalCommandManager.exec(jobId, exportRenderDBCommand);
                }
            };

            jobProcessor.process(new Job(jobId, commands));
        }
        catch (Exception e) {
            String msg = "Error processing dataset clipping request!";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }
}
