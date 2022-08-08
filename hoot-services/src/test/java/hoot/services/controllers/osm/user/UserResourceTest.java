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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.user;

import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Set;

import javax.ws.rs.NotAcceptableException;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.xpath.XPath;

import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Changesets;
import hoot.services.models.db.QChangesets;
import hoot.services.models.osm.Changeset;
import hoot.services.utils.XmlUtils;


public class UserResourceTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testGetById() throws Exception {
        Document responseData = target("api/0.6/user/" + userId).request(MediaType.TEXT_XML).get(Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        assertEquals(userId + "::MapUtils::insertUser()", xpath.evaluate("//osm/user/@display_name", responseData));
        assertEquals(-1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
        assertNotNull(xpath.evaluate("//osm/user/@account_last_authorized", responseData));
        assertNotNull(xpath.evaluate("//osm/user/@account_created", responseData));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetByName() throws Exception {
        final String display_name = userId + "::MapUtils::insertUser()";
        Document responseData = target("api/0.6/user/name/" + display_name).request(MediaType.TEXT_XML).get(Document.class);

        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        assertEquals(userId + "::MapUtils::insertUser()", xpath.evaluate("//osm/user/@display_name", responseData));
        assertEquals(-1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
        assertNotNull(xpath.evaluate("//osm/user/@account_last_authorized", responseData));
        assertNotNull(xpath.evaluate("//osm/user/@account_created", responseData));

    }

    @Test
    @Category(UnitTest.class)
    public void testGetWithChangesetsModified() throws Exception {
        double originalMinLon = -78.02265434416296;
        double originalMinLat = 38.90089748801109;
        double originalMaxLon = -77.9224564416296;
        double originalMaxLat = 39.00085678801109;

        BoundingBox originalBounds = new BoundingBox(originalMinLon, originalMinLat, originalMaxLon, originalMaxLat);

        // link some changesets to the user
        Set<Long> changesetIds = new LinkedHashSet<>();

        long changesetId = Changeset.insertNew(mapId, userId, new HashMap<String, String>());
        changesetIds.add(changesetId);

        (new Changeset(mapId, changesetId)).setBounds(originalBounds);

        QChangesets changesets = QChangesets.changesets;

        Changesets changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertEquals(userId, (long) changeset.getUserId());

        changesetId = Changeset.insertNew(mapId, userId, new HashMap<String, String>());
        changesetIds.add(changesetId);

        (new Changeset(mapId, changesetId)).setBounds(originalBounds);

        changeset = createQuery(mapId)
                .select(changesets)
                .from(changesets)
                .where(changesets.id.eq(changesetId))
                .fetchOne();

        assertNotNull(changeset);
        assertEquals(userId, (long) changeset.getUserId());

        Document responseData = target("api/0.6/user/" + userId).request(MediaType.TEXT_XML).get(Document.class);
        assertNotNull(responseData);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        assertEquals(userId + "::MapUtils::insertUser()", xpath.evaluate("//osm/user/@display_name", responseData));
        assertEquals(-1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetInvalidUserId() throws Exception {
        // TODO: change this to something randomly generated and very large
        try {
            long invalidUserId = 999999;
            target("api/0.6/user/" + invalidUserId).request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(404, r.getStatus());
            throw e;
        }
    }

    @Test(expected = NotAcceptableException.class)
    @Category(UnitTest.class)
    public void testGetEmptyUserId() throws Exception {
        try {
            target("api/0.6/user/").request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotAcceptableException e) {
            Response r = e.getResponse();
            assertEquals(Status.NOT_ACCEPTABLE.getStatusCode(), r.getStatus());
            throw e;
        }
    }

    @Test(expected = NotAcceptableException.class)
    @Category(UnitTest.class)
    public void testGetMissingUserId() throws Exception {
        try {
            target("api/0.6/user").request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotAcceptableException e) {
            Response r = e.getResponse();
            assertEquals(Status.NOT_ACCEPTABLE.getStatusCode(), r.getStatus());
            throw e;
        }
    }
}
