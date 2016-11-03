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
package hoot.services.controllers.auxiliaryservices;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.management.ManagementFactory;
import java.util.ArrayList;
import java.util.List;

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
        final Process serverProcess = builder.start();

        // Pumping the output to service output stream
        // Also, if there is no handler to pump out the std and stderr stream
        // for Processbuilder (Also applies to Runtime.exe)
        // the outputs get built up and then end up locking up process. Quite nasty!
        new Thread() {
            @Override
            public void run() {
                try {
                    processStreamHandler(serverProcess);
                }
                catch (IOException ioe) {
                    logger.error("Error during a call to processStreamHandler()", ioe);
                }
            }
        }.start();

        return serverProcess;
    }

    static void stopServer(String processSignature) throws IOException {
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

            // And then gets the status
            boolean isRunning = false;
            if (transServerPID > 0) {
                try {
                    Runtime runtime = Runtime.getRuntime();

                    // kill -0 checks if a process with the given PID is running
                    // and you have the permission to send a signal to it.
                    Process statusProcess = runtime.exec(new String[] { "kill", "-0", String.valueOf(transServerPID)});

                    // usually we should not get any output but just in case if we get some error..
                    processStreamHandler(statusProcess);

                    int exitCode = statusProcess.waitFor();
                    isRunning = (exitCode == 0);
                }
                catch (Exception e) {
                    throw new RuntimeException("Error running 'kill -0 " + transServerPID + "'", e);
                }
            }

            return isRunning;
        }
        else {
            logger.error("server has started but failed to get process ID."
                    + " You will not be able to stop server through the service. To stop, use the manual process!");

            return false;
        }
    }

    private static void closeAllServers(String processSignature) throws IOException {
        // Note that we kill process that contains the name of server script
        Process killProc = Runtime.getRuntime().exec("pkill -f " + processSignature);
        processStreamHandler(killProc);
    }

    private static void processStreamHandler(Process proc) throws IOException {
        try (InputStreamReader stdStream = new InputStreamReader(proc.getInputStream())) {
            try (BufferedReader stdInput = new BufferedReader(stdStream)) {
                try (InputStreamReader stdErrStream = new InputStreamReader(proc.getErrorStream())) {
                    try (BufferedReader stdError = new BufferedReader(stdErrStream)) {
                        String s;
                        while ((s = stdInput.readLine()) != null) {
                            logger.info(s);
                        }

                        while ((s = stdError.readLine()) != null) {
                            logger.error(s);
                        }
                    }
                }
            }
        }
    }
}
