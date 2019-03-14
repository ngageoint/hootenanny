package hoot.services.job;

import static hoot.services.models.db.QCommandStatus.commandStatus;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.assertEquals;

import java.sql.Timestamp;
import java.util.List;
import java.util.UUID;

import org.junit.Before;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.ApplicationContextUtils;
import hoot.services.UnitTest;
import hoot.services.command.Command;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.models.db.CommandStatus;
import hoot.services.utils.MapUtils;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class JobStatusManagerImplTest {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Autowired
    private ApplicationContext applicationContext;

    @Before
    public void setAppContext() {
        new ApplicationContextUtils().setApplicationContext(applicationContext);
    }

    @Test
    @Category(UnitTest.class)
    @Transactional
    public void testJobErrors() throws Exception {
        long userId = MapUtils.insertUser();

        JobStatusManager jobMgr = new JobStatusManagerImpl();
        String jobId = UUID.randomUUID().toString();
        Command[] comms = {};
        jobMgr.addJob(new Job(jobId, userId, comms, JobType.UNKNOWN));

        CommandStatus cmdStatus = new CommandStatus();
        cmdStatus.setCommand("hoot version");
        cmdStatus.setExitCode(-1);
        cmdStatus.setFinish(new Timestamp(System.currentTimeMillis()));
        cmdStatus.setStart(new Timestamp(System.currentTimeMillis() + 1000));
        cmdStatus.setJobId(jobId);
        String expectedError = "This command returned error.\nNo such command.";
        cmdStatus.setStderr(expectedError);
        cmdStatus.setStdout("Running hoot version command...");

        Long id = createQuery().insert(commandStatus).populate(cmdStatus).executeWithKey(commandStatus.id);

        List<String> errors = jobMgr.getJobErrors(jobId, userId);

        assertEquals(1, errors.size());
        assertEquals(expectedError, errors.get(0));
    }
}
