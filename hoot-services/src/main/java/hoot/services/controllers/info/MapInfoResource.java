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
package hoot.services.controllers.info;

import static hoot.services.HootProperties.*;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.codehaus.jettison.json.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.utils.DbUtils;


@Path("/map")
public class MapInfoResource {
    private static final Logger logger = LoggerFactory.getLogger(MapInfoResource.class);

    private static final String[] maptables = {
            "changesets",
            "current_nodes",
            "current_relation_members",
            "current_relations",
            "current_way_nodes",
            "current_ways"
    };


    public MapInfoResource() {
    }

    /**
     * Service method endpoint for retrieving the physical size of a map record.
     * 
     * GET hoot-services/info/map/size?mapid=1
     * 
     * @param mapIds
     *            id of the map for which to retrieve size
     * @return JSON containing size information
     */
    @GET
    @Path("/size")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getCombinedMapSize(@QueryParam("mapid") String mapIds) {
        long combinedMapSize = 0;

        try {
            String[] mapids = mapIds.split(",");
            for (String mapId : mapids) {
                if (Long.parseLong(mapId) != -1) { // skips OSM API db layer
                    for (String table : maptables) {
                        combinedMapSize += getTableSizeInBytes(table + "_" + mapId);
                    }
                }
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String message = "Error getting combined map size for: " + mapIds;
            throw new WebApplicationException(ex, Response.serverError().entity(message).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("mapid", mapIds);
        entity.put("size_byte", combinedMapSize);

        return Response.ok(entity.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Service method endpoint for retrieving the physical size of multiple map records.
     * 
     * GET hoot-services/info/map/sizes?mapid=54,62
     * 
     * @param mapIds
     *            ids of the maps for which to retrieve sizes
     * @return JSON containing a list of size information for all current maps
     */
    @GET
    @Path("/sizes")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMapSizes(@QueryParam("mapid") String mapIds) {
        JSONArray layers = new JSONArray();

        try {
            String[] mapids = mapIds.split(",");
            for (String mapId : mapids) {
                long mapSize = 0;
                for (String table : maptables) {
                    if (Long.parseLong(mapId) != -1) { // skips OSM API db layer
                        mapSize += getTableSizeInBytes(table + "_" + mapId);
                    }
                }
                JSONObject layer = new JSONObject();
                layer.put("id", Long.parseLong(mapId));
                layer.put("size", mapSize);
                layers.put(layer);
            }
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception ex) {
            String message = "Error getting map size: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(message).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("layers", layers);

        return Response.ok(entity.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Service endpoint for maximum data size for export conflate and ingest.
     * 
     * GET hoot-services/info/map/thresholds
     * 
     * @return JSON containing threshold information
     */
    @GET
    @Path("/thresholds")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getThresholds() {
        JSONObject entity = new JSONObject();
        entity.put("conflate_threshold", CONFLATE_SIZE_THRESHOLD);
        entity.put("ingest_threshold", INGEST_SIZE_THRESHOLD);
        entity.put("export_threshold", EXPORT_SIZE_THRESHOLD);

        return Response.ok(entity.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Returns table size in byte
     */
    private static long getTableSizeInBytes(String tableName) {
        long tableSize = 0;

        try (Connection conn = DbUtils.createConnection()) {
            try (Statement stmt = conn.createStatement()) {
                String sql = "select pg_total_relation_size('" + tableName + "') as tablesize";
                try (ResultSet rs = stmt.executeQuery(sql)){
                    while (rs.next()) {
                        tableSize = rs.getLong("tablesize");
                    }
                }
            }
        }
        catch (SQLException e) {
            String msg = "Error retrieving table size in bytes of " + tableName + " table!";
            throw new RuntimeException(msg, e);
        }

        return tableSize;
    }
}
