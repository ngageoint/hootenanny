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

import java.sql.Connection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.regex.Pattern;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.w3c.dom.Document;

import com.mysema.query.Tuple;

import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.OsmResponseHeaderGenerator;


/**
 * Service endpoint for retrieving elements by ID
 */
@Path("/api/0.6")
public class ElementResource {
    private static final Logger log = LoggerFactory.getLogger(ElementResource.class);

    // <map id>_<first letter of the element type>_<element id>
    private static final String UNIQUE_ELEMENT_ID_REGEX = "\\d+_(n|w|r)_\\d+";
    private static final Pattern UNIQUE_ELEMENT_ID_PATTERN = Pattern.compile(UNIQUE_ELEMENT_ID_REGEX);

    @SuppressWarnings("unused")
    private ClassPathXmlApplicationContext appContext;

    public ElementResource() {
        appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    }

    /**
     * Returns a single element item's XML for a given map without its element
     * children
     *
     * @param mapId
     *            ID of the map the element belongs to
     * @param elementId
     *            OSM element ID of the element to retrieve
     * @param elementType
     *            OSM element type of the element to retrieve; valid values are:
     *            node, way, or relation
     * @return element XML document
     * @throws Exception
     */
    @GET
    @Path("{elementType: node|way|relation}/{elementId}")
    @Consumes({ MediaType.TEXT_PLAIN })
    @Produces({ MediaType.TEXT_XML })
    public Response getElement(@QueryParam("mapId") String mapId, @PathParam("elementId") long elementId,
            @PathParam("elementType") String elementType) throws Exception {
        ElementType elementTypeVal = Element.elementTypeFromString(elementType);
        if (elementTypeVal == null) {
            ResourceErrorHandler.handleError("Invalid element type: " + elementType, Status.BAD_REQUEST, log);
        }

        Connection conn = DbUtils.createConnection();
        Document elementDoc = null;
        try {
            log.debug("Initializing database connection...");

            elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, false, conn);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug(
                "Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
        return Response.ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
                .header("Content-type", MediaType.APPLICATION_XML).build();
    }

    /**
     * Returns a single element item's XML for a given map without its element
     * children
     *
     * @param elementId
     *            a "hoot" formatted element id of the format: <map id>_<first
     *            letter of the element type>_<element id>; valid element types
     *            include: node, way, or relation
     * @return element XML document
     * @throws Exception
     */
    @GET
    @Path("/element/{elementId}")
    @Consumes({ MediaType.TEXT_PLAIN })
    @Produces({ MediaType.TEXT_XML })
    public Response getElementByUniqueId(@PathParam("elementId") String elementId) throws Exception {
        Connection conn = DbUtils.createConnection();
        Document elementDoc = null;
        try {
            log.debug("Initializing database connection...");

            if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches()) {
                ResourceErrorHandler.handleError("Invalid element ID: " + elementId, Status.BAD_REQUEST, log);
            }

            String[] elementIdParts = elementId.split("_");
            ElementType elementTypeVal = Element.elementTypeFromString(elementIdParts[1]);
            if (elementTypeVal == null) {
                ResourceErrorHandler.handleError("Invalid element type: " + elementIdParts[1], Status.BAD_REQUEST, log);
            }

            elementDoc = getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementTypeVal, true,
                    false, conn);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug(
                "Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");

        return Response.ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
                .header("Content-type", MediaType.APPLICATION_XML).build();
    }

    /**
     * Returns a single element item's XML for a given map with all of its
     * element children
     *
     * @param mapId
     *            ID of the map the element belongs to
     * @param elementId
     *            OSM element ID of the element to retrieve
     * @param elementType
     *            OSM element type of the element to retrieve; valid values are:
     *            way or relation
     * @return element XML document
     * @throws Exception
     */
    @GET
    @Path("/{elementType: way|relation}/{elementId}/full")
    @Consumes({ MediaType.TEXT_PLAIN })
    @Produces({ MediaType.TEXT_XML })
    public Response getFullElement(@QueryParam("mapId") String mapId, @PathParam("elementId") long elementId,
            @PathParam("elementType") String elementType) throws Exception {
        ElementType elementTypeVal = Element.elementTypeFromString(elementType);
        if (elementTypeVal == null) {
            ResourceErrorHandler.handleError("Invalid element type: " + elementType, Status.BAD_REQUEST, log);
        }

        Connection conn = DbUtils.createConnection();
        Document elementDoc = null;
        try {
            log.debug("Initializing database connection...");

            elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, true, conn);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug(
                "Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
        return Response.ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
                .header("Content-type", MediaType.APPLICATION_XML).build();
    }

    /**
     * Returns a single element item's XML for a given map with all of its
     * element children
     *
     * @param elementId
     *            a "hoot" formatted element id of the format: <map id>_<first
     *            letter of the element type>_<element id>; valid element types
     *            include: way or relation
     * @return element XML document
     * @throws Exception
     */
    @GET
    @Path("/element/{elementId}/full")
    @Consumes({ MediaType.TEXT_PLAIN })
    @Produces({ MediaType.TEXT_XML })
    public Response getFullElementByUniqueId(@PathParam("elementId") String elementId) throws Exception {
        if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches()) {
            ResourceErrorHandler.handleError("Invalid element ID: " + elementId, Status.BAD_REQUEST, log);
        }

        String[] elementIdParts = elementId.split("_");
        ElementType elementType = Element.elementTypeFromString(elementIdParts[1]);

        if (elementType == null) {
            ResourceErrorHandler.handleError("Invalid element type: " + elementType, Status.BAD_REQUEST, log);
        }
        else if (elementType.equals(ElementType.Node)) {
            ResourceErrorHandler.handleError("Get Full Element Request Invalid for type = Node", Status.BAD_REQUEST,
                    log);
        }

        Connection conn = DbUtils.createConnection();
        Document elementDoc = null;
        try {
            log.debug("Initializing database connection...");

            elementDoc = getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementType, true, true,
                    conn);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug(
                "Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
        return Response.ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
                .header("Content-type", MediaType.APPLICATION_XML).build();
    }

    @SuppressWarnings("null")
    private static Document getElementXml(String mapId, long elementId, ElementType elementType,
            boolean multiLayerUniqueElementIds, boolean addChildren, Connection dbConn) throws Exception {
        long mapIdNum = -1;
        try {
            QMaps maps = QMaps.maps;
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, dbConn, maps, maps.id, maps.displayName);
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

        Set<Long> elementIds = new HashSet<>();
        elementIds.add(elementId);

        @SuppressWarnings("unchecked")
        List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType,
                elementIds, dbConn);
        if ((elementRecords == null) || (elementRecords.isEmpty())) {
            ResourceErrorHandler.handleError(
                    "Element with ID: " + elementId + " and type: " + elementType + " does not exist.",
                    Status.NOT_FOUND, log);
        }

        assert (elementRecords.size() == 1);

        Element element = ElementFactory.getInstance().create(elementType, elementRecords.get(0), dbConn,
                Long.parseLong(mapId));

        Users usersTable = elementRecords.get(0).get(QUsers.users);
        Document elementDoc = XmlDocumentBuilder.create();
        org.w3c.dom.Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(elementDoc);
        elementDoc.appendChild(elementRootXml);
        org.w3c.dom.Element elementXml = element.toXml(elementRootXml, usersTable.getId(), usersTable.getDisplayName(),
                multiLayerUniqueElementIds, addChildren);
        elementRootXml.appendChild(elementXml);

        return elementDoc;
    }

    /**
     * Returns a single element item's XML for a given map without its element
     * children
     *
     * @param mapId
     *            ID of the map the element belongs to
     * @param elementIds
     *            OSM element IDs of the elements to retrieve
     * @param elemType
     *            OSM element type of the element to retrieve; valid values are:
     *            node, way, or relation
     * @return element XML document
     * @throws Exception
     */
    @GET
    @Path("{elementType: nodes|ways|relations}")
    @Consumes({ MediaType.TEXT_PLAIN })
    @Produces({ MediaType.TEXT_XML })
    public Response getElements(@QueryParam("mapId") String mapId, @QueryParam("elementIds") String elementIds,
            @PathParam("elementType") String elemType) throws Exception {
        String elementType = elemType.substring(0, elemType.length() - 1);
        String[] elemIds = elementIds.split(",");

        ElementType elementTypeVal = Element.elementTypeFromString(elementType);
        if (elementTypeVal == null) {
            ResourceErrorHandler.handleError("Invalid element type: " + elementType, Status.BAD_REQUEST, log);
        }

        Connection conn = DbUtils.createConnection();
        Document elementDoc = null;
        try {
            log.debug("Initializing database connection...");

            elementDoc = getElementsXml(mapId, elemIds, elementTypeVal, false, true, conn);
        }
        finally {
            DbUtils.closeConnection(conn);
        }

        log.debug("Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 10000)
                + " ...");
        return Response.ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
                .header("Content-type", MediaType.APPLICATION_XML).build();
    }

    @SuppressWarnings("null")
    private static Document getElementsXml(String mapId, String[] elementIdsStr, ElementType elementType,
            boolean multiLayerUniqueElementIds, boolean addChildren, Connection dbConn) throws Exception {
        long mapIdNum = -1;
        try {
            QMaps maps = QMaps.maps;
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, dbConn, maps, maps.id, maps.displayName);
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

        Set<Long> elementIds = new HashSet<>();
        for (String elemId : elementIdsStr) {
            long elementId = Long.parseLong(elemId);
            elementIds.add(elementId);
        }

        @SuppressWarnings("unchecked")
        List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType,
                elementIds, dbConn);
        if ((elementRecords == null) || (elementRecords.isEmpty())) {
            ResourceErrorHandler.handleError("Elements with IDs: " + StringUtils.join(elementIdsStr, ",")
                    + " and type: " + elementType + " does not exist.", Status.NOT_FOUND, log);
        }
        assert (elementRecords.size() == 1);

        Document elementDoc = XmlDocumentBuilder.create();
        org.w3c.dom.Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(elementDoc);
        elementDoc.appendChild(elementRootXml);

        for (int i = 0; i < elementRecords.size(); i++) {
            Element element = ElementFactory.getInstance().create(elementType, elementRecords.get(i), dbConn,
                    Long.parseLong(mapId));
            Users usersTable = elementRecords.get(i).get(QUsers.users);

            org.w3c.dom.Element elementXml = element.toXml(elementRootXml, usersTable.getId(),
                    usersTable.getDisplayName(), multiLayerUniqueElementIds, addChildren);
            elementRootXml.appendChild(elementXml);
        }

        return elementDoc;
    }
}
