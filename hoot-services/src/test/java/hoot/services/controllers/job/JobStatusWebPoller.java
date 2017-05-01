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
package hoot.services.controllers.job;

import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;

import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.MediaType;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.junit.Assert;
import org.springframework.beans.BeansException;

import hoot.services.job.JobStatus;
import hoot.services.models.db.QJobStatus;


/**
 * Utility class for polling hoot service jobs in unit tests
 */
public class JobStatusWebPoller {

    private final WebTarget webTarget;
    private final Connection conn;
    private final int jobStatusPollDelayMs;

    public JobStatusWebPoller(WebTarget webTarget, Connection conn) {
        this.webTarget = webTarget;
        this.conn = conn;

        // increase this to something long when debugging in debugger to the
        // poller from polling and changing program flow while you're trying to debug service code.
        // Amount of time in ms the job status polling class used only in test code waits before attempting
        // to determine the status of a running job; 250 is a good value for development environments and
        // prevents the services tests from taking a long time to run; may need to increase to as much as
        // 2000 or more in some continuous integration environments where postgres seems to respond more
        // slowly to queries
        this.jobStatusPollDelayMs = 250;
    }

    /**
     * Queries the job service for a job's status
     *
     * @param jobId
     *            ID of the job to query
     * @return a hoot job status
     * @throws ParseException
     */
    private JobStatus pollJobStatus(String jobId) throws ParseException {
        String response = webTarget.path("status/" + jobId).request(MediaType.APPLICATION_JSON).get(String.class);
        JSONObject responseObj = (JSONObject) (new JSONParser()).parse(response);
        assert (responseObj.get("jobId").equals(jobId));
        return JobStatus.valueOf(((String) responseObj.get("status")).toUpperCase());
    }

    /**
     * Queries the job service for a job's status
     *
     * @param jobId
     *            ID of the job to query
     * @return a hoot job status object
     * @throws ParseException
     */
    private JSONObject pollJobStatusObj(String jobId) throws ParseException {
        String response = webTarget.path("status/" + jobId).request(MediaType.APPLICATION_JSON).get(String.class);
        JSONObject responseObj = (JSONObject) (new JSONParser()).parse(response);
        assert (responseObj.get("jobId").equals(jobId));
        return responseObj;
    }

    /**
     * Queries the job service for a job's status until the job completes or
     * fails
     *
     * @param jobId
     *            ID of the job to query
     * @param jobShouldFail
     *            if set to true; method expects job to fail and behaves
     *            accordingly; otherwise, does not expect it to fail
     * @throws Exception
     * @throws BeansException
     */
    public void pollJobStatusUntilCompleteOrFail(String jobId, boolean jobShouldFail) throws Exception {
        JobStatus jobStatus = JobStatus.UNKNOWN;
        while ((jobStatus == JobStatus.UNKNOWN) || (jobStatus == JobStatus.RUNNING)) {
            Thread.sleep(jobStatusPollDelayMs);
            JSONObject jobStatusObj = pollJobStatusObj(jobId);
            jobStatus = JobStatus.valueOf(((String) jobStatusObj.get("status")).toUpperCase());
            if (jobStatus == JobStatus.FAILED) {
                if (jobShouldFail) {
                    verifyJobStatusInDb(jobId, JobStatus.FAILED);
                }
                else {
                    Assert.fail("Job failed: " + jobStatusObj.get("statusDetail"));
                }
            }
        }
        if (!jobShouldFail) {
            Assert.assertEquals(JobStatus.COMPLETE, jobStatus);
            verifyJobStatusInDb(jobId, JobStatus.COMPLETE);
        }
    }

    /**
     * Unit test method which verifies the status of a particular job in the
     * services database
     * <p>
     * Method has test assertions which fail if the job does not exist, does not
     * have the specified status, or has improperly marked timestamps.
     *
     * @param jobId
     *            ID of the job to verify
     * @param status
     *            status the job should have in the database
     */
    private void verifyJobStatusInDb(String jobId, JobStatus status) {
        QJobStatus jobStatus = QJobStatus.jobStatus;
        hoot.services.models.db.JobStatus finalJobStatus = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(jobStatus.jobId.eq(jobId))
                .fetchOne();

        Assert.assertNotNull(finalJobStatus);
        Assert.assertEquals(jobId, finalJobStatus.getJobId());
        Assert.assertEquals((Integer) status.ordinal(), finalJobStatus.getStatus());
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        Assert.assertTrue(finalJobStatus.getStart().before(now));
        Assert.assertTrue(finalJobStatus.getEnd().before(now));
    }
}
