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

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.CapabilitiesResponseWriter;


/**
 * Service endpoint for OSM capabilities
 */
@Path("/api/capabilities")
public class CapabilitiesResource {
    private static final Logger log = LoggerFactory.getLogger(CapabilitiesResource.class);

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
        try {
            log.info("Retrieving capabilities...");

            Document responseDoc = (new CapabilitiesResponseWriter()).writeResponse();
            log.debug("Returning response: " + StringUtils.abbreviate(XmlDocumentBuilder.toString(responseDoc), 100)
                    + " ...");
            return Response.ok(new DOMSource(responseDoc), MediaType.TEXT_XML)
                    .header("Content-type", MediaType.TEXT_XML).build();
        }
        catch (Exception e) {
            final String message = "Error retrieving capabilities: " + e.getMessage();
            log.error(message);
            return Response.status(Status.INTERNAL_SERVER_ERROR).entity(message).build();
        }
    }
}
