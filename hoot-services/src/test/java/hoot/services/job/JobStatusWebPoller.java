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
package hoot.services.job;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;

import javax.ws.rs.core.MediaType;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.junit.Assert;
import org.springframework.beans.BeansException;

import com.mysema.query.sql.SQLQuery;
import com.sun.jersey.api.client.WebResource;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.QJobStatus;
import hoot.services.job.JobStatusManager.JOB_STATUS;


/**
 * Utility class for polling hoot service jobs in unit tests
 */
public class JobStatusWebPoller {
    private WebResource webResource;
    private Connection conn;
    private int jobStatusPollDelayMs;

    public JobStatusWebPoller(final WebResource webResource, Connection conn) throws NumberFormatException {
        this.webResource = webResource;
        this.conn = conn;
        // increase this to something long when debugging in debugger to the
        // poller from polling and changing
        // program flow while you're trying to debug service code
        this.jobStatusPollDelayMs = Integer.parseInt(HootProperties.getPropertyOrDefault("testJobStatusPollerTimeout"));
    }

    /**
     * Queries the job service for a job's status
     *
     * @param jobId
     *            ID of the job to query
     * @return a hoot job status
     * @throws ParseException
     */
    @SuppressWarnings("unused")
    private JOB_STATUS pollJobStatus(final String jobId) throws ParseException {
        String response = webResource.path("status/" + jobId).accept(MediaType.APPLICATION_JSON).get(String.class);
        JSONObject responseObj = (JSONObject) (new JSONParser()).parse(response);
        assert (responseObj.get("jobId").equals(jobId));
        return JOB_STATUS.fromString((String) responseObj.get("status"));
    }

    /**
     * Queries the job service for a job's status
     *
     * @param jobId
     *            ID of the job to query
     * @return a hoot job status object
     * @throws ParseException
     */
    private JSONObject pollJobStatusObj(final String jobId) throws ParseException {
        String response = webResource.path("status/" + jobId).accept(MediaType.APPLICATION_JSON).get(String.class);
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
    public void pollJobStatusUntilCompleteOrFail(final String jobId, final boolean jobShouldFail)
            throws BeansException, Exception {
        JOB_STATUS jobStatus = JOB_STATUS.UNKNOWN;
        while (jobStatus.equals(JOB_STATUS.UNKNOWN) || jobStatus.equals(JOB_STATUS.RUNNING)) {
            Thread.sleep(jobStatusPollDelayMs);
            JSONObject jobStatusObj = pollJobStatusObj(jobId);
            jobStatus = JOB_STATUS.fromString((String) jobStatusObj.get("status"));
            if (jobStatus.equals(JOB_STATUS.FAILED)) {
                if (jobShouldFail) {
                    verifyJobStatusInDb(jobId, JOB_STATUS.FAILED);
                }
                else {
                    Assert.fail("Job failed: " + jobStatusObj.get("statusDetail"));
                }
            }
        }
        if (!jobShouldFail) {
            Assert.assertEquals(JOB_STATUS.COMPLETE, jobStatus);
            verifyJobStatusInDb(jobId, JOB_STATUS.COMPLETE);
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
    private void verifyJobStatusInDb(final String jobId, final JOB_STATUS status) {
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());
        QJobStatus jobStatus = QJobStatus.jobStatus;

        hoot.services.db2.JobStatus finalJobStatus = query.from(jobStatus).where(jobStatus.jobId.eq(jobId))
                .singleResult(jobStatus);

        Assert.assertNotNull(finalJobStatus);
        Assert.assertEquals(jobId, finalJobStatus.getJobId());
        Assert.assertEquals((Integer) status.toInt(), finalJobStatus.getStatus());
        final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        Assert.assertTrue(finalJobStatus.getStart().before(now));
        Assert.assertTrue(finalJobStatus.getEnd().before(now));
    }
}
