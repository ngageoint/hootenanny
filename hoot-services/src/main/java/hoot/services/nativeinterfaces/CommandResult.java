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
package hoot.services.nativeinterfaces;


import java.time.Duration;
import java.time.LocalDateTime;


public class CommandResult {

    public static final int SUCCESS = 0;
    public static final int FAILURE = -1;

    private String command;
    private int exitStatus;
    private String stdout;
    private String stderr;

    private LocalDateTime start;
    private LocalDateTime finish;
    private Duration duration;
    private boolean hasWarnings;

    public CommandResult() {}

    public CommandResult(String cmdString, int result, String stdout, String stderr) {
        this.command = cmdString;
        this.exitStatus = result;
        this.stdout = stdout;
        this.stderr = stderr;
    }

    public String getCommand() {
        return command;
    }

    public int getExitStatus() {
        return exitStatus;
    }

    public LocalDateTime getStart() {
        return start;
    }

    public void setStart(LocalDateTime start) {
        this.start = start;
    }

    public LocalDateTime getFinish() {
        return finish;
    }

    public void setFinish(LocalDateTime finish) {
        this.finish = finish;
    }

    public String getStderr() {
        return stderr;
    }

    public Duration getDuration() {
        if ((start != null) && (finish != null)) {
            return Duration.between(finish, start);
        }
        else {
            return null;
        }
    }

    public String getStdout() {
        return stdout;
    }

    public void setStdout(String stdout) {
        this.stdout = stdout;
    }

    public void setStderr(String stderr) {
        this.stderr = stderr;
    }

    public boolean hasWarnings() {
        return (stdout != null) && stdout.contains(" WARN ");
    }

    public boolean failed() {
        return (exitStatus != SUCCESS);
    }

    @Override
    public String toString() {
        return "CommandResult{" +
                "command='" + command + '\'' +
                ", exitStatus=" + exitStatus +
                ", stdout='" + stdout + '\'' +
                ", stderr='" + stderr + '\'' +
                ", start='" + start + '\'' +
                ", finish='" + finish + '\'' +
                '}';
    }
}
