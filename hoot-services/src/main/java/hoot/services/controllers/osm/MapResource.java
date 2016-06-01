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

import java.io.File;
import java.io.StringWriter;
import java.io.Writer;
import java.net.SocketException;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;
import java.util.Calendar;
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
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.mysema.query.Tuple;
import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.types.expr.NumberExpression;
import com.mysema.query.types.template.NumberTemplate;

import hoot.services.db.DbUtils;
import hoot.services.db2.FolderMapMappings;
import hoot.services.db2.Folders;
import hoot.services.db2.Maps;
import hoot.services.db2.QFolderMapMappings;
import hoot.services.db2.QFolders;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobExecutioner;
import hoot.services.job.JobStatusManager;
import hoot.services.models.dataset.FolderRecords;
import hoot.services.models.dataset.LinkRecords;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayers;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.MapQueryResponseWriter;


/**
 * Service endpoint for maps containing OSM data
 */
@Path("/api/0.6/map")
public class MapResource {
    private static final Logger log = LoggerFactory.getLogger(MapResource.class);

    private static QMaps maps = QMaps.maps;

    @SuppressWarnings("unused")
    private ClassPathXmlApplicationContext appContext;

    public MapResource() {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    }

    /**
     * Returns a list of all map layers in the services database
     * 
     * GET hoot-services/osm/api/0.6/map/layers
     *
     * @return a JSON object containing a list of map layers
     * @throws Exception
     */
    @GET
    @Path("/layers")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public MapLayers getLayers() throws Exception {
        Connection conn = DbUtils.createConnection();
        MapLayers mapLayers = null;
        try {
            log.info("Retrieving map layers list...");

            log.debug("Initializing database connection...");

            QMaps maps = QMaps.maps;
            SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

            final List<Maps> mapLayerRecords = query.from(maps).orderBy(maps.displayName.asc()).list(maps);

            mapLayers = Map.mapLayerRecordsToLayers(mapLayerRecords);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }
        String message = "Returning map layers response";
        if (mapLayers != null && mapLayers.getLayers() != null) {
            message += " of size: " + mapLayers.getLayers().length;
        }
        log.debug(message);
        return mapLayers;
    }

    /**
     * <NAME>Map Service - List Folders </NAME> <DESCRIPTION> Returns a list of
     * all folders in the services database. </DESCRIPTION>
     * <PARAMETERS> </PARAMETERS> <OUTPUT> a JSON object containing a list of
     * folders </OUTPUT>
     * <EXAMPLE> <URL>http://localhost:8080/hoot-services/osm/api/0.6/map/
     * folders</URL> <REQUEST_TYPE>GET</REQUEST_TYPE> <INPUT> </INPUT>
     * <OUTPUT> { "folders": [ { "id": 1, "name": "layer 1", "parentid":0, }, {
     * "id": 2, "name": "layer 2", "parentid":1, } ] } </OUTPUT> </EXAMPLE>
     *
     * Returns a list of all folders in the services database
     *
     * @return a JSON object containing a list of folders
     * @throws Exception
     */

    @GET
    @Path("/folders")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public FolderRecords getFolders() throws Exception {
        Connection conn = DbUtils.createConnection();
        FolderRecords folderRecords = null;
        try {
            log.info("Retrieving folders list...");

            log.debug("Initializing database connection...");

            QFolders folders = QFolders.folders;
            SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

            final List<Folders> folderRecordSet = query.from(folders).orderBy(folders.displayName.asc()).list(folders);

            folderRecords = Map.mapFolderRecordsToFolders(folderRecordSet);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }
        String message = "Returning map layers response";
        if (folderRecords != null && folderRecords.getFolders() != null) {
            message += " of size: " + folderRecords.getFolders().length;
        }
        log.debug(message);
        return folderRecords;
    }

    /**
     * Returns a list of all folders in the services database
     * 
     * GET hoot-services/osm/api/0.6/map/links
     *
     * @return a JSON object containing a list of folders
     * @throws Exception
     */
    @GET
    @Path("/links")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public LinkRecords getLinks() throws Exception {
        Connection conn = DbUtils.createConnection();
        Configuration configuration = DbUtils.getConfiguration();
        LinkRecords linkRecords = null;
        try {
            log.info("Retrieving links list...");

            log.debug("Initializing database connection...");

            QFolderMapMappings folderMapMappings = QFolderMapMappings.folderMapMappings;
            QMaps maps = QMaps.maps;
            SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

            new SQLDeleteClause(conn, configuration, folderMapMappings)
                    .where(new SQLSubQuery().from(maps).where(folderMapMappings.mapId.eq(maps.id)).notExists())
                    .execute();

            try {
                SQLInsertClause insertMissing = new SQLInsertClause(conn, configuration, folderMapMappings)
                        .columns(folderMapMappings.mapId, folderMapMappings.folderId)
                        .select(new SQLSubQuery().from(maps)
                                .where(maps.id.notIn(new SQLSubQuery().distinct().from(folderMapMappings)
                                        .list(folderMapMappings.mapId)))
                                .list(maps.id, NumberTemplate.create(Long.class, "0")));

                insertMissing.execute();
            }
            catch (Exception e) {
                log.error("Could not add missing records...");
            }

            final List<FolderMapMappings> linkRecordSet = query.from(folderMapMappings)
                    .orderBy(folderMapMappings.folderId.asc()).list(folderMapMappings);

            linkRecords = Map.mapLinkRecordsToLinks(linkRecordSet);
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }
        String message = "Returning links response";
        log.debug(message);
        return linkRecords;
    }

    private static Document _generateExtentOSM(String maxlon, String maxlat, String minlon, String minlat)
            throws Exception {
        SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
        Date now = new Date();
        String strDate = sdfDate.format(now);

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

        // The ID's for these fabricated nodes were stepping on the ID's of
        // actual nodes, so their
        // ID's need to be made negative and large, so they have no chance of
        // stepping on anything.

        final long node1Id = Long.MIN_VALUE + 3;
        final long node2Id = Long.MIN_VALUE + 2;
        final long node3Id = Long.MIN_VALUE + 1;
        final long node4Id = Long.MIN_VALUE;

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
        Writer out = new StringWriter();
        tf.transform(new DOMSource(doc), new StreamResult(out));

        // System.out.println(out.toString());
        log.debug("Layer Extent OSM: " + out.toString());

        return doc;
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
     * @throws Exception
     */
    @GET
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_XML)
        public Response get(@QueryParam("mapId") final String mapId, @QueryParam("bbox") final String BBox,
            @QueryParam("extent") final String extent, @QueryParam("autoextent") final String auto,
            @DefaultValue("false") @QueryParam("multiLayerUniqueElementIds") final boolean multiLayerUniqueElementIds)
            throws Exception {
        Connection conn = DbUtils.createConnection();
        Document responseDoc = null;
        try {
            log.info("Retrieving map data for map with ID: " + mapId + " and bounds " + BBox + " ...");

            log.debug("Initializing database connection...");
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

                minX = minX > 180 ? 180 : minX;
                minX = minX < -180 ? -180 : minX;

                maxX = maxX > 180 ? 180 : maxX;
                maxX = maxX < -180 ? -180 : maxX;

                minY = minY > 90 ? 90 : minY;
                minY = minY < -90 ? -90 : minY;

                maxY = maxY > 90 ? 90 : maxY;
                maxY = maxY < -90 ? -90 : maxY;

                bbox = "" + minX + "," + minY + "," + maxX + "," + maxY;
            }

            QMaps maps = QMaps.maps;
            long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
            assert (mapIdNum != -1);

            BoundingBox queryBounds = null;
            try {
                queryBounds = new BoundingBox(bbox);
                log.debug("Query bounds area: " + queryBounds.getArea());
            }
            catch (Exception e) {
                throw new Exception(
                        "Error parsing bounding box from bbox param: " + bbox + " (" + e.getMessage() + ")");
            }

            boolean doDefault = true;
            if (auto != null && extent != null) {
                if (auto.equalsIgnoreCase("manual")) {
                    if (extent.length() > 0) {
                        String[] coords = extent.split(",");
                        if (coords.length == 4) {
                            String maxlon = coords[0].trim();
                            String maxlat = coords[1].trim();
                            String minlon = coords[2].trim();
                            String minlat = coords[3].trim();
                            responseDoc = _generateExtentOSM(maxlon, maxlat, minlon, minlat);
                            doDefault = false;
                        }
                    }

                }
            }

            if (doDefault) {
                final java.util.Map<ElementType, java.util.Map<Long, Tuple>> results = (new Map(mapIdNum, conn))
                        .query(queryBounds);

                responseDoc = (new MapQueryResponseWriter(mapIdNum, conn)).writeResponse(results, queryBounds,
                        multiLayerUniqueElementIds);
            }
        }
        catch (Exception e) {
            handleError(e, mapId, BBox);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        return Response.ok(new DOMSource(responseDoc), MediaType.TEXT_XML).header("Content-type", MediaType.TEXT_XML)
                .header("Content-Disposition", "attachment; filename=\"map.osm\"").build();
    }

    @POST
    @Path("/nodescount")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response getTileNodesCounts(String params) throws Exception {
        Connection conn = DbUtils.createConnection();
        JSONObject ret = new JSONObject();
        String mapId = "";
        String bbox = "";
        long nodeCnt = 0;
        try {
            log.info("Retrieving map data for map with ID: " + mapId + " ...");

            log.debug("Initializing database connection...");
            JSONParser parser = new JSONParser();
            JSONArray paramsArray = (JSONArray) parser.parse(params);

            for (int i = 0; i < paramsArray.size(); i++) {
                JSONObject param = (JSONObject) paramsArray.get(i);
                mapId = (String) param.get("mapId");
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

                    minX = minX > 180 ? 180 : minX;
                    minX = minX < -180 ? -180 : minX;

                    maxX = maxX > 180 ? 180 : maxX;
                    maxX = maxX < -180 ? -180 : maxX;

                    minY = minY > 90 ? 90 : minY;
                    minY = minY < -90 ? -90 : minY;

                    maxY = maxY > 90 ? 90 : maxY;
                    maxY = maxY < -90 ? -90 : maxY;

                    bbox = "" + minX + "," + minY + "," + maxX + "," + maxY;
                }

                QMaps maps = QMaps.maps;
                long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
                assert (mapIdNum != -1);

                BoundingBox queryBounds = null;
                try {
                    queryBounds = new BoundingBox(bbox);
                    log.debug("Query bounds area: " + queryBounds.getArea());
                }
                catch (Exception e) {
                    throw new Exception(
                            "Error parsing bounding box from bbox param: " + bbox + " (" + e.getMessage() + ")");
                }
                Map currMap = new Map(mapIdNum, conn);
                nodeCnt += currMap.getNodesCount(queryBounds);
            }

            ret.put("nodescount", nodeCnt);
        }
        catch (Exception e) {
            handleError(e, mapId, bbox);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        return Response.ok(ret.toString(), MediaType.APPLICATION_JSON).build();
    }

    @GET
    @Path("/mbr")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMBR(@QueryParam("mapId") final String mapId) throws Exception {
        Connection conn = DbUtils.createConnection();
        JSONObject ret = new JSONObject();
        try {
            log.info("Retrieving map data for map with ID: " + mapId + " ...");

            log.debug("Initializing database connection...");

            QMaps maps = QMaps.maps;
            long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
            assert (mapIdNum != -1);

            BoundingBox queryBounds = null;
            try {
                queryBounds = new BoundingBox("-180,-90,180,90");
                log.debug("Query bounds area: " + queryBounds.getArea());
            }
            catch (Exception e) {
                throw new Exception("Error parsing bounding box from bbox param: " + "-180,-90,180,90" + " ("
                        + e.getMessage() + ")");
            }

            Map currMap = new Map(mapIdNum, conn);
            final JSONObject extents = currMap.retrieveNodesMBR(queryBounds);

            if (extents.get("minlat") == null || extents.get("maxlat") == null || extents.get("minlon") == null
                    || extents.get("maxlon") == null) {
                throw new Exception("Map is empty.");

            }
            final JSONObject anode = currMap.retrieveANode(queryBounds);
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
        catch (Exception e) {
            handleError(e, mapId, "-180,-90,180,90");
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        return Response.ok(ret.toString(), MediaType.APPLICATION_JSON).build();
    }

    private static void handleError(final Exception e, final String mapId, final String requestSnippet) {
        if (e instanceof SocketException && e.getMessage().toLowerCase().contains("broken pipe")) {
            // This occurs when iD aborts a tile request before it is finished.
            // This
            // happens quite
            // frequently but is acceptable, so let's catch this and just log as
            // debug
            // rather than an
            // error to make the logs cleaner.
            log.debug(e.getMessage());
        }
        else if (e.getMessage().startsWith("Multiple records exist")) {
            ResourceErrorHandler.handleError(e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"),
                    Status.NOT_FOUND, log);
        }
        else if (e.getMessage().startsWith("No record exists")) {
            ResourceErrorHandler.handleError(e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"),
                    Status.NOT_FOUND, log);
        }
        else if (e.getMessage().startsWith("Map is empty")) {
            ResourceErrorHandler.handleError(e.getMessage(), Status.NOT_FOUND, log);
        }
        else if (e.getMessage().startsWith("Error parsing bounding box from bbox param")) {
            ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
        }
        else if (e.getMessage().contains("The maximum bbox size is")) {
            ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
        }
        else if (e.getMessage().contains("The maximum number of nodes that may be returned in a map query")) {
            ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
        }
        else {
            if (mapId != null) {
                ResourceErrorHandler.handleError(
                        "Error querying map with ID: " + mapId + " - data: (" + e.getMessage() + ") " + requestSnippet,
                        Status.INTERNAL_SERVER_ERROR, log);
            }
            else {
                ResourceErrorHandler.handleError(
                        "Error listing layers for map - data: (" + e.getMessage() + ") " + requestSnippet,
                        Status.INTERNAL_SERVER_ERROR, log);
            }
        }
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
     * @throws Exception
     */
    @POST
    @Path("/delete")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response deleteLayers(@QueryParam("mapId") final String mapId) throws Exception {
        JSONObject command = new JSONObject();
        command.put("mapId", mapId);
        command.put("execImpl", "ResourcesCleanUtil");

        final String jobId = UUID.randomUUID().toString();

        (new JobExecutioner(jobId, command)).start();

        JSONObject res = new JSONObject();
        res.put("jobId", jobId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
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
     * @param _modName
     *            The new name for the dataset
     * @param inputType
     *            Flag for either dataset or folder
     * @return jobId Success = True/False
     * @throws Exception
     */
    @POST
    @Path("/modify")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response modifyName(@QueryParam("mapId") final String mapId, @QueryParam("modName") final String _modName,
            @QueryParam("inputType") final String inputType) throws Exception {
        Long _mapId = Long.parseLong(mapId);
        Connection conn = DbUtils.createConnection();
        String _inputType = inputType.toLowerCase();

        try {
            log.debug("Initializing database connection...");

            if (_inputType.toLowerCase(Locale.ENGLISH).equals("dataset")) {
                QMaps maps = QMaps.maps;
                Configuration configuration = DbUtils.getConfiguration();

                new SQLUpdateClause(conn, configuration, maps).where(maps.id.eq(_mapId)).set(maps.displayName, _modName)
                        .execute();

                log.debug("Renamed map with id " + mapId + " " + _modName + "...");
            }
            else if (_inputType.toLowerCase(Locale.ENGLISH).equals("folder")) {
                QFolders folders = QFolders.folders;
                Configuration configuration = DbUtils.getConfiguration();

                new SQLUpdateClause(conn, configuration, folders).where(folders.id.eq(_mapId))
                        .set(folders.displayName, _modName).execute();

                log.debug("Renamed folder with id " + mapId + " " + _modName + "...");
            }

        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("success", true);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    /**
     * Adds new dataset folder
     * 
     * POST
     * hoot-services/osm/api/0.6/map/addfolder?folderName={foldername}&parentId=
     * {parentId}
     * 
     * @param folderName
     *            Display name of folder
     * @param parentId
     *            The parent folder of the new folder. If at root level, is
     *            equal to 0.
     * @return jobId Success = True/False
     * @throws Exception
     */
    @POST
    @Path("/addfolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response addFolder(@QueryParam("folderName") final String folderName,
            @QueryParam("parentId") final String parentId) throws Exception {
        Long _parentId = Long.parseLong(parentId);
        Long newId = (long) -1;
        NumberExpression<Long> expression = NumberTemplate.create(Long.class, "nextval('folders_id_seq')");
        Connection conn = DbUtils.createConnection();

        QFolders folders = QFolders.folders;
        Configuration configuration = DbUtils.getConfiguration();
        SQLQuery query = new SQLQuery(conn, configuration);

        long userId = 1;

        try {
            List<Long> ids = query.from().list(expression);

            if (ids != null && ids.size() > 0) {
                newId = ids.get(0);
                final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());

                new SQLInsertClause(conn, configuration, folders).columns(folders.id, folders.createdAt,
                        folders.displayName, folders.publicCol, folders.userId, folders.parentId)
                        .values(newId, now, folderName, true, userId, _parentId).execute();
            }
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("success", true);
        res.put("folderId", newId);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
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
     * @throws Exception
     */
    @POST
    @Path("/deletefolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response deleteFolder(@QueryParam("folderId") final String folderId) throws Exception {
        Long _folderId = Long.parseLong(folderId);
        Connection conn = DbUtils.createConnection();

        QFolders folders = QFolders.folders;
        QFolderMapMappings folderMapMappings = QFolderMapMappings.folderMapMappings;
        Configuration configuration = DbUtils.getConfiguration();

        try {
            SQLQuery query = new SQLQuery(conn, configuration);
            List<Long> parentId = query.from(folders).where(folders.id.eq(_folderId)).list(folders.parentId);

            Long _parentId = Long.parseLong("0");

            try {
                _parentId = parentId.get(0);
            }
            catch (Exception e) {
                _parentId = Long.parseLong("0");
            }

            new SQLUpdateClause(conn, configuration, folders).where(folders.parentId.eq(_folderId))
                    .set(folders.parentId, _parentId).execute();

            new SQLDeleteClause(conn, configuration, folders).where(folders.id.eq(_folderId)).execute();

            new SQLUpdateClause(conn, configuration, folderMapMappings).where(folderMapMappings.folderId.eq(_folderId))
                    .set(folderMapMappings.folderId, Long.parseLong("0")).execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("success", true);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
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
     * @throws Exception
     */
    @POST
    @Path("/updateParentId")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response updateParentId(@QueryParam("folderId") final String folderId,
            @QueryParam("parentId") final String parentId, @QueryParam("newRecord") final Boolean newRecord)
            throws Exception {
        Long _folderId = Long.parseLong(folderId);
        Long _parentId = Long.parseLong(parentId);
        Connection conn = DbUtils.createConnection();

        QFolders folders = QFolders.folders;
        Configuration configuration = DbUtils.getConfiguration();

        try {
            new SQLUpdateClause(conn, configuration, folders).where(folders.id.eq(_folderId))
                    .set(folders.parentId, _parentId).execute();
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("success", true);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
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
     * @throws Exception
     */
    @POST
    @Path("/linkMapFolder")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_PLAIN)
    public Response updateFolderMapLink(@QueryParam("folderId") final String folderId,
            @QueryParam("mapId") final String mapId, @QueryParam("updateType") final String updateType)
            throws Exception {
        Long _folderId = Long.parseLong(folderId);
        Long _mapId = Long.parseLong(mapId);
        Long newId = (long) -1;
        NumberExpression<Long> expression = NumberTemplate.create(Long.class, "nextval('folder_map_mappings_id_seq')");
        Connection conn = DbUtils.createConnection();

        QFolderMapMappings folderMapMappings = QFolderMapMappings.folderMapMappings;
        Configuration configuration = DbUtils.getConfiguration();
        SQLQuery query = new SQLQuery(conn, configuration);

        /*
         * long _mapId = 0;
         *
         * try { _mapId = ModelDaoUtils.getRecordIdForInputString(mapId, conn,
         * maps, maps.id, maps.displayName); } catch (Exception e){ _mapId = 0;
         * }
         */

        try {

            // Delete any existing to avoid duplicate entries
            new SQLDeleteClause(conn, configuration, folderMapMappings)
                    .where(folderMapMappings.mapId.eq(Long.parseLong(mapId))).execute();

            if (updateType.equalsIgnoreCase("new") || updateType.equalsIgnoreCase("update")) {
                List<Long> ids = query.from().list(expression);

                if (ids != null && ids.size() > 0) {
                    newId = ids.get(0);

                    new SQLInsertClause(conn, configuration, folderMapMappings)
                            .columns(folderMapMappings.id, folderMapMappings.mapId, folderMapMappings.folderId)
                            .values(newId, _mapId, _folderId).execute();
                }
            }
        }
        catch (Exception e) {
            handleError(e, null, null);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        JSONObject res = new JSONObject();
        res.put("success", true);
        return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
    }

    public String updateTagsDirect(final java.util.Map<String, String> tags, final String mapName) throws Exception {
        // _zoomLevels
        Connection conn = DbUtils.createConnection();

        String jobId = UUID.randomUUID().toString();

        JobStatusManager jobStatusManager = null;
        try {
            // Currently we do not have any way to get map id directly from hoot
            // core
            // command when it runs
            // so for now we need get the all the map ids matching name and pick
            // first
            // one..
            // THIS WILL NEED TO CHANGE when we implement handle map by Id
            // instead of
            // name..

            List<Long> mapIds = DbUtils.getMapIdsByName(conn, mapName);
            if (mapIds.size() > 0) {
                // we are expecting the last one of duplicate name to be the one
                // resulted from the conflation
                // This can be wrong if there is race condition. REMOVE THIS
                // once core
                // implement map Id return
                long mapId = mapIds.get(mapIds.size() - 1);
                jobStatusManager = new JobStatusManager(conn);
                jobStatusManager.addJob(jobId);

                // Hack alert!
                // Add special handling of stats tag key
                // We need to read the file in here, because the file doesn't
                // exist at the time
                // the updateMapsTagsCommand job is created in
                // ConflationResource.java
                String statsKey = "stats";
                if (tags.containsKey(statsKey)) {
                    String statsName = tags.get(statsKey).toString();
                    File statsFile = new File(statsName);
                    if (statsFile.exists()) {
                        log.debug("Found " + statsName);
                        String stats = FileUtils.readFileToString(statsFile, "UTF-8");
                        tags.put(statsKey, stats);
                        statsFile.delete();
                    }
                    else {
                        log.error("Can't find " + statsName);
                        tags.remove(statsKey);
                    }
                }

                DbUtils.updateMapsTableTags(tags, mapId, conn);
                jobStatusManager.setComplete(jobId);
            }

        }
        catch (SQLException sqlEx) {
            if (jobStatusManager != null) {
                jobStatusManager.setFailed(jobId, sqlEx.getMessage());
            }
            ResourceErrorHandler.handleError(
                    "Failure update map tags resource " + sqlEx.getMessage() + " SQLState: " + sqlEx.getSQLState(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        catch (Exception ex) {
            if (jobStatusManager != null) {
                jobStatusManager.setFailed(jobId, ex.getMessage());
            }
            ResourceErrorHandler.handleError("Failure update map tags resource" + ex.getMessage(),
                    Status.INTERNAL_SERVER_ERROR, log);
        }
        finally {
            DbUtils.closeConnection(conn);
        }
        return jobId;
    }

    @GET
    @Path("/tags")
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.APPLICATION_JSON)
    public Response getMapTags(@QueryParam("mapid") final String mapId) throws Exception {
        Connection conn = DbUtils.createConnection();
        JSONObject ret = new JSONObject();
        try {
            log.info("Retrieving map tags for map with ID: " + mapId + " ...");

            log.debug("Initializing database connection...");

            QMaps maps = QMaps.maps;
            long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
            assert (mapIdNum != -1);

            try {
                java.util.Map<String, String> tags = DbUtils.getMapsTableTags(mapIdNum, conn);
                ret.putAll(tags);
                Object oInput1 = ret.get("input1");
                if (oInput1 != null) {
                    String dispName = DbUtils.getDisplayNameById(conn, new Long(oInput1.toString()));
                    ret.put("input1Name", dispName);
                }

                Object oInput2 = ret.get("input2");
                if (oInput2 != null) {
                    String dispName = DbUtils.getDisplayNameById(conn, new Long(oInput2.toString()));
                    ret.put("input2Name", dispName);
                }
            }
            catch (Exception e) {
                throw new Exception("Error getting map tags. :" + e.getMessage());
            }

        }
        catch (Exception e) {
            handleError(e, mapId, "");
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        return Response.ok(ret.toString(), MediaType.APPLICATION_JSON).build();
    }

    public static long validateMap(final String mapId, Connection conn) {
        long mapIdNum = -1;
        try {
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
            assert (mapIdNum != -1);
        }
        catch (Exception e) {
            if (e.getMessage().startsWith("Multiple records exist")) {
                ResourceErrorHandler.handleError(
                        e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), Status.NOT_FOUND,
                        log);
            }
            else if (e.getMessage().startsWith("No record exists")) {
                ResourceErrorHandler.handleError(
                        e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), Status.NOT_FOUND,
                        log);
            }
            ResourceErrorHandler.handleError("Error requesting map with ID: " + mapId + " (" + e.getMessage() + ")",
                    Status.BAD_REQUEST, log);
        }
        return mapIdNum;
    }
}
