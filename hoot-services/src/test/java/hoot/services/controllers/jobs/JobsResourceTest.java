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
 * @copyright Copyright (C) 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.jobs;

import static hoot.services.job.JobStatus.CANCELLED;
import static hoot.services.job.JobStatus.COMPLETE;
import static hoot.services.job.JobStatus.FAILED;
import static hoot.services.job.JobStatus.RUNNING;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
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
    public final String prefix = "test-JobsResourceTest-";
    final int jobTypeCount = JobType.values().length;
    final int totalJobs = jobTypeCount * 4;

    @Before
    public void before() throws Exception {
        // Truncate job status table:
        createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith(prefix)).execute();

        final long now = System.currentTimeMillis();
        final String jobIdPrefix = prefix + now + "-";

        for (int i = 0; i < totalJobs; i++) {

            JobStatus jobStatus = new JobStatus();
            jobStatus.setJobId(jobIdPrefix + i);
            jobStatus.setJobType(i % jobTypeCount);
            jobStatus.setUserId(Users.TEST_USER.getId());
            if (i % jobTypeCount == 0) {
                jobStatus.setStatus(RUNNING.ordinal());
                jobStatus.setStatusDetail("RUNNING");
                jobStatus.setPercentComplete(50);
                //make two running jobs owned by someone else
                if (i <= 20 ) jobStatus.setUserId(-13L);
            } else {
                jobStatus.setStatus(COMPLETE.ordinal());
                jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
                jobStatus.setPercentComplete(100);
                //make 4 history jobs owned by someone else
                if (i % jobTypeCount == 9) jobStatus.setUserId(-13L);
                //make 4 history jobs failed
                if (i % jobTypeCount == 11) jobStatus.setStatus(FAILED.ordinal());
                //make 4 history jobs cancelled
                if (i % jobTypeCount == 13) jobStatus.setStatus(CANCELLED.ordinal());
            }

            Timestamp ts = new Timestamp(now + i);
            jobStatus.setStart(ts);
            jobStatus.setEnd(new Timestamp(now + i + (jobStatus.getJobType() * 1000)));

            createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();
        }

    }

    @After
    public void after() throws Exception {
        createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.startsWith(prefix)).execute();
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryDefaults() throws Exception {
        Response response = target("/history")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(25, jobs.size()); //default limit
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue(j.getStart() <= previous.getStart());
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryLimit() throws Exception {
        Response response = target("/history")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals((totalJobs-8), jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue(j.getStart() <= previous.getStart());
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryLimitOffset() throws Exception {
        Response response = target("/history")
                .queryParam("limit", totalJobs)
                .queryParam("offset", 25)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(23, jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue(j.getStart() <= previous.getStart());
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryOffset() throws Exception {
        Response response = target("/history")
                .queryParam("offset", 25)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(23, jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue(j.getStart() <= previous.getStart());
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortStartAsc() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "+start")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(25, jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue(j.getStart() >= previous.getStart());
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortDurationDesc() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "-duration")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(25, jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue((j.getEnd() - j.getStart()) <= (previous.getEnd() - previous.getStart()));
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortDurationAsc() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "+duration")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(25, jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        JobStatusResponse previous = null;
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            if (previous != null) {
                Assert.assertTrue((j.getEnd() - j.getStart()) >= (previous.getEnd() - previous.getStart()));
            }
            previous = j;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortTypeAscWithLimit() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "+type")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals((totalJobs-8), jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
        }
        Assert.assertEquals(JobType.EXPORT.toString(), jobs.get(0).getJobType());
        Assert.assertEquals(JobType.values()[JobType.values().length-1].toString(), jobs.get(jobs.size() - 1).getJobType());
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortTypeDescWithLimit() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "-type")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals((totalJobs-8), jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
        }
        Assert.assertEquals(JobType.values()[JobType.values().length-1].toString(), jobs.get(0).getJobType());
        Assert.assertEquals(JobType.EXPORT.toString(), jobs.get(jobs.size() - 1).getJobType());
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortStatusAscWithLimit() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "+status")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals((totalJobs-8), jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
        }
        //RUNNING is the first job status enum but those filtered out in job history
        Assert.assertEquals(hoot.services.job.JobStatus.COMPLETE.toString(), jobs.get(0).getStatus());
        Assert.assertEquals(hoot.services.job.JobStatus.CANCELLED.toString(), jobs.get(jobs.size() - 1).getStatus());
    }

    @Test
    @Category(UnitTest.class)
    public void testHistorySortStatusDescWithLimit() throws Exception {
        Response response = target("/history")
                .queryParam("sort", "-status")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals((totalJobs-8), jobs.size());
        Assert.assertEquals((totalJobs-8), history.getTotal().longValue()); //non-running total owned by test user
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
        }
        //RUNNING is the first job status enum but those filtered out in job history
        Assert.assertEquals(hoot.services.job.JobStatus.CANCELLED.toString(), jobs.get(0).getStatus());
        Assert.assertEquals(hoot.services.job.JobStatus.COMPLETE.toString(), jobs.get(jobs.size() - 1).getStatus());
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryFilterType() throws Exception {
        Response response = target("/history")
                .queryParam("jobType", "conflate")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(4, jobs.size());
        for (JobStatusResponse j : jobs) {
            Assert.assertFalse(("running").equalsIgnoreCase(j.getStatus()));
            Assert.assertTrue(("conflate").equalsIgnoreCase(j.getJobType()));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryFilterStatus() throws Exception {
        Response response = target("/history")
                .queryParam("status", "cancelled")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(4, jobs.size());
        Assert.assertEquals(4L, history.getTotal().longValue()); //non-running total owned by test user
        for (JobStatusResponse j : jobs) {
            Assert.assertTrue(("cancelled").equalsIgnoreCase(j.getStatus()));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryFilterTypeStatus() throws Exception {
        Response response = target("/history")
                .queryParam("jobType", "export,conflate,clip")
                .queryParam("status", "complete")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        JobHistory history = objectMapper.readValue(actualResult, new TypeReference<JobHistory>(){});
        List<JobStatusResponse> jobs = history.getJobs();
        Assert.assertEquals(12, jobs.size());
        Assert.assertEquals(12L, history.getTotal().longValue()); //non-running total owned by test user
        List<String> expectedJobTypes = new ArrayList<>();
        expectedJobTypes.add("export");
        expectedJobTypes.add("conflate");
        expectedJobTypes.add("clip");
        for (JobStatusResponse j : jobs) {
            Assert.assertTrue(("complete").equalsIgnoreCase(j.getStatus()));
            Assert.assertTrue((expectedJobTypes).contains(j.getJobType()));
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryFilterStatusBad() throws Exception {
        Response response = target("/history")
                .queryParam("status", "foo")
                .queryParam("limit", totalJobs)
                .request(MediaType.APPLICATION_JSON)
                .get();
        Assert.assertEquals(400, response.getStatus());
    }

    @Test
    @Category(UnitTest.class)
    public void testHistoryFilterTypeBad() throws Exception {
        Response response = target("/history")
                .queryParam("jobType", "bar")
                .request(MediaType.APPLICATION_JSON)
                .get();
        Assert.assertEquals(400, response.getStatus());
    }

    @Test
    @Category(UnitTest.class)
    public void testRunning() throws Exception {
        Response response = target("/running")
                .request(MediaType.APPLICATION_JSON)
                .get();
        String actualResult = response.readEntity(String.class);
        ObjectMapper objectMapper = new ObjectMapper();
        List<JobStatusResponse> jobs = objectMapper.readValue(actualResult, new TypeReference<List<JobStatusResponse>>(){});
        Assert.assertEquals(4, jobs.size());
        for (JobStatusResponse j : jobs) {
            Assert.assertNull(j.getStatus());
            Assert.assertNotNull(j.getPercentComplete());
        }
    }

}
