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

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.ApplicationContextUtils;
import hoot.services.nativeinterfaces.CommandResult;


class ProcessChainJobRunnable implements Runnable {
    private static final Logger logger = LoggerFactory.getLogger(ProcessChainJobRunnable.class);

    private final String jobId;
    private final Command[] commands;
    private final JobStatusManager jobStatusManager;


    ProcessChainJobRunnable(String jobId, Command[] commands, JobStatusManager jobStatusManager) {
        this.jobId = jobId;
        this.commands = commands;
        this.jobStatusManager = jobStatusManager;
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

    private void processCommand() {
        logger.debug("Start processing chain Job with jobId = {}", jobId);

        jobStatusManager.addJob(jobId);

        try {
            int jobNumber = 0;
            for (Command command : commands) {
                CommandResult result = command.execute();

                if (result.failed()) {
                    jobStatusManager.setFailed(jobId, "Chain job FAILED due to --> " + result.getCommand());
                    break;
                }
                else {
                    jobNumber++;
                    Double percentComplete = ((jobNumber * 100.0d) / commands.length);
                    jobStatusManager.updateJob(jobId, null, percentComplete);
                }
            }

            jobStatusManager.setCompleted(jobId, "PROCESSED");
        }
        catch (Exception e) {
            jobStatusManager.setFailed(jobId,
                    "Chain job FAILED with the following error --> " + ExceptionUtils.getStackTrace(e));
            throw e;
        }

        logger.debug("End processing chain Job with jobId = {}", jobId);
    }
}
