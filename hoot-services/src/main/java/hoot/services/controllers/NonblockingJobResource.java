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
package hoot.services.controllers;

import static hoot.services.HootProperties.INTERNAL_JOB_THREAD_SIZE;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import javax.ws.rs.core.Response;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import hoot.services.command.ExternalCommandManager;
import hoot.services.command.InternalCommandManager;
import hoot.services.job.Job;
import hoot.services.job.JobRunnable;
import hoot.services.job.JobStatusManager;


public class NonblockingJobResource {
    private static final Logger logger = LoggerFactory.getLogger(NonblockingJobResource.class);

    // Shared thread pool for job processing
    private static final ExecutorService jobThreadExecutor =
            Executors.newFixedThreadPool(Integer.parseInt(INTERNAL_JOB_THREAD_SIZE));

    @Autowired
    private JobStatusManager jobStatusManager;

    @Autowired
    protected ExternalCommandManager externalCommandManager;

    @Autowired
    protected InternalCommandManager internalCommandManager;


    public void processJob(Job job) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());
        Runnable work = new JobRunnable(job, jobStatusManager);
        jobThreadExecutor.execute(work);
    }

    protected Response createJobIdResponse(String jobId) {
        JSONObject json = new JSONObject();
        json.put("jobid", jobId);

        return Response.ok(json.toJSONString()).build();
    }
}