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
package hoot.services.command;

import static hoot.services.HootProperties.CORE_SCRIPT_PATH;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;


@Transactional
@Component
//@Profile("production")
public class ExternalCommandManagerImpl implements ExternalCommandManager {
    private static final Logger logger = LoggerFactory.getLogger(ExternalCommandManagerImpl.class);

    // This contains the command runner objects for the executing commands. Used for job cancellation and tracking.
    private static final Map<String, ExternalCommandRunner> activeCommands = new ConcurrentHashMap<>();

    @Override
    public String getProgress(String jobId) {
        String stdStr = "";
        ExternalCommandRunner commandRunner = activeCommands.get(jobId);
        if (commandRunner != null) {
            stdStr = commandRunner.getStdout();
        }

        return stdStr;
    }

    @Override
    public void terminate(String jobId) {
        ExternalCommandRunner cmdRunner = activeCommands.get(jobId);
        if (cmdRunner != null) {
            cmdRunner.terminate();
        }
    }

    @Override
    public CommandResult exec(String jobId, ExternalCommand command) {
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
            throw new IllegalArgumentException("Invalid exectype specified");
        }

        ExternalCommandRunner cmdRunner = new ExternalCommandRunnerImpl();

        if (jobId != null) {
            activeCommands.put(jobId, cmdRunner);
        }

        CommandResult commandResult;
        try {
            commandResult = cmdRunner.exec(commandArr, jobId, command.get("caller").toString());
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

        return execCmd.toArray(new String[execCmd.size()]);
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

        return execCmd.toArray(new String[execCmd.size()]);
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

        return execCmd.toArray(new String[execCmd.size()]);
    }
}