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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.jobs;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobStatusResponse;
import hoot.services.jobs.JobsStatusesManager;
import hoot.services.models.db.Users;

@Controller
@Path("")
@Transactional
public class JobsResource {
    private static final Logger logger = LoggerFactory.getLogger(JobsResource.class);

    @Autowired
    private JobsStatusesManager jobsStatusesManager;


    public JobsResource() {}

    /**
     * This service allows for tracking the status of running Hootenanny jobs launched all users.
     *
     * GET hoot-services/jobs/running
     *
     * @return job status JSON
     */
    @GET
    @Path("/running")
    @Produces(MediaType.APPLICATION_JSON)
    public List<JobStatusResponse> getRunningJobs() {
        return jobsStatusesManager.getRunningJobs();
    }

    /**
     * This service allows for tracking the status of a users Hootenanny job history.
     *
     * GET hoot-services/jobs/history
     *
     * @return job status JSON
     */
    @GET
    @Path("/history")
    @Produces(MediaType.APPLICATION_JSON)
    public JobHistory getHistoryJobs(@Context HttpServletRequest request,
            @QueryParam("sort") @DefaultValue("") String sort,
            @QueryParam("offset") @DefaultValue("0") long offset,
            @QueryParam("limit") @DefaultValue("25") long limit,
            @QueryParam("jobType") @DefaultValue("") String type,
            @QueryParam("status") @DefaultValue("") String status,
            @QueryParam("groupJobId") @DefaultValue("") String groupJobId) {
        Users user = Users.fromRequest(request);
        try {
            return jobsStatusesManager.getJobsHistory(user, sort, offset, limit, type, status, groupJobId);
        } catch (IllegalArgumentException iae) {
            logger.error(iae.getMessage());
            throw new WebApplicationException(iae, Response.status(Response.Status.BAD_REQUEST).entity(iae.getMessage()).build());
        }
    }


}
