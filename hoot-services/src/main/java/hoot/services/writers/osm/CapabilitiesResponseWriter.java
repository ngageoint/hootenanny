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
package hoot.services.writers.osm;

import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.HootProperties;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Writes the response to a capabilities request
 */
public class CapabilitiesResponseWriter {
    private static final Logger log = LoggerFactory.getLogger(CapabilitiesResponseWriter.class);

    /**
     * Writes the capabilities response to an XML document
     *
     * @return an XML document
     */
    public Document writeResponse() {
        Document responseDoc = null;
        try {
            log.debug("Building response...");

            responseDoc = XmlDocumentBuilder.create();

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
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError(
                    "Error creating response for capabilities query. (" + e.getMessage() + ") ",
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        return responseDoc;
    }
}
