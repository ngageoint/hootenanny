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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;

import java.net.SocketException;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.querydsl.core.types.dsl.Expressions;

import hoot.services.command.CommandResult;
import hoot.services.command.InternalCommand;
import hoot.services.models.db.Maps;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayer;
import hoot.services.models.osm.MapLayers;
import hoot.services.utils.DbUtils;


/**
 * Updates the database by adding the specified jobs folder and mapping the inputs to the folder
 */
class UpdateDbCommand implements InternalCommand {
    private static final Logger logger = LoggerFactory.getLogger(UpdateDbCommand.class);

    private final GrailParams params;
    private final String jobId;
    private final Class<?> caller;

    UpdateDbCommand(GrailParams params, String jobId, Class<?> caller) {
        this.params = params;
        this.jobId = jobId;
        this.caller = caller;
    }

    @Override
    public CommandResult execute() {
        CommandResult commandResult = new CommandResult();
        commandResult.setJobId(jobId);
        commandResult.setCommand("[Update DB] for " + params.getOutput());
        commandResult.setStart(LocalDateTime.now());
        commandResult.setCaller(caller.getName());

        updateDb();

        commandResult.setFinish(LocalDateTime.now());
        commandResult.setExitCode(CommandResult.SUCCESS);

        return commandResult;
    }

    private void updateDb() {
        logger.info("Params: " + params);

        // NOTE: This is just functions taken from MapResource.java with some tweaking
        // Make a new folder
        Long folderId = -1L;
        Long parentId = 0L;  // 0 == The root folder

        try {
            folderId = createQuery()
                    .select(Expressions.numberTemplate(Long.class, "nextval('folders_id_seq')"))
                    .from()
                    .fetchOne();

            Timestamp now = new Timestamp(System.currentTimeMillis());

            long userId = (params.getUser() != null) ? params.getUser().getId() : 1;
          //TODO: replace with call to create folder
            createQuery().insert(folders)
                    .columns(folders.id, folders.createdAt, folders.displayName, folders.publicCol, folders.userId,
                            folders.parentId)
                    .values(folderId, now, params.getParentId(), false, userId, parentId)
                    .execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        // Find the mapId's for the input file(s)
        MapLayers mapLayers = null;
        try {
            List<Maps> mapLayerRecords = createQuery().select(maps).from(maps).orderBy(maps.displayName.asc()).fetch();
            mapLayers = Map.mapLayerRecordsToLayers(mapLayerRecords);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        Long apiMapId = findMapId(mapLayers, params.getInput1());
        if (apiMapId == -1) {
            throw new IllegalArgumentException("Cannot find a mapId for " + params.getInput1());
        }

        updateFolder(apiMapId, folderId);

        Long overpassMapId = findMapId(mapLayers, params.getInput2());
        if (overpassMapId == -1) {
            throw new IllegalArgumentException("Cannot find a mapId for " + params.getInput2());
        }
        updateFolder(overpassMapId, folderId);
    }

    private Long findMapId(MapLayers mapLayers, String layerName) {
        Long mapId = -1L;

        for (MapLayer tLayer : mapLayers.getLayers()) {
            if (layerName.equals(tLayer.getName())) {
                mapId = tLayer.getId();
                break;
            }
        }

        return mapId;
    }

    private void updateFolder(Long mapId, Long folderId) {
        try {
            DbUtils.updateFolderMapping(mapId, folderId);
        }
        catch (Exception e) {
            handleError(e, mapId, null);
        }
    }

    // Taken directly from MapResource.java
    private static void handleError(Exception e, Long mapId, String requestSnippet) {
        if ((e instanceof SocketException) && e.getMessage().toLowerCase().contains("broken pipe")) {
            // This occurs when iD aborts a tile request before it is finished.
            // This happens quite frequently but is acceptable, so let's catch this and just logger as
            // debug rather than an error to make the logs cleaner.
            logger.debug(e.getMessage());
        }
        else if (!StringUtils.isEmpty(e.getMessage())) {
            if (e.getMessage().startsWith("Multiple records exist") ||
                    e.getMessage().startsWith("No record exists")) {
                String msg = e.getMessage().replaceAll("records", "maps").replaceAll("record", "map");
                throw new WebApplicationException(e, Response.status(Response.Status.NOT_FOUND).entity(msg).build());
            }
            else if (e.getMessage().startsWith("Map is empty")) {
                String msg = e.getMessage();
                throw new WebApplicationException(Response.status(Response.Status.NOT_FOUND).entity(msg).build());
            }
            else if (e.getMessage().startsWith("Error parsing bounding box from bbox param") ||
                    e.getMessage().contains("The maximum bbox size is") ||
                    e.getMessage().contains("The maximum number of nodes that may be returned in a map query")) {
                String msg = e.getMessage();
                throw new WebApplicationException(e, Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
            }
        }
        else {
            if (mapId != null) {
                String msg = "Error querying map with ID: " + mapId + " - data: " + requestSnippet;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
            else {
                String msg = "Error listing layers for map - data: " + requestSnippet;
                throw new WebApplicationException(e, Response.serverError().entity(msg).build());
            }
        }

        String msg = e.getMessage();
        throw new WebApplicationException(e, Response.serverError().entity(msg).build());
    }
}
