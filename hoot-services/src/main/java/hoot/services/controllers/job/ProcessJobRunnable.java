package hoot.services.controllers.job;

import static hoot.services.utils.JsonUtils.paramsToMap;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.ApplicationContextUtils;
import hoot.services.job.JobStatusManager;
import hoot.services.nativeinterfaces.JobExecutionManager;
import hoot.services.nativeinterfaces.NativeInterfaceException;


class ProcessJobRunnable implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(ProcessJobRunnable.class);

    private final String jobId;
    private final String params;
    private final JobStatusManager jobStatusManager;
    private final JobExecutionManager jobExecMan;


    ProcessJobRunnable(String jobid, String jobParams, JobStatusManager jobStatusManager, JobExecutionManager jobExecMan) {
        this.jobId = jobid;
        this.params = jobParams;
        this.jobStatusManager = jobStatusManager;
        this.jobExecMan = jobExecMan;
    }

    @Override
    public void run() {
        ApplicationContext appContext = ApplicationContextUtils.getApplicationContext();
        PlatformTransactionManager txManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);

        TransactionStatus transactionStatus = null;
        try {
            transactionStatus = txManager.getTransaction(null);
            processCommand();
            txManager.commit(transactionStatus);
        }
        catch (Exception e) {
            if ((transactionStatus != null) && !transactionStatus.isCompleted()) {
                txManager.rollback(transactionStatus);
            }
            logger.error("Error executing job with ID = {}", jobId, e);
        }
    }

    private void processCommand() throws Exception {
        logger.debug("Processing job: {}", jobId);

        jobStatusManager.addJob(jobId);

        JSONObject command = null;
        try {
            JSONParser parser = new JSONParser();
            command = (JSONObject) parser.parse(params);

            //log.debug(JsonUtils.objectToJson(command));
            JSONObject result = processJob(jobId, command);
            //log.debug(JsonUtils.objectToJson(result));

            String warnings = null;
            Object oWarn = result.get("warnings");
            if (oWarn != null) {
                warnings = oWarn.toString();
            }

            String statusDetail = "";
            if (warnings != null) {
                statusDetail += "WARNINGS: " + warnings;
            }

            Map<String, String> params = paramsToMap(command);
            if (params.containsKey("writeStdOutToStatusDetail")
                    && Boolean.parseBoolean(params.get("writeStdOutToStatusDetail"))) {
                statusDetail += "INFO: " + result.get("stdout");
            }

            if (StringUtils.trimToNull(statusDetail) != null) {
                jobStatusManager.setComplete(jobId, statusDetail);
            }
            else {
                jobStatusManager.setComplete(jobId);
            }
        }
        catch (Exception e) {
            jobStatusManager.setFailed(jobId, e.getMessage());
            throw e;
        }
        finally {
            logger.debug("End process Job: {}", jobId);
        }
    }

    private JSONObject processJob(String jobId, JSONObject command) throws NativeInterfaceException {
        logger.debug("processing Job: {}", jobId);

        command.put("jobId", jobId);

        String resourceName = command.get("caller").toString();
        JobFieldsValidator validator = new JobFieldsValidator(resourceName);

        Map<String, String> paramsMap = paramsToMap(command);

        List<String> missingList = new ArrayList<>();
        if (!validator.validateRequiredExists(paramsMap, missingList)) {
            logger.error("Missing following required field(s): {}", missingList);
        }

        logger.debug("calling native request Job: {}", jobId);

        return jobExecMan.exec(command);
    }
}
