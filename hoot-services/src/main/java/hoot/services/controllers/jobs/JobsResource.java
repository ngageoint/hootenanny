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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.jobs;

import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;

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
    private static final int MAX_SIZE = 10;

    @Autowired
    private JobsStatusesManager jobsStatusesManager;


    public JobsResource() {}

    /**
     * This service allows for tracking the status of Hootenanny jobs launched by other web services.
     *
     * GET hoot-services/jobs/recent
     *
     * @return job status JSON
     */
    @GET
    @Path("/recent")
    @Produces(MediaType.APPLICATION_JSON)
    public List<JobStatusResponse> getJobStatus(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);
        return this.jobsStatusesManager.getRecentJobs(user, MAX_SIZE);
    }

}
