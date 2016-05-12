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

import java.io.IOException;
import java.io.Writer;
import java.util.Map;


/**
 *
 */
public interface ICommandRunner {

    /**
     * Runs a process using Runtime.exec(). The command to run is passed as a
     * String. Collects all output on System.out and System.err, passing it to
     * the CommandResult. Waits for all output and process completion then
     * returns the process exit status.
     */
    public abstract CommandResult exec(String cmd) throws IOException, InterruptedException;

    /**
     * Runs a process using Runtime.exec() with the given environment. System
     * environment inclusion can be controlled via the useSysEnv argument. The
     * command to run is passed as a String. Collects all output on System.out
     * and System.err, passing it to the CommandResult. Waits for all output and
     * process completion then returns the process exit status.
     */
    public abstract CommandResult exec(String cmd, Map<String, String> env, boolean includeSysEnv)
            throws IOException, InterruptedException;

    /**
     * Runs a process using Runtime.exec(). The command to run is passed as a
     * String[]. Collects all output on System.out and System.err, passing it to
     * the CommandResult. Waits for all output and process completion then
     * returns the process exit status.
     */

    public abstract CommandResult exec(String[] cmd) throws IOException, InterruptedException;

    /**
     * Runs a process using Runtime.exec(). The command to run is passed as a
     * String[]. Collects all output on System.out and System.err, passing it to
     * the CommandResult. Waits for all output and process completion then
     * returns the process exit status.
     */

    public abstract CommandResult exec(String[] cmd, Map<String, String> env, boolean includeSysEnv)
            throws IOException, InterruptedException;

    /**
     * Runs a process using Runtime.exec(). The command to run is passed as a
     * String array. Collects all output on System.out and System.err, passing
     * it to the two Writer parameters. Waits for all output and process
     * completion, writing to the two Writers via the system default character
     * encoding, then returns the process exit status.
     * <p>
     * Note that if one of the Writers blocks, this may block the child process.
     */
    public abstract CommandResult exec(String[] pCmd, Writer pOut, Writer pErr)
            throws IOException, InterruptedException;

    /**
     * Runs a process using Runtime.exec(). The command to run is passed as a
     * String. Collects all output on System.out and System.err, passing it to
     * the two Writer parameters. Waits for all output and process completion,
     * writing to the two Writers via the system default character encoding,
     * then returns the process exit status.
     * <p>
     * Note that if one of the Writers blocks, this may block the child process.
     */
    public abstract CommandResult exec(String pCmd, Writer pOut, Writer pErr) throws IOException, InterruptedException;

    /**
     * Terminates the command process
     *
     * @throws Exception
     */
    public abstract void terminateClean() throws Exception;

    public abstract String getStdOut();

    public abstract int getProcessState();

}