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
import hoot.services.utils.XmlUtils;


public class CapabilitiesResourceTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testGetCapabilities() throws Exception {
        Document responseInXML = target("api/capabilities").request(MediaType.TEXT_XML).get(Document.class);

        assertNotNull(responseInXML);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/version").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/area").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/waynodes").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/timeout").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/changesets").getLength());

        assertEquals(OSM_VERSION, xpath.evaluate("//osm/@version", responseInXML));
        assertEquals(GENERATOR, xpath.evaluate("//osm/@generator", responseInXML));
        assertEquals(COPYRIGHT, xpath.evaluate("//osm/@copyright", responseInXML));
        assertEquals(ATTRIBUTION, xpath.evaluate("//osm/@attribution", responseInXML));
        assertEquals(LICENSE, xpath.evaluate("//osm/@license", responseInXML));
        assertEquals(OSM_VERSION, xpath.evaluate("//osm/api/version/@minimum", responseInXML));
        assertEquals(OSM_VERSION, xpath.evaluate("//osm/api/version/@maximum", responseInXML));
        assertEquals(MAX_QUERY_AREA_DEGREES, xpath.evaluate("//osm/api/area/@maximum", responseInXML));
        assertEquals(MAXIMUM_WAY_NODES, xpath.evaluate("//osm/api/waynodes/@maximum", responseInXML));
        assertEquals(MAXIMUM_CHANGESET_ELEMENTS, xpath.evaluate("//osm/api/changesets/@maximum_elements", responseInXML));
        assertEquals(Integer.parseInt(CHANGESET_IDLE_TIMEOUT_MINUTES) * 60,
                     Integer.parseInt(xpath.evaluate("//osm/api/timeout/@seconds", responseInXML)));
        assertEquals(1, XPathAPI.selectNodeList(responseInXML, "//osm/api/status").getLength());
        assertEquals("online", xpath.evaluate("//osm/api/status/@database", responseInXML));
        assertEquals("online", xpath.evaluate("//osm/api/status/@api", responseInXML));
        assertEquals("offline", xpath.evaluate("//osm/api/status/@gpx", responseInXML));
    }
}
