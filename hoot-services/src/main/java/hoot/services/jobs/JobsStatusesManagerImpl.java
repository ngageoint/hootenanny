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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.jobs;

import static hoot.services.job.JobStatus.RUNNING;
import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;
import java.util.stream.Collectors;

import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobStatusResponse;
import hoot.services.job.JobType;
import hoot.services.models.db.JobStatus;
import hoot.services.models.db.Users;

@Component
@Transactional(propagation = Propagation.REQUIRES_NEW) // Run inside of a new transaction.  This is intentional.
public class JobsStatusesManagerImpl implements JobsStatusesManager {
    public JobsStatusesManagerImpl() {}

    @Override
    public List<JobStatusResponse> getRecentJobs(Users user, int limit) {
        long past12 = System.currentTimeMillis() - 43200000 /* 12 hours */;
        List<JobStatus> recentJobs = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where((jobStatus.start.after(new Timestamp(past12)).and(jobStatus.userId.eq(user.getId())))
                        .or(jobStatus.status.eq(RUNNING.ordinal())))
                .orderBy(jobStatus.start.desc())
                .fetch();
        if(recentJobs.size() < limit) {
            recentJobs = createQuery().select(jobStatus)
                    .from(jobStatus)
                    .where(jobStatus.userId.eq(user.getId())
                            .or(jobStatus.status.eq(RUNNING.ordinal()))
                    )
                    .orderBy(jobStatus.start.desc()).limit(limit).fetch();
        }


        //format jobs
        return recentJobs.stream().map(j -> {
            JobStatusResponse response = new JobStatusResponse();
            response.setJobId(j.getJobId());
            response.setJobType(JobType.fromInteger(
                    (j.getJobType() != null) ? j.getJobType() : JobType.UNKNOWN.ordinal()
                ).toString());
            response.setUserId(j.getUserId());
            response.setMapId(j.getResourceId());
            response.setStart(j.getStart().getTime());
            response.setEnd(j.getEnd().getTime());
            response.setStatus(hoot.services.job.JobStatus.fromInteger(j.getStatus()).toString());
            response.setPercentComplete(j.getPercentComplete());

            return response;
        }).collect(Collectors.toList());
    }

    @Override
    public List<JobStatusResponse> getJobsHistory(Users user) {
        List<JobStatus> jobsHistory = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(jobStatus.userId.eq(user.getId())
                    .and(jobStatus.status.gt(0)))
                .orderBy(jobStatus.start.desc())
                .fetch();


        //format jobs
        return jobsHistory.stream().map(j -> {
            JobStatusResponse response = new JobStatusResponse();
            response.setJobId(j.getJobId());
            response.setJobType(JobType.fromInteger(
                    (j.getJobType() != null) ? j.getJobType() : JobType.UNKNOWN.ordinal()
                ).toString());
            response.setMapId(j.getResourceId());
            response.setStart(j.getStart().getTime());
            response.setEnd(j.getEnd().getTime());
            response.setStatus(hoot.services.job.JobStatus.fromInteger(j.getStatus()).toString());

            return response;
        }).collect(Collectors.toList());
    }
    @Override
    public List<JobStatusResponse> getRunningJobs() {
        List<JobStatus> runningJobs = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(jobStatus.status.eq(RUNNING.ordinal()))
                .orderBy(jobStatus.start.desc())
                .fetch();

        //format jobs
        return runningJobs.stream().map(j -> {
            JobStatusResponse response = new JobStatusResponse();
            response.setJobId(j.getJobId());
            response.setJobType(JobType.fromInteger(
                    (j.getJobType() != null) ? j.getJobType() : JobType.UNKNOWN.ordinal()
                ).toString());
            response.setUserId(j.getUserId());
            response.setStart(j.getStart().getTime());
            response.setPercentComplete(j.getPercentComplete());

            return response;
        }).collect(Collectors.toList());
    }

}
