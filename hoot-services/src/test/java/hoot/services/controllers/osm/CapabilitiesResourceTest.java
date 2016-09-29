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
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;

import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.UnitTest;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.utils.XmlUtils;


public class CapabilitiesResourceTest extends OsmResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testGetCapabilities() throws Exception {
        Document responseData = target("api/capabilities").request(MediaType.TEXT_XML).get(Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/version").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/area").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/waynodes").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/timeout").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/changesets").getLength());

        assertEquals(OSM_VERSION, xpath.evaluate("//osm/@version", responseData));
        assertEquals(GENERATOR, xpath.evaluate("//osm/@generator", responseData));
        assertEquals(COPYRIGHT, xpath.evaluate("//osm/@copyright", responseData));
        assertEquals(ATTRIBUTION, xpath.evaluate("//osm/@attribution", responseData));
        assertEquals(LICENSE, xpath.evaluate("//osm/@license", responseData));
        assertEquals(OSM_VERSION, xpath.evaluate("//osm/api/version/@minimum", responseData));
        assertEquals(OSM_VERSION, xpath.evaluate("//osm/api/version/@maximum", responseData));
        assertEquals(MAX_QUERY_AREA_DEGREES, xpath.evaluate("//osm/api/area/@maximum", responseData));
        assertEquals(MAXIMUM_WAY_NODES, xpath.evaluate("//osm/api/waynodes/@maximum", responseData));
        assertEquals(MAXIMUM_CHANGESET_ELEMENTS, xpath.evaluate("//osm/api/changesets/@maximum_elements", responseData));
        assertEquals(Integer.parseInt(CHANGESET_IDLE_TIMEOUT_MINUTES) * 60,
                     Integer.parseInt(xpath.evaluate("//osm/api/timeout/@seconds", responseData)));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/status").getLength());
        assertEquals("online", xpath.evaluate("//osm/api/status/@database", responseData));
        assertEquals("online", xpath.evaluate("//osm/api/status/@api", responseData));
        assertEquals("offline", xpath.evaluate("//osm/api/status/@gpx", responseData));
    }
}
