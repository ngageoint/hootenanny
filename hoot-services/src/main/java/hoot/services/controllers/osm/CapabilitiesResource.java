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

import static hoot.services.HootProperties.*;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for OSM capabilities
 */
@Controller
@Path("/api/capabilities")
public class CapabilitiesResource {
    private static final Logger logger = LoggerFactory.getLogger(CapabilitiesResource.class);

    /**
     * The Hootenanny Capabilities Service implements the OSM Capabilities
     * Service v0.6 with some differences. The Hootenanny API does not support
     * the following capabilities properties: tracepoints.
     * 
     * GET hoot-services/osm/api/capabilities
     * 
     * @return Capability OSM XML
     */
    @GET
    @Produces(MediaType.TEXT_XML)
    public Response get() {
        Document responseDoc;

        try {
            responseDoc = writeResponse();
        }
        catch (Exception e) {
            String message = "Error retrieving capabilities. Cause: " + e.getMessage();
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        return Response.ok(new DOMSource(responseDoc)).build();
    }

    /**
     * Writes the capabilities response to an XML document
     *
     * @return an XML document
     */
    private static Document writeResponse() throws ParserConfigurationException {
        Document responseDoc = XmlDocumentBuilder.create();

        Element osmElement = OsmResponseHeaderGenerator.getOsmDataHeader(responseDoc);
        Element apiElement = responseDoc.createElement("api");

        Element versionElement = responseDoc.createElement("version");
        versionElement.setAttribute("minimum", OSM_VERSION);
        versionElement.setAttribute("maximum", OSM_VERSION);
        apiElement.appendChild(versionElement);

        Element areaElement = responseDoc.createElement("area");
        areaElement.setAttribute("maximum", MAX_QUERY_AREA_DEGREES);
        apiElement.appendChild(areaElement);

        Element wayNodesElement = responseDoc.createElement("waynodes");
        wayNodesElement.setAttribute("maximum", MAXIMUM_WAY_NODES);
        apiElement.appendChild(wayNodesElement);

        Element changesetsElement = responseDoc.createElement("changesets");
        changesetsElement.setAttribute("maximum_elements", MAXIMUM_CHANGESET_ELEMENTS);
        apiElement.appendChild(changesetsElement);

        Element timeoutElement = responseDoc.createElement("timeout");
        timeoutElement.setAttribute("seconds", String.valueOf(Integer.parseInt(CHANGESET_IDLE_TIMEOUT_MINUTES) * 60));
        apiElement.appendChild(timeoutElement);

        Element statusElement = responseDoc.createElement("status");
        statusElement.setAttribute("database", "online");
        statusElement.setAttribute("api", "online");
        statusElement.setAttribute("gpx", "offline");
        apiElement.appendChild(statusElement);

        osmElement.appendChild(apiElement);
        responseDoc.appendChild(osmElement);

        return responseDoc;
    }
}
