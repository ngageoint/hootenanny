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
package hoot.services.command;

import static hoot.services.HootProperties.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Profile;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.controllers.job.JobFieldsValidator;
import hoot.services.utils.JsonUtils;


/**
 * @author Jong Choi
 *         <p>
 *         The purpose of this class is to run command line execution in a
 *         separate process. It inherit from INativeInterface so it can be
 *         switched out with any other class implementing INativeInterface if
 *         needed. It is called by nativeInterface and it gets associated
 *         through spring entry located in applicationContext.xml. This class
 *         handles 2 types of execution format. 1. direct exec call like hoot
 *         --ogr2osm target input output if the "exectype" is "hoot" 2. make
 *         file script based execution where format is make -f [some makefile]
 *         [any argument make file uses] when "exectype" is "make"
 */
@Transactional
@Component
@Profile("production")
class ExternalCommandInterfaceImpl implements ExternalCommandInterface {
    private static final Logger logger = LoggerFactory.getLogger(ExternalCommandInterfaceImpl.class);

    // This contains the command runner objects for the executing commands. Used for job cancellation and tracking.
    private static final Map<String, CommandRunner> activeCommands = new ConcurrentHashMap<>();

    @Override
    public String getJobProgress(String jobId) {
        String stdStr = "";
        CommandRunner commandRunner = activeCommands.get(jobId);
        if (commandRunner != null) {
            stdStr = commandRunner.getStdout();
        }

        return stdStr;
    }

    @Override
    public void terminate(String jobId) {
        CommandRunner cmdRunner = activeCommands.get(jobId);
        if (cmdRunner != null) {
            cmdRunner.terminate();
        }
    }

    @Override
    public CommandResult exec(String jobId, JSONObject command) {
        String exec = command.get("exectype").toString();

        // Always add jobId to every command executed
        command.put("jobId", jobId);

        String[] commandArr;
        if (exec.equalsIgnoreCase("hoot")) {
            commandArr = createCmdArray(command);
        }
        else if (exec.equalsIgnoreCase("make")) {
            commandArr = createScriptCmdArray(command);
        }
        else if (exec.equalsIgnoreCase("bash")) {
            commandArr = createBashScriptCmdArray(command);
        }
        else {
            throw new IllegalArgumentException("Invalid exectype specified");
        }

        validate(command);

        CommandRunner cmdRunner = new CommandRunnerImpl();

        if (jobId != null) {
            activeCommands.put(jobId, cmdRunner);
        }

        CommandResult commandResult;
        try {
            commandResult = cmdRunner.exec(commandArr, jobId);
        }
        catch (Exception e) {
            throw new RuntimeException("Failed to execute: " + command, e);
        }
        finally {
            if ((jobId != null) && activeCommands.containsKey(jobId)) {
                activeCommands.remove(jobId);
            }
        }

        return commandResult;
    }

    /**
     * Creates direct exec call like hoot --ogr2osm target input output if the "exectype" is "hoot"
     */
    private static String[] createCmdArray(JSONObject cmd) {
        List<String> execCmd = new ArrayList<>();
        execCmd.add("hoot");
        execCmd.add("--" + cmd.get("exec"));

        JSONArray params = (JSONArray) cmd.get("params");
        for (Object o : params) {
            JSONObject param = (JSONObject) o;

            for (Object entry : param.entrySet()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) entry;
                String arg = (String) mEntry.getValue();
                execCmd.add(arg);
            }
        }

        Object[] objectArray = execCmd.toArray();

        return Arrays.copyOf(objectArray, objectArray.length, String[].class);
    }

    /**
     * Creates command for make file script based call if exectype = "make"
     * output looks like make -f [some makefile] [any argument make file uses]
     */
    private static String[] createScriptCmdArray(JSONObject cmd) {
        List<String> execCmd = new ArrayList<>();
        execCmd.add("make");
        execCmd.add("-f");
        execCmd.add(CORE_SCRIPT_PATH + "/" + cmd.get("exec"));

        JSONArray params = (JSONArray) cmd.get("params");
        for (Object o : params) {
            JSONObject param = (JSONObject) o;

            for (Object entry : param.entrySet()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) entry;
                String key = (String) mEntry.getKey();
                String arg = (String) mEntry.getValue();
                execCmd.add(key + "=" + arg);
            }
        }

        if (cmd.get("jobId") != null) {
            String jobid = cmd.get("jobId").toString();
            execCmd.add("jobid=" + jobid);
        }

        String dbURL = "hootapidb://" + DB_USER_ID + ":" + DB_PASSWORD + "@" + DB_HOST + ":" + DB_PORT + "/" + DB_NAME;
        String osmAPIDBURL = "osmapidb://" + OSM_API_DB_USER_ID + ":" + OSM_API_DB_PASSWORD + "@" + OSM_API_DB_HOST + ":" + OSM_API_DB_PORT + "/" + OSM_API_DB_NAME;

        execCmd.add("DB_URL=" + dbURL);
        execCmd.add("OSM_API_DB_URL=" + osmAPIDBURL);

        Object[] objectArray = execCmd.toArray();

        return Arrays.copyOf(objectArray, objectArray.length, String[].class);
    }

    private static String[] createBashScriptCmdArray(JSONObject cmd) {
        List<String> execCmd = new ArrayList<>();
        execCmd.add("bash");
        execCmd.add(CORE_SCRIPT_PATH + "/" + cmd.get("exec"));

        JSONArray params = (JSONArray) cmd.get("params");
        for (Object o : params) {
            JSONObject param = (JSONObject) o;

            for (Object rename : param.entrySet()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) rename;
                String arg = (String) mEntry.getValue();
                execCmd.add(arg);
            }
        }

        if (cmd.get("jobId") != null) {
            String jobid = cmd.get("jobId").toString();
            execCmd.add(jobid);
        }

        Object[] objectArray = execCmd.toArray();

        return Arrays.copyOf(objectArray, objectArray.length, String[].class);
    }

    private void validate(JSONObject command) {
        String resourceName = command.get("caller").toString();
        JobFieldsValidator validator = new JobFieldsValidator(resourceName);

        Map<String, String> paramsMap = JsonUtils.paramsToMap(command);

        List<String> missingList = new ArrayList<>();
        if (!validator.validateRequiredExists(paramsMap, missingList)) {
            throw new RuntimeException("Missing following required field(s): " + missingList);
        }
    }
}
