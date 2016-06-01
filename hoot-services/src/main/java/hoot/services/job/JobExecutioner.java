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

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import hoot.services.db.DbUtils;


/**
 * Thread class for executing async jobs
 */
public class JobExecutioner extends Thread {
    private static final Logger log = LoggerFactory.getLogger(JobExecutioner.class);

    private ClassPathXmlApplicationContext appContext = new ClassPathXmlApplicationContext(
            "hoot/spring/CoreServiceContext.xml");
    @SuppressWarnings("unused")
    private ClassPathXmlApplicationContext dbAppContext = new ClassPathXmlApplicationContext("db/spring-database.xml");

    private JSONObject command;

    private String jobId;

    public JobExecutioner(final String jobId, final JSONObject command) {
        this.command = command;
        this.jobId = jobId;
    }

    /**
     * execImpl command parameter tells executioner which job bean to execute
     */
    @SuppressWarnings("unchecked")
    @Override
    public void run() {
        log.debug("Handling job exec request...");

        Connection conn = DbUtils.createConnection();
        JobStatusManager jobStatusManager = null;
        try {

            jobStatusManager = new JobStatusManager(conn);

            command.put("jobId", jobId);
            jobStatusManager.addJob(jobId);
            Executable executable = (Executable) appContext.getBean((String) command.get("execImpl"));
            executable.exec(command);
            jobStatusManager.setComplete(jobId, executable.getFinalStatusDetail());
        }
        catch (Exception e) {
            if (jobStatusManager != null) {
                jobStatusManager.setFailed(jobId, e.getMessage());
            }
        }
        finally {
            try {
                DbUtils.closeConnection(conn);
            }
            catch (Exception e) {
                //
            }
        }
    }
}
