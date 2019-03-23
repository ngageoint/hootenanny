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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.jobs;

import static hoot.services.job.JobStatus.COMPLETE;
import static hoot.services.job.JobStatus.RUNNING;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import hoot.services.UnitTest;
import hoot.services.controllers.job.JobStatusResponse;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.job.JobType;
import hoot.services.models.db.JobStatus;
import hoot.services.models.db.QJobStatus;
import hoot.services.models.db.Users;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class JobsResourceTest extends HootServicesJerseyTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testRecent() throws Exception {
        // Truncate job status table:
        createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        final long now = System.currentTimeMillis();
        final String jobIdPrefix = "test-JobsResourceTest-testRecent-" + now + "-";
        try {
            for(int i = 0; i < 20; i++) {

                    JobStatus jobStatus = new JobStatus();
                    jobStatus.setJobId(jobIdPrefix + i);
                    jobStatus.setJobType(JobType.UNKNOWN.ordinal());
                    jobStatus.setStatus(COMPLETE.ordinal());
                    jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                    jobStatus.setPercentComplete(100.0);
                    jobStatus.setUserId(Users.TEST_USER.getId());

                    Timestamp ts = new Timestamp(System.currentTimeMillis());
                    jobStatus.setStart(ts);
                    jobStatus.setEnd(new Timestamp(System.currentTimeMillis() + 1000));

                    createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
            }

            Response response = target("/recent")
                    .request(MediaType.APPLICATION_JSON)
                    .get();
            String actualResult = response.readEntity(String.class);
            ObjectMapper objectMapper = new ObjectMapper();
            List<JobStatusResponse> jobs = objectMapper.readValue(actualResult, new TypeReference<List<JobStatusResponse>>(){});
            Assert.assertEquals(20, jobs.size());
            for(JobStatusResponse j : jobs) {
                Assert.assertTrue(j.getJobId().startsWith(jobIdPrefix));
            }
        } finally {
            createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        }


    }

    @Test
    @Category(UnitTest.class)
    public void testRecentOlder() throws Exception {
        // Truncate job status table:
        createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        final long now = System.currentTimeMillis();
        final long old = System.currentTimeMillis() - 3600000 * 72;
        final String jobIdPrefix = "test-JobsResourceTest-testRecentOlder-" + now + "-";
        try {
            for(int i = 0; i < 5; i++) {

                    JobStatus jobStatus = new JobStatus();
                    jobStatus.setJobId(jobIdPrefix + i);
                    jobStatus.setJobType(JobType.UNKNOWN.ordinal());
                    jobStatus.setStatus(COMPLETE.ordinal());
                    jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                    jobStatus.setPercentComplete(100.0);
                    jobStatus.setUserId(Users.TEST_USER.getId());

                    Timestamp ts = new Timestamp(now);
                    jobStatus.setStart(ts);
                    jobStatus.setEnd(new Timestamp(now + 1000));

                    createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
            }
            // Add 15 jobs that are older than 12 hours:
            for(int i = 5; i < 20; i++) {

                JobStatus jobStatus = new JobStatus();
                jobStatus.setJobId(jobIdPrefix + i);
                jobStatus.setJobType(JobType.UNKNOWN.ordinal());
                jobStatus.setStatus(COMPLETE.ordinal());
                jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                jobStatus.setPercentComplete(100.0);
                jobStatus.setUserId(Users.TEST_USER.getId());

                Timestamp ts = new Timestamp(old);
                jobStatus.setStart(ts);
                jobStatus.setEnd(new Timestamp(old + 1000));

                createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
            }


            Response response = target("/recent")
                    .request(MediaType.APPLICATION_JSON)
                    .get();
            String actualResult = response.readEntity(String.class);
            ObjectMapper objectMapper = new ObjectMapper();
            List<JobStatusResponse> jobs = objectMapper.readValue(actualResult, new TypeReference<List<JobStatusResponse>>(){});
            Assert.assertEquals(10, jobs.size());
            for(JobStatusResponse j : jobs) {
                Assert.assertTrue(j.getJobId().startsWith(jobIdPrefix));
            }
        } finally {
            createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testRecentOlderRunning() throws Exception {
        // Truncate job status table:
        createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        final long now = System.currentTimeMillis();
        final long old = System.currentTimeMillis() - 3600000 * 72;
        final String jobIdPrefix = "test-JobsResourceTest-testRecentOlder-" + now + "-";
        try {
            for(int i = 0; i < 5; i++) {

                    JobStatus jobStatus = new JobStatus();
                    jobStatus.setJobId(jobIdPrefix + i);
                    jobStatus.setJobType(JobType.UNKNOWN.ordinal());
                    jobStatus.setUserId(Users.TEST_USER.getId());
                    if (i % 2 == 0) {
                        jobStatus.setStatus(COMPLETE.ordinal());
                        jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                        jobStatus.setPercentComplete(100.0);
                    } else {
                        jobStatus.setStatus(RUNNING.ordinal());
                        jobStatus.setStatusDetail("RUNNING");
                        jobStatus.setPercentComplete(50.0);
                    }

                    Timestamp ts = new Timestamp(now);
                    jobStatus.setStart(ts);
                    jobStatus.setEnd(new Timestamp(now + 1000));

                    createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
            }
            // Add 15 jobs that are older than 12 hours but 6 are still running:
            for(int i = 5; i < 20; i++) {

                JobStatus jobStatus = new JobStatus();
                jobStatus.setJobId(jobIdPrefix + i);
                jobStatus.setJobType(JobType.UNKNOWN.ordinal());
                jobStatus.setUserId(Users.TEST_USER.getId());
                if (i % 2 == 0) {
                    jobStatus.setStatus(COMPLETE.ordinal());
                    jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                    jobStatus.setPercentComplete(100.0);
                } else {
                    jobStatus.setStatus(RUNNING.ordinal());
                    jobStatus.setStatusDetail("RUNNING");
                    jobStatus.setPercentComplete(50.0);
                }

                Timestamp ts = new Timestamp(old);
                jobStatus.setStart(ts);
                jobStatus.setEnd(new Timestamp(old + 1000));

                createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
            }


            Response response = target("/recent")
                    .request(MediaType.APPLICATION_JSON)
                    .get();
            String actualResult = response.readEntity(String.class);
            ObjectMapper objectMapper = new ObjectMapper();
            List<JobStatusResponse> jobs = objectMapper.readValue(actualResult, new TypeReference<List<JobStatusResponse>>(){});
            Assert.assertEquals(13, jobs.size());
            for(JobStatusResponse j : jobs) {
                Assert.assertTrue(j.getJobId().startsWith(jobIdPrefix));
            }
        } finally {
            createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith("test")).execute();
        }
    }

}

