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
package hoot.services.controllers.osm;

import static hoot.services.models.db.QFolderMapMappings.folderMapMappings;
import static hoot.services.models.db.QFolders.folders;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;

import java.io.File;
import java.io.StringWriter;
import java.io.Writer;
import java.net.SocketException;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
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

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.dsl.Expressions;
import com.querydsl.sql.SQLQuery;

import hoot.services.controllers.job.JobControllerBase;
import hoot.services.controllers.job.JobId;
import hoot.services.controllers.job.JobStatusManager;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.FolderMapMappings;
import hoot.services.models.db.Folders;
import hoot.services.models.db.Maps;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayers;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.DbUtils;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for maps containing OSM data
 */
@Controller
@Path("/api/0.6/map")
@Transactional
public class MapResource extends JobControllerBase {
    private static final Logger logger = LoggerFactory.getLogger(MapResource.class);

    @Autowired
    private JobStatusManager jobStatusManager;

    public MapResource() {
        super(null);
    }

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
    public MapLayers getLayers() {
        MapLayers mapLayers = null;
        try {
            List<Maps> mapLayerRecords = createQuery().select(maps).from(maps).orderBy(maps.displayName.asc()).fetch();
            mapLayers = Map.mapLayerRecordsToLayers(mapLayerRecords);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        String message = "Returning map layers response";
        if ((mapLayers != null) && (mapLayers.getLayers() != null)) {
            message += " of size: " + mapLayers.getLayers().length;
        }

        logger.debug(message);

        return mapLayers;
    }

    /**
     * <NAME>Map Service - List Folders </NAME> <DESCRIPTION> Returns a list of
     * all folders in the services database. </DESCRIPTION> <PARAMETERS>
     * </PARAMETERS> <OUTPUT> a JSON object containing a list of folders
     * </OUTPUT> <EXAMPLE>
     * <URL>http://localhost:8080/hoot-services/osm/api/0.6/map/ folders</URL>
     * <REQUEST_TYPE>GET</REQUEST_TYPE> <INPUT> </INPUT> <OUTPUT> { "folders": [
     * { "id": 1, "name": "layer 1", "parentid":0, }, { "id": 2, "name":
     * "layer 2", "parentid":1, } ] } </OUTPUT> </EXAMPLE>
     *
     * Returns a list of all folders in the services database
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Path("/folders")
    @Produces(MediaType.APPLICATION_JSON)
    public FolderRecords getFolders() {
        FolderRecords folderRecords = null;
        try {
            List<Folders> folderRecordSet = createQuery()
                    .select(folders)
                    .from(folders)
                    .orderBy(folders.displayName.asc())
                    .fetch();

            folderRecords = mapFolderRecordsToFolders(folderRecordSet);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        String message = "Returning map layers response";
        if ((folderRecords != null) && (folderRecords.getFolders() != null)) {
            message += " of size: " + folderRecords.getFolders().length;
        }

        logger.debug(message);

        return folderRecords;
    }

    /**
     * Returns a list of all folders in the services database
     * 
     * GET hoot-services/osm/api/0.6/map/links
     *
     * @return a JSON object containing a list of folders
     */
    @GET
    @Path("/links")
    @Produces(MediaType.APPLICATION_JSON)
    public LinkRecords getLinks() {
        LinkRecords linkRecords = null;

        try {
            createQuery().delete(folderMapMappings)
                    .where(new SQLQuery<>()
                            .from(maps)
                            .where(folderMapMappings.mapId.eq(maps.id))
                            .notExists())
                    .execute();

            try {
                createQuery().insert(folderMapMappings)
                        .columns(folderMapMappings.mapId, folderMapMappings.folderId)
                        .select(new SQLQuery<>()
                                .select(maps.id, Expressions.numberTemplate(Long.class, "0"))
                                .from(maps)
                                .where(maps.id.notIn(new SQLQuery<>()
                                        .select(folderMapMappings.mapId)
                                        .distinct()
                                        .from(folderMapMappings))))
                        .execute();
            }
            catch (Exception e) {
                logger.error("Could not add missing records...", e);
            }

            List<FolderMapMappings> linkRecordSet = createQuery()
                    .select(folderMapMappings)
                    .from(folderMapMappings)
                    .orderBy(folderMapMappings.folderId.asc())
                    .fetch();

            linkRecords = mapLinkRecordsToLinks(linkRecordSet);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        return linkRecords;
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
     * GET hoot-services/osm/api/0.6/map?mapId=dc-admin&bbox
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
    @Produces(MediaType.TEXT_XML)
    public Response get(@QueryParam("mapId") String mapId,
                        @QueryParam("bbox") String BBox,
                        @QueryParam("extent") String extent,
                        @QueryParam("autoextent") String auto,
                        @DefaultValue("false") @QueryParam("multiLayerUniqueElementIds") boolean multiLayerUniqueElementIds) {
        Document responseDoc = null;
        try {
            logger.debug("Retrieving map data for map with ID: {} and bounds {} ...", mapId, BBox);

            long mapIdNum = -2;
            try {
                mapIdNum = Long.parseLong(mapId);
            }
            catch (NumberFormatException ignored) {
                //
            }
            if (mapIdNum == -1)
            {
                // OSM API database data can't be displayed on a hoot map, due to differences
                // between the display code, so we return no data here.
                responseDoc = writeEmptyResponse();
            }
            else {
                String bbox = BBox;
                String[] Coords = bbox.split(",");
                if (Coords.length == 4) {
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
                }

                mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);

                BoundingBox queryBounds;
                try {
                    queryBounds = new BoundingBox(bbox);
                    logger.debug("Query bounds area: {}", queryBounds.getArea());
                }
                catch (Exception e) {
                    throw new RuntimeException("Error parsing bounding box from bbox param: " +
                            bbox + " (" + e.getMessage() + ")", e);
                }

                boolean doDefault = true;
                if ((auto != null) && (extent != null)) {
                    if (auto.equalsIgnoreCase("manual")) {
                        if (!extent.isEmpty()) {
                            String[] coords = extent.split(",");
                            if (coords.length == 4) {
                                String maxlon = coords[0].trim();
                                String maxlat = coords[1].trim();
                                String minlon = coords[2].trim();
                                String minlat = coords[3].trim();
                                responseDoc = generateExtentOSM(maxlon, maxlat, minlon, minlat);
                                doDefault = false;
                            }
                        }

                    }
                }

                if (doDefault) {
                    java.util.Map<ElementType, java.util.Map<Long, Tuple>> results =
                            (new Map(mapIdNum)).query(queryBounds);

                    responseDoc = writeResponse(results, queryBounds, multiLayerUniqueElementIds, mapIdNum);
                }
            }
        }
        catch (Exception e) {
            handleError(e, mapId, BBox);
        }

        return Response.ok(new DOMSource(responseDoc))
                .header("Content-Disposition", "attachment; filename=\"map.osm\"").build();
    }

    @POST
    @Path("/nodescount")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTileNodesCounts(String params) {
        JSONObject ret = new JSONObject();
        String mapId = "";
        String bbox = "";
        try {
            JSONParser parser = new JSONParser();
            JSONArray paramsArray = (JSONArray) parser.parse(params);

            long nodeCnt = 0;
            for (Object aParamsArray : paramsArray) {
                JSONObject param = (JSONObject) aParamsArray;
                mapId = (String) param.get("mapId");
                long mapIdNum = -2;
                try {
                    mapIdNum = Long.parseLong(mapId);
                }
                catch (NumberFormatException ignored) {
                    //
                }
                // OSM API database data can't be displayed on a hoot map, due to differences
                // between the display code, so we return a zero count if its that layer.
                if (mapIdNum != -1) {
                    logger.debug("Retrieving node count for map with ID: {} ...", mapId);
                    bbox = (String) param.get("tile");
                    String[] coords = bbox.split(",");
                    if (coords.length == 4) {
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
                    }

                    mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);

                    BoundingBox queryBounds;
                    try {
                        queryBounds = new BoundingBox(bbox);
                        logger.debug("Query bounds area: {}", queryBounds.getArea());
                    }
                    catch (Exception e) {
                        throw new RuntimeException("Error parsing bounding box from bbox param: " + bbox + " ("
                                + e.getMessage() + ")", e);
                    }

                    Map currMap = new Map(mapIdNum);
                    nodeCnt += currMap.getNodesCount(queryBounds);
                }
            }

            ret.put("nodescount", nodeCnt);
        }
        catch (Exception e) {
            handleError(e, mapId, bbox);
        }

        return Response.ok(ret.toString()).build();
    }

    @GET
    @Path("/mbr")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMBR(@QueryParam("mapId") String mapId) {
        JSONObject ret = new JSONObject();
        try {
            logger.debug("Retrieving MBR for map with ID: {} ...", mapId);

            long mapIdNum = -2;
            try {
                mapIdNum = Long.parseLong(mapId);
            }
            catch (NumberFormatException ignored) {
                //
            }
            if (mapIdNum == -1) // OSM API db
            {
                // OSM API database data can't be displayed on a hoot map, due to differences
                // between the display code, so we arbitrarily returning roughly a CONUS bounds
                // here...not quite sure what else to return...
                ret.put("minlon", -110);
                ret.put("maxlon", -75);
                ret.put("minlat", 20);
                ret.put("maxlat", 50);
                ret.put("firstlon", 0);
                ret.put("firstlat", 0);
                ret.put("nodescount", 0);
            }
            else {
                mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);

                BoundingBox queryBounds;
                try {
                    queryBounds = new BoundingBox("-180,-90,180,90");
                    logger.debug("Query bounds area: {}", queryBounds.getArea());
                }
                catch (Exception e) {
                    throw new RuntimeException("Error parsing bounding box from bbox param: " + "-180,-90,180,90" + " ("
                            + e.getMessage() + ")", e);
                }

                Map currMap = new Map(mapIdNum);
                JSONObject extents = currMap.retrieveNodesMBR(queryBounds);

                if ((extents.get("minlat") == null) || (extents.get("maxlat") == null) || (extents.get("minlon") == null)
                        || (extents.get("maxlon") == null)) {
                    throw new Exception("Map is empty.");
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
            }
        }
        catch (Exception e) {
            handleError(e, mapId, "-180,-90,180,90");
        }

        return Response.ok(ret.toString()).build();
    }

    private static void handleError(Exception e, String mapId, String requestSnippet) {
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
                throw new WebApplicationException(e, Response.status(Status.NOT_FOUND).entity(msg).build());
            }
            else if (e.getMessage().startsWith("Map is empty")) {
                String msg = e.getMessage();
                throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(msg).build());
            }
            else if (e.getMessage().startsWith("Error parsing bounding box from bbox param") ||
                    e.getMessage().contains("The maximum bbox size is") ||
                    e.getMessage().contains("The maximum number of nodes that may be returned in a map query")) {
                String msg = e.getMessage();
                throw new WebApplicationException(e, Response.status(Status.BAD_REQUEST).entity(msg).build());
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

    /**
     * Deletes a map
     * 
     * POST hoot-services/osm/api/0.6/map/delete?mapId={Map ID}
     *
     * //TODO: should be an HTTP DELETE
     *
     * @param mapId
     *            ID of map record to be deleted
     * @return id of the deleted map
     */
    @POST
    @Path("/delete")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public JobId deleteLayers(@QueryParam("mapId") String mapId) {
        String uuid = UUID.randomUUID().toString();

        try {
            JSONObject commandParams = new JSONObject();
            commandParams.put("value", mapId);
            commandParams.put("paramtype", String.class.getName());
            commandParams.put("isprimitivetype", "false");

            JSONArray commandArgs = new JSONArray();
            commandArgs.add(commandParams);

            JSONObject command = createReflectionJobReq(commandArgs, MapResourcesCleaner.class.getName(), "exec");

            JSONArray jobArgs = new JSONArray();
            jobArgs.add(command);

            super.postChainJobRequest(uuid, jobArgs.toJSONString());
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error submitting delete map request for map with id =  " + mapId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new JobId(uuid);
    }

    /**
     * 
     * POST
     * hoot-services/osm/api/0.6/map/modify?mapId=123456&inputType='Dataset'&
     * modName='New Dataset'
     *
     * //TODO: should be an HTTP PUT
     *
     * @param mapId
     *            ID of map record or folder to be modified
     * @param modName
     *            The new name for the dataset
     * @param inputType
     *            Flag for either dataset or folder
     * @return jobId Success = True/False
     */
    @POST
    @Path("/modify")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response modifyName(@QueryParam("mapId") String mapId,
                               @QueryParam("modName") String modName,
                               @QueryParam("inputType") String inputType) {
        try {
            if (inputType.toLowerCase(Locale.ENGLISH).equals("dataset")) {
                createQuery().update(maps)
                        .where(maps.id.eq(Long.valueOf(mapId)))
                        .set(maps.displayName, modName)
                        .execute();

                logger.debug("Renamed map with id {} {}...", mapId, modName);
            }
            else if (inputType.toLowerCase(Locale.ENGLISH).equals("folder")) {
                createQuery().update(folders)
                        .where(folders.id.eq(Long.valueOf(mapId)))
                        .set(folders.displayName, modName)
                        .execute();

                logger.debug("Renamed folder with id {} {}...", mapId, modName);
            }
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        JSONObject json = new JSONObject();
        json.put("success", true);

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Adds new dataset folder
     * 
     * POST hoot-services/osm/api/0.6/map/addfolder?folderName={foldername}&
     * parentId= {parentId}
     * 
     * @param folderName
     *            Display name of folder
     * @param parentId
     *            The parent folder of the new folder. If at root level, is
     *            equal to 0.
     * @return jobId Success = True/False
     */
    @POST
    @Path("/addfolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response addFolder(@QueryParam("folderName") String folderName,
                              @QueryParam("parentId") Long parentId) {

        Long newId = -1L;
        try {
            newId = createQuery()
                    .select(Expressions.numberTemplate(Long.class, "nextval('folders_id_seq')"))
                    .from()
                    .fetchOne();

            Timestamp now = new Timestamp(System.currentTimeMillis());

            long userId = 1;
            createQuery().insert(folders)
                    .columns(folders.id, folders.createdAt, folders.displayName, folders.publicCol, folders.userId,
                            folders.parentId)
                    .values(newId, now, folderName, true, userId, parentId)
                    .execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        JSONObject json = new JSONObject();
        json.put("success", true);
        json.put("folderId", newId);

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Deletes a dataset folder.
     * 
     * POST hoot-services/osm/api/0.6/map/deletefolder?folderId={folderId}
     * 
     * //TODO: should be an HTTP DELETE
     *
     * @param folderId
     *            Folder Id
     * @return jobId
     */
    @POST
    @Path("/deletefolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response deleteFolder(@QueryParam("folderId") Long folderId) {
        try {
            List<Long> parentId = createQuery()
                    .select(folders.id)
                    .from(folders)
                    .where(folders.id.eq(folderId))
                    .fetch();

            Long prntId = !parentId.isEmpty() ? parentId.get(0) : 0L;

            createQuery().update(folders)
                    .where(folders.parentId.eq(folderId))
                    .set(folders.parentId, prntId)
                    .execute();

            createQuery().delete(folders)
                    .where(folders.id.eq(folderId))
                    .execute();

            createQuery().update(folderMapMappings)
                    .where(folderMapMappings.folderId.eq(folderId))
                    .set(folderMapMappings.folderId, 0L)
                    .execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        JSONObject json = new JSONObject();
        json.put("success", true);

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * 
     * POST hoot-services/osm/api/0.6/map/updateParentId?folderId={folderId}
     *
     * //TODO: should be an HTTP PUT
     *
     * @param folderId
     *            ID of folder
     * @param parentId
     *            ?
     * @param newRecord
     *            ?
     * @return jobId Success = True/False
     */
    @POST
    @Path("/updateParentId")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateParentId(@QueryParam("folderId") Long folderId,
                                   @QueryParam("parentId") Long parentId,
                                   @QueryParam("newRecord") Boolean newRecord) {

        try {
            createQuery().update(folders)
                    .where(folders.id.eq(folderId))
                    .set(folders.parentId, parentId)
                    .execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        JSONObject json = new JSONObject();
        json.put("success", true);

        return Response.ok(json.toJSONString()).build();
    }

    /**
     * Adds or modifies record in folder_map_mappings if layer is created or
     * modified
     * 
     * @param folderId
     *            ID of folder
     * @param mapId
     *            ID of map
     * @param updateType
     *            new: creates new link; update: updates link delete: deletes
     *            link
     * @return jobId Success = True/False
     */
    @POST
    @Path("/linkMapFolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response updateFolderMapLink(@QueryParam("folderId") Long folderId,
                                        @QueryParam("mapId") Long mapId,
                                        @QueryParam("updateType") String updateType) {
        try {
            // Delete any existing to avoid duplicate entries
            createQuery().delete(folderMapMappings).where(folderMapMappings.mapId.eq(mapId)).execute();

            if (updateType.equalsIgnoreCase("new") || updateType.equalsIgnoreCase("update")) {
                Long newId = createQuery()
                        .select(Expressions.numberTemplate(Long.class, "nextval('folder_map_mappings_id_seq')"))
                        .from()
                        .fetchOne();

                createQuery().insert(folderMapMappings)
                        .columns(folderMapMappings.id, folderMapMappings.mapId, folderMapMappings.folderId)
                        .values(newId, mapId, folderId)
                        .execute();
            }
        }
        catch (Exception e) {
            handleError(e, null, null);
        }

        JSONObject json = new JSONObject();
        json.put("success", true);

        return Response.ok(json.toJSONString()).build();
    }

    public String updateTagsDirect(java.util.Map<String, String> tags, String mapName) {
        // _zoomLevels
        String jobId = UUID.randomUUID().toString();

        try {
            // Currently we do not have any way to get map id directly from hoot
            // core command when it runs so for now we need get the all the map ids matching name and pick
            // first one..
            // THIS WILL NEED TO CHANGE when we implement handle map by Id
            // instead of name..

            Long mapId = DbUtils.getMapIdByName(mapName);
            if (mapId != null) {
                jobStatusManager.addJob(jobId);

                // Hack alert!
                // Add special handling of stats tag key
                // We need to read the file in here, because the file doesn't
                // exist at the time
                // the updateMapsTagsCommand job is created in
                // ConflationResource.java
                String statsKey = "stats";
                if (tags.containsKey(statsKey)) {
                    String statsName = tags.get(statsKey);
                    File statsFile = new File(statsName);
                    if (statsFile.exists()) {
                        logger.debug("Found {}", statsName);
                        String stats = FileUtils.readFileToString(statsFile, "UTF-8");
                        tags.put(statsKey, stats);

                        if (!statsFile.delete()) {
                            logger.error("Error deleting {} file", statsFile.getAbsolutePath());
                        }
                    }
                    else {
                        logger.error("Can't find {}", statsName);
                        tags.remove(statsKey);
                    }
                }

                DbUtils.updateMapsTableTags(tags, mapId);
                jobStatusManager.setComplete(jobId);
            }
        }
        catch (Exception ex) {
            if (jobStatusManager != null) {
                jobStatusManager.setFailed(jobId, ex.getMessage());
            }

            String msg = "Failure update map tags resource" + ex.getMessage();
            throw new WebApplicationException(ex, Response.serverError().entity(msg).build());
        }

        return jobId;
    }

    @GET
    @Path("/tags")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTags(@QueryParam("mapid") String mapId) {
        JSONObject json = new JSONObject();
        try {
            if (!StringUtils.isEmpty(mapId)) {
                long mapIdNum = -2;
                try {
                    mapIdNum = Long.parseLong(mapId);
                }
                catch (NumberFormatException ignored) {
                    // a map name string could also be passed in here
                }

                if (mapIdNum != -1) // not OSM API db
                {
                    mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);
                    logger.debug("Retrieving map tags for map with ID: {} ...", mapIdNum);

                    try {
                        java.util.Map<String, String> tags = DbUtils.getMapsTableTags(mapIdNum);
                        if (tags != null) {
                            logger.debug(tags.toString());
                        }
                        json.putAll(tags);
                        Object oInput1 = json.get("input1");
                        if (oInput1 != null) {
                            String dispName = DbUtils.getDisplayNameById(Long.valueOf(oInput1.toString()));
                            json.put("input1Name", dispName);
                        }

                        Object oInput2 = json.get("input2");
                        if (oInput2 != null) {
                            String dispName = DbUtils.getDisplayNameById(Long.valueOf(oInput2.toString()));
                            json.put("input2Name", dispName);
                        }
                    }
                    catch (Exception e) {
                        throw new RuntimeException("Error getting map tags. :" + e.getMessage(), e);
                    }
                }
            }
        }
        catch (Exception e) {
            handleError(e, mapId, "");
        }

        return Response.ok(json.toString()).build();
    }

    public static long validateMap(String mapId) {
        long mapIdNum;

        try {
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, maps, maps.id, maps.displayName);
        }
        catch (Exception ex) {
            if (ex.getMessage().startsWith("Multiple records exist") || ex.getMessage().startsWith("No record exists")) {
                String msg = ex.getMessage().replaceAll("records", "maps").replaceAll("record", "map");
                throw new WebApplicationException(ex, Response.status(Status.NOT_FOUND).entity(msg).build());
            }
            else {
                String msg = "Error requesting map with ID: " + mapId + " (" + ex.getMessage() + ")";
                throw new WebApplicationException(ex, Response.status(Status.BAD_REQUEST).entity(msg).build());
            }
        }

        return mapIdNum;
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

                            Users usersTable = record.get(QUsers.users);
                            Element elementXml = element.toXml(elementRootXml, usersTable.getId(),
                                    usersTable.getDisplayName(), multiLayerUniqueElementIds, true);
                            elementRootXml.appendChild(elementXml);
                        }
                    }
                }
            }
        }

        return responseDoc;
    }

    /**
     * Converts a set of folder database records into an object returnable by a
     * web service
     *
     * @param folderRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    private static FolderRecords mapFolderRecordsToFolders(List<Folders> folderRecordSet) {
        FolderRecords folderRecords = new FolderRecords();
        List<FolderRecord> folderRecordList = new ArrayList<>();

        for (Folders folderRecord : folderRecordSet) {
            FolderRecord folder = new FolderRecord();
            folder.setId(folderRecord.getId());
            folder.setName(folderRecord.getDisplayName());
            folder.setParentId(folderRecord.getParentId());
            folderRecordList.add(folder);
        }

        folderRecords.setFolders(folderRecordList.toArray(new FolderRecord[folderRecordList.size()]));

        return folderRecords;
    }

    /**
     * Converts a set of database records into an object returnable by a web
     * service
     *
     * @param linkRecordSet
     *            set of map layer records
     * @return folders web service object
     */
    private static LinkRecords mapLinkRecordsToLinks(List<FolderMapMappings> linkRecordSet) {
        LinkRecords linkRecords = new LinkRecords();
        List<LinkRecord> linkRecordList = new ArrayList<>();

        for (FolderMapMappings linkRecord : linkRecordSet) {
            LinkRecord link = new LinkRecord();
            link.setId(linkRecord.getId());
            link.setFolderId(linkRecord.getFolderId());
            link.setMapId(linkRecord.getMapId());
            linkRecordList.add(link);
        }

        linkRecords.setLinks(linkRecordList.toArray(new LinkRecord[linkRecordList.size()]));

        return linkRecords;
    }
}
