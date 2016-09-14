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
import java.sql.SQLException;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ApplicationContext;

import hoot.services.HootProperties;
import hoot.services.utils.DbUtils;


/**
 * Thread class for executing async jobs
 */
public class JobExecutioner extends Thread {
    private static final Logger logger = LoggerFactory.getLogger(JobExecutioner.class);

    private final JSONObject command;
    private final String jobId;

    public JobExecutioner(String jobId, JSONObject command) {
        this.command = command;
        this.jobId = jobId;
    }

    /**
     * execImpl command parameter tells executioner which job bean to execute
     */
    @Override
    public void run() {
        logger.debug("Handling job exec request...");

        try (Connection connection = DbUtils.createConnection()) {
            JobStatusManager jobStatusManager = new JobStatusManager(connection);
            command.put("jobId", jobId);

            try {
                jobStatusManager.addJob(jobId);

                ApplicationContext applicationContext = HootProperties.getSpringContext();
                Executable executable = (Executable) applicationContext.getBean((String) command.get("execImpl"));

                executable.exec(command);

                jobStatusManager.setComplete(jobId, executable.getFinalStatusDetail());
            }
            catch (Exception e) {
                logger.error("Error executing job with ID = {}", jobId, e);
                jobStatusManager.setFailed(jobId, e.getMessage());
            }
        }
        catch (SQLException sqle) {
            logger.error("Error executing job with ID = {}", jobId, sqle);
        }
    }
}
