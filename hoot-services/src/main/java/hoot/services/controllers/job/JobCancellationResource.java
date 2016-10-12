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
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;


@Controller
@Path("/cancel")
public class JobCancellationResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(JobCancellationResource.class);

    public JobCancellationResource() {
        super(null);
    }

    // TODO: Review this controller to make sure that cancellation logic works correctly

    /**
     * Cancel job.
     * 
     * @param args
     *            - json containing following parameters jobid: Target job id
     *            mpaid: Target map id.
     * 
     *            Example: {"jobid":"123", "mapid":"45"}
     * 
     * @return json containing cancel job id Example: {"jobid":"321"}
     */
    @POST
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response process(String args) {
        String jobId = UUID.randomUUID().toString();

        try {
            JSONParser parser = new JSONParser();
            JSONObject command = (JSONObject) parser.parse(args);
            String cancelJobId = command.get("jobid").toString();
            String cancelMapId = command.get("mapid").toString();

            JSONArray cancelArgs = new JSONArray();

            JSONObject param = new JSONObject();
            param.put("value", cancelJobId);
            param.put("paramtype", String.class.getName());
            param.put("isprimitivetype", "false");
            cancelArgs.add(param);

            param = new JSONObject();
            param.put("value", cancelMapId);
            param.put("paramtype", String.class.getName());
            param.put("isprimitivetype", "false");
            cancelArgs.add(param);

            JSONObject jobCancellationCommand = createReflectionJobReq(cancelArgs,
                    JobResource.class.getName(), "terminateJob");

            JSONArray jobArgs = new JSONArray();
            jobArgs.add(jobCancellationCommand);

            postChainJobRequest(jobId, jobArgs.toJSONString());
        }
        catch (Exception ex) {
            String msg = "Error process data clean request: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        JSONObject res = new JSONObject();
        res.put("jobid", jobId);

        return Response.ok(res.toJSONString()).build();
    }
}
