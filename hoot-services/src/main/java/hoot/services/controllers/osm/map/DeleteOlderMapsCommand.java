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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm.map;


import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.List;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.models.db.Maps;
import hoot.services.utils.DbUtils;


public class DeleteOlderMapsCommand implements InternalCommand {

    private final Timestamp timestamp;
    private final String jobId;
    private final Class<?> caller;

    DeleteOlderMapsCommand(Timestamp timestamp, String jobId, Class<?> caller) {
        this.timestamp = timestamp;
        this.jobId = jobId;
        this.caller = caller;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setCommand("[Delete Older Maps] last accessed before " + this.timestamp);
        commandResult.setStart(LocalDateTime.now());

        try {
            deleteOlderMaps(timestamp);
        }
        catch (Exception e) {
            String msg = "Error deleting maps last accessed before " + this.timestamp;
            throw new RuntimeException(msg, e);
        }

        commandResult.setExitCode(CommandResult.SUCCESS);
        commandResult.setFinish(LocalDateTime.now());
        commandResult.setJobId(jobId);
        commandResult.setCaller(this.caller.getName());

        return commandResult;
    }

    private static void deleteOlderMaps(Timestamp timestamp) {

        //Get older maps
        List<Maps> olderMaps = DbUtils.getOldMaps(timestamp);

        //Delete older maps
        olderMaps.stream().forEach(m -> {
            DbUtils.deleteBookmarksByMapId(m.getId());
            DbUtils.deleteOSMRecordById(m.getId());
        });

        //Delete empty folders
        DbUtils.deleteEmptyFolders();
    }

}
