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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import static hoot.services.HootProperties.*;

import org.w3c.dom.Document;
import org.w3c.dom.Element;


/**
 * Generates an OSM XML response header
 */
final class OsmResponseHeaderGenerator {
    private OsmResponseHeaderGenerator() {
    }

    /**
     * Creates an OSM data header for a web response
     *
     * @param document
     *            owning XML document
     * @return an XML Element
     */
    static Element getOsmDataHeader(Document document) {
        Element osmElement = getOsmHeader(document);
        osmElement.setAttribute("copyright", COPYRIGHT);
        osmElement.setAttribute("attribution", ATTRIBUTION);
        osmElement.setAttribute("license", LICENSE);
        return osmElement;
    }

    /**
     * Creates an OSM header for a web response
     *
     * @param document
     *            owning XML document
     * @return an XML Element
     */
    static Element getOsmHeader(Document document) {
        Element osmElement = document.createElement("osm");
        osmElement.setAttribute("version", OSM_VERSION);
        osmElement.setAttribute("generator", GENERATOR);
        return osmElement;
    }
}
