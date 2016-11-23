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

import static hoot.services.HootProperties.INTERNAL_JOB_THREAD_SIZE;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
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
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.db.JobStatus;
import hoot.services.nativeinterfaces.JobExecutionManager;
import hoot.services.nativeinterfaces.NativeInterfaceException;


/**
 * @author Jong Choi
 *
 *         Servlet class for handling job execution internally.
 *
 */
@Controller
@Path("")
@Transactional
public class JobResource {
    private static final Logger logger = LoggerFactory.getLogger(JobResource.class);

    // Thread pool for chain and job processing
    private static final ExecutorService jobThreadExecutor =
            Executors.newFixedThreadPool(Integer.parseInt(INTERNAL_JOB_THREAD_SIZE));

    @Autowired
    private JobExecutionManager jobExecMan;

    @Autowired
    private JobStatusManager jobStatusManager;


    public JobResource() {}

    @POST
    @Path("/chain/{jobid}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processChainJob(@PathParam("jobid") String jobId, String jobs) {
        logger.debug("Curent JobResource thread count:{}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());

        try {
            Runnable chainJobWorker = new ProcessChainJobRunnable(jobId, jobs, jobStatusManager, jobExecMan);
            jobThreadExecutor.execute(chainJobWorker);
        }
        catch (Exception e) {
            String msg = "Error during processing of a chain job!" + " jobId = " + jobId + ", jobs = " + jobs;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok().build();
    }

    @POST
    @Path("/{jobid}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response processJob(@PathParam("jobid") String jobId, String params) {
        logger.debug("Curent JobResource thread count:{}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());

        try {
            Runnable jobWorker = new ProcessJobRunnable(jobId, params, jobStatusManager, jobExecMan);
            jobThreadExecutor.execute(jobWorker);
        }
        catch (Exception e) {
            String msg = "Error during processing a job with jobId = " + jobId + ", params = " + params;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok().build();
    }

    /**
     * This service allows for executing Hootenanny tasks and tracking the
     * status of Hootenanny jobs launched by other web services. Not all
     * Hootenanny web services create jobs which can be tracked by this service.
     * 
     * GET hoot-services/job/status/{Job Id}
     * 
     * @param jobId
     *            id of the job to track
     * @return job status JSON
     */
    @GET
    @Path("/status/{jobId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getJobStatus(@PathParam("jobId") String jobId) {
        String jobStatus = "unknown";
        try {
            JSONObject status = getJobStatusObj(jobId);

            if (status != null) {
                try {
                    String detail = (status.get("statusDetail") != null) ? status.get("statusDetail").toString() : null;
                    if ((detail != null) && (!detail.trim().isEmpty()) && detail.trim().startsWith("{")) {
                        JSONParser p = new JSONParser();
                        JSONObject stat = (JSONObject) p.parse(detail);

                        // for now we will check for chain job only since ogr2osm is called for such job
                        if (stat.get("chainjobstatus") != null) {
                            String strCnt = stat.get("childrencount").toString();
                            int nCnt = Integer.parseInt(strCnt);
                            int maxPartPercent = 100 / nCnt;

                            int nComplete = 0;

                            Map<String, Integer> progMap = new LinkedHashMap<>();

                            JSONArray children = (JSONArray) stat.get("children");

                            String lastLine = "";
                            for (Object aChildren : children) {
                                JSONObject child = (JSONObject) aChildren;
                                String childId = child.get("id").toString();
                                if (child.get("status").toString().endsWith("running")) {

                                    String progress = getProgressText(childId);
                                    String[] lines = progress.split("\n");

                                    Integer childProg = progMap.get(childId);
                                    if (childProg == null) {
                                        childProg = 0;
                                        progMap.put(childId, 0);
                                    }
                                    for (String line : lines) {
                                        String currLine = line.trim();
                                        if (currLine.startsWith("{")) {
                                            JSONObject progInfo = (JSONObject) p.parse(currLine);
                                            JSONObject progStatus = (JSONObject) progInfo.get("status");
                                            if (progStatus.get("jobFinished").toString().equals("true")) {
                                                // If completed before we can get the progress info.
                                                // (This happens because the job status ping time may
                                                // be longer than the whole job)
                                                childProg = maxPartPercent;
                                            }
                                            else {
                                                long currPerc = (Long) progStatus.get("percentComplete");
                                                childProg = (int) (maxPartPercent * ((double) currPerc / 100));
                                            }
                                        }
                                        progMap.put(childId, childProg);
                                        lastLine = currLine;
                                    }
                                }
                                else {
                                    progMap.put(childId, maxPartPercent);
                                    nComplete++;
                                }
                            }

                            int totalPercent = 0;
                            if (nComplete == nCnt) {
                                totalPercent = 100;
                            }
                            else {
                                for (Map.Entry<String, Integer> entry : progMap.entrySet()) {
                                    totalPercent += entry.getValue();
                                }
                            }

                            status.put("percentcomplete", totalPercent);
                            status.put("lasttext", lastLine);
                        }
                    }
                }
                catch (Exception ex) {
                    // if something goes wrong we will not put progress
                    logger.error("Error during job status retrieval!", ex);
                }

                jobStatus = status.toJSONString();
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String msg = "Error retrieving job status for job: " + jobId + " Error: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return Response.ok(jobStatus).build();
    }

    /**
     * Return job status
     */
    private JSONObject getJobStatusObj(String jobId) {
        JSONObject status = new JSONObject();

        status.put("jobId", jobId);
        JobStatus jobStatusObj = this.jobStatusManager.getJobStatusObj(jobId);
        if (jobStatusObj == null) {
            return null;
        }

        JOB_STATUS stat = JOB_STATUS.fromInteger(jobStatusObj.getStatus());
        if (stat == null) {
            stat = JOB_STATUS.UNKNOWN;
        }

        status.put("status", stat.toString());
        status.put("statusDetail", jobStatusObj.getStatusDetail());

        return status;
    }

    private String getProgressText(String jobId) throws NativeInterfaceException {
        return this.jobExecMan.getProgress(jobId);
    }
}
