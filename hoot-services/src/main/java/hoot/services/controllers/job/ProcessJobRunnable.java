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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.job;

import static hoot.services.utils.JsonUtils.paramsToMap;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.ApplicationContextUtils;
import hoot.services.nativeinterfaces.CommandResult;
import hoot.services.nativeinterfaces.JobExecutionManager;


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

        try {
            JSONParser parser = new JSONParser();
            JSONObject command = (JSONObject) parser.parse(params);
            command.put("jobId", jobId);

            CommandResult result = processJob(command);

            if (result.failed()) {
                jobStatusManager.setFailed(jobId, result.getStderr());
            }
            else {
                jobStatusManager.setCompleted(jobId, result.getStdout());
            }
        }
        catch (Exception e) {
            jobStatusManager.setFailed(jobId, ExceptionUtils.getStackTrace(e));
            throw e;
        }
        finally {
            logger.debug("End process Job: {}", jobId);
        }
    }

    private CommandResult processJob(JSONObject command) {
        String resourceName = command.get("caller").toString();
        JobFieldsValidator validator = new JobFieldsValidator(resourceName);

        Map<String, String> paramsMap = paramsToMap(command);

        List<String> missingList = new ArrayList<>();
        if (!validator.validateRequiredExists(paramsMap, missingList)) {
            logger.error("Missing following required field(s): {}", missingList);
        }

        return jobExecMan.exec(command);
    }
}
