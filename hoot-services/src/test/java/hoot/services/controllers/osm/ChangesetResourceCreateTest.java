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

import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.Calendar;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.querydsl.sql.SQLExpressions;

import hoot.services.UnitTest;
import hoot.services.models.db.Maps;
import hoot.services.models.db.QChangesets;
import hoot.services.models.db.QMaps;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;


public class ChangesetResourceCreateTest extends OsmResourceTestAbstract {
    public ChangesetResourceCreateTest() {}

    @Test
    @Category(UnitTest.class)
    public void testCreatePreflight() throws Exception {
        String responseData = null;
        try {
            responseData = target("api/0.6/changeset/create")
                    .queryParam("mapId", "1")
                    //.type(MediaType.APPLICATION_FORM_URLENCODED)
                    .request(MediaType.TEXT_PLAIN)
                    .options(String.class);
        }
        catch (WebApplicationException e) {
            Assert.fail("Unexpected response: " + e.getResponse());
        }

        Assert.assertEquals("", responseData);
    }

    @Test
    @Category(UnitTest.class)
    public void testCreateSendingMapId() throws Exception {
        // Create a changeset, specifying the its map by ID.
        try {
            String responseData = target("api/0.6/changeset/create")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);

            Assert.assertNotNull(responseData);
            Long changesetId = Long.parseLong(responseData);
            OsmTestUtils.verifyTestChangesetCreatedByRequest(changesetId);
        }
        catch (WebApplicationException e) {
            Assert.fail("Unexpected response: " + e.getResponse());
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testCreateSendingMapName() throws Exception {
        // Create a changeset, specifying its map by name.
        try {
            String responseData = target("api/0.6/changeset/create")
                    .queryParam("mapId", "map-with-id-" + mapId)
                    .request(MediaType.TEXT_PLAIN)
                    .put(Entity.entity(
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>", MediaType.TEXT_XML_TYPE), String.class);
            Assert.assertNotNull(responseData);
            Long changesetId = Long.parseLong(responseData);
            OsmTestUtils.verifyTestChangesetCreatedByRequest(changesetId);
        }
        catch (WebApplicationException e) {
            Assert.fail("Unexpected response: " + e.getResponse());
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateByNonUniqueMapName() throws Exception {
        // insert another map with the same name as the test map
        Maps map = new Maps();

        long nextMapId = createQuery(mapId)
                .select(SQLExpressions.nextval(Long.class, "maps_id_seq"))
                .from()
                .fetchOne();

        map.setId(nextMapId);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        map.setCreatedAt(now);
        map.setDisplayName("map-with-id-" + mapId);
        map.setUserId(userId);

        QMaps maps = QMaps.maps;
        createQuery(mapId).insert(maps).populate(map).execute();

        // Create a changeset, providing a map name that isn't unique. A failure
        // should occur and no data in system should be modified.
        try {
            // try to create a changeset from a map name that is linked to multiple map IDs
            String mapName = "map-with-id-" + mapId;
            target("api/0.6/changeset/create")
                    .queryParam("mapId", mapName)
                    .request(MediaType.TEXT_PLAIN)
                    .put(Entity.entity(
                         "<osm>" +
                             "<changeset version=\"0.3\" generator=\"iD\">" +
                                 "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                 "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                 "<tag k=\"comment\" v=\"my edit\"/>" +
                             "</changeset>" +
                         "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.readEntity(String.class).contains("Multiple maps exist"));

            throw e;
        }
        finally {
            createQuery().delete(maps).where(maps.id.eq(nextMapId)).execute();
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateMissingMapParam() throws Exception {
        // Try to create a changeset without specifying its map. A failure
        // should occur and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.readEntity(String.class).contains("No map exists with ID"));
            Assert.assertFalse(changesetDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateEmptyMapId() throws Exception {
        // Try to create a changeset, specifying an empty map ID string. A
        // failure should occur and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .queryParam("mapId", "")
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.readEntity(String.class).contains("No map exists with ID"));
            Assert.assertFalse(changesetDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateInvalidMapId() throws Exception {
        // Try to create a changeset, specifying an ID of a map that doesn't
        // exist. A failure should occur and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .queryParam("mapId", "-1")
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.readEntity(String.class).contains("No map exists"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateBadXml() throws Exception {
        // Try to create a changeset with malformed XML. A failure should occur
        // and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm" + // missing closing tag
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.readEntity(String.class).contains("Error parsing changeset XML"));
            Assert.assertFalse(changesetDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateEmptyTag() throws Exception {
        // Try to create a changeset with a tag that has no attributes. A
        // failure should occur and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag />" + // tag with no attributes
                            "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.readEntity(String.class).contains("Error inserting tags"));
            Assert.assertFalse(changesetDataExistsInServicesDb());
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testCreateMissingTagValue() throws Exception {
        // Try to create a changeset with a tag missing its attribute value. A
        // failure should occur and no data in system should be modified.
        try {
            target("api/0.6/changeset/create")
                .queryParam("mapId", String.valueOf(mapId))
                .request(MediaType.TEXT_PLAIN)
                .put(Entity.entity(
                    "<osm>" +
                        "<changeset version=\"0.3\" generator=\"iD\">" +
                            "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                            "<tag k=\"created_by\"/>" + // tag with no value attribute
                            "<tag k=\"comment\" v=\"my edit\"/>" +
                        "</changeset>" +
                    "</osm>", MediaType.TEXT_XML_TYPE), String.class);
        }
        catch (WebApplicationException e) {
            Response r = e.getResponse();
            Assert.assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.readEntity(String.class).contains("Error inserting tags"));
            Assert.assertFalse(changesetDataExistsInServicesDb());
            throw e;
        }
    }

    /**
     * Determines whether any changeset data exists in the services database
     *
     * @return true if changeset data exists; false otherwise
     */
    private static boolean changesetDataExistsInServicesDb() {
        long recordCtr = createQuery()
                .from(QChangesets.changesets)
                .fetchCount();
        return (recordCtr > 0);
    }
}
