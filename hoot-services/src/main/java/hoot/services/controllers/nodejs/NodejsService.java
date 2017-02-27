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
package hoot.services.controllers.nodejs;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.lang.management.ManagementFactory;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.exec.CommandLine;
import org.apache.commons.exec.DefaultExecutor;
import org.apache.commons.exec.ExecuteException;
import org.apache.commons.exec.ExecuteStreamHandler;
import org.apache.commons.exec.Executor;
import org.apache.commons.exec.PumpStreamHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


abstract class NodejsService {
    private static final Logger logger = LoggerFactory.getLogger(NodejsService.class);

    static Process startServer(String port, String threadCount, String serverScript) throws IOException {
        List<String> command = new ArrayList<>();
        command.add("node");
        command.add(serverScript);
        command.add("port=" + port);
        command.add("threadcount=" + threadCount);

        ProcessBuilder builder = new ProcessBuilder(command);

        // This combines stdout and stderr into one stream
        builder.redirectErrorStream(true);
        Process serverProcess = builder.start();

        // Pumping the output to service output stream
        // Also, if there is no handler to pump out the std and stderr stream
        // for Processbuilder (Also applies to Runtime.exe)
        // the outputs get built up and then end up locking up process. Quite nasty!
        new Thread(() -> {
            try {
                try (InputStreamReader combinedStream = new InputStreamReader(serverProcess.getInputStream())) {
                    try (BufferedReader bufferedCombinedStream = new BufferedReader(combinedStream)) {
                        StringBuilder combinedOutput = new StringBuilder();
                        String line;
                        while ((line = bufferedCombinedStream.readLine()) != null) {
                            combinedOutput.append(line).append(System.lineSeparator());
                        }
                        logger.info(combinedOutput.toString());
                    }
                }
            }
            catch (IOException ioe) {
                logger.error("Error during a call to processStreamHandler()", ioe);
            }
        }).start();

        return serverProcess;
    }

    static void stopServer(String processSignature) {
        closeAllServers(processSignature);
    }

    private static Integer getProcessId() {
        // this may not work with every JVM implementation
        return Integer.parseInt(ManagementFactory.getRuntimeMXBean().getName().split("@")[0]);
    }

    static boolean getStatus(Process serverProc) {
        // We first get the server process ID
        if (serverProc.getClass().getName().equals("java.lang.UNIXProcess")) {
            Integer transServerPID = getProcessId();
            return (transServerPID > 0) && isProcessRunning(String.valueOf(transServerPID));
        }
        else {
            logger.error("Server has started but failed to get process ID.  " +
                         "You will not be able to stop server through the service.  " +
                         "To stop, use the manual process!");

            return false;
        }
    }

    private static void closeAllServers(String processSignature) {
        // First check if the process is still running
        if (isProcessRunning(processSignature)) {
            // The process is running.  We want to stop it.
            // Note that we kill process that contains the name of server script
            String[] command = {"pkill", "-f", processSignature};

            try (OutputStream stdout = new ByteArrayOutputStream();
                 OutputStream stderr = new ByteArrayOutputStream()) {

                CommandLine cmdLine = new CommandLine(command[0]);
                for (int i = 1; i < command.length; i++) {
                    cmdLine.addArgument(command[i], false);
                }

                ExecuteStreamHandler executeStreamHandler = new PumpStreamHandler(stdout, stderr);
                Executor executor = new DefaultExecutor();
                executor.setStreamHandler(executeStreamHandler);
                executor.execute(cmdLine);
            }
            catch (ExecuteException ignored) {
                logger.error("Failed to execute {}!", Arrays.toString(command));
            }
            catch (IOException e) {
                logger.error("Error encountered while executing {} command!", Arrays.toString(command), e);
            }
        }
    }

    private static boolean isProcessRunning(String processSignature) {
        // kill -0 checks if a process with the given PID is running
        // and you have the permission to send a signal to it.
        String[] command = { "pkill", "-0", processSignature};

        try (OutputStream stdout = new ByteArrayOutputStream();
             OutputStream stderr = new ByteArrayOutputStream()) {

            CommandLine cmdLine = new CommandLine(command[0]);
            for (int i = 1; i < command.length; i++) {
                cmdLine.addArgument(command[i], false);
            }

            ExecuteStreamHandler executeStreamHandler = new PumpStreamHandler(stdout, stderr);
            Executor executor = new DefaultExecutor();
            executor.setStreamHandler(executeStreamHandler);
            executor.execute(cmdLine);
        }
        catch (ExecuteException ignored) {
            return false;
        }
        catch (IOException e) {
            logger.error("Error executing {} command!", Arrays.toString(command), e);
        }

        return false;
    }
}
