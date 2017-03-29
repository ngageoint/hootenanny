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


import static hoot.services.HootProperties.replaceSensitiveData;
import static hoot.services.models.db.QCommandStatus.commandStatus;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Timestamp;
import java.time.LocalDateTime;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.exec.ExecuteStreamHandler;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.Executor;
import org.apache.commons.exec.PumpStreamHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.models.db.CommandStatus;


/**
 * Utility class for running a subprocess synchronously from Java.
 */
public class ExternalCommandRunnerImpl implements ExternalCommandRunner {
    private static final Logger logger = LoggerFactory.getLogger(ExternalCommandRunnerImpl.class);

    private ExecuteWatchdog watchDog;
    private OutputStream stdout;
    private OutputStream stderr;

    public ExternalCommandRunnerImpl() {}

    @Override
    public CommandResult exec(String command, String jobId, String caller, File workingDir, Boolean trackable) {
        logger.debug("About to execute the following command: {}", commandArrayToString(command, caller));

        try (OutputStream stdout = new ByteArrayOutputStream();
             OutputStream stderr = new ByteArrayOutputStream()) {

            this.stdout = stdout;
            this.stderr = stderr;

            ExecuteStreamHandler executeStreamHandler = new PumpStreamHandler(stdout, stderr);
            Executor executor = new DefaultExecutor();
            this.watchDog = new ExecuteWatchdog(ExecuteWatchdog.INFINITE_TIMEOUT);
            executor.setWatchdog(this.watchDog);
            executor.setStreamHandler(executeStreamHandler);

            if (workingDir != null) {
                executor.setWorkingDirectory(workingDir);
            }

            LocalDateTime start = null;
            Exception exception = null;
            int exitCode;

            try {
                CommandLine cmdLine = CommandLine.parse(replaceSensitiveData(command));

                start = LocalDateTime.now();

                logger.debug("Command {} started at: {}", commandArrayToString(command, caller), start);

                exitCode = executor.execute(cmdLine);
            }
            catch (Exception e) {
                exitCode = CommandResult.FAILURE;
                exception = e;
            }

            if (executor.isFailure(exitCode) && this.watchDog.killedProcess()) {
                // it was killed on purpose by the watchdog
                logger.info("Process for {} command was killed!", commandArrayToString(command, caller));
            }

            LocalDateTime finish = LocalDateTime.now();

            //, exitCode, stdout.toString(), stderr.toString()
            CommandResult commandResult = new CommandResult();
            commandResult.setCommand(command);
            commandResult.setCaller(caller);
            commandResult.setExitCode(exitCode);
            commandResult.setStderr(stderr.toString());
            commandResult.setStdout(stdout.toString());
            commandResult.setStart(start);
            commandResult.setFinish(finish);
            commandResult.setJobId(jobId);
            commandResult.setWorkingDir(workingDir);

            if (trackable) {
                updateDatabase(commandResult);
            }

            if (commandResult.failed()) {
                logger.error("FAILURE of: {}", commandResult, exception);
            }
            else {
                logger.debug("SUCCESS of: {}", commandResult);
            }

            return commandResult;
        }
        catch (IOException e) {
            throw new RuntimeException("Error executing: " + commandArrayToString(command, caller), e);
        }
    }

    private static void updateDatabase(CommandResult commandResult) {
        CommandStatus cmdStatus = new CommandStatus();
        cmdStatus.setCommand(commandResult.getCommand());
        cmdStatus.setExitCode(commandResult.getExitCode());
        cmdStatus.setFinish(Timestamp.valueOf(commandResult.getFinish()));
        cmdStatus.setStart(Timestamp.valueOf(commandResult.getStart()));
        cmdStatus.setJobId(commandResult.getJobId());
        cmdStatus.setStderr(commandResult.getStderr());
        cmdStatus.setStdout(commandResult.getStdout());

        Long id = createQuery().insert(commandStatus).populate(cmdStatus).executeWithKey(commandStatus.id);

        commandResult.setId(id);
    }

    private static String commandArrayToString(String command, String caller) {
        return "[" + command + "], Caller=" + caller;
    }

    @Override
    public String getStdout() {
        return this.stdout.toString();
    }

    @Override
    public String getStderr() {
        return this.stderr.toString();
    }

    @Override
    public void terminate() {
        if (!this.watchDog.killedProcess()) {
            this.watchDog.destroyProcess();
        }
    }
}