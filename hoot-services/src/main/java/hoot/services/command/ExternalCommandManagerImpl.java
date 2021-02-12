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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.command;

import java.io.File;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Transactional;


@Transactional
@Component
public class ExternalCommandManagerImpl implements ExternalCommandManager {
    private static final Logger logger = LoggerFactory.getLogger(ExternalCommandManagerImpl.class);

    // Contains currently active ExternalCommandRunner instances. Used for job tracking.
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
    public CommandResult exec(String jobId, ExternalCommand externalCommand) {
        ExternalCommandRunner cmdRunner = new ExternalCommandRunnerImpl();

        String command = externalCommand.getCommand();
        File workDir = externalCommand.getWorkDir();
        String caller = externalCommand.getCaller();
        Boolean trackable = externalCommand.getTrackable();
        Map<String, ?> substitutionMap = externalCommand.getSubstitutionMap();

        if (jobId != null) {
            activeCommands.put(jobId, cmdRunner);
        }

        CommandResult commandResult;
        try {
            commandResult = cmdRunner.exec(command, substitutionMap, jobId, caller, workDir, trackable);
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

    @Override
    public CommandResult exec(ExternalCommand externalCommand) {
        return exec(null, externalCommand);
    }
}