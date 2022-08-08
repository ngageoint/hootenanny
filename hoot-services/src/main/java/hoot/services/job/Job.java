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
 * @copyright Copyright (C) 2016, 2017, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.job;


import java.util.Map;
import hoot.services.command.Command;
import hoot.services.utils.PostgresUtils;


public class Job {
    private String jobId;
    private Long userId;
    private Long mapId;
    private Command[] commands;
    private JobType jobType;
    private Object tags;

    public Job(String jobId, Long userId, Command[] commands, JobType jobType, Long mapId, Map<String, Object> tags) {
        this.jobId = jobId;
        this.userId = userId;
        this.commands = commands;
        this.jobType = jobType;
        this.mapId = mapId;
        this.tags = tags;
    }

    public Job(String jobId, Long userId, Command[] commands, JobType jobType, Long mapId) {
        this(jobId, userId, commands, jobType, mapId, null);
    }

    public Job(String jobId, Long userId, Command[] commands, JobType jobType, Map<String, Object> tags) {
        this(jobId, userId, commands, jobType, null, tags);
    }

    public Job(String jobId, Long userId, Command[] commands, JobType jobType) {
        this(jobId, userId, commands, jobType, null, null);
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    public Command[] getCommands() {
        return commands;
    }

    public void setCommands(Command[] commands) {
        this.commands = commands;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public JobType getJobType() {
        return jobType;
    }

    public void setJobType(JobType jobType) {
        this.jobType = jobType;
    }

    public Long getMapId() {
        return mapId;
    }

    public void setMapId(Long mapId) {
        this.mapId = mapId;
    }

    public void setTags(Object tags) {
        this.tags = tags;
    }

    public Map<String, String> getTags() {
        return PostgresUtils.postgresObjToHStore(tags);
    }
}
