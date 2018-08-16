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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.jobs;

import static hoot.services.models.db.QJobStatus.jobStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.models.db.JobStatus;

@Component
@Transactional(propagation = Propagation.REQUIRES_NEW) // Run inside of a new transaction.  This is intentional.
public class JobsStatusesManagerImpl implements JobsStatusesManager {
	private static final Logger logger = LoggerFactory.getLogger(JobsStatusesManagerImpl.class);
	public JobsStatusesManagerImpl() {}
	@Override
	public List<JobStatus> getRecentJobs(int limit) {
		long past12 = System.currentTimeMillis() - 43200000 /* 12 hours */;
		List<JobStatus> recentJobs = createQuery()
				.select(jobStatus)
				.from(jobStatus)
				.where(jobStatus.start.after(new Timestamp(past12)))
				.orderBy(jobStatus.start.desc())
				.limit(limit).fetch();
		if(recentJobs.size() < 10) {
			recentJobs = createQuery().select(jobStatus).from(jobStatus).orderBy(jobStatus.start.desc()).limit(limit).fetch();
		}
		return recentJobs;
	}

}
