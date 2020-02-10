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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.command.ExternalCommandManager;
import hoot.services.job.JobStatusManager;
import hoot.services.models.db.JobStatus;
import hoot.services.models.db.Users;


@Controller
@Path("/cancel")
@Transactional
public class JobCancellationResource {

    @Autowired
    private ExternalCommandManager externalCommandInterface;

    @Autowired
    private JobStatusManager jobStatusManager;


    public JobCancellationResource() {}

    /**
     * Cancel job.
     *
     * @param params
     *            - json containing following parameters jobid: Target job id; mapid: Target map id.
     *
     *            Example: {"jobid":"123", "mapid":"45"}
     *
     * @return json containing cancel job id Example: {"jobid":"321"}
     */
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response process(@Context HttpServletRequest request, JobCancellationParams params) {

        Users user = Users.fromRequest(request);

        String jobIdToCancel = params.getJobId();
        JobStatus jobStatus = this.jobStatusManager.getJobStatusObj(jobIdToCancel, user.getId());

        if (jobStatus == null) {
            throw new ForbiddenException("HTTP" /* This Parameter required, but will be cleared by ExceptionFilter */);
        } else {
            try {
                this.jobStatusManager.setCancelled(jobIdToCancel, "Cancelled by user!");
                this.externalCommandInterface.terminate(jobIdToCancel);
            } catch (Exception e) {
                String msg = "Error cancelling job with ID = " + jobIdToCancel;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobIdToCancel);

        return Response.ok(json.toJSONString()).build();
    }

    @GET
    @Path("/{jobId}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response process(@Context HttpServletRequest request, @PathParam("jobId") String jobId) {
    	

        Users user = Users.fromRequest(request);

        JobStatus jobStatus = this.jobStatusManager.getJobStatusObj(jobId, user.getId());

        if (jobStatus == null) {
            throw new ForbiddenException("HTTP" /* This Parameter required, but will be cleared by ExceptionFilter */);
        } else {
            try {
                this.jobStatusManager.setCancelled(jobId, "Cancelled by user!");
                this.externalCommandInterface.terminate(jobId);
            } catch (Exception e) {
                String msg = "Error cancelling job with ID = " + jobId;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        }

        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }

}
