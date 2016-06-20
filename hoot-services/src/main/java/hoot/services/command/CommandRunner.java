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
package hoot.services.command;

import static org.apache.commons.io.IOUtils.closeQuietly;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.TreeMap;

import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.exception.ExceptionUtils;
import org.apache.commons.lang3.mutable.MutableBoolean;
import org.apache.commons.lang3.time.StopWatch;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Utility class for running a subprocess synchronously from Java and retrieving
 * the contents of System.out and/or system.err.
 */
public class CommandRunner implements ICommandRunner {
    private static final Logger logger = LoggerFactory.getLogger(CommandRunner.class);

    private final MutableBoolean sigInterrupt;
    private final List<CharPump> outputList = new ArrayList<>();

    private Process process;
    private int processState = -1;
    private StringWriter out;

    public CommandRunner() {
        sigInterrupt = new MutableBoolean(false);
    }

    public void terminate() {
        if (process != null) {
            process.destroy();
        }
    }

    @Override
    public int getProcessState() {
        return processState;
    }

    @Override
    public void terminateClean() throws Exception {
        sigInterrupt.setValue(true);

        if (outputList.size() > 1) {
            CharPump p1 = outputList.get(0);
            if (p1 != null) {
                synchronized (p1) {
                    p1.wait(1000);
                }
            }

            CharPump p2 = outputList.get(1);

            if (p2 != null) {
                synchronized (p2) {
                    p2.wait(1000);
                }
            }
        }
        cleanUpProcess();
    }

    @Override
    public String getStdOut() {
        return out.toString();
    }

    @Override
    public CommandResult exec(String cmd) throws IOException, InterruptedException {
        try (StringWriter out = new StringWriter();
             StringWriter err = new StringWriter()) {
            CommandResult result = exec(cmd, out, err);
            return result;
        }
    }

    @Override
    public CommandResult exec(String cmd, Map<String, String> env, boolean includeSysEnv)
            throws IOException, InterruptedException {
        CommandResult result = exec(cmd.split(" "), env, includeSysEnv);
        return result;
    }

    public CommandResult exec(String[] pCmd, String[] pEnv) throws IOException, InterruptedException {
        return exec(pCmd, pEnv, null);
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, File dir) throws IOException, InterruptedException {
        Map<String, String> env = new HashMap<>();
        for (String aPEnv : pEnv) {
            StringTokenizer st = new StringTokenizer(aPEnv, "=");
            env.put(st.nextToken(), st.nextToken());
        }
        return exec(pCmd, env, dir);
    }

    @Override
    public CommandResult exec(String[] cmd) throws IOException, InterruptedException {
        out = new StringWriter();

        try (StringWriter err = new StringWriter()) {
            CommandResult result = exec(cmd, out, err);
            return result;
        }
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv) throws IOException, InterruptedException {
        return exec(pCmd, pEnv, true);
    }

    @Override
    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, boolean useSysEnv)
            throws IOException, InterruptedException {
        try (StringWriter out = new StringWriter();
             StringWriter err = new StringWriter()) {
            CommandResult result = exec(pCmd, pEnv, useSysEnv, out, err);
            return result;
        }
    }

    // TRAC 1801
    private void cleanUpProcess() throws IOException {
        if (process == null) {
            return;
        }
        OutputStream os = process.getOutputStream();
        InputStream is = process.getInputStream();
        InputStream es = process.getErrorStream();

        // TRAC 1801
        if (os != null) {
            // System.err.println("Closing os");
            os.flush();
        }
        closeQuietly(os);
        closeQuietly(is);
        closeQuietly(es);

        process.destroy();
        process = null;
        // System.gc();
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {

        String pCmdString = ArrayUtils.toString(pCmd);
        if (logger.isInfoEnabled()) {
            logger.info("Executing '{}' with Environment '{}'", pCmdString, ArrayUtils.toString(pEnv));
        }

        StopWatch clock = new StopWatch();
        clock.start();

        int out = 0;
        try {
            process = Runtime.getRuntime().exec(pCmd, pEnv);
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, File dir, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {

        String pCmdString = ArrayUtils.toString(pCmd);
        if (logger.isInfoEnabled()) {
            logger.info("Executing '{}' with Environment '{}'", pCmdString, ArrayUtils.toString(pEnv));
        }

        StopWatch clock = new StopWatch();
        clock.start();

        int out = 0;
        try {
            process = Runtime.getRuntime().exec(pCmd, pEnv, dir);
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, File dir)
            throws IOException, InterruptedException {
        return exec(pCmd, pEnv, true, dir);
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, boolean useSysEnv, File dir)
            throws IOException, InterruptedException {
        try (StringWriter out = new StringWriter();
             StringWriter err = new StringWriter()) {
            CommandResult result = exec(pCmd, pEnv, useSysEnv, dir, out, err);
            return result;
        }
    }

    public CommandResult exec(String pCmd, File dir) throws IOException, InterruptedException {
        try (StringWriter out = new StringWriter();
             StringWriter err = new StringWriter()) {
            CommandResult result = exec(pCmd.split(" "), dir, out, err);
            return result;
        }
    }

    public CommandResult exec(String[] pCmd, File dir) throws IOException, InterruptedException {
        return exec(pCmd, dir, null);
    }

    @Override
    public CommandResult exec(String[] pCmd, Writer pOut, Writer pErr) throws IOException, InterruptedException {
        ProcessBuilder builder = new ProcessBuilder();
        Map<String, String> env = builder.environment();

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = Runtime.getRuntime().exec(pCmd);
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        catch (Exception e) {
            logger.error("Error occurred: ", e);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, boolean useSysEnv, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        ProcessBuilder builder = new ProcessBuilder();
        builder.command(pCmd);

        Map<String, String> env = builder.environment();
        if (!useSysEnv) {
            env.clear();
        }
        for (Map.Entry<String, String> entry : pEnv.entrySet()) {
            env.put(entry.getKey(), entry.getValue());
        }

        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = builder.start();
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, boolean useSysEnv, File dir, Writer pOut,
            Writer pErr) throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        ProcessBuilder builder = new ProcessBuilder();
        builder.command(pCmd);
        Map<String, String> env = builder.environment();
        if (!useSysEnv) {
            env.clear();
        }
        for (Map.Entry<String, String> entry : pEnv.entrySet()) {
            env.put(entry.getKey(), entry.getValue());
        }
        builder.directory(dir);

        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = builder.start();
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    @Override
    public CommandResult exec(String pCmd, Writer pOut, Writer pErr) throws IOException, InterruptedException {
        return exec(pCmd.split(" "), pOut, pErr);
    }

    public CommandResult exec(String pCmdString, File dir, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {
        return exec(pCmdString.split(" "), dir, pOut, pErr);
    }

    public CommandResult exec(String[] pCmd, File dir, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {
        ProcessBuilder builder = new ProcessBuilder();
        Map<String, String> env = builder.environment();

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = Runtime.getRuntime().exec(pCmd, null, dir);
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }

        if (sigInterrupt.getValue()) {
            out = -9999;
        }

        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    // private methods

    private int handleProcess(Process pProcess, String pOrigCmd, Writer pOut, Writer pErr, List<CharPump> stdOutErrList,
            MutableBoolean interrupt) throws InterruptedException {

        processState = 0;
        int res = handleProcessStatic(pProcess, pOrigCmd, pOut, pErr, stdOutErrList, interrupt);
        processState = 1;
        return res;
    }

    private static int handleProcessStatic(Process pProcess, String pOrigCmd, Writer pOut, Writer pErr,
            List<CharPump> stdOutErrList, MutableBoolean interrupt) throws InterruptedException {

        CharPump outpump = new CharPump(new BufferedReader(new InputStreamReader(pProcess.getInputStream())), pOut, interrupt);
        stdOutErrList.add(outpump);

        CharPump errpump = new CharPump(new BufferedReader(new InputStreamReader(pProcess.getErrorStream())), pErr, interrupt);
        stdOutErrList.add(errpump);

        outpump.start();
        errpump.start();
        outpump.join();
        errpump.join();

        if (logger.isInfoEnabled()) {
            logger.info("Waiting for '{}' to complete.", pOrigCmd);
        }

        int status = pProcess.waitFor();
        return status;
    }

    /** Inner class, copies from Reader to Writer. */
    public static class CharPump extends Thread {
        private static final Logger pumpLog = LoggerFactory.getLogger(CharPump.class);

        private final Reader iIn;
        private final Writer iOut;
        private final MutableBoolean interruptSig;

        private int bufSize;

        public CharPump(Reader pIn, Writer pOut, MutableBoolean pInterrupt) {
            interruptSig = pInterrupt;
            iIn = pIn;
            iOut = pOut;
            String bufSizeString = System.getProperty("ew.util.os.charPumpBuffer", "1024");
            try {
                bufSize = Integer.parseInt(bufSizeString);
            }
            catch (NumberFormatException ignored) {
                bufSize = 1024;
            }
            pumpLog.debug("CharPump will use buffer size = {}", bufSize);
        }

        @Override
        public void run() {
            boolean ok = true;
            char[] buf = new char[bufSize];
            synchronized (this) {
                try {
                    while (ok) {
                        ok = !interruptSig.getValue();
                        //?????????????????
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }

                        int n = iIn.read(buf, 0, buf.length);

                        if (n < 0) {
                            pumpLog.debug("CharPump has encountered EOF");
                            break;
                        }

                        //????????
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }

                        //????????
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }

                        iOut.write(buf, 0, n);

                        //?????????
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }

                        iOut.flush();
                    }
                    iOut.flush();

                }
                catch (Exception e) {
                    logger.error("", e);
                }
                finally {
                    try {
                        if (iIn != null) {
                            iIn.close();
                        }
                        if (iOut != null) {
                            iOut.close();
                        }
                    }
                    catch (IOException ignored) {
                    }
                    notifyAll();
                }
            }
        }
    }

    /**
     * Main routine, for testing. The name of the executable to run and its
     * arguments are the command line arguments.
     */
    // Disabled to prevent Commandline injection (Fortify)
    /*
     * public static void main(String[] pArgs) { try { ICommandRunner runner =
     * new CommandRunner(); if (1 == pArgs.length) { StringWriter out = new
     * StringWriter(); StringWriter err = new StringWriter();
     * runner.exec(pArgs[0], out, err); } else { runner.exec(pArgs); } //
     * System.out.println ( result.getStdout () ); // System.out.println (
     * result.getStderr () ); } catch (Exception e) {
     * System.err.println(e.toString()); } }
     */
    public CommandResult exec(String[] pCmd, File dir, String[] env) throws IOException, InterruptedException {
        try (StringWriter out = new StringWriter();
             StringWriter err = new StringWriter()) {
            CommandResult result = exec(pCmd, dir, out, err, env);
            return result;
        }
    }

    private CommandResult exec(String[] pCmd, File dir, StringWriter pOut, StringWriter pErr, String[] env)
            throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);

        if (logger.isDebugEnabled()) {
            logger.debug("Executing '{}' with Environment '{}'", pCmdString, ArrayUtils.toString(env));
        }

        StopWatch clock = new StopWatch();
        clock.start();

        try {
            process = Runtime.getRuntime().exec(pCmd, env, dir);
            out = handleProcess(process, pCmdString, pOut, pErr, outputList, sigInterrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (logger.isInfoEnabled()) {
                logger.info("'{}' completed in {} ms", pCmdString, clock.getTime());
            }
        }
        if (sigInterrupt.getValue()) {
            out = -9999;
        }
        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    private static void logExec(String pCmdString, Map<String, String> unsortedEnv) throws IOException {
        if (logger.isInfoEnabled()) {
            Map<String, String> env = new TreeMap<>();
            env.putAll(unsortedEnv);

            logger.info("Executing '{}'", pCmdString);
            logger.debug("Enviroment:");
            FileWriter writer = null;
            try {
                if (logger.isDebugEnabled()) {
                    File envvarFile = File.createTempFile("envvars", ".txt");
                    writer = new FileWriter(envvarFile);
                    logger.debug("ENVVARS will be written to {}", envvarFile.getAbsolutePath());
                }
                for (Map.Entry<String, String> entry : env.entrySet()) {
                    logger.debug(String.format("  %s", entry.getKey() + "=" + entry.getValue()));
                    if (logger.isDebugEnabled() && (writer != null)) {
                        writer.write(String.format("  %s%n", new Object[]{entry.getKey() + "=" + entry.getValue()}));
                    }
                }
                if (logger.isDebugEnabled() && (writer != null)) {
                    writer.close();
                }
            }
            catch (Exception e) {
                logger.error("Unable to log exec call: {}", ExceptionUtils.getStackTrace(e));
            }
            finally {
                if (writer != null) {
                    writer.close();
                }
            }
        }
    }
}