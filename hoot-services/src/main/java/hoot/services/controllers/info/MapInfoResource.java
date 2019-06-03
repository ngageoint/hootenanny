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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.info;

import static hoot.services.HootProperties.*;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.List;

import hoot.services.utils.DbUtils;


@Controller
@Path("/map")
@Transactional(readOnly = true)
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


    public MapInfoResource() {}

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
            List<Long> idList = new ArrayList<>();
            if(mapIds.isEmpty()) {
                idList = DbUtils.getAllMapIds();
            } else {
                String[] stringIdsList = mapIds.split(",");
                for (String id : stringIdsList) {
                    idList.add(Long.parseLong(id));
                }
            }

            for (Long mapId : idList) {
                long mapSize = 0;
                for (String table : maptables) {
                    if (mapId != -1) { // skips OSM API db layer
                        mapSize += DbUtils.getTableSizeInBytes(table + "_" + mapId);
                    }
                }
                JSONObject layer = new JSONObject();
                layer.put("id", mapId);
                layer.put("size", mapSize);
                layers.add(layer);
            }
        }
        catch (Exception e) {
            String message = "Error getting map size.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        JSONObject entity = new JSONObject();
        entity.put("layers", layers);

        return Response.ok(entity.toJSONString()).build();
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

        return Response.ok(entity.toJSONString()).build();
    }
}
