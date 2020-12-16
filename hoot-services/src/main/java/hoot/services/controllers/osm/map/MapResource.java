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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm.map;

import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.StringWriter;
import java.io.Writer;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.BadRequestException;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.GET;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.querydsl.core.Tuple;

import hoot.services.command.Command;
import hoot.services.command.InternalCommand;
import hoot.services.controllers.osm.OsmResponseHeaderGenerator;
import hoot.services.controllers.osm.user.UserResource;
import hoot.services.geo.BoundingBox;
import hoot.services.job.Job;
import hoot.services.job.JobProcessor;
import hoot.services.job.JobType;
import hoot.services.models.db.Maps;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayer;
import hoot.services.models.osm.MapLayers;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for maps containing OSM data
 */
@Controller
@Path("/api/0.6/map")
@Transactional
public class MapResource {
    private static final Logger logger = LoggerFactory.getLogger(MapResource.class);

    private static final String[] maptables = {
            "changesets",
            "current_nodes",
            "current_relation_members",
            "current_relations",
            "current_way_nodes",
            "current_ways"
    };

    @Autowired
    private JobProcessor jobProcessor;

    @Autowired
    private DeleteMapResourcesCommandFactory deleteMapResourcesCommandFactory;

    @Autowired
    private DeleteOlderMapsCommandFactory deleteOlderMapsCommandFactory;


    /**
     * Returns a list of all map layers in the services database
     *
     * GET hoot-services/osm/api/0.6/map/layers
     *
     * @return a JSON object containing a list of map layers
     */
    @GET
    @Path("/layers")
    @Produces(MediaType.APPLICATION_JSON)
    public MapLayers getLayers(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);

        List<Tuple> mapLayerRecords = DbUtils.getMapsForUser(user);

        // The query above is only a rough filter, we need to make sure
        // that the folder is recursively visible to the user based on folder
        // visibility:
        List<Maps> mapLayersOut = new ArrayList<Maps>(mapLayerRecords.size());
        Set<Long> foldersTheUserCanSee = DbUtils.getFolderIdsForUser(user);
        for(Tuple t : mapLayerRecords) {
            Long parentFolder = t.get(folders.id);
            Boolean parentFolderIsPublic = t.get(folders.publicCol);
            // [!] If data set in root folder (0L), publicCol will be null
            // fall back to public.
            if(parentFolderIsPublic == null) {
                parentFolderIsPublic = Boolean.valueOf(true);
            }
            if(parentFolder == null || parentFolder.equals(0L) || foldersTheUserCanSee.contains(parentFolder)) {
                Maps m = t.get(maps);
                m.setSize(getMapSize(m.getId()));
                m.setPublicCol(parentFolderIsPublic);
                m.setFolderId(parentFolder);
                mapLayersOut.add(m);
            }
        }
        return Map.mapLayerRecordsToLayers(mapLayersOut);
    }

    /**
     * For retrieving the physical size of a map record
     * @param mapId
     * @return physical size of a map record
     */
    private Long getMapSize(long mapId) {
        long mapSize = 0;

        try {
            for (String table : maptables) {
                if (mapId != -1) { // skips OSM API db layer
                    mapSize += DbUtils.getTableSizeInBytes(table + "_" + mapId);
                }
            }

            return mapSize;
        }
        catch (Exception e) {
            String message = "Error getting map size.  Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }
    }

    private static Document generateExtentOSM(String maxlon, String maxlat, String minlon, String minlat) {
        SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
        Date now = new Date();
        String strDate = sdfDate.format(now);

        try {
            DocumentBuilderFactory dbf = XmlDocumentBuilder.getSecureDocBuilderFactory();
            dbf.setValidating(false);
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.newDocument();

            Element osmElem = doc.createElement("osm");
            osmElem.setAttribute("version", "0.6");
            osmElem.setAttribute("generator", "hootenanny");
            doc.appendChild(osmElem);

            Element boundsElem = doc.createElement("bounds");
            boundsElem.setAttribute("minlat", minlat);
            boundsElem.setAttribute("minlon", minlon);
            boundsElem.setAttribute("maxlat", maxlat);
            boundsElem.setAttribute("maxlon", maxlon);
            osmElem.appendChild(boundsElem);

            // The ID's for these fabricated nodes were stepping on the ID's of actual nodes, so their ID's need to be
            // made negative and large, so they have no chance of stepping on anything.

            long node1Id = Long.MIN_VALUE + 3;
            long node2Id = Long.MIN_VALUE + 2;
            long node3Id = Long.MIN_VALUE + 1;
            long node4Id = Long.MIN_VALUE;

            Element nodeElem = doc.createElement("node");
            nodeElem.setAttribute("id", String.valueOf(node1Id));
            nodeElem.setAttribute("timestamp", strDate);
            nodeElem.setAttribute("user", "hootenannyuser");
            nodeElem.setAttribute("visible", "true");
            nodeElem.setAttribute("version", "1");
            nodeElem.setAttribute("lat", maxlat);
            nodeElem.setAttribute("lon", minlon);
            osmElem.appendChild(nodeElem);

            nodeElem = doc.createElement("node");
            nodeElem.setAttribute("id", String.valueOf(node2Id));
            nodeElem.setAttribute("timestamp", strDate);
            nodeElem.setAttribute("user", "hootenannyuser");
            nodeElem.setAttribute("visible", "true");
            nodeElem.setAttribute("version", "1");
            nodeElem.setAttribute("lat", maxlat);
            nodeElem.setAttribute("lon", maxlon);
            osmElem.appendChild(nodeElem);

            nodeElem = doc.createElement("node");
            nodeElem.setAttribute("id", String.valueOf(node3Id));
            nodeElem.setAttribute("timestamp", strDate);
            nodeElem.setAttribute("user", "hootenannyuser");
            nodeElem.setAttribute("visible", "true");
            nodeElem.setAttribute("version", "1");
            nodeElem.setAttribute("lat", minlat);
            nodeElem.setAttribute("lon", maxlon);
            osmElem.appendChild(nodeElem);

            nodeElem = doc.createElement("node");
            nodeElem.setAttribute("id", String.valueOf(node4Id));
            nodeElem.setAttribute("timestamp", strDate);
            nodeElem.setAttribute("user", "hootenannyuser");
            nodeElem.setAttribute("visible", "true");
            nodeElem.setAttribute("version", "1");
            nodeElem.setAttribute("lat", minlat);
            nodeElem.setAttribute("lon", minlon);
            osmElem.appendChild(nodeElem);

            Element wayElem = doc.createElement("way");
            wayElem.setAttribute("id", String.valueOf(Long.MIN_VALUE));
            wayElem.setAttribute("timestamp", strDate);
            wayElem.setAttribute("user", "hootenannyuser");
            wayElem.setAttribute("visible", "true");
            wayElem.setAttribute("version", "1");

            Element ndElem = doc.createElement("nd");
            ndElem.setAttribute("ref", String.valueOf(node1Id));
            wayElem.appendChild(ndElem);

            ndElem = doc.createElement("nd");
            ndElem.setAttribute("ref", String.valueOf(node2Id));
            wayElem.appendChild(ndElem);

            ndElem = doc.createElement("nd");
            ndElem.setAttribute("ref", String.valueOf(node3Id));
            wayElem.appendChild(ndElem);

            ndElem = doc.createElement("nd");
            ndElem.setAttribute("ref", String.valueOf(node4Id));
            wayElem.appendChild(ndElem);

            ndElem = doc.createElement("nd");
            ndElem.setAttribute("ref", String.valueOf(node1Id));
            wayElem.appendChild(ndElem);

        /*
         * ndElem = doc.createElement("tag"); ndElem.setAttribute("k", "area");
         * ndElem.setAttribute("v", "yes"); wayElem.appendChild(ndElem);
         */

            osmElem.appendChild(wayElem);

            Transformer tf = TransformerFactory.newInstance().newTransformer();

            // Fortify may require this, but it doesn't work.
            // TransformerFactory transformerFactory =
            // XmlDocumentBuilder.getSecureTransformerFactory();
            tf.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
            tf.setOutputProperty(OutputKeys.INDENT, "yes");

            try (Writer out = new StringWriter()) {
                tf.transform(new DOMSource(doc), new StreamResult(out));
                logger.debug("Layer Extent OSM: {}", out);
            }

            return doc;
        }
        catch (Exception e) {
            throw new RuntimeException("Error generating OSM extent", e);
        }
    }

    /**
     * GET hoot-services/osm/api/0.6/map/{mapId}/{bbox}
     *
     * @param mapId
     *            ID of the map to query
     * @param BBox
     *            geographic bounding box the requested entities should reside
     *            in
     * @param multiLayerUniqueElementIds
     *            if true, returned element IDs are prepended with <map
     *            id>_<first letter of the element type>_; this setting
     *            activated is not compatible with standard OSM clients
     *            (specific to Hootenanny iD); defaults to false
     * @return response containing the data of the requested elements
     */
    @GET
    @Path("/{mapId}/{BBox}")
    @Produces(MediaType.TEXT_XML)
    public Response get(@Context HttpServletRequest request,
            @PathParam("mapId") String mapId,
            @PathParam("BBox") String BBox,
            @QueryParam("manualExtent") String manualExtent,
            @DefaultValue("false") @QueryParam("multiLayerUniqueElementIds") boolean multiLayerUniqueElementIds) {

        Document responseDoc = null;
        logger.debug("Retrieving map data for map with ID: {} and bounds {} ...", mapId, BBox);

        if (mapId.equals("-1")) {
            // OSM API database data can't be displayed on a hoot map, due
            // to differences
            // between the display code, so we return no data here.
            responseDoc = writeEmptyResponse();
            return Response.ok(new DOMSource(responseDoc))
                    .header("Content-Disposition", "attachment; filename=\"map.osm\"").build();

        }
        Map currMap = getMapForRequest(request, mapId, true, false);
        String bbox = BBox;
        String[] Coords = bbox.split(",");
        if (Coords.length != 4) {
            return Response.status(Status.BAD_REQUEST).entity("").build();
        }

        String sMinX = Coords[0];
        String sMinY = Coords[1];
        String sMaxX = Coords[2];
        String sMaxY = Coords[3];

        double minX = Double.parseDouble(sMinX);
        double minY = Double.parseDouble(sMinY);
        double maxX = Double.parseDouble(sMaxX);
        double maxY = Double.parseDouble(sMaxY);

        minX = (minX > 180) ? 180 : minX;
        minX = (minX < -180) ? -180 : minX;

        maxX = (maxX > 180) ? 180 : maxX;
        maxX = (maxX < -180) ? -180 : maxX;

        minY = (minY > 90) ? 90 : minY;
        minY = (minY < -90) ? -90 : minY;

        maxY = (maxY > 90) ? 90 : maxY;
        maxY = (maxY < -90) ? -90 : maxY;

        bbox = minX + "," + minY + "," + maxX + "," + maxY;

        BoundingBox queryBounds = new BoundingBox(bbox);
        logger.debug("Calculating query bounds area for bounds: {}", queryBounds);
        logger.debug("Query bounds area: {}", queryBounds.getArea());


        if (manualExtent != null && !manualExtent.isEmpty()) {
            String[] coords = manualExtent.split(",");
            if (coords.length != 4) {
                return Response.status(Status.BAD_REQUEST).entity("").build();
            }
            String maxlon = coords[0].trim();
            String maxlat = coords[1].trim();
            String minlon = coords[2].trim();
            String minlat = coords[3].trim();

            responseDoc = generateExtentOSM(maxlon, maxlat, minlon, minlat);

        } else {
            java.util.Map<ElementType, java.util.Map<Long, Tuple>> results = null;
            try {
                results = currMap.query(queryBounds);
            }
            catch (/* tables did not exist */DataAccessException e) {
                return Response.status(Status.NOT_FOUND).type(MediaType.TEXT_PLAIN).entity("No map with that id exists")
                        .build();
            }
            responseDoc = writeResponse(results, queryBounds, multiLayerUniqueElementIds, currMap.getId());
        }


        return Response.ok(new DOMSource(responseDoc)).header("Content-Disposition", "attachment; filename=\"map.osm\"")
                .build();
    }

    @POST
    @Path("/nodescount")
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTileNodesCounts(@Context HttpServletRequest request, String params) {
        // Forward declarations
        Users user = Users.fromRequest(request);
        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        String mapId = "";
        String bbox = "";

        JSONParser parser = new JSONParser();
        JSONArray paramsArray = null;
        try {
            paramsArray = (JSONArray) parser.parse(params);
        }
        catch (ParseException e) {
            return Response.status(Status.BAD_REQUEST).build();
        }

        long nodeCnt = 0;
        for (Object aParamsArray : paramsArray) {
            JSONObject param = (JSONObject) aParamsArray;
            mapId = param.get("mapId").toString();
            bbox = (String) param.get("tile");

            // OSM API database data can't be displayed on a hoot map, due to
            // differences between the display code, so we return a zero count
            // if its that layer.
            if (mapId.isEmpty() || mapId.equals("-1")) {
                continue;
            }

            logger.debug("Retrieving node count for map with ID: {} ...", mapId);

            String[] coords = bbox.split(",");
            if (coords.length != 4) {
                return Response.status(Status.BAD_REQUEST).build();
            }
            String sMinX = coords[0];
            String sMinY = coords[1];
            String sMaxX = coords[2];
            String sMaxY = coords[3];

            double minX = Double.parseDouble(sMinX);
            double minY = Double.parseDouble(sMinY);
            double maxX = Double.parseDouble(sMaxX);
            double maxY = Double.parseDouble(sMaxY);

            minX = (minX > 180) ? 180 : minX;
            minX = (minX < -180) ? -180 : minX;

            maxX = (maxX > 180) ? 180 : maxX;
            maxX = (maxX < -180) ? -180 : maxX;

            minY = (minY > 90) ? 90 : minY;
            minY = (minY < -90) ? -90 : minY;

            maxY = (maxY > 90) ? 90 : maxY;
            maxY = (maxY < -90) ? -90 : maxY;

            bbox = minX + "," + minY + "," + maxX + "," + maxY;

            // Get map numerical identifier from database:
            long mapIdNum = DbUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);

            BoundingBox queryBounds;
            try {
                queryBounds = new BoundingBox(bbox);
                logger.debug("Query bounds area: {}", queryBounds.getArea());
            }
            catch (Exception e) {
                // If we can't parse bounds, quit now.
                return Response.status(Status.BAD_REQUEST).build();
            }
            Map m = new Map(mapIdNum);

            if(user != null && !m.isVisibleTo(user)) {
                continue;
            }

            nodeCnt += m.getNodesCount(queryBounds);
        } // for

        ret.put("nodescount", nodeCnt);
        return Response.ok(new JSONObject(ret).toString()).build();
    }

    @GET
    @Path("/{mapId}/mbr")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMBR(@Context HttpServletRequest request, @PathParam("mapId") String mapId) {
        Map currMap = getMapForRequest(request, mapId, true, false);

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        if (mapId.equals("-1")) {
            // OSM API database data can't be displayed on a hoot map, due to
            // differences
            // between the display code, so we arbitrarily returning roughly a
            // CONUS bounds
            // here...not quite sure what else to return...
            ret.put("minlon", -110);
            ret.put("maxlon", -75);
            ret.put("minlat", 20);
            ret.put("maxlat", 50);
            ret.put("firstlon", 0);
            ret.put("firstlat", 0);
            ret.put("nodescount", 0);

            return Response.status(Status.OK).entity(ret).build();
        }

        logger.debug("Retrieving MBR for map with ID: {} ...", mapId);


        BoundingBox queryBounds = new BoundingBox("-180,-90,180,90");
        logger.debug("Query bounds area: {}", queryBounds.getArea());

        JSONObject extents = currMap.retrieveNodesMBR(queryBounds);

        if ((extents.get("minlat") == null) || (extents.get("maxlat") == null) || (extents.get("minlon") == null)
                || (extents.get("maxlon") == null)) {

            // check for bbox tag in maps
            String boundsTag = DbUtils.getMapBounds(Long.parseLong(mapId));
            if(boundsTag != null) {
                //This is how UI sends a bbox `${minx},${miny},${maxx},${maxy}`
                String[] bboxCoords = boundsTag.split(",");
                ret.put("minlon", Double.parseDouble(bboxCoords[0]));
                ret.put("maxlon", Double.parseDouble(bboxCoords[2]));
                ret.put("minlat", Double.parseDouble(bboxCoords[1]));
                ret.put("maxlat", Double.parseDouble(bboxCoords[3]));
            } else {
                ret.put("minlon", -180);
                ret.put("maxlon", 180);
                ret.put("minlat", -90);
                ret.put("maxlat", 90);
            }

            ret.put("firstlon", 0);
            ret.put("firstlat", 0);
            ret.put("nodescount", 0);

            return Response.status(Status.OK).entity(ret).build();
        }

        JSONObject anode = currMap.retrieveANode(queryBounds);
        long nodeCnt = currMap.getNodesCount(queryBounds);

        double dMinLon = (Double) extents.get("minlon");
        double dMaxLon = (Double) extents.get("maxlon");
        double dMinLat = (Double) extents.get("minlat");
        double dMaxLat = (Double) extents.get("maxlat");

        double dFirstLon = (Double) anode.get("lon");
        double dFirstLat = (Double) anode.get("lat");

        ret.put("minlon", dMinLon);
        ret.put("maxlon", dMaxLon);
        ret.put("minlat", dMinLat);
        ret.put("maxlat", dMaxLat);
        ret.put("firstlon", dFirstLon);
        ret.put("firstlat", dFirstLat);
        ret.put("nodescount", nodeCnt);


        return Response.ok().entity(ret).build();
    }

    /**
     * Deletes a map
     *
     * DELETE hoot-services/osm/api/0.6/map/delete/{id}
     *
     *
     * @param mapId
     *            ID of map record to be deleted
     * @return id of the deleted map
     */
    @DELETE
    @Path("/{mapId}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteLayers(@Context HttpServletRequest request, @PathParam("mapId") String mapId) {
        Users user = Users.fromRequest(request);

        // handles some ACL logic for us...
        getMapForRequest(request, mapId, false, true);

        String jobId = UUID.randomUUID().toString();
        try {
            Command[] workflow = {
                () -> {
                    InternalCommand mapResourcesCleaner = deleteMapResourcesCommandFactory.build(mapId, jobId, this.getClass());
                    return mapResourcesCleaner.execute();
                }
            };

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow, JobType.DELETE, DbUtils.getMapIdFromRef(mapId, user.getId())));
        }
        catch (Exception e) {
            String msg = "Error submitting delete map request for map with id =  " + mapId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("jobid", jobId);

        return Response.ok().entity(ret).build();
    }

    /**
     * Deletes maps older than ?
     *
     * DELETE hoot-services/osm/api/0.6/map/delete/stale/{months}
     *
     *
     * @param months
     *            number of months back
     * @return job id
     */
    @DELETE
    @Path("/stale/{months}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteStaleLayers(@Context HttpServletRequest request, @PathParam("months") Integer months) {
        Users user = Users.fromRequest(request);

        if (!UserResource.adminUserCheck(user)) {
            return Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to delete old map data").build();
        }

        String jobId = UUID.randomUUID().toString();

        Calendar cal = Calendar.getInstance();
        cal.add(Calendar.MONTH, -1 * months);
        Timestamp monthsAgo = new Timestamp(cal.getTime().getTime());

        try {
            Command[] workflow = {
                () -> {
                    InternalCommand mapResourcesCleaner = deleteOlderMapsCommandFactory.build(monthsAgo, jobId, this.getClass());
                    return mapResourcesCleaner.execute();
                }
            };

            jobProcessor.submitAsync(new Job(jobId, user.getId(), workflow, JobType.DELETE));
        }
        catch (Exception e) {
            String msg = "Error submitting delete map request for maps older than " + months;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("jobid", jobId);

        return Response.ok().entity(ret).build();
    }

    /**
     * Deletes maps older than ?
     *
     * GET hoot-services/osm/api/0.6/map/delete/{id}
     *
     *
     * @param months
     *            number of months back
     * @return job id
     */
    @GET
    @Path("/stale/{months}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getStaleLayers(@Context HttpServletRequest request, @PathParam("months") Integer months) {
        Users user = Users.fromRequest(request);

        if (!UserResource.adminUserCheck(user)) {
            return Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to delete old map data").build();
        }

        Calendar cal = Calendar.getInstance();
        cal.add(Calendar.MONTH, -1 * months);
        Timestamp monthsAgo = new Timestamp(cal.getTime().getTime());

        return Response.ok().entity(DbUtils.getStaleMapsSummary(monthsAgo)).build();
    }

    /**
     *
     * PUT hoot-services/osm/api/0.6/map/123456/rename/new_name'
     *
     * @param mapId
     *            ID of map record or folder to be modified
     * @param modName
     *            The new name for the dataset
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/{mapId}/rename/{name}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response modifyName(@Context HttpServletRequest request, @PathParam("mapId") String mapId, @PathParam("name") String modName) {
        Map m = getMapForRequest(request, mapId, false, true);
        createQuery().update(maps).where(maps.id.eq(m.getId())).set(maps.displayName, modName).execute();

        logger.debug("Renamed map with id {} {}...", mapId, modName);
        updateLastAccessed(m.getId());

        return Response.ok().build();
    }

    /**
     * Adds or modifies record in folder_map_mappings if layer is created or
     * modified
     *
     * @param folderId
     *            ID of folder
     * @param mapId
     *            ID of map
     *
     * @return jobId Success = True/False
     */
    @PUT
    @Path("/{mapId}/move/{folderId : \\d+}")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateFolderMapLink(@Context HttpServletRequest request, @PathParam("folderId") Long folderId, @PathParam("mapId") String mapId) {

        Users user = Users.fromRequest(request);

        // These functions ensure the map + folder are
        // either owned by the user -or- public.
        // getMapForUser(Users user, String mapId, boolean allowOSM, boolean userDesiresModify)
        Map m = getMapForUser(user, mapId, false, true);
        // Handle some CRUD
        FolderResource.getFolderForUser(user, folderId);
        DbUtils.updateFolderMapping(m.getId(), folderId);

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        ret.put("success", true);
        return Response.ok().entity(ret).build();
    }

    @GET
    @Path("/{mapId}/tags")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTags(@Context HttpServletRequest request, @PathParam("mapId") String mapId) {
        Map m = getMapForRequest(request, mapId, true, false);

        java.util.Map<String, Object> ret = new HashMap<String, Object>();
        java.util.Map<String, String> tags = updateLastAccessed(m.getId());
        logger.debug(String.format("getTags(): retrieved tags for map with id: '%s': '%s'", mapId, tags));

        ret.putAll(tags);

        if (tags.containsKey("params")) {
            JSONParser jp = new JSONParser();
            String unescaped = tags.get("params").replace("\\\"", "\"").replace("\\\\", "").replace("\"{", "{").replace("}\"", "}");
            try {
                ret.put("params", jp.parse(unescaped));
            } catch (ParseException ex) {
                logger.error(unescaped, ex);
            }
        }

        Object oInput1 = ret.get("input1");
        if (oInput1 != null) {
            String dispName = DbUtils.getDisplayNameById(Long.valueOf(oInput1.toString()));
            ret.put("input1Name", dispName);
        }

        Object oInput2 = ret.get("input2");
        if (oInput2 != null) {
            String dispName = DbUtils.getDisplayNameById(Long.valueOf(oInput2.toString()));
            ret.put("input2Name", dispName);
        }

        return Response.ok().entity(ret).build();
    }

    @GET
    @Path("/{mapId}/startingIndex")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAllIds(@Context HttpServletRequest request, @PathParam("mapId") String mapId) {
        Map m = getMapForRequest(request, mapId, true, false);

        HashMap <String, Long> getIdsMap = m.getIdIndex();

        return Response.ok().entity(getIdsMap).build();
    }

    /**
     * Writes a map query response with no element data
     *
     * @return a XML document response
     */
    private static Document writeEmptyResponse() {
        Document responseDoc;
        try {
            responseDoc = XmlDocumentBuilder.create();
        }
        catch (ParserConfigurationException e) {
            throw new RuntimeException("Error creating XmlDocumentBuilder!", e);
        }

        Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(responseDoc);
        responseDoc.appendChild(elementRootXml);

        return responseDoc;
    }

    /**
     * Writes the query response to an XML document
     *
     * @param results
     *            query results; a mapping of element IDs to records, grouped by
     *            element type
     * @param queryBounds
     *            bounds of the query
     * @param multiLayerUniqueElementIds
     *            if true, IDs are prepended with <map id>_<first letter of the
     *            element type>_; this setting activated is not compatible with
     *            standard OSM clients (specific to Hootenanny iD)
     * @return an XML document
     */
    private static Document writeResponse(java.util.Map<ElementType, java.util.Map<Long, Tuple>> results,
            BoundingBox queryBounds, boolean multiLayerUniqueElementIds, long mapId) {
        Document responseDoc;
        try {
            responseDoc = XmlDocumentBuilder.create();
        }
        catch (ParserConfigurationException e) {
            throw new RuntimeException("Error creating XmlDocumentBuilder!", e);
        }

        Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(responseDoc);
        elementRootXml.setAttribute("mapid", String.valueOf(mapId));
        responseDoc.appendChild(elementRootXml);

        if (!results.isEmpty()) {
            elementRootXml.appendChild(queryBounds.toXml(elementRootXml));

            for (ElementType elementType : ElementType.values()) {
                if (elementType != ElementType.Changeset) {
                    java.util.Map<Long, Tuple> resultsForType = results.get(elementType);
                    if (resultsForType != null) {
                        for (java.util.Map.Entry<Long, Tuple> entry : resultsForType.entrySet()) {
                            Tuple record = entry.getValue();

                            hoot.services.models.osm.Element element = ElementFactory.create(elementType, record, mapId);

                            // the query that sent this in should have
                            // already handled filtering out invisible elements

                            boolean addChildren = elementType == ElementType.Way;

                            Users usersTable = record.get(QUsers.users);
                            Element elementXml = element.toXml(elementRootXml, usersTable.getId(),
                                    usersTable.getDisplayName(), multiLayerUniqueElementIds, addChildren);
                            elementRootXml.appendChild(elementXml);
                        }
                    }
                }
            }
        }

        return responseDoc;
    }

    /**
     * Updates the lastAccessed key in the maps
     * tags hstore to the current timestamp
     *
     * @param mapid
     *            id of the maps record
     */
    public static java.util.Map<String,String> updateLastAccessed(Long mapid) {
        java.util.Map<String, String> tags = DbUtils.getMapsTableTags(mapid);

        DateFormat dateFormat = MapLayer.format;
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        tags.put("lastAccessed", dateFormat.format(now));

        long rowsAffected = DbUtils.updateMapsTableTags(tags, mapid);
        assert(rowsAffected > 0); // weird state, should never happen.
        return tags;
    }
    public static Map getMapForUser(Users user, String mapId, boolean allowOSM, boolean userDesiresModify) throws WebApplicationException {
        if(!allowOSM && mapId.equals("-1")) {
            throw new BadRequestException();
        }
        long mapIdNum = -2;
        try {
            mapIdNum = Long.parseLong(mapId);
        }
        catch (NumberFormatException ignored) {
            mapIdNum = DbUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);
        }
        if(mapIdNum < 0) {
            throw new NotFoundException("No map with that id exists");
        }
        Map m = new Map(mapIdNum);
        if(user != null && !m.isVisibleTo(user)) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to this map").build());
        }
        if(user != null && userDesiresModify && !m.getUserId().equals(user.getId()) && !UserResource.adminUserCheck(user)) {
            throw new ForbiddenException(Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You must own the map to modify it").build());
        }
        return m;
    }
    public static Map getMapForRequest(HttpServletRequest request, String mapId, boolean allowOSM, boolean userDesiresModify) throws WebApplicationException {
        Users user = null;
        if(request != null) {
            user = (Users) request.getAttribute(hoot.services.HootUserRequestFilter.HOOT_USER_ATTRIBUTE);
        }

        return getMapForUser(user, mapId, allowOSM, userDesiresModify);
    }
}
