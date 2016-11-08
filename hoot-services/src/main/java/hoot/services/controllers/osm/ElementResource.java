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


import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.regex.Pattern;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;

import com.querydsl.core.Tuple;

import hoot.services.models.db.QMaps;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for retrieving elements by ID
 */
@Controller
@Path("/api/0.6")
@Transactional
public class ElementResource {
    private static final Logger logger = LoggerFactory.getLogger(ElementResource.class);

    // <map id>_<first letter of the element type>_<element id>
    private static final String UNIQUE_ELEMENT_ID_REGEX = "\\d+_(n|w|r)_\\d+";
    private static final Pattern UNIQUE_ELEMENT_ID_PATTERN = Pattern.compile(UNIQUE_ELEMENT_ID_REGEX);


    public ElementResource() {}

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
     */
    @GET
    @Path("{elementType: node|way|relation}/{elementId}")
    @Produces(MediaType.TEXT_XML)
    public Response getElement(@QueryParam("mapId") String mapId,
                               @PathParam("elementId") long elementId,
                               @PathParam("elementType") String elementType) {
        ElementType elementTypeVal = Element.elementTypeFromString(elementType);

        if (elementTypeVal == null) {
            String msg = "Invalid element type: " + elementType;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Document elementDoc;
        try {
            elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, false);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting element elementId = " +
                    elementId + ", elementType = " + elementType + ", mapId = " + mapId;
            throw new WebApplicationException(e, Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        return Response.ok(new DOMSource(elementDoc)).build();
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
     */
    @GET
    @Path("/element/{elementId}")
    @Produces(MediaType.TEXT_XML)
    public Response getElementByUniqueId(@PathParam("elementId") String elementId) {
        if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches()) {
            String msg = "Invalid element ID: " + elementId;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        String[] elementIdParts = elementId.split("_");
        ElementType elementTypeVal = Element.elementTypeFromString(elementIdParts[1]);

        if (elementTypeVal == null) {
            String msg = "Invalid element type: " + elementIdParts[1];
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Document elementDoc;
        try {
            elementDoc = getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementTypeVal, true, false);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting element by unique id.  elementId = " + elementId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(new DOMSource(elementDoc)).build();
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
     */
    @GET
    @Path("/{elementType: way|relation}/{elementId}/full")
    @Produces(MediaType.TEXT_XML)
    public Response getFullElement(@QueryParam("mapId") String mapId,
                                   @PathParam("elementId") long elementId,
                                   @PathParam("elementType") String elementType) {
        ElementType elementTypeVal = Element.elementTypeFromString(elementType);

        if (elementTypeVal == null) {
            String msg = "Invalid element type: " + elementType;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Document elementDoc = null;
        try {
            elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, true);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting full element data!" +
                    "  mapId = " + mapId + ", elementId = " + elementId + ", elementType = " + elementType;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(new DOMSource(elementDoc)).build();
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
     */
    @GET
    @Path("/element/{elementId}/full")
    @Produces(MediaType.TEXT_XML)
    public Response getFullElementByUniqueId(@PathParam("elementId") String elementId) {
        if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches()) {
            String msg = "Invalid element ID: " + elementId;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        String[] elementIdParts = elementId.split("_");
        ElementType elementType = Element.elementTypeFromString(elementIdParts[1]);

        if (elementType == null) {
            String msg = "Invalid element type: null";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        if (elementType == ElementType.Node) {
            String msg = "Get Full Element Request Invalid for type = Node";
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Document elementDoc = null;
        try {
            elementDoc = getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementType, true, true);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting full element data by unique id! elementId = " + elementId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(new DOMSource(elementDoc)).build();
    }

    private static Document getElementXml(String mapId, long elementId, ElementType elementType,
            boolean multiLayerUniqueElementIds, boolean addChildren) {
        long mapIdNum;
        try {
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, QMaps.maps, QMaps.maps.id, QMaps.maps.displayName);
        }
        catch (Exception ex) {
            if (ex.getMessage().startsWith("Multiple records exist")
                    || ex.getMessage().startsWith("No record exists")) {
                String msg = ex.getMessage().replaceAll("records", "maps").replaceAll("record", "map");
                throw new WebApplicationException(ex, Response.status(Status.NOT_FOUND).entity(msg).build());
            }

            String msg = "Error requesting map with ID: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Set<Long> elementIds = new HashSet<>();
        elementIds.add(elementId);

        List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType,
                elementIds);

        if ((elementRecords == null) || (elementRecords.isEmpty())) {
            String msg = "Element with ID: " + elementId + " and type: " + elementType + " does not exist.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(msg).build());
        }

        Element element = ElementFactory.create(elementType, elementRecords.get(0), Long.parseLong(mapId));

        Users usersTable = elementRecords.get(0).get(QUsers.users);
        Document elementDoc = null;
        try {
            elementDoc = XmlDocumentBuilder.create();
        }
        catch (ParserConfigurationException e) {
            String msg = "Error building XMLDocumentBuilder while processing " +
                    "elementId = " + elementId + ", mapId = " + mapId;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

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
     */
    @GET
    @Path("{elementType: nodes|ways|relations}")
    @Produces(MediaType.TEXT_XML)
    public Response getElements(@QueryParam("mapId") String mapId,
                                @QueryParam("elementIds") String elementIds,
                                @PathParam("elementType") String elemType) {
        String elementType = elemType.substring(0, elemType.length() - 1);
        String[] elemIds = elementIds.split(",");

        ElementType elementTypeVal = Element.elementTypeFromString(elementType);
        if (elementTypeVal == null) {
            String msg = "Invalid element type: " + elementType;
            throw new WebApplicationException(Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Document elementDoc;
        try {
            elementDoc = getElementsXml(mapId, elemIds, elementTypeVal, false, true);
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String msg = "Error getting elements!" + "  mapId = " + mapId +
                    ", elementIds = " + elementIds + ", elementType = " + elemType;
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return Response.ok(new DOMSource(elementDoc)).build();
    }

    private static Document getElementsXml(String mapId, String[] elementIdsStr, ElementType elementType,
            boolean multiLayerUniqueElementIds, boolean addChildren) {
        long mapIdNum;
        try {
            // input mapId may be a map ID or a map name
            mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, QMaps.maps, QMaps.maps.id, QMaps.maps.displayName);
        }
        catch (Exception ex) {
            if (ex.getMessage().startsWith("Multiple records exist") ||
                    ex.getMessage().startsWith("No record exists")) {
                String msg = ex.getMessage().replaceAll("records", "maps").replaceAll("record", "map");
                throw new WebApplicationException(ex, Response.status(Status.NOT_FOUND).entity(msg).build());
            }

            String msg = "Error requesting map with ID: " + mapId + " (" + ex.getMessage() + ")";
            throw new WebApplicationException(ex, Response.status(Status.BAD_REQUEST).entity(msg).build());
        }

        Set<Long> elementIds = new HashSet<>();
        for (String elemId : elementIdsStr) {
            long elementId = Long.parseLong(elemId);
            elementIds.add(elementId);
        }

        List<Tuple> elementRecords = (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType,
                elementIds);
        if ((elementRecords == null) || (elementRecords.isEmpty())) {
            String msg = "Elements with IDs: " + StringUtils.join(elementIdsStr, ",")
                    + " and type: " + elementType + " does not exist.";
            throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(msg).build());
        }

        Document elementDoc;
        try {
            elementDoc = XmlDocumentBuilder.create();
        }
        catch (ParserConfigurationException e) {
            throw new RuntimeException("Error creating XmlDocumentBuilder", e);
        }

        org.w3c.dom.Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(elementDoc);
        elementDoc.appendChild(elementRootXml);

        for (Tuple elementRecord : elementRecords) {
            Element element = ElementFactory.create(elementType, elementRecord, Long.parseLong(mapId));
            Users usersTable = elementRecord.get(QUsers.users);
            org.w3c.dom.Element elementXml = element.toXml(elementRootXml, usersTable.getId(),
                    usersTable.getDisplayName(), multiLayerUniqueElementIds, addChildren);
            elementRootXml.appendChild(elementXml);
        }

        return elementDoc;
    }
}
