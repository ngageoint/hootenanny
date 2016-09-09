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

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionStatus;

import hoot.services.ApplicationContextUtils;


/**
 * Thread class for executing async jobs
 */
public class JobExecutioner extends Thread {
    private static final Logger logger = LoggerFactory.getLogger(JobExecutioner.class);

    private final JobStatusManager jobStatusManager;
    private final JSONObject command;
    private final String jobId;

    public JobExecutioner(String jobId, JSONObject command, JobStatusManager jobStatusManager) {
        this.command = command;
        this.jobId = jobId;
        this.jobStatusManager = jobStatusManager;
    }

    /**
     * execImpl command parameter tells executioner which job bean to execute
     */
    @Override
    public void run() {
        logger.debug("Handling job exec request...");

        try {
            ApplicationContext appContext = ApplicationContextUtils.getApplicationContext();
            PlatformTransactionManager tm = appContext.getBean("transactionManager", PlatformTransactionManager.class);
            Executable executable = appContext.getBean((String) command.get("execImpl"), Executable.class);

            TransactionStatus transactionStatus = null;

            try {
                transactionStatus = tm.getTransaction(null);

                command.put("jobId", jobId);
                jobStatusManager.addJob(jobId);
                executable.exec(command);
                jobStatusManager.setComplete(jobId, executable.getFinalStatusDetail());

                tm.commit(transactionStatus);
            }
            catch (Exception e) {
                if ((transactionStatus != null) && !transactionStatus.isCompleted()) {
                    tm.rollback(transactionStatus);
                }

                logger.error("Error executing job with ID = {}", jobId, e);
                jobStatusManager.setFailed(jobId, e.getMessage());
            }
        }
        catch (Exception e) {
            logger.error("Error executing job with ID = {}", jobId, e);
        }
    }
}
