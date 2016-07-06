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

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.codehaus.jettison.json.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;


@Path("/map")
public class MapInfoResource {
    private static final Logger logger = LoggerFactory.getLogger(MapInfoResource.class);
    private static final Long CONFLATE_THRESHOLD;
    private static final Long INGEST_THRESHOLD;
    private static final Long EXPORT_THRESHOLD;

    static {
        try {
            String value = HootProperties.getProperty("conflateSizeThreshold");
            CONFLATE_THRESHOLD = Long.parseLong(value);
        }
        catch (NumberFormatException nfe) {
            logger.error("Invalid value specified for conflateSizeThreshold!");
            throw nfe;
        }

        try {
            String value = HootProperties.getProperty("ingestSizeThreshold");
            INGEST_THRESHOLD = Long.parseLong(value);
        }
        catch (NumberFormatException nfe) {
            logger.error("Invalid value specified for ingestSizeThreshold!");
            throw nfe;
        }

        try {
            String value = HootProperties.getProperty("exportSizeThreshold");
            EXPORT_THRESHOLD = Long.parseLong(value);
        }
        catch (NumberFormatException nfe) {
            logger.error("Invalid value specified for exportSizeThreshold!");
            throw nfe;
        }
    }

    public MapInfoResource() {
    }

    /**
     * Service method endpoint for retrieving the physical size of a map record.
     * 
     * GET hoot-services/info/map/size?mapid=1
     * 
     * @param mapIds
     *            ids of the maps for which to retrieve sizes
     * @return JSON containing size information
     */
    @GET
    @Path("/size")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getMapSize(@QueryParam("mapid") String mapIds) {
        long nsize = 0;
        String[] maptables = {
                "changesets",
                "current_nodes",
                "current_relation_members",
                "current_relations",
                "current_way_nodes",
                "current_ways"
        };

        try {
            String[] mapids = mapIds.split(",");
            for (String mapId : mapids) {
                if (Long.parseLong(mapId) != -1) { // skips OSM API db layer
                    for (String table : maptables) {
                        nsize += DbUtils.getTableSizeInByte(table + "_" + mapId);
                    }
                }
            }
        }
        catch (Exception ex) {
            String message = "Error getting map size: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(message).build());
        }

        JSONObject res = new JSONObject();
        res.put("mapid", mapIds);
        res.put("size_byte", nsize);

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Service method endpoint for retrieving the physical size of multiple map
     * records.
     * 
     * GET hoot-services/info/map/sizes?mapid=54,62
     * 
     * @param mapIds
     *            ids of the maps for which to retrieve sizes
     * @return JSON containing a list of size information for all current maps
     */
    @GET
    @Path("/sizes")
    @Produces(MediaType.TEXT_PLAIN)
    public Response getMapSizes(@QueryParam("mapid") String mapIds) {
        String[] maptables = {
                "changesets",
                "current_nodes",
                "current_relation_members",
                "current_relations",
                "current_way_nodes",
                "current_ways"
        };

        JSONArray retval = new JSONArray();

        try {
            String[] mapids = mapIds.split(",");
            for (String mapId : mapids) {
                JSONObject jo = new JSONObject();
                long nsize = 0;
                try {
                    for (String table : maptables) {
                        if (Long.parseLong(mapId) != -1) { // skips OSM API db layer
                            nsize += DbUtils.getTableSizeInByte(table + "_" + mapId);
                        }
                    }
                }
                finally {
                    jo.put("id", Long.parseLong(mapId));
                    jo.put("size", nsize);
                    retval.put(jo);
                }
            }
        }
        catch (Exception ex) {
            String message = "Error getting map size: " + ex.getMessage();
            throw new WebApplicationException(ex, Response.status(Status.INTERNAL_SERVER_ERROR).entity(message).build());
        }

        JSONObject res = new JSONObject();
        res.put("layers", retval);

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
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
    @Produces(MediaType.TEXT_PLAIN)
    public Response getThresholds() {
        JSONObject res = new JSONObject();
        res.put("conflate_threshold", CONFLATE_THRESHOLD);
        res.put("ingest_threshold", INGEST_THRESHOLD);
        res.put("export_threshold", EXPORT_THRESHOLD);

        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }
}
