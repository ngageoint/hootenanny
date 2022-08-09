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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.map;

import java.time.LocalDateTime;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.models.db.Users;
import hoot.services.utils.DbUtils;


public class UpdateParentCommand implements InternalCommand {
    private final String jobId;
    private final String mapName;
    private final Long folderId;
    private final Users user;
    private final Class<?> caller;

    UpdateParentCommand( String jobId, Long folderId, String mapName, Users user, Class<?> caller) {
        this.jobId = jobId;
        this.mapName = mapName;
        this.folderId = folderId;
        this.user = user;
        this.caller = caller;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Update Parent Directory] of map with name = " + mapName);
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        updateParentDirectory();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void updateParentDirectory() {
        try {
            Long mapId = DbUtils.getMapIdByJobId(jobId);

            // These functions ensure the map + folder are either owned by the user -or- public.
            MapResource.getMapForUser(user, mapId.toString(), false, true);
            FolderResource.getFolderForUser(user, folderId);

            DbUtils.updateFolderMapping(mapId, folderId);
        }
        catch (Exception ex) {
            String msg = "Failure update parent folder: " + ex.getMessage();
            throw new RuntimeException(msg, ex);
        }
    }
}
