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
package hoot.services.controllers.nonblocking;

import static hoot.services.HootProperties.INTERNAL_JOB_THREAD_SIZE;

import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;

import hoot.services.command.ExternalCommand;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.InternalCommandManager;
import hoot.services.job.ChainJob;
import hoot.services.job.Job;
import hoot.services.job.JobChainRunnable;
import hoot.services.job.JobRunnable;
import hoot.services.job.JobStatusManager;


public class AsynchronousJobResource {
    private static final Logger logger = LoggerFactory.getLogger(AsynchronousJobResource.class);

    // Shared thread pool for job processing
    private static final ExecutorService jobThreadExecutor =
            Executors.newFixedThreadPool(Integer.parseInt(INTERNAL_JOB_THREAD_SIZE));

    @Autowired
    private JobStatusManager jobStatusManager;

    @Autowired
    protected ExternalCommandManager externalCommandManager;

    @Autowired
    protected InternalCommandManager internalCommandManager;

    private final String processScriptName;


    public AsynchronousJobResource(String processScriptName) {
        this.processScriptName = processScriptName;
    }

    public void processChainJob(ChainJob chainJob) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());
        Runnable work = new JobChainRunnable(chainJob, jobStatusManager);
        jobThreadExecutor.execute(work);
    }

    public void processJob(Job job) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());
        Runnable work = new JobRunnable(job, jobStatusManager);
        jobThreadExecutor.execute(work);
    }

    public static JSONArray parseParams(String params) throws ParseException {
        JSONParser parser = new JSONParser();
        JSONObject command = (JSONObject) parser.parse(params);
        JSONArray commandArgs = new JSONArray();

        for (Object o : command.entrySet()) {
            Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) o;
            String key = (String) mEntry.getKey();
            String val = (String) mEntry.getValue();

            JSONObject arg = new JSONObject();
            arg.put(key, val);
            commandArgs.add(arg);
        }

        return commandArgs;
    }

    protected String getParameterValue(String key, JSONObject jsonObject) {
        return (jsonObject.get(key) != null) ? jsonObject.get(key).toString() : null;
    }

    protected ExternalCommand createMakeScriptJobReq(JSONArray args) {
        ExternalCommand command = new ExternalCommand();
        command.put("exectype", "make");
        command.put("exec", this.processScriptName);
        command.put("caller", this.getClass().getName());
        command.put("params", args);
        return command;
    }
}