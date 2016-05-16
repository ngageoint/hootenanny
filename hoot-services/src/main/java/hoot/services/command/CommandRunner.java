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
import org.apache.log4j.Logger;


/**
 * Utility class for running a subprocess synchronously from Java and retrieving
 * the contents of System.out and/or system.err.
 */
public class CommandRunner implements ICommandRunner {

    private static final Logger _log = Logger.getLogger(CommandRunner.class);
    private Process process;
    private MutableBoolean sig_interrupt;
    private List<CharPump> _outputList = new ArrayList<CharPump>();

    private int _processState = -1;

    private StringWriter _out = null;
    private StringWriter _err = null;

    public CommandRunner() {
        sig_interrupt = new MutableBoolean(false);
    }

    public void terminate() {
        if (process != null) {
            process.destroy();
        }
    }

    @Override
    public int getProcessState() {
        return _processState;
    }

    @Override
    public void terminateClean() throws Exception {
        sig_interrupt.setValue(true);

        if (_outputList.size() > 1) {
            CharPump p1 = _outputList.get(0);
            if (p1 != null) {
                synchronized (p1) {
                    p1.wait(1000);
                }
            }

            CharPump p2 = _outputList.get(1);

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
        return _out.toString();
    }

    @Override
    public CommandResult exec(String pCmd) throws IOException, InterruptedException {
        StringWriter out = new StringWriter();
        StringWriter err = new StringWriter();

        CommandResult result = exec(pCmd, out, err);
        return result;
    }

    @Override
    public CommandResult exec(String pCmd, Map<String, String> env, boolean includeSysEnv)
            throws IOException, InterruptedException {

        CommandResult result = exec(pCmd.split(" "), env, includeSysEnv);
        return result;
    }

    public CommandResult exec(String[] pCmd, String[] pEnv) throws IOException, InterruptedException {
        return exec(pCmd, pEnv, null);
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, File dir) throws IOException, InterruptedException {
        Map<String, String> env = new HashMap<String, String>();
        for (int i = 0; i < pEnv.length; i++) {
            StringTokenizer st = new StringTokenizer(pEnv[i], "=");
            env.put(st.nextToken(), st.nextToken());
        }
        return exec(pCmd, env, dir);
    }

    @Override
    public CommandResult exec(String[] pCmd) throws IOException, InterruptedException {
        _out = new StringWriter();
        _err = new StringWriter();

        CommandResult result = exec(pCmd, _out, _err);
        return result;
    }

    public CommandResult exec(String[] pCmd, Map<String, String> pEnv) throws IOException, InterruptedException {
        return exec(pCmd, pEnv, true);
    }

    @Override
    public CommandResult exec(String[] pCmd, Map<String, String> pEnv, boolean useSysEnv)
            throws IOException, InterruptedException {
        StringWriter out = new StringWriter();
        StringWriter err = new StringWriter();

        CommandResult result = exec(pCmd, pEnv, useSysEnv, out, err);
        return result;
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

        if (process != null) {
            process.destroy();
            process = null;
        }
        // System.gc();
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        if (_log.isInfoEnabled())
            _log.info("Executing '" + pCmdString + "' with Environment '" + ArrayUtils.toString(pEnv) + "'");
        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = Runtime.getRuntime().exec(pCmd, pEnv);
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmdString + "' completed in " + clock.getTime() + " ms");
        }
        if (sig_interrupt.getValue() == true) {
            out = -9999;
        }
        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    public CommandResult exec(String[] pCmd, String[] pEnv, File dir, Writer pOut, Writer pErr)
            throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        if (_log.isInfoEnabled())
            _log.info("Executing '" + pCmdString + "' with Environment '" + ArrayUtils.toString(pEnv) + "'");
        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = Runtime.getRuntime().exec(pCmd, pEnv, dir);
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmdString + "' completed in " + clock.getTime() + " ms");
        }

        if (sig_interrupt.getValue() == true) {
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
        StringWriter out = new StringWriter();
        StringWriter err = new StringWriter();

        CommandResult result = exec(pCmd, pEnv, useSysEnv, dir, out, err);
        return result;
    }

    public CommandResult exec(String pCmd, File dir) throws IOException, InterruptedException {
        StringWriter out = new StringWriter();
        StringWriter err = new StringWriter();

        CommandResult result = exec(pCmd.split(" "), dir, out, err);
        return result;
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
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        catch (Exception e) {
            // System.out.println(e.fillInStackTrace().toString());
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmdString + "' completed in " + clock.getTime() + " ms");
        }

        if (sig_interrupt.getValue() == true) {
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
        if (!useSysEnv)
            env.clear();
        for (String name : pEnv.keySet()) {
            env.put(name, pEnv.get(name));
        }

        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = builder.start();
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmdString + "' completed in " + clock.getTime() + " ms");
        }

        if (sig_interrupt.getValue() == true) {
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
        if (!useSysEnv)
            env.clear();
        for (String name : pEnv.keySet()) {
            env.put(name, pEnv.get(name));
        }
        builder.directory(dir);

        logExec(pCmdString, env);

        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = builder.start();
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmdString + "' completed in " + clock.getTime() + " ms");
        }
        if (sig_interrupt.getValue() == true) {
            out = -9999;
        }
        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    @Override
    public CommandResult exec(String pCmdString, Writer pOut, Writer pErr) throws IOException, InterruptedException {
        return exec(pCmdString.split(" "), pOut, pErr);
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
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmd + "' completed in " + clock.getTime() + " ms");
        }
        if (sig_interrupt.getValue() == true) {
            out = -9999;
        }
        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    // private methods

    private int handleProcess(Process pProcess, String pOrigCmd, Writer pOut, Writer pErr, List<CharPump> stdOutErrList,
            MutableBoolean interrupt) throws InterruptedException {

        _processState = 0;
        int res = handleProcessStatic(pProcess, pOrigCmd, pOut, pErr, stdOutErrList, interrupt);
        _processState = 1;
        return res;
    }

    private static int handleProcessStatic(Process pProcess, String pOrigCmd, Writer pOut, Writer pErr,
            List<CharPump> stdOutErrList, MutableBoolean interrupt) throws InterruptedException {

        CharPump outpump = new CharPump(new BufferedReader(new InputStreamReader(pProcess.getInputStream())), pOut,
                interrupt);
        stdOutErrList.add(outpump);
        CharPump errpump = new CharPump(new BufferedReader(new InputStreamReader(pProcess.getErrorStream())), pErr,
                interrupt);
        stdOutErrList.add(errpump);
        outpump.start();
        errpump.start();
        outpump.join();
        errpump.join();

        if (_log.isInfoEnabled())
            _log.info("Waiting for '" + pOrigCmd + "' to complete.");
        int status = pProcess.waitFor();

        return status;
    }

    /** Inner class, copies from Reader to Writer. */
    public static class CharPump extends Thread {

        private static Logger pumpLog = Logger.getLogger(CharPump.class);

        private Reader iIn;

        private Writer iOut;

        private int bufSize;

        private MutableBoolean interrupt_sig;

        public CharPump(Reader pIn, Writer pOut, MutableBoolean pInterrupt) {
            interrupt_sig = pInterrupt;
            iIn = pIn;
            iOut = pOut;
            String bufSizeString = System.getProperty("ew.util.os.charPumpBuffer", "1024");
            try {
                bufSize = Integer.parseInt(bufSizeString);
            }
            catch (NumberFormatException e) {
                e.printStackTrace();
                bufSize = 1024;
            }
            pumpLog.debug("CharPump will use buffer size = " + bufSize);
        }

        @Override
        public void run() {
            boolean ok = true;
            char[] buf = new char[bufSize];
            synchronized (this) {
                try {
                    while (ok) {
                        ok = !interrupt_sig.getValue();
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }

                        int n = iIn.read(buf, 0, buf.length);
                        if (0 > n) {
                            pumpLog.debug("CharPump has encountered EOF");
                            break;
                        }
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }
                        iOut.write(buf, 0, n);
                        if (pumpLog.isDebugEnabled()) {
                            System.currentTimeMillis();
                        }
                        iOut.flush();
                    }
                    iOut.flush();

                }
                catch (Exception e) {
                    _log.error(e);
                    e.printStackTrace();
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
                    catch (IOException ioe) {
                        ioe.printStackTrace();
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
        StringWriter out = new StringWriter();
        StringWriter err = new StringWriter();

        CommandResult result = exec(pCmd, dir, out, err, env);
        return result;
    }

    private CommandResult exec(String[] pCmd, File dir, StringWriter pOut, StringWriter pErr, String[] env)
            throws IOException, InterruptedException {

        int out = 0;
        String pCmdString = ArrayUtils.toString(pCmd);
        if (_log.isDebugEnabled())
            _log.debug("Executing '" + pCmdString + "' with Environment '" + ArrayUtils.toString(env) + "'");
        StopWatch clock = new StopWatch();
        clock.start();
        try {
            process = Runtime.getRuntime().exec(pCmd, env, dir);
            out = handleProcess(process, pCmdString, pOut, pErr, _outputList, sig_interrupt);
        }
        finally {
            this.cleanUpProcess();
            clock.stop();
            if (_log.isInfoEnabled())
                _log.info("'" + pCmd + "' completed in " + clock.getTime() + " ms");
        }
        if (sig_interrupt.getValue() == true) {
            out = -9999;
        }
        CommandResult result = new CommandResult(pCmdString, out, pOut.toString(), pErr.toString());
        return result;
    }

    private static void logExec(String pCmdString, Map<String, String> unsortedEnv) throws IOException {
        if (_log.isInfoEnabled()) {
            TreeMap<String, String> env = new TreeMap<String, String>();
            env.putAll(unsortedEnv);

            _log.info("Executing '" + pCmdString + "'");
            _log.debug("Enviroment:");
            FileWriter writer = null;
            try {
                if (_log.isDebugEnabled()) {
                    File envvarFile = File.createTempFile("envvars", ".txt");
                    writer = new FileWriter(envvarFile);
                    _log.debug("ENVVARS will be written to " + envvarFile.getAbsolutePath());
                }
                for (String key : env.keySet()) {
                    _log.debug(String.format("  %s", new Object[] { key + "=" + env.get(key) }));
                    if (_log.isDebugEnabled() && writer != null)
                        writer.write(String.format("  %s%n", new Object[] { key + "=" + env.get(key) }));
                }
                if (_log.isDebugEnabled() && writer != null)
                    writer.close();
            }
            catch (Exception e) {
                _log.error("Unable to log exec call: " + ExceptionUtils.getStackTrace(e));
            }
            finally {
                if (writer != null) {
                    writer.close();
                }
            }
        }
    }
}