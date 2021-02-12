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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.job;

import static hoot.services.HootProperties.CHANGESETS_FOLDER;
import static hoot.services.HootProperties.TEMP_OUTPUT_PATH;
import static hoot.services.job.JobStatus.CANCELLED;
import static hoot.services.job.JobStatus.COMPLETE;
import static hoot.services.job.JobStatus.FAILED;
import static hoot.services.job.JobStatus.RUNNING;
import static hoot.services.models.db.QCommandStatus.commandStatus;
import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.File;
import java.io.IOException;
import java.sql.Timestamp;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import org.apache.commons.io.FileUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.dsl.BooleanExpression;

import hoot.services.command.Command;
import hoot.services.command.ExternalCommand;
import hoot.services.controllers.osm.user.UserResource;
import hoot.services.models.db.CommandStatus;
import hoot.services.utils.DbUtils;
import hoot.services.utils.PostgresUtils;


@Component
@Transactional(propagation = Propagation.REQUIRES_NEW) // Run inside of a new transaction.  This is intentional.
public class JobStatusManagerImpl implements JobStatusManager {
    private static final Logger logger = LoggerFactory.getLogger(JobStatusManagerImpl.class);

    public JobStatusManagerImpl() {}

    @Override
    public void addJob(Job job) {
        try {

            hoot.services.models.db.JobStatus newJobStatus = new hoot.services.models.db.JobStatus();
            newJobStatus.setJobId(job.getJobId());
            newJobStatus.setUserId(job.getUserId());
            newJobStatus.setJobType(job.getJobType().ordinal());
            newJobStatus.setStatus(RUNNING.ordinal());
            newJobStatus.setStatusDetail("PROCESSING");
            newJobStatus.setPercentComplete(0);
            newJobStatus.setResourceId(job.getMapId());
            Timestamp ts = new Timestamp(System.currentTimeMillis());  //Is this UTC?
            newJobStatus.setStart(ts);
            newJobStatus.setTags(job.getTags());

            // We only get the external command count because they take the longest to run so they have
            // the biggest impact on the math for job progress
            int commandCounter = 0;
            for(Command command: job.getCommands()) {
                if(command instanceof ExternalCommand) {
                    commandCounter++;
                }
            }
            newJobStatus.setCommandCount(commandCounter);

            createQuery().insert(jobStatus).populate(newJobStatus).execute();

        }
        catch (Exception e) {
            logger.error("Error adding a new job with ID = {} ", job.getJobId(), e);
            throw e;
        }
    }


    @Override
    public void deleteJob(String jobId, Long userId) throws IOException {
        try {
            hoot.services.models.db.JobStatus js = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(jobStatus.jobId.eq(jobId).and(jobStatus.userId.eq(userId)))
                .fetchFirst();

            //hack to check that user owns this job before deleting commands
            //no cascade delete in querydsl
            if (js != null) {

                createQuery().delete(commandStatus)
                    .where(commandStatus.jobId.eq(jobId))
                    .execute();

                createQuery().delete(jobStatus)
                    .where(jobStatus.jobId.eq(jobId).and(jobStatus.userId.eq(userId)))
                    .execute();

                File workDir;
                Integer jobType = js.getJobType();

                // Clean up left over data related to specific jobs
                if (jobType == JobType.DERIVE_CHANGESET.ordinal() || jobType == JobType.UPLOAD_CHANGESET.ordinal()) {
                    List<String> fileFilters;

                    if (jobType == JobType.DERIVE_CHANGESET.ordinal()) {
                        // these are the files that can be produced from a derive changeset job
                        fileFilters = Arrays.asList("diff.osc", "diff.tags.osc", "reference.osm", "secondary.osm", "stats.json", "stats.tags.json");

                        workDir = new File(CHANGESETS_FOLDER, js.getJobId());
                    } else {
                        // these are the files that can be produced from a upload changeset job
                        fileFilters = Arrays.asList("diff-error.osc", "diff-remaining.osc");

                        // An upload jobs work directory is actually the parents(derive changeset) directory
                        Map<String, String> tags = PostgresUtils.postgresObjToHStore(js.getTags());
                        workDir = new File(CHANGESETS_FOLDER, tags.get("parentId"));
                    }


                    if (workDir.exists()) {
                        // Only delete the files that are in the fileFilters list
                        File[] files = workDir.listFiles(pathname -> fileFilters.contains(pathname.getName()));

                        if (files != null && files.length > 0) {
                            for(File file : files) {
                                file.delete();
                            }
                        }

                        // only delete if empty.
                        // this is due to a case where if user deletes a derive changeset, we still want to keep upload changeset files if they exist. if not delete. Same for deleting an upload job if the changeset job still exists
                        if (workDir.list().length == 0) {
                            FileUtils.deleteDirectory(workDir);
                        }
                    }
                } else if (jobType == JobType.EXPORT.ordinal()) {
                    workDir = new File(TEMP_OUTPUT_PATH, js.getJobId());

                    if (workDir.exists()) {
                        FileUtils.deleteDirectory(workDir);
                    }
                }
            }

        }
        catch (Exception e) {
            logger.error("Error deleting job with ID = {} ", jobId, e);
            throw e;
        }
    }

    @Override
    public void updateJob(String jobId, String statusDetail, Integer percentComplete) {
        try {
            this.updateJob(jobId, RUNNING, statusDetail, percentComplete);
        }
        catch (Exception e) {
            logger.error("Error updating RUNNING job with ID = {} with new status detail", jobId, e);
            throw e;
        }
    }

    @Override
    public void setCancelled(String jobId, String reason) {
        try {
            this.updateJob(jobId, CANCELLED, reason, null);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to CANCELLED", jobId, e);
            throw e;
        }
    }

    @Override
    public void setCompleted(String jobId, String statusDetail) {
        try {
            this.updateJob(jobId, COMPLETE, statusDetail, 100);
        }
        catch (Exception e) {
            logger.error("Error setting job with ID = {} status to COMPLETE with status detail = '{}'", jobId, statusDetail, e);
            throw e;
        }
    }

    @Override
    public void setFailed(String jobId, String statusDetail) {
        try {
            this.updateJob(jobId, FAILED, statusDetail, null);
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
    @Override
    public hoot.services.models.db.JobStatus getJobStatusObj(String jobId) {
        try {
            return createQuery().select(jobStatus).from(jobStatus).where(jobStatus.jobId.eq(jobId)).fetchOne();
        }
        catch (Exception e) {
            logger.error("{} failed to fetch job status.", jobId, e);
        }

        return null;
    }

    /**
     * Returns a job status record by ID and User
     *
     * @param jobId
     *            ID of the job to retrieve the status for
     * @param userId
     *            user to compare to job owner
     * @return a job status record
     */
    @Override
    public List<String> getJobErrors(String jobId, Long userId) {
        try {
            List<CommandStatus> commands =  getCommandDetail(jobId, userId);

            return commands.stream().map(comm -> comm.getStderr())
                    .filter(error -> !error.isEmpty())
                    .collect(Collectors.toList());
        }
        catch (Exception e) {
            logger.error("{} failed to fetch job status.", jobId, e);
        }

        return null;
    }

    /**
     * Returns a job status record by ID and User
     *
     * @param jobId
     *            ID of the job to retrieve the status for
     * @param userId
     *            user to compare to job owner
     * @return a job status record
     */
    @Override
    public hoot.services.models.db.JobStatus getJobStatusObj(String jobId, Long userId) {
        try {
            BooleanExpression isVisible = jobStatus.jobId.eq(jobId);

            // if user is not admin enforce visiblity rules
            // admins can see everything
            if (!UserResource.adminUserCheck(DbUtils.getUser(userId))) {
                isVisible = isVisible.and(
                        jobStatus.userId.eq(userId)
                        .or(jobStatus.status.eq(RUNNING.ordinal()))
                    );
            }

            return createQuery().select(jobStatus).from(jobStatus).where(isVisible).fetchOne();
        }
        catch (Exception e) {
            logger.error("{} failed to fetch job status.", jobId, e);
        }

        return null;
    }

    @Override
    public List<CommandStatus> getCommandDetail(String jobId, Long userId) {
        try {
            BooleanExpression isVisible = jobStatus.jobId.eq(jobId)
                    .and(jobStatus.jobId.eq(commandStatus.jobId));

            // if user is not admin enforce visiblity rules
            // admins can see everything
            if (!UserResource.adminUserCheck(DbUtils.getUser(userId))) {
                isVisible = isVisible.and(jobStatus.userId.eq(userId));
            }

            return createQuery().select(commandStatus).from(commandStatus, jobStatus).where(isVisible).orderBy(commandStatus.start.asc()).fetch();
        }
        catch (Exception e) {
            logger.error("{} failed to fetch command status(es) for job with ID = {}", jobId, e);
            throw e;
        }
    }

    /**
     * Updates job. This one should be used to any storage behavior like add or
     * update Since the serialization routine can change.
     */
    private void updateJob(String jobId, JobStatus jobStatus, String statusDetail, Integer percentComplete) {
        try {
            updateJobStatus(jobId, jobStatus, statusDetail, percentComplete);
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
    private void updateJobStatus(String jobId, JobStatus newStatus, String statusDetail, Integer percentComplete) {
        hoot.services.models.db.JobStatus currentJobStatus = createQuery().select(jobStatus).from(jobStatus).where(jobStatus.jobId.eq(jobId)).fetchOne();

        if ((currentJobStatus != null) && (currentJobStatus.getStatus() == RUNNING.ordinal())) {
            currentJobStatus.setStatus(newStatus.ordinal());

            if (statusDetail != null) {
                currentJobStatus.setStatusDetail(statusDetail);
            }

            if (percentComplete != null) {
                currentJobStatus.setPercentComplete(percentComplete);
            }

            if ((newStatus == COMPLETE) || (newStatus == FAILED) || (newStatus == CANCELLED)) {
                currentJobStatus.setEnd(new Timestamp(System.currentTimeMillis()));
            }

            createQuery().update(jobStatus).where(jobStatus.jobId.eq(jobId)).populate(currentJobStatus).execute();
        }
        else if (currentJobStatus == null) {
            currentJobStatus = new hoot.services.models.db.JobStatus();
            currentJobStatus.setJobId(jobId);
            currentJobStatus.setStatus(newStatus.ordinal());

            if (statusDetail != null) {
                currentJobStatus.setStatusDetail(statusDetail);
            }

            if (percentComplete != null) {
                currentJobStatus.setPercentComplete(percentComplete);
            }

            Timestamp ts = new Timestamp(System.currentTimeMillis());
            currentJobStatus.setStart(ts);

            if ((newStatus == COMPLETE) || (newStatus == FAILED) || (newStatus == CANCELLED)) {
                currentJobStatus.setEnd(ts);
            }

           createQuery().insert(jobStatus).populate(currentJobStatus).execute();
        }
    }
}
