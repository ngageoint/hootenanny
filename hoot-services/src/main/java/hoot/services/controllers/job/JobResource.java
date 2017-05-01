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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import java.util.List;

import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.job.JobStatus;
import hoot.services.job.JobStatusManager;
import hoot.services.models.db.CommandStatus;


@Controller
@Path("")
@Transactional
public class JobResource {
    private static final Logger logger = LoggerFactory.getLogger(JobResource.class);

    @Autowired
    private JobStatusManager jobStatusManager;


    public JobResource() {}

    /**
     * This service allows for tracking the status of Hootenanny jobs launched by other web services.
     *
     * GET hoot-services/job/status/{Job Id}
     * 
     * @param jobId
     *            ID of the job to track.
     *
     * @param includeCommandDetail
     *            flag to signal whether to include command level detail.
     *
     * @return job status JSON
     */
    @GET
    @Path("/status/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public JobStatusResponse getJobStatus(@PathParam("jobId") String jobId,
                                          @QueryParam("includeCommandDetail") @DefaultValue("false") Boolean includeCommandDetail) {
        JobStatusResponse response = new JobStatusResponse();

        try {
            hoot.services.models.db.JobStatus jobStatus = this.jobStatusManager.getJobStatusObj(jobId);

            if (jobStatus != null) {
                response.setJobId(jobId);
                response.setStatus(JobStatus.fromInteger(jobStatus.getStatus()).toString());
                response.setStatusDetail(jobStatus.getStatusDetail());
                response.setPercentComplete(jobStatus.getPercentComplete());
                response.setLastText(jobStatus.getStatusDetail());

                if (includeCommandDetail) {
                    List<CommandStatus> commandDetail = this.jobStatusManager.getCommandDetail(jobId);
                    response.setCommandDetail(commandDetail);
                }
            }
            else {
                response.setJobId(jobId);
                response.setStatus(JobStatus.UNKNOWN.toString());
            }
        }
        catch (Exception ex) {
            String msg = "Error retrieving job status for job with ID = " + jobId + ".  Cause: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return response;
    }
}
