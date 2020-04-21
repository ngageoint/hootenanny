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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.jobs;

import static hoot.services.job.JobStatus.RUNNING;
import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import com.querydsl.core.types.OrderSpecifier;
import com.querydsl.core.types.Predicate;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.core.types.dsl.NumberExpression;

import hoot.services.controllers.job.JobStatusResponse;
import hoot.services.controllers.jobs.JobHistory;
import hoot.services.job.JobType;
import hoot.services.models.db.JobStatus;
import hoot.services.models.db.Users;
import hoot.services.utils.PostgresUtils;

@Component
@Transactional(propagation = Propagation.REQUIRES_NEW) // Run inside of a new transaction.  This is intentional.
public class JobsStatusesManagerImpl implements JobsStatusesManager {
    public JobsStatusesManagerImpl() {}


    @Override
    public JobHistory getJobsHistory(Users user, String sort, long offset, long limit, String type, String status, String groupJobId) throws IllegalArgumentException {
        OrderSpecifier<?> sorter = jobStatus.start.desc();
        List<Integer> types = new ArrayList<>();
        List<Integer> statuses = new ArrayList<>();

        //Hack, but no other way I could find to get timestamp as epoch with query dsl
        //so this calculates the duration between start and end timestamps in milliseconds
        NumberExpression<Long> duration = jobStatus.end.dayOfYear().castToNum(Long.class).multiply(Expressions.constant(86400000d))
                .add(jobStatus.end.hour().castToNum(Long.class).multiply(Expressions.constant(3600000d)))
                .add(jobStatus.end.minute().castToNum(Long.class).multiply(Expressions.constant(60000d)))
                .add(jobStatus.end.second().castToNum(Long.class).multiply(Expressions.constant(1000d)))
                .add(jobStatus.end.milliSecond().castToNum(Long.class))
                .subtract(
                        jobStatus.start.dayOfYear().castToNum(Long.class).multiply(Expressions.constant(86400000d))
                        .add(jobStatus.start.hour().castToNum(Long.class).multiply(Expressions.constant(3600000d)))
                        .add(jobStatus.start.minute().castToNum(Long.class).multiply(Expressions.constant(60000d)))
                        .add(jobStatus.start.second().castToNum(Long.class).multiply(Expressions.constant(1000d)))
                        .add(jobStatus.start.milliSecond().castToNum(Long.class))
                );

        switch (sort) {
        case "+type":
            sorter = jobStatus.jobType.asc();
            break;
        case "-type":
            sorter = jobStatus.jobType.desc();
            break;
        case "+status":
            sorter = jobStatus.status.asc();
            break;
        case "-status":
            sorter = jobStatus.status.desc();
            break;
        case "+duration":
            sorter = duration.asc();
            break;
        case "-duration":
            sorter = duration.desc();
            break;
        case "+start":
            sorter = jobStatus.start.asc();
            break;
        case "-start":
        default:
            sorter = jobStatus.start.desc();
            break;
        }

        if (!type.isEmpty()) {
            types = Arrays.stream(type.split(","))
                .map(String::toUpperCase)
                .map(JobType::valueOf)
                .map(JobType::ordinal)
                .collect(Collectors.toList());
        }

        if (!status.isEmpty()) {
            statuses = Arrays.stream(status.split(","))
                .map(String::toUpperCase)
                .map(hoot.services.job.JobStatus::valueOf)
                .map(hoot.services.job.JobStatus::ordinal)
                .collect(Collectors.toList());
        }

        Predicate whereClause = jobStatus.userId.eq(user.getId()) //filter by user
                .and(jobStatus.status.gt(0)) //filter out running jobs
                .and((!types.isEmpty()) ? jobStatus.jobType.in(types) : jobStatus.jobType.isNotNull()) //filter by type
                .and((!statuses.isEmpty()) ? jobStatus.status.in(statuses) : jobStatus.status.isNotNull()) //filter by status
                ;

        List<JobStatus> jobsHistory = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(whereClause)
                .orderBy(sorter)
                .offset(offset)
                .limit(limit)
                .fetch();

        List<String> familyIds = new ArrayList<>();
        if (!groupJobId.equals("")) {
            List<String> processedIds = new ArrayList<>();
            findJobsFamily(familyIds, processedIds, groupJobId);
        }

        List<JobStatusResponse> jobs = jobsHistory.stream()
            .filter(j -> {
                if (!familyIds.isEmpty()) {
                    return familyIds.contains(j.getJobId());
                }

                return true;
            })
            .map(j -> {
            JobStatusResponse response = new JobStatusResponse();
            response.setJobId(j.getJobId());
            response.setJobType(JobType.fromInteger(
                    (j.getJobType() != null) ? j.getJobType() : JobType.UNKNOWN.ordinal()
                ).toString());
            response.setMapId(j.getResourceId());
            response.setStart(j.getStart().getTime());
            response.setEnd(j.getEnd().getTime());
            response.setStatus(hoot.services.job.JobStatus.fromInteger(j.getStatus()).toString());
            response.setStatusDetail(j.getStatusDetail());
            response.setTags(PostgresUtils.postgresObjToHStore(j.getTags()));

            return response;
        }).collect(Collectors.toList());

        Long total = createQuery()
                .select(jobStatus)
                .from(jobStatus)
                .where(whereClause)
                .fetchCount();

        //format jobs history
        return new JobHistory(total, jobs);
    }

    public void findJobsFamily(List<String> familyIds, List<String> processedIds, String currentId) {
        familyIds.add(currentId);
        processedIds.add(currentId);

        // Get the 'parent' job
        List<String> ids = createQuery()
                .select(Expressions.stringTemplate("tags->'parentId'"))
                .from(jobStatus)
                .where(jobStatus.jobId.eq(currentId))
                .fetch();

        // get the 'child' jobs
        List<String> childrenIds = createQuery()
                .select(jobStatus.jobId)
                .from(jobStatus)
                .where(Expressions.booleanTemplate(String.format("tags->'parentId'='%s'", currentId)))
                .fetch();

        ids.addAll(childrenIds);

        for(String jobId : ids) {
            if (!processedIds.contains(jobId) && jobId != null) {
                findJobsFamily(familyIds, processedIds, jobId);
            }
        }
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
