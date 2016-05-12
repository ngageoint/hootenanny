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
import java.util.Locale;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.JobStatus;
import hoot.services.db2.QJobStatus;
import hoot.services.nativeInterfaces.JobExecutionManager;


/**
 * @author Jong Choi
 *
 *         This job status management class. It tracks current execution status.
 *
 */
public class JobStatusManager {
    public enum JOB_STATUS {
        RUNNING(0) {
            @Override
            public String toString() {
                return "running";
            }
        },
        COMPLETE(1) {
            @Override
            public String toString() {
                return "complete";
            }
        },
        FAILED(2) {
            @Override
            public String toString() {
                return "failed";
            }
        },
        UNKNOWN(3) {
            @Override
            public String toString() {
                return "unknown";
            }
        };
        private int value;

        @Override
        public abstract String toString();

        private JOB_STATUS(int value) {
            this.value = value;
        }

        public int toInt() {
            return this.value;
        }

        public static JOB_STATUS fromInteger(int x) {
            switch (x) {
                case 0:
                    return RUNNING;
                case 1:
                    return COMPLETE;
                case 2:
                    return FAILED;
                default:
                    return UNKNOWN;
            }
        }

        public static JOB_STATUS fromString(String statusStr) {
            if (statusStr.toLowerCase(Locale.ENGLISH).equals("running")) {
                return RUNNING;
            }
            else if (statusStr.toLowerCase(Locale.ENGLISH).equals("complete")) {
                return COMPLETE;
            }
            else if (statusStr.toLowerCase(Locale.ENGLISH).equals("failed")) {
                return FAILED;
            }
            return UNKNOWN;
        }
    }

    private static final Logger log = LoggerFactory.getLogger(JobExecutionManager.class);
    private Connection conn;

    /**
     * Constructor
     */
    public JobStatusManager(Connection conn) {
        this.conn = conn;
    }

    /**
     * Creates job status. It sets JOB_STATUS_RUNNING
     *
     * @param jobId
     */
    public void addJob(String jobId) {
        try {
            updateJob(jobId, JOB_STATUS.RUNNING.toInt(), null);
        }
        catch (Exception e) {
            log.error(jobId + " failed to add job status.");
        }
    }

    public void updateJob(String jobId, String statusDetail) {
        try {
            updateJob(jobId, JOB_STATUS.RUNNING.toInt(), statusDetail);
        }
        catch (Exception e) {
            log.error(jobId + " failed to add job status.");
        }
    }

    /**
     * Updates to Complete. (Convenience function)
     *
     * @param jobId
     */
    public void setComplete(String jobId) {
        setComplete(jobId, null);
    }

    /**
     * Updates to Complete. (Convenience function)
     *
     * @param jobId
     * @param statusDetail
     *            final job status detail message
     */
    public void setComplete(String jobId, String statusDetail) {
        try {
            updateJob(jobId, JOB_STATUS.COMPLETE.toInt(), statusDetail);
        }
        catch (Exception e) {
            log.error(jobId + " failed to update job status.");
        }
    }

    /**
     * Updates to Failed. (Convenience function) and logs it.
     *
     * @param jobId
     */
    public void setFailed(String jobId) {
        log.error("Job with ID: " + jobId + " failed.");
        try {
            updateJob(jobId, JOB_STATUS.FAILED.toInt(), null);
        }
        catch (Exception e) {
            log.error(jobId + " failed to update job status.");
        }
    }

    /**
     * Updates to Failed and adds cause of failure. (Convenience function) and
     * logs it.
     *
     * @param jobId
     *            ID of the job being updated
     * @param statusDetail
     *            detail on the job failure
     */
    public void setFailed(String jobId, String statusDetail) {
        log.error("Job with ID: " + jobId + " failed: " + statusDetail);
        try {
            updateJob(jobId, JOB_STATUS.FAILED.toInt(), statusDetail);
        }
        catch (Exception e) {
            log.error(jobId + " failed to update job status.");
        }
    }

    /**
     * Returns a job status record by ID
     *
     * @param jobId
     *            ID of the job to retrieve the status for
     * @return a job status record
     * @throws Exception
     */
    public JobStatus getJobStatusObj(String jobId) throws Exception {
        try {
            QJobStatus jobStatusTbl = QJobStatus.jobStatus;
            SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());
            return query.from(jobStatusTbl).where(jobStatusTbl.jobId.eq(jobId)).singleResult(jobStatusTbl);
        }
        catch (Exception e) {
            log.error(jobId + " failed to fetch job status.");
        }
        return null;
    }

    /**
     * Updates job. This one should be used to any storage behavior like add or
     * update Since the serialization routine can change.
     */
    private boolean updateJob(String jobId, int status, String statusDetail) throws Exception {
        try {
            boolean isComplete = (status != 0);
            DbUtils.updateJobStatus(jobId, status, isComplete, statusDetail, conn);
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
        return true;
    }
}
