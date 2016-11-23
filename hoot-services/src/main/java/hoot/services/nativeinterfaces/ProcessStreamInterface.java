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
package hoot.services.nativeinterfaces;

import static hoot.services.HootProperties.*;

import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.ArrayUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.annotation.Profile;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;


/**
 * @author Jong Choi
 *         <p>
 *         The purpose of this class is to run command line execution in a
 *         separate process. It inherit from INativeInterface so it can be
 *         switched out with any other class implementing INativeInterface if
 *         needed. It is called by jobExecutionManager and it gets associated
 *         through spring entry located in applicationContext.xml. This class
 *         handles 2 types of execution format. 1. direct exec call like hoot
 *         --ogr2osm target input output if the "exectype" is "hoot" 2. make
 *         file script based execution where format is make -f [some makefile]
 *         [any argument make file uses] when "exectype" is "make"
 */
@Transactional
@Component
@Profile("production")
class ProcessStreamInterface implements NativeInterface {
    private static final Logger logger = LoggerFactory.getLogger(ProcessStreamInterface.class);

    // This contains the command runner objects for the executing processes. Used for job cancellation and tracking.
    private static final Map<String, CommandRunner> jobProcesses = new ConcurrentHashMap<>();

    @Override
    public String getJobProgress(String jobId) {
        String stdStr = "";
        CommandRunner commandRunner = jobProcesses.get(jobId);
        if (commandRunner != null) {
            stdStr = commandRunner.getStdout();
        }

        return stdStr;
    }

    @Override
    public void terminate(String jobId) throws NativeInterfaceException {
        try {
            CommandRunner cmdRunner = jobProcesses.get(jobId);
            if (cmdRunner != null) {
                cmdRunner.terminate();
            }
        }
        catch (Exception e) {
            throw new NativeInterfaceException("Failed to execute." + e.getMessage(),
                    NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }

    @Override
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        logger.debug("Executing command : {}", command.toJSONString());

        JSONObject json = new JSONObject();

        String exec = command.get("exectype").toString();

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
            logger.error("Failed to parse params: {}", command.toJSONString());
            throw new NativeInterfaceException("Failed to parse params.", NativeInterfaceException.HttpCode.BAD_RQUEST);
        }

        String commandStr = ArrayUtils.toString(commandArr);
        logger.debug("Native call: {}", commandStr);
        CommandRunner cmdRunner = new CommandRunnerImpl();

        String jobId = null;
        if (command.get("jobId") != null) {
            jobId = command.get("jobId").toString();
            if (jobId != null) {
                jobProcesses.put(jobId, cmdRunner);
            }
        }

        logger.debug("Start: {}", new Date().getTime());

        try {
            CommandResult commandResult = cmdRunner.exec(commandArr);

            logger.debug("End: {}", new Date().getTime());

            if (commandResult != null) {
                if (commandResult.getExitStatus() == 0) {
                    String stdOut = commandResult.getStdout();
                    logger.debug("stdout: {}", stdOut);
                    json.put("stdout", stdOut);

                    StringBuffer warnings = new StringBuffer();
                    List<String> stdLines = IOUtils.readLines(new StringReader(stdOut));
                    for (String line : stdLines) {
                        if (line.contains(" WARN ")) {
                            warnings.append(line);
                            warnings.append(System.lineSeparator());
                        }

                        // we will cap the maximum length of warnings to 1k
                        if (warnings.length() > 1028) {
                            warnings.append(" more ..");
                            break;
                        }
                    }

                    if (warnings.length() > 0) {
                        json.put("warnings", warnings);
                    }
                }
                else {
                    logger.debug("Command failed.  {}", commandResult);
                    String err = commandResult.getStderr();

                    boolean doThrowException = true;
                    if (command.containsKey("throwerror")) {
                        doThrowException = Boolean.parseBoolean(command.get("throwerror").toString());
                    }

                    if (doThrowException) {
                        throw new Exception(err);
                    }

                    String stdOut = commandResult.getStdout();
                    json.put("stdout", stdOut);
                    json.put("stderr", err);
                }
            }
        }
        catch (Exception e) {
            throw new NativeInterfaceException("Failed to execute.  Cause: " + e.getMessage(),
                    NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
        finally {
            if ((jobId != null) && jobProcesses.containsKey(jobId)) {
                jobProcesses.remove(jobId);
            }
        }

        return json;
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

        execCmd.add("DB_URL=" + DB_URL);
        execCmd.add("OSM_API_DB_URL=" + OSM_API_DB_URL);

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
}
