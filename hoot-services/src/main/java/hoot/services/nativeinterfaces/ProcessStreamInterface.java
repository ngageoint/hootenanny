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
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.ArrayUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.CommandRunner;
import hoot.services.command.ICommandRunner;


/**
 * @author Jong Choi
 *         <p>
 *         The purpose of this class is to run command line execution in a
 *         separate process. It inherit from INativeInterface so it can be
 *         switched out with any other class implementing INativeInterface if
 *         needed. It is called by jobExecutionManager and it gets associated
 *         through spring entry located in CoreServiceContext.xml. This class
 *         handles 2 types of execution format. 1. direct exec call like hoot
 *         --ogr2osm target input output if the "exectype" is "hoot" 2. make
 *         file script based execution where format is make -f [some makefile]
 *         [any argument make file uses] when "exectype" is "make"
 */
public class ProcessStreamInterface implements INativeInterface {
    private static final Logger logger = LoggerFactory.getLogger(ProcessStreamInterface.class);

    // This contains the command runner objects for the executing processes.
    // Used for job cancellation and tracking
    private static final Map<String, ICommandRunner> jobProcesses = new ConcurrentHashMap<>();
    private static final Map<String, ICommandRunner> progProcesses = new ConcurrentHashMap<>();

    public ProcessStreamInterface() {
    }

    @Override
    public String getJobProgress(String jobId) {
        String stdStr = "";
        Object oCmdRunner = progProcesses.get(jobId);
        if (oCmdRunner != null) {
            ICommandRunner cmdRunner = (ICommandRunner) oCmdRunner;
            stdStr = cmdRunner.getStdOut();

            // process exited
            if (cmdRunner.getProcessState() == 1) {
                // we will get the stat for last time since process exited
                removeFromProgressProcessQ(jobId);
            }
        }

        return stdStr;
    }

    @Override
    public JSONObject exec(JSONObject command) throws NativeInterfaceException {
        logger.debug("Executing command : {}", command.toJSONString());
        JSONObject ret = new JSONObject();
        CommandResult res = null;
        boolean addedToQ = false;
        try {
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
                throw new NativeInterfaceException("Failed to parse params.",
                        NativeInterfaceException.HttpCode.BAD_RQUEST);
            }

            String commandStr = ArrayUtils.toString(commandArr);
            logger.debug("Native call: {}", commandStr);

            if ((commandArr == null) || (commandArr.length == 0)) {
                throw new NativeInterfaceException("Failed to parse params.",
                        NativeInterfaceException.HttpCode.BAD_RQUEST);
            }

            ICommandRunner cmdRunner = new CommandRunner();
            addedToQ = addToProcessQ(command, cmdRunner);

            logger.debug("Start: {}", new Date().getTime());
            res = cmdRunner.exec(commandArr);
            logger.debug("End: {}", new Date().getTime());

            if (res != null) {
                if (res.getExitStatus() == 0) {
                    String stdOut = res.getStdout();
                    logger.debug("stdout: {}", stdOut);
                    ret.put("stdout", stdOut);

                    String warnings = "";
                    List<String> stdLines = IOUtils.readLines(new StringReader(stdOut));
                    for (String ln : stdLines) {
                        if (ln.contains(" WARN ")) {
                            warnings += ln;
                            warnings += "\n";
                        }

                        // we will cap the maximum length of warnings to 1k
                        if (warnings.length() > 1028) {
                            warnings += " more ..";
                            break;
                        }
                    }

                    if (!warnings.isEmpty()) {
                        System.out.println(stdOut);
                        ret.put("warnings", warnings);
                    }
                }
                else {
                    String err = res.getStderr();
                    if (res.getExitStatus() == -9999) {
                        throw new Exception("User requested termination.");
                    }

                    boolean doThrowException = true;
                    if (command.containsKey("throwerror")) {
                        doThrowException = Boolean.parseBoolean(command.get("throwerror").toString());
                    }

                    if (doThrowException) {
                        throw new Exception(err);
                    }

                    String stdOut = res.getStdout();
                    ret.put("stdout", stdOut);
                    ret.put("stderr", err);
                }
            }
        }
        catch (Exception e) {
            if ((res != null) && (res.getExitStatus() == -9999)) {
                throw new NativeInterfaceException("Failed to execute." + e.getMessage(),
                        NativeInterfaceException.HttpCode.USER_CANCEL, e);
            }
            throw new NativeInterfaceException("Failed to execute." + e.getMessage(),
                    NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
        finally {
            if (addedToQ) {
                removeFromProcessQ(command);
            }
        }

        return ret;

    }

    /**
     * add to process tracking queue if command contains job id
     *
     * @param cmd
     * @param cmdRunner
     * @return true if successfully added
     */
    private static boolean addToProcessQ(JSONObject cmd, ICommandRunner cmdRunner) {
        boolean success = false;
        if (cmd.containsKey("jobId")) {
            jobProcesses.put(cmd.get("jobId").toString(), cmdRunner);
            success = true;
        }

        if (cmd.containsKey("jobId")) {
            progProcesses.put(cmd.get("jobId").toString(), cmdRunner);
            success = true;
        }

        return success;
    }

    /**
     * Remove from process queue if job exists
     *
     * @param cmd
     */
    private static void removeFromProcessQ(JSONObject cmd) {
        if (cmd.containsKey("jobId")) {
            String jobId = cmd.get("jobId").toString();
            jobProcesses.remove(jobId);
        }
    }

    private static void removeFromProgressProcessQ(String jobId) {
        if (progProcesses.containsKey(jobId)) {
            progProcesses.remove(jobId);
        }
    }

    /**
     * Terminates job if exists in the queue
     *
     * @param jobId
     *            : Job Id to terminate
     * @throws NativeInterfaceException
     */
    @Override
    public void terminate(String jobId) throws NativeInterfaceException {
        try {
            ICommandRunner cmdRunner = jobProcesses.get(jobId);
            if (cmdRunner != null) {
                cmdRunner.terminateClean();
            }
        }
        catch (Exception e) {
            throw new NativeInterfaceException("Failed to execute." + e.getMessage(),
                    NativeInterfaceException.HttpCode.SERVER_ERROR, e);
        }
    }

    /**
     * see CoreServiceContext.xml
     */
    public void init() {
        //
    }

    /**
     * see CoreServiceContext.xml
     */
    public void destroy() {
        //
    }

    /**
     * Creates direct exec call like hoot --ogr2osm target input output if the
     * "exectype" is "hoot"
     */
    private static String[] createCmdArray(JSONObject cmd) {
        List<String> execCmd = new ArrayList<>();
        execCmd.add("hoot");
        execCmd.add("--" + cmd.get("exec"));
        JSONArray params = (JSONArray) cmd.get("params");

        for (Object o : params) {
            JSONObject param = (JSONObject) o;
            Iterator<Object> iter = param.entrySet().iterator();
            String arg = "";
            while (iter.hasNext()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) iter.next();
                arg = (String) mEntry.getValue();
            }
            execCmd.add(arg);
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
            Iterator<Object> iter = param.entrySet().iterator();

            String arg = "";
            String key = "";
            while (iter.hasNext()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) iter.next();
                key = (String) mEntry.getKey();
                arg = (String) mEntry.getValue();
            }

            execCmd.add(key + "=" + arg);
        }

        String jobid = cmd.get("jobId").toString();
        execCmd.add("jobid=" + jobid);
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
            Iterator<Object> iter = param.entrySet().iterator();

            String arg = "";
            while (iter.hasNext()) {
                Map.Entry<Object, Object> mEntry = (Map.Entry<Object, Object>) iter.next();
                arg = (String) mEntry.getValue();
            }
            execCmd.add(arg);
        }

        if (cmd.get("jobId") != null) {
            String jobid = cmd.get("jobId").toString();
            execCmd.add(jobid);
        }

        Object[] objectArray = execCmd.toArray();

        return Arrays.copyOf(objectArray, objectArray.length, String[].class);
    }
}
