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

import java.io.IOException;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.HootProperties;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.OsmResponseHeaderGenerator;


/**
 * Service endpoint for OSM capabilities
 */
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
    @Consumes(MediaType.TEXT_PLAIN)
    @Produces(MediaType.TEXT_XML)
    public Response get() {
        Document responseDoc;

        try {
            logger.info("Retrieving capabilities...");

            responseDoc = writeResponse();
        }
        catch (Exception e) {
            String message = "Error retrieving capabilities: " + e.getMessage();
            throw new WebApplicationException(e, Response.status(Status.INTERNAL_SERVER_ERROR).entity(message).build());
        }

        try {
            logger.debug("Returning response: {} ...", StringUtils.abbreviate(XmlDocumentBuilder.toString(responseDoc), 100));
        }
        catch (IOException ignored) {
        }

        return Response.ok(new DOMSource(responseDoc), MediaType.TEXT_XML)
                .header("Content-type", MediaType.TEXT_XML).build();
    }

    /**
     * Writes the capabilities response to an XML document
     *
     * @return an XML document
     */
    private static Document writeResponse() throws ParserConfigurationException {
        logger.debug("Building response...");

        Document responseDoc = XmlDocumentBuilder.create();

        Element osmElement = OsmResponseHeaderGenerator.getOsmDataHeader(responseDoc);
        Element apiElement = responseDoc.createElement("api");

        Element versionElement = responseDoc.createElement("version");
        versionElement.setAttribute("minimum", HootProperties.getPropertyOrDefault("osmVersion"));
        versionElement.setAttribute("maximum", HootProperties.getPropertyOrDefault("osmVersion"));
        apiElement.appendChild(versionElement);

        Element areaElement = responseDoc.createElement("area");
        areaElement.setAttribute("maximum", HootProperties.getPropertyOrDefault("maxQueryAreaDegrees"));
        apiElement.appendChild(areaElement);

        Element wayNodesElement = responseDoc.createElement("waynodes");
        wayNodesElement.setAttribute("maximum", HootProperties.getPropertyOrDefault("maximumWayNodes"));
        apiElement.appendChild(wayNodesElement);

        Element changesetsElement = responseDoc.createElement("changesets");
        changesetsElement.setAttribute("maximum_elements",
                HootProperties.getPropertyOrDefault("maximumChangesetElements"));
        apiElement.appendChild(changesetsElement);

        Element timeoutElement = responseDoc.createElement("timeout");
        timeoutElement.setAttribute("seconds", String.valueOf(
                Integer.parseInt(HootProperties.getPropertyOrDefault("changesetIdleTimeoutMinutes")) * 60));
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
