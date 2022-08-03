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
 * @copyright Copyright (C) 2016, 2017, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.command;


import static hoot.services.HootProperties.replaceSensitiveData;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
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
import org.apache.commons.exec.LogOutputStream;
import org.apache.commons.exec.PumpStreamHandler;
import org.apache.commons.exec.util.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.utils.DbUtils;


/**
 * Utility class for running a subprocess synchronously from Java.
 */
public class ExternalCommandRunnerImpl implements ExternalCommandRunner {
    private static final Logger logger = LoggerFactory.getLogger(ExternalCommandRunnerImpl.class);
    private static final Pattern pattern = Pattern.compile("(STATUS\\s+(.*)\\w+\\s+)\\((\\d+)%\\):"); //eg. STATUS Convert (59%):

    private ExecuteWatchdog watchDog;
    private OutputStream stdout;
    private OutputStream stderr;

    public ExternalCommandRunnerImpl() {}

    public String obfuscateConsoleLog(String in) {
        //strip out the path to the cpp code
        String out = in.replaceAll("([\\w.]+\\/).+?\\(\\s+\\d+\\)\\s*", "");

        //strip out leading newlines
        out = out.replaceFirst("^\\n", "");

        //strip out db connection string
        //e.g. hootapidb://hoot:
        out = out.replaceAll("hootapidb:\\/\\/\\w+:", "");
        // seperated because some of the status output doesnt include the above text
        //e.g. hoottest@localhost:5432/hoot
        out = out.replaceAll("\\w+@\\w+:\\d+\\/\\w+", "<hootapidb>");

        //strip out hoot path string
        //e.g. /home/vagrant/hoot/userfiles/tmp/upload
        //e.g. /home/vagrant/hoot/userfiles/tmp
        out = out.replaceAll(HootProperties.HOME_FOLDER, "<path>");

        //escape single quotes
        out = out.replaceAll("'", "''");

        //strip out oauth tokens
        out = Pattern.compile("(hoot\\.osm\\.auth\\.\\S+)=\\w+\\s+")
            .matcher(out)
            .replaceAll("$1=<redacted> ");

        //strip out oauth2 tokens
        out = Pattern.compile("(hoot\\.services\\.oauth2\\.\\S+)=\\S+\\s+")
                .matcher(out)
                .replaceAll("$1=<redacted> ");

        //strip out osm api url, but not other remote data urls
        out = out.replaceAll("https?://\\S+?/api/0.6/map", "<osmapi>/api/0.6/map");

        return out;
    }

    @Override
    public CommandResult exec(String commandTemplate, Map<String, ?> substitutionMap, String jobId, String caller, File workingDir, Boolean trackable) {
        String obfuscatedCommand = commandTemplate;

        CommandResult commandResult = new CommandResult();

        this.stdout = new LogOutputStream() {
            @Override
            protected void processLine(String line, int level) {
                String currentOut = commandResult.getStdout() != null ? commandResult.getStdout() : "";
                String currentLine = obfuscateConsoleLog(line) + "\n";

                // Had to add because ran into case where same line was processed twice in a row
                if(!currentOut.equals(currentLine)) {
                    currentOut = currentOut.concat(currentLine);
                    commandResult.setStdout(currentOut);

                    if (trackable) {
                        // if includes percent progress, update that as well
                        Matcher matcher = pattern.matcher(currentLine);
                        if (matcher.find()) {
                            commandResult.setPercentProgress(Integer.parseInt(matcher.group(3))); // group 3 is the percent from the pattern regex
                        }

                        // update command status table stdout
                        DbUtils.upsertCommandStatus(commandResult);
                        logger.info("Command stdout: {}", currentLine);
                    }
                }
            }
        };

        this.stderr = new LogOutputStream() {
            @Override
            protected void processLine(String line, int level) {
                String currentErr = commandResult.getStderr() != null ? commandResult.getStderr() : "";
                String currentLine = obfuscateConsoleLog(line) + "\n";

                // Had to add because ran into case where same line was processed twice in a row
                if(!currentErr.equals(currentLine)) {
                    logger.error(line);

                    currentErr = currentErr.concat(currentLine);
                    commandResult.setStderr(currentErr);

                    if (trackable) {
                        // update command status table stderr
                        DbUtils.upsertCommandStatus(commandResult);
                    }
                }
            }
        };

        ExecuteStreamHandler executeStreamHandler = new PumpStreamHandler(this.stdout, this.stderr);
        Executor executor = new DefaultExecutor();
        this.watchDog = new ExecuteWatchdog(ExecuteWatchdog.INFINITE_TIMEOUT);
        executor.setWatchdog(this.watchDog);
        executor.setStreamHandler(executeStreamHandler);

        if (workingDir != null) {
            executor.setWorkingDirectory(workingDir);
        }

        CommandLine cmdLine = parse(commandTemplate, expandSensitiveProperties(substitutionMap));

        // Sensitive params obfuscated
        obfuscatedCommand = obfuscateConsoleLog(Arrays.stream(parse(commandTemplate, substitutionMap).toStrings())
                .collect(Collectors.joining(" ")));

        LocalDateTime start = LocalDateTime.now();
        Exception exception = null;
        int exitCode;

        commandResult.setStart(start);
        commandResult.setCommand(obfuscatedCommand);
        commandResult.setJobId(jobId);
        commandResult.setCaller(caller);
        commandResult.setWorkingDir(workingDir);
        commandResult.setStdout("");
        commandResult.setStderr("");

        if (trackable) {
            // Add the new command to the command status table
            DbUtils.upsertCommandStatus(commandResult);
        }

        try {
            logger.info("Command {} started at: [{}]", obfuscatedCommand, start);

            // TODO: Async approach but some commands need to be executed serially
            // TODO: Will require changing JobRunnable.class processJob() as well because it blocks
            // executor.execute(cmdLine, new ExecuteResultHandler() {
            //     @Override
            //     public void onProcessComplete(int exitValue) {
            //
            //     }
            //
            //     @Override
            //     public void onProcessFailed(ExecuteException e) {
            //
            //     }
            // });

            exitCode = executor.execute(cmdLine);
        }
        catch (Exception e) {
            exitCode = CommandResult.FAILURE;
            exception = e;

            // Need to perform cleanup for import jobs
            Long mapId = jobId != null ? DbUtils.getMapIdByJobId(jobId) : null;
            if (mapId != null) {
                DbUtils.deleteMapRelatedTablesByMapId(mapId);
                DbUtils.deleteMap(mapId);
            }
        }
        finally {
            try {
                this.stdout.flush();
                this.stderr.flush();

                this.stdout.close();
                this.stderr.close();
            } catch (IOException e) {
                logger.error("Failed to close output streams", e);
            }
        }

        LocalDateTime finish = LocalDateTime.now();

        commandResult.setExitCode(exitCode);
        commandResult.setFinish(finish);

        if (trackable) {
            DbUtils.completeCommandStatus(commandResult);
            logger.info("Command completed: {}", obfuscatedCommand);
        }

        if (commandResult.failed()) {
            if(this.watchDog.killedProcess()) {
                // it was killed on purpose by the watchdog
                logger.info("Process for {} command was killed!", obfuscatedCommand);
            } else {
                logger.error("FAILURE of: {}", commandResult, exception);
            }
        }
        else {
            logger.debug("SUCCESS of: {}", commandResult);
        }

        return commandResult;
    }

    public static CommandLine parse(String commandTemplate, Map<String, ?> substitutionMap) {
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
            logger.info("destroy processd");
        } else {
            logger.info("process already killed");
        }
    }
}
