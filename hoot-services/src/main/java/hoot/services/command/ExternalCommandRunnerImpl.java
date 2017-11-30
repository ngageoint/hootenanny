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
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.exec.ExecuteStreamHandler;
import org.apache.commons.exec.ExecuteWatchdog;
import org.apache.commons.exec.Executor;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.commons.exec.util.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
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
    public CommandResult exec(String commandTemplate, Map<String, ?> substitutionMap, String jobId, String caller, File workingDir, Boolean trackable) {
        String obfuscatedCommand = commandTemplate;

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

            CommandLine cmdLine = parse(commandTemplate, expandSensitiveProperties(substitutionMap));

            // Sensitive params included
            String actualCommand = Arrays.stream(quoteArgsWithSpaces(cmdLine.toStrings()))
                    .collect(Collectors.joining(" "));

            // Sensitive params obfuscated
            obfuscatedCommand = Arrays.stream(quoteArgsWithSpaces(parse(commandTemplate, substitutionMap).toStrings()))
                    .collect(Collectors.joining(" "));

            LocalDateTime start = null;
            Exception exception = null;
            int exitCode;

            try {
                start = LocalDateTime.now();

                logger.info("Command {} started at: [{}]", obfuscatedCommand, start);

                exitCode = executor.execute(cmdLine);
            }
            catch (Exception e) {
                exitCode = CommandResult.FAILURE;
                exception = e;
            }

            if (executor.isFailure(exitCode) && this.watchDog.killedProcess()) {
                // it was killed on purpose by the watchdog
                logger.info("Process for {} command was killed!", obfuscatedCommand);
            }

            LocalDateTime finish = LocalDateTime.now();

            //, exitCode, stdout.toString(), stderr.toString()
            CommandResult commandResult = new CommandResult();
            commandResult.setCommand(obfuscatedCommand);
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
            throw new RuntimeException("Error executing: " + obfuscatedCommand, e);
        }
    }

    /**
     * Adds quotes to arguments that contain spaces
     *
     * @param args arguments to quotes
     * @return array of arguments with the ones with spaces having quotes around them
     */
    private static String[] quoteArgsWithSpaces(String[] args) {
        return Arrays.stream(args).map(s -> "\"" + s + "\"").toArray(String[]::new);
    }

    private static CommandLine parse(String commandTemplate, Map<String, ?> substitutionMap) {
        String[] tokens = commandTemplate.split(" ");

        CommandLine cl = new CommandLine((String) expandToken(tokens[0].trim(), substitutionMap));

        for (int i = 1; i < tokens.length; i++) {
            Object token = expandToken(tokens[i].trim(), substitutionMap);
            if (token instanceof String) {
                String arg = (String) token;
                addArgument(cl, arg);
            }
            else if (token instanceof Collection) {
                Collection<String> args = (Collection<String>) token;
                args.forEach(arg -> addArgument(cl, arg));
            }
            else {
                throw new RuntimeException("No substitute provided for : ${" + token + "}");
            }
        }

        return cl;
    }

    private static void addArgument(CommandLine cl, String arg) {
        if (!org.apache.commons.lang3.StringUtils.isBlank(arg)) {
            cl.addArgument(arg, StringUtils.isQuoted(arg));
        }
    }

    private static Object expandToken(String token, Map<String, ?> substitutionMap) {
        Pattern pattern = Pattern.compile("\\$\\{(.*?)\\}"); // matches ${} pattern
        Matcher matcher = pattern.matcher(token);

        if (matcher.find()) {
            String key = matcher.group(1);
            Object replacement = substitutionMap.get(key);
            if (replacement == null) {
                throw new RuntimeException("Could not expand: " + token + ".  No substitute provided!");
            }
            else if (replacement instanceof Collection) {
                return replacement;
            }
            else {
                return token.substring(0, matcher.start()) + replacement + token.substring(matcher.end(), token.length());
            }
        }
        else {
            return token;
        }
    }

    private static Map<String, ?> expandSensitiveProperties(Map<String, ?> substituteMap) {
        Map<String, Object> newMap = new HashMap<>();

        for (Map.Entry<String, ?> entry : substituteMap.entrySet()) {
            if (entry.getValue() instanceof String) {
                newMap.put(entry.getKey(), replaceSensitiveData((String) entry.getValue()));
            }
            else {
                Collection<String> values = (Collection<String>) entry.getValue();
                newMap.put(entry.getKey(), values.stream().map(HootProperties::replaceSensitiveData).collect(Collectors.toList()));
            }
        }

        return newMap;
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