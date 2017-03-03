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


import static hoot.services.job.JobStatusManager.JOB_STATUS.COMPLETE;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.sql.Timestamp;
import java.util.UUID;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.models.db.CommandStatus;
import hoot.services.models.db.JobStatus;
import hoot.services.models.db.QCommandStatus;
import hoot.services.models.db.QJobStatus;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class JobResourceTest extends HootServicesJerseyTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testJobIdStatusUNKNOWN() throws Exception {
        JobResource jobResource = new JobResource();
        String jobId = UUID.randomUUID().toString();

        Response response = target("/status/" + jobId)
                .queryParam("includeCommandDetail", false)
                .request(MediaType.APPLICATION_JSON)
                .get();

        String expectedResult = "{\"jobId\":\"" + jobId + "\",\"status\":\"unknown\"}";
        String actualResult = response.readEntity(String.class);

        assertEquals(expectedResult, actualResult);
    }

    @Test
    @Category(UnitTest.class)
    public void testJobStatusWithoutCommandDetail() {
        String jobId = UUID.randomUUID().toString();

        try {
            JobStatus jobStatus = new JobStatus();
            jobStatus.setJobId(jobId);
            jobStatus.setStatus(COMPLETE.ordinal());
            jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
            jobStatus.setPercentComplete(100.0);

            Timestamp ts = new Timestamp(System.currentTimeMillis());
            jobStatus.setStart(ts);
            jobStatus.setEnd(new Timestamp(System.currentTimeMillis() + 1000));

            createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();

            CommandStatus listCommandStatus = new CommandStatus();
            listCommandStatus.setCommand("ls -la command");
            listCommandStatus.setExitCode(0);
            listCommandStatus.setStart(new Timestamp(System.currentTimeMillis()));
            listCommandStatus.setFinish(new Timestamp(System.currentTimeMillis() + 1000));
            listCommandStatus.setJobId(jobId);
            listCommandStatus.setStderr("Some stderr output of the ls command");
            listCommandStatus.setStdout("Some stdout output of the ls command");

            createQuery().insert(QCommandStatus.commandStatus).populate(listCommandStatus).execute();

            CommandStatus rmCommandStatus = new CommandStatus();
            rmCommandStatus.setCommand("rm command");
            rmCommandStatus.setExitCode(0);
            rmCommandStatus.setStart(new Timestamp(System.currentTimeMillis()));
            rmCommandStatus.setFinish(new Timestamp(System.currentTimeMillis() + 2000));
            rmCommandStatus.setJobId(jobId);
            rmCommandStatus.setStderr("Some stderr output of the rm command");
            rmCommandStatus.setStdout("Some stdout output of the rm command");

            createQuery().insert(QCommandStatus.commandStatus).populate(rmCommandStatus).execute();

            Response response = target("/status/" + jobId)
                    .queryParam("includeCommandDetail", false)
                    .request(MediaType.APPLICATION_JSON)
                    .get();

            String expectedResult =
                    "{\"jobId\":\"" + jobId + "\"," +
                            "\"lasttext\":\"FINISHED SUCCESSFULLY\"," +
                            "\"percentcomplete\":100.0," +
                            "\"statusDetail\":\"FINISHED SUCCESSFULLY\"," +
                            "\"status\":\"complete\"}";

            String actualResult = response.readEntity(String.class);

            assertEquals(expectedResult, actualResult);
        }
        finally {
            createQuery().delete(QCommandStatus.commandStatus).where(QCommandStatus.commandStatus.jobId.eq(jobId)).execute();
            createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.eq(jobId)).execute();
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testJobStatusWithCommandDetail() {
        String jobId = UUID.randomUUID().toString();

        try {
            JobStatus jobStatus = new JobStatus();
            jobStatus.setJobId(jobId);
            jobStatus.setStatus(COMPLETE.ordinal());
            jobStatus.setStatusDetail("FINISHED SUCCESSFULLY");
            jobStatus.setPercentComplete(100.0);

            Timestamp ts = new Timestamp(System.currentTimeMillis());
            jobStatus.setStart(ts);
            jobStatus.setEnd(new Timestamp(System.currentTimeMillis() + 1000));

            createQuery().insert(QJobStatus.jobStatus).populate(jobStatus).execute();

            CommandStatus listCommandStatus = new CommandStatus();
            listCommandStatus.setCommand("ls -la command");
            listCommandStatus.setExitCode(0);
            listCommandStatus.setStart(new Timestamp(System.currentTimeMillis()));
            listCommandStatus.setFinish(new Timestamp(System.currentTimeMillis() + 1000));
            listCommandStatus.setJobId(jobId);
            listCommandStatus.setStderr("Some stderr output of the ls command");
            listCommandStatus.setStdout("Some stdout output of the ls command");

            createQuery().insert(QCommandStatus.commandStatus).populate(listCommandStatus).execute();

            CommandStatus rmCommandStatus = new CommandStatus();
            rmCommandStatus.setCommand("rm command");
            rmCommandStatus.setExitCode(0);
            rmCommandStatus.setStart(new Timestamp(System.currentTimeMillis()));
            rmCommandStatus.setFinish(new Timestamp(System.currentTimeMillis() + 2000));
            rmCommandStatus.setJobId(jobId);
            rmCommandStatus.setStderr("Some stderr output of the rm command");
            rmCommandStatus.setStdout("Some stdout output of the rm command");

            createQuery().insert(QCommandStatus.commandStatus).populate(rmCommandStatus).execute();

            Response response = target("/status/" + jobId)
                    .queryParam("includeCommandDetail", true)
                    .request(MediaType.APPLICATION_JSON)
                    .get();

            String actualResult = response.readEntity(String.class);
            assertTrue(actualResult.contains("ls -la command"));
            assertTrue(actualResult.contains("Some stderr output of the ls command"));
            assertTrue(actualResult.contains("Some stdout output of the ls command"));

            assertTrue(actualResult.contains("rm command"));
            assertTrue(actualResult.contains("Some stderr output of the rm command"));
            assertTrue(actualResult.contains("Some stdout output of the rm command"));
        }
        finally {
            createQuery().delete(QCommandStatus.commandStatus).where(QCommandStatus.commandStatus.jobId.eq(jobId)).execute();
            createQuery().delete(QJobStatus.jobStatus).where(QJobStatus.jobStatus.jobId.eq(jobId)).execute();
        }
    }
}
