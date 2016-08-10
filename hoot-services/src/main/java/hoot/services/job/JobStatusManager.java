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

import static hoot.services.job.JobStatusManager.JOB_STATUS.*;
import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.getConfiguration;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.sql.Configuration;
import com.querydsl.sql.SQLQuery;
import com.querydsl.sql.dml.SQLInsertClause;
import com.querydsl.sql.dml.SQLUpdateClause;

import hoot.services.models.db.JobStatus;
import hoot.services.models.db.QJobStatus;


/**
 * @author Jong Choi
 *
 *         This job status management class. It tracks current execution status.
 *
 */
public class JobStatusManager {
    private static final Logger logger = LoggerFactory.getLogger(JobStatusManager.class);

    private final Connection connection;


    public enum JOB_STATUS {
        RUNNING, COMPLETE, FAILED, UNKNOWN;

        public static JOB_STATUS fromInteger(int value) {
            if ((value >= 0) && (value < JOB_STATUS.values().length)) {
                return JOB_STATUS.values()[value];
            }
            return UNKNOWN;
        }

        @Override
        public String toString() {
            return this.name().toLowerCase();
        }
    }

    public JobStatusManager(Connection conn) {
        this.connection = conn;
    }

    /**
     * Creates job status. It sets JOB_STATUS_RUNNING
     *
     * @param jobId
     */
    public void addJob(String jobId) {
        try {
            this.updateJob(jobId, RUNNING, null);
        }
        catch (Exception e) {
            logger.error("Error adding a new job with ID = {} ", jobId, e);
            throw e;
        }
    }

    public void updateJob(String jobId, String statusDetail) {
        try {
            this.updateJob(jobId, RUNNING, statusDetail);
        }
        catch (Exception e) {
            logger.error("Error updating RUNNING job with ID = {} with new status detail", jobId, e);
            throw e;
        }
    }

    /**
     * Updates to Complete. (Convenience function)
     *
     * @param jobId
     */
    public void setComplete(String jobId) {
        try {
            this.updateJob(jobId, COMPLETE, null);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to COMPLETE", jobId, e);
            throw e;
        }
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
            this.updateJob(jobId, COMPLETE, statusDetail);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to COMPLETE with status detail = '{}'", jobId, statusDetail, e);
            throw e;
        }
    }

    /**
     * Updates to Failed. (Convenience function) and logs it.
     *
     * @param jobId
     */
    public void setFailed(String jobId) {
        logger.error("Job with ID: {} failed.", jobId);
        try {
            this.updateJob(jobId, FAILED, null);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID: {} status to FAILED", jobId, e);
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
        logger.error("Job with ID: {} failed: {}", jobId, statusDetail);
        try {
            this.updateJob(jobId, FAILED, statusDetail);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID: {} status to FAILED with status detal = '{}'", jobId, statusDetail, e);
        }
    }

    /**
     * Returns a job status record by ID
     *
     * @param jobId
     *            ID of the job to retrieve the status for
     * @return a job status record
     */
    public JobStatus getJobStatusObj(String jobId) {
        try {
            return new SQLQuery<>(connection, getConfiguration())
                    .select(jobStatus)
                    .from(jobStatus)
                    .where(jobStatus.jobId.eq(jobId))
                    .fetchOne();
        }
        catch (Exception e) {
            logger.error("{} failed to fetch job status.", jobId, e);
        }
        return null;
    }

    /**
     * Updates job. This one should be used to any storage behavior like add or
     * update Since the serialization routine can change.
     */
    private void updateJob(String jobId, JOB_STATUS jobStatus, String statusDetail) {
        try {
            boolean isComplete = (jobStatus != RUNNING);
            updateJobStatus(jobId, jobStatus.ordinal(), isComplete, statusDetail, connection);
        }
        catch (Exception e) {
            logger.error("Failed to update job status of job with ID = {} and status detail = {}", jobId, statusDetail, e);
            throw e;
        }
    }

    /**
     * Updates job status. If the record does not exist then creates.
     *
     * @param jobId
     * @param jobStatus
     * @param isComplete
     * @param conn
     */
    private static void updateJobStatus(String jobId, int jobStatus, boolean isComplete, String statusDetail, Connection conn) {
        Configuration configuration = getConfiguration();

        JobStatus stat = new SQLQuery<>(conn, configuration)
                .select(QJobStatus.jobStatus)
                .from(QJobStatus.jobStatus)
                .where(QJobStatus.jobStatus.jobId.eq(jobId))
                .fetchOne();

        if (stat != null) {
            if (isComplete) {
                stat.setPercentComplete(100.0);
                stat.setEnd(new Timestamp(Calendar.getInstance().getTimeInMillis()));
            }

            stat.setStatus(jobStatus);

            if (statusDetail != null) {
                stat.setStatusDetail(statusDetail);
            }

            new SQLUpdateClause(conn, configuration, QJobStatus.jobStatus)
                    .populate(stat)
                    .where(QJobStatus.jobStatus.jobId.eq(stat.getJobId()))
                    .execute();
        }
        else {
            stat = new JobStatus();
            stat.setJobId(jobId);
            stat.setStatus(jobStatus);
            Timestamp ts = new Timestamp(Calendar.getInstance().getTimeInMillis());
            stat.setStart(ts);

            if (isComplete) {
                stat.setEnd(ts);
            }

            new SQLInsertClause(conn, configuration, QJobStatus.jobStatus).populate(stat).execute();
        }
    }
}
