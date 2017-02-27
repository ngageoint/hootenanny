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
package hoot.services.command;


public interface ExternalCommandManager {

    /**
     * Performs external command execution.
     *
     * @param jobId id of the job this command belongs to
     * @param command command to execute
     */
    CommandResult exec(String jobId, ExternalCommand command);

    /**
     * Returns stdout of the currently executing command that belongs to the job defined by jobId.
     *
     * @param jobId ID of the job
     *
     * @return stdout of the currently executing command that runs in the context of the job
     */
    String getProgress(String jobId);

    /**
     * Terminates currently executing command for the job.  This should abort the entire job.
     *
     * @param jobId ID of the job for which to terminate its currently executing command
     */
    void terminate(String jobId);
}
