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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.job;

import static hoot.services.HootProperties.INTERNAL_JOB_THREAD_SIZE;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;


@Component
public class JobProcessor {
    private static final Logger logger = LoggerFactory.getLogger(JobProcessor.class);

    // Shared thread pool for job processing
    private static final ExecutorService jobThreadExecutor =
            Executors.newFixedThreadPool(Integer.parseInt(INTERNAL_JOB_THREAD_SIZE));

    @Autowired
    private JobStatusManager jobStatusManager;

    public void process(Job job) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());
        Runnable work = new JobRunnable(job, jobStatusManager);
        jobThreadExecutor.execute(work);
    }
}
