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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.command;


import java.io.File;
import java.time.Duration;
import java.time.LocalDateTime;


public class CommandResult {

    public static final int SUCCESS = 0;
    public static final int FAILURE = -1;

    private Long id;
    private String command;
    private Integer exitCode;
    private String stdout;
    private String stderr;
    private LocalDateTime start;
    private LocalDateTime finish;
    private String jobId;
    private String caller;
    private File workingDir;
    private Integer percentProgress;

    public CommandResult() {
        percentProgress = 0;
    }

    public String getCommand() {
        return command;
    }

    public Integer getExitCode() {
        return exitCode;
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
        return ((start != null) && (finish != null)) ? Duration.between(finish, start) : null;
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

    public boolean failed() {
        return (exitCode == FAILURE);
    }

    public void setCommand(String command) {
        this.command = command;
    }

    public void setExitCode(Integer exitCode) {
        this.exitCode = exitCode;
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getCaller() {
        return caller;
    }

    public void setCaller(String caller) {
        this.caller = caller;
    }

    public File getWorkingDir() {
        return workingDir;
    }

    public void setWorkingDir(File workingDir) {
        this.workingDir = workingDir;
    }

    public Integer getPercentProgress() {
        return percentProgress;
    }

    public void setPercentProgress(Integer progress) {
        this.percentProgress = progress;
    }

    @Override
    public String toString() {
        return "CommandResult{" +
                "command=[" + command + "]" +
                ", jobId=[" + jobId + ']' +
                ", command_id=[" + id + ']' +
                ", caller=[" + caller + ']' +
                ", workingDir=[" + workingDir.getAbsolutePath() + ']' +
                ", start=[" + start + ']' +
                ", finish=[" + finish + ']' +
                ", duration=[" + getDuration() + "]" +
                ", exitCode=[" + exitCode + ']' +
                ", stdout=[" + stdout + ']' +
                ", stderr=[" + stderr + ']' +
                '}';
    }
}
