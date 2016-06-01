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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.nodeJs;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.management.ManagementFactory;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


public class ServerControllerBase {
    private static final Logger log = LoggerFactory.getLogger(ServerControllerBase.class);

    private Process _serverProc = null;

    protected Process startServer(final String currPort, final String currThreadCnt, final String serverScript)
            throws Exception {
        List<String> command = new ArrayList<String>();
        command.add("node");
        command.add(serverScript);
        command.add("port=" + currPort);
        command.add("threadcount=" + currThreadCnt);
        ProcessBuilder builder = new ProcessBuilder(command);

        // This combines stdout and stderr into one stream
        builder.redirectErrorStream(true);
        _serverProc = builder.start();

        // Pumping the output to service output stream
        // Also, if there is no handler to pump out the std and stderr stream
        // for Processbuilder (Also applies to Runtime.exe)
        // the outputs get built up and then end up locking up process. Quite
        // nesty!
        new Thread() {
            @Override
            public void run() {
                try {
                    _processStreamHandler(_serverProc, true);
                }
                catch (IOException e) {
                    log.error(e.getMessage());
                }
            }
        }.start();

        return _serverProc;
    }

    protected void stopServer(final String processSignature) throws Exception {
        _closeAllServers(processSignature);
    }

    private static Integer getProcessId() {
        // this may not work with every JVM implementation
        return Integer.parseInt(ManagementFactory.getRuntimeMXBean().getName().split("@")[0]);
    }

    protected boolean getStatus(final Process serverProc) throws Exception {
        boolean isRunning = false;
        // We first get the server process ID
        Integer transServerPID = null;
        if (serverProc.getClass().getName().equals("java.lang.UNIXProcess")) {
            transServerPID = getProcessId();
        }
        else {
            log.error("server has started but failed to get process ID."
                    + " You will not be able to stop server through service. To stop use manual process!");
        }

        // And then gets the status
        if (transServerPID != null && transServerPID > 0) {
            try {
                Runtime runtime = Runtime.getRuntime();

                Process statusProcess = runtime.exec(new String[] { "kill", "-0", "" + transServerPID });

                // usually we should not get any output but just in case if we
                // get some error..
                _processStreamHandler(statusProcess, false);

                int exitCode = statusProcess.waitFor();
                if (exitCode == 0) {
                    isRunning = true;
                }
            }
            catch (Exception e) {
                isRunning = false;
            }
        }
        return isRunning;
    }

    private static void _closeAllServers(final String processSignature) throws IOException {
        // Note that we kill process that contains the name of server script
        Process killProc = Runtime.getRuntime().exec("pkill -f " + processSignature);
        _processStreamHandler(killProc, false);
    }

    private static void _processStreamHandler(final Process proc, boolean doSendToStdout) throws IOException {
        // usually we should not get any output but just in case if we get some
        // error..
        InputStreamReader stdStream = null;
        BufferedReader stdInput = null;

        InputStreamReader stdErrStream = null;
        BufferedReader stdError = null;

        try {
            stdStream = new InputStreamReader(proc.getInputStream());
            stdInput = new BufferedReader(stdStream);

            stdErrStream = new InputStreamReader(proc.getErrorStream());
            stdError = new BufferedReader(stdErrStream);

            String s = null;
            while ((s = stdInput.readLine()) != null) {
                log.info(s);
                if (doSendToStdout) {
                    System.out.println(s);
                }
            }

            while ((s = stdError.readLine()) != null) {
                log.error(s);
                if (doSendToStdout) {
                    System.out.println(s);
                }
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
        }
        finally {
            if (stdStream != null) {
                stdStream.close();
            }
            if (stdInput != null) {
                stdInput.close();
            }

            if (stdErrStream != null) {
                stdErrStream.close();
            }
            if (stdError != null) {
                stdError.close();
            }
        }
    }
}
