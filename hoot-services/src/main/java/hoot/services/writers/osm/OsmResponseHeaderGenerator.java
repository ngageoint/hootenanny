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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.writers.osm;

import java.io.IOException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.HootProperties;


/**
 * Generates an OSM XML response header
 */
public class OsmResponseHeaderGenerator {
    /**
     * Creates an OSM data header for a web response
     * 
     * @param document
     *            owning XML document
     * @return an XML Element
     * @throws IOException
     *             if unable to read settings from config file
     */
    public static Element getOsmDataHeader(Document document) throws IOException {
        Element osmElement = getOsmHeader(document);
        osmElement.setAttribute("copyright",
                HootProperties.getInstance().getProperty("copyright", HootProperties.getDefault("copyright")));
        osmElement.setAttribute("attribution",
                HootProperties.getInstance().getProperty("attribution", HootProperties.getDefault("attribution")));
        osmElement.setAttribute("license",
                HootProperties.getInstance().getProperty("license", HootProperties.getDefault("license")));
        return osmElement;
    }

    /**
     * Creates an OSM header for a web response
     * 
     * @param document
     *            owning XML document
     * @return an XML Element
     * @throws IOException
     *             if unable to read settings from config file
     */
    public static Element getOsmHeader(Document document) throws IOException {
        Element osmElement = document.createElement("osm");
        osmElement.setAttribute("version",
                HootProperties.getInstance().getProperty("osmVersion", HootProperties.getDefault("osmVersion")));
        osmElement.setAttribute("generator",
                HootProperties.getInstance().getProperty("generator", HootProperties.getDefault("generator")));
        return osmElement;
    }
}
