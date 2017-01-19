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

import static hoot.services.controllers.job.JobStatusManager.JOB_STATUS.*;
import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Profile;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.models.db.JobStatus;


/**
 * @author Jong Choi
 *
 *         This job status management class. It tracks current execution status.
 *
 */
@Component
@Transactional(propagation = Propagation.REQUIRES_NEW)
@Profile("production")
public class JobStatusManager {
    private static final Logger logger = LoggerFactory.getLogger(JobStatusManager.class);

    public enum JOB_STATUS {
        RUNNING, COMPLETED, FAILED, CANCELLED, UNKNOWN, COMPLETED_WITH_WARNINGS;

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

    public JobStatusManager() {}

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

    public void setCompletedWithWarnings(String jobId, String warnings) {
        try {
            this.updateJob(jobId, COMPLETED_WITH_WARNINGS, warnings);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to COMPLETED_WITH_WARNINGS", jobId, e);
            throw e;
        }
    }

    public void setCancelled(String jobId, String reason) {
        try {
            this.updateJob(jobId, CANCELLED, reason);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to CANCELLED", jobId, e);
            throw e;
        }
    }

    public void setCompleted(String jobId, String statusDetail) {
        try {
            this.updateJob(jobId, COMPLETED, statusDetail);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to COMPLETED with status detail = '{}'", jobId, statusDetail, e);
            throw e;
        }
    }

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
            return createQuery().select(jobStatus).from(jobStatus).where(jobStatus.jobId.eq(jobId)).fetchOne();
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
            updateJobStatus(jobId, jobStatus, statusDetail);
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
     * @param newStatus
     */
    private void updateJobStatus(String jobId, JOB_STATUS newStatus, String statusDetail) {
        JobStatus currentJobStatus = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(jobStatus.jobId.eq(jobId))
                .fetchOne();

        if ((currentJobStatus != null) && (currentJobStatus.getStatus() == RUNNING.ordinal())) {
            if ((newStatus == COMPLETED) || (newStatus == FAILED) || (newStatus == COMPLETED_WITH_WARNINGS)) {
                currentJobStatus.setPercentComplete(100.0);
                currentJobStatus.setEnd(new Timestamp(System.currentTimeMillis()));
            }

            currentJobStatus.setStatus(newStatus.ordinal());

            if (statusDetail != null) {
                currentJobStatus.setStatusDetail(statusDetail);
            }

            createQuery().update(jobStatus).where(jobStatus.jobId.eq(jobId)).populate(currentJobStatus).execute();
        }
        else if (currentJobStatus == null) {
            currentJobStatus = new JobStatus();
            currentJobStatus.setJobId(jobId);
            currentJobStatus.setStatus(newStatus.ordinal());
            Timestamp ts = new Timestamp(System.currentTimeMillis());
            currentJobStatus.setStart(ts);

            if ((newStatus == COMPLETED) || (newStatus == FAILED) || (newStatus == COMPLETED_WITH_WARNINGS)) {
                currentJobStatus.setPercentComplete(100.0);
                currentJobStatus.setEnd(ts);
            }
            else {
                currentJobStatus.setPercentComplete(0.0);
            }

           createQuery().insert(jobStatus).populate(currentJobStatus).execute();
        }
    }
}
