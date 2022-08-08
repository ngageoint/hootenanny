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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.map;


import static hoot.services.models.db.QMaps.maps;
import static hoot.services.models.db.QReviewBookmarks.reviewBookmarks;
import static hoot.services.utils.DbUtils.createQuery;
import static hoot.services.utils.DbUtils.deleteMapRelatedTablesByMapId;

import java.time.LocalDateTime;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.utils.DbUtils;


public class DeleteMapResourcesCommand implements InternalCommand {

    private final String mapName;
    private final String jobId;
    private final Class<?> caller;

    DeleteMapResourcesCommand(String mapName, String jobId, Class<?> caller) {
        this.mapName = mapName;
        this.jobId = jobId;
        this.caller = caller;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setCommand("[Delete Map Resources] of map with ID = " + this.mapName);
        commandResult.setStart(LocalDateTime.now());

        try {
            deleteLayerBy(mapName);
        }
        catch (Exception e) {
            String msg = "Error deleting layer where mapName = " +  mapName;
            throw new RuntimeException(msg, e);
        }

        commandResult.setExitCode(CommandResult.SUCCESS);
        commandResult.setFinish(LocalDateTime.now());
        commandResult.setJobId(jobId);
        commandResult.setCaller(this.caller.getName());

        return commandResult;
    }

    private static void deleteLayerBy(String mapName) {
        Long mapIdNum = -2L;
        try {
            mapIdNum = Long.parseLong(mapName);
        }
        catch (NumberFormatException ignored) {
            mapIdNum = DbUtils.getRecordIdForInputString(mapName, maps, maps.id, maps.displayName);
        }
        if(mapIdNum == null || mapIdNum < 0) {
            throw new IllegalArgumentException(mapName + " doesn't have a corresponding map ID associated with it!");
        }

        deleteBookmarksBy(mapIdNum);
        deleteOSMRecordByName(mapIdNum);
    }

    private static void deleteOSMRecordByName(Long mapId) {
        deleteMapRelatedTablesByMapId(mapId);
        DbUtils.deleteMap(mapId);
    }

    private static void deleteBookmarksBy(Long mapId) {
        createQuery().delete(reviewBookmarks).where(reviewBookmarks.mapId.eq(mapId)).execute();
    }
}
