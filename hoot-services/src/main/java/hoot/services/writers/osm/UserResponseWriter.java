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

import java.sql.Connection;

import javax.ws.rs.core.Response.Status;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.models.osm.User;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Writes the response to a user get request
 */
public class UserResponseWriter {
    private static final Logger log = LoggerFactory.getLogger(UserResponseWriter.class);

    /**
     * Writes the user response to an XML document
     * 
     * @param user
     *            user information to write to the response
     * @param conn
     *            JDBC Connection
     * @return an XML document
     */
    public Document writeResponse(final User user, Connection conn) {
        Document responseDoc = null;
        try {
            log.debug("Building response...");

            responseDoc = XmlDocumentBuilder.create();

            Element osmElement = OsmResponseHeaderGenerator.getOsmHeader(responseDoc);

            Element userElement = user.toXml(osmElement,
                    /* user.numChangesetsModified() */-1);
            osmElement.appendChild(userElement);

            responseDoc.appendChild(osmElement);
        }
        catch (Exception e) {
            ResourceErrorHandler.handleError("Error creating response for user query. (" + e.getMessage() + ") ",
                    Status.INTERNAL_SERVER_ERROR, log);
        }

        return responseDoc;
    }
}
