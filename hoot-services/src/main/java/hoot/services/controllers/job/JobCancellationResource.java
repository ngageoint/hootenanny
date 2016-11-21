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

import hoot.services.nativeinterfaces.JobExecutionManager;


@Controller
@Path("/cancel")
public class JobCancellationResource {
    private static final Logger logger = LoggerFactory.getLogger(JobCancellationResource.class);

    @Autowired
    private JobExecutionManager jobExecManager;

    @Autowired
    private JobStatusManager jobStatusManager;


    public JobCancellationResource() {}

    /**
     * Cancel job.
     * 
     * @param args
     *            - json containing following parameters jobid: Target job id; mapid: Target map id.
     * 
     *            Example: {"jobid":"123", "mapid":"45"}
     * 
     * @return json containing cancel job id Example: {"jobid":"321"}
     */
    @POST
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response process(String args) {
        String jobIdToCancel = null;
        String mapDisplayName = null;

        try {
            JSONParser parser = new JSONParser();
            JSONObject command = (JSONObject) parser.parse(args);
            jobIdToCancel = command.get("jobid").toString();
            mapDisplayName = command.get("mapid").toString();

            this.jobExecManager.terminate(jobIdToCancel);
            this.jobStatusManager.setCancelled(jobIdToCancel);

            // TODO: should be trying to cleanup any files DB data already created by the cancelled job?
            // TODO: Is this where mapId could come in handy?
        }
        catch (Exception e) {
            String msg = "Error cancellating job with ID = " + jobIdToCancel;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobIdToCancel);

        return Response.ok(json.toJSONString()).build();
    }
}
