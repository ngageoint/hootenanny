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
package hoot.services.controllers.job;

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

import hoot.services.command.Command;
import hoot.services.command.ExternalCommandInterface;
import hoot.services.command.InternalCommandInterface;


public class JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(JobControllerBase.class);

    // Thread pool for chain and job processing
    private static final ExecutorService jobThreadExecutor =
            Executors.newFixedThreadPool(Integer.parseInt(INTERNAL_JOB_THREAD_SIZE));

    @Autowired
    private JobStatusManager jobStatusManager;

    @Autowired
    protected ExternalCommandInterface externalCommandInterface;

    @Autowired
    protected InternalCommandInterface internalCommandInterface;

    private final String processScriptName;


    public JobControllerBase(String processScriptName) {
        this.processScriptName = processScriptName;
    }

    public void processChainJob(String jobId, Command[] commands) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());

        Runnable chainJobWorker = new JobChainRunnable(jobId, commands, jobStatusManager);
        jobThreadExecutor.execute(chainJobWorker);
    }

    public void processJob(String jobId, Command command) {
        logger.debug("Current jobThreadExecutor's thread count: {}", ((ThreadPoolExecutor) jobThreadExecutor).getActiveCount());

        Runnable jobWorker = new JobRunnable(jobId, command, jobStatusManager);
        jobThreadExecutor.execute(jobWorker);
    }

    protected JSONObject createMakeScriptJobReq(JSONArray args) {
        String resourceName = this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "make");
        command.put("exec", this.processScriptName);
        command.put("caller", resourceName);
        command.put("params", args);

        return command;
    }

    protected JSONObject createBashScriptJobReq(JSONArray args) {
        String resourceName = this.getClass().getSimpleName();

        JSONObject command = new JSONObject();
        command.put("exectype", "bash");
        command.put("exec", this.processScriptName);
        command.put("caller", resourceName);
        command.put("params", args);

        return command;
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

    protected String getParameterValue(String key, JSONArray args) {
        for (Object arg : args) {
            JSONObject o = (JSONObject) arg;
            if (o.containsKey(key)) {
                return o.get(key).toString();
            }
        }

        return null;
    }
}