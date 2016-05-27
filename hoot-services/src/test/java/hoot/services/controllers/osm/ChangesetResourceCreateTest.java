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

import java.sql.Timestamp;
import java.util.Calendar;

import javax.ws.rs.core.MediaType;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.ClientResponse.Status;
import com.sun.jersey.api.client.UniformInterfaceException;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.Maps;
import hoot.services.db2.QMaps;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;


public class ChangesetResourceCreateTest extends OsmResourceTestAbstract {
    private static final Logger logger = LoggerFactory.getLogger(ChangesetResourceCreateTest.class);

    public ChangesetResourceCreateTest() {
        super("hoot.services.controllers.osm");
    }

    @Test
    @Category(UnitTest.class)
    public void testCreatePreflight() throws Exception {
        try {
            String responseData = null;
            try {
                responseData = resource()
                        .path("api/0.6/changeset/create")
                        .queryParam("mapId", "1")
                        .type(MediaType.APPLICATION_FORM_URLENCODED)
                        .accept(MediaType.TEXT_PLAIN)
                        .options(String.class);
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }

            Assert.assertEquals("", responseData);
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testCreateSendingMapId() throws Exception {
        // Create a changeset, specifying the its map by ID.
        try {
            String responseData = resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");

            Assert.assertNotNull(responseData);
            Long changesetId = Long.parseLong(responseData);

            OsmTestUtils.verifyTestChangesetCreatedByRequest(changesetId);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testCreateSendingMapName() throws Exception {
        // Create a changeset, specifying its map by name.
        try {
            String responseData = resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", "map-with-id-" + mapId)
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
            Assert.assertNotNull(responseData);
            Long changesetId = Long.parseLong(responseData);

            OsmTestUtils.verifyTestChangesetCreatedByRequest(changesetId);
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
        }
        catch (Exception e) {
            logger.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateByNonUniqueMapName() throws Exception {
        // insert another map with the same name as the test map
        Maps map = new Maps();
        QMaps maps = QMaps.maps;
        SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId));
        long nextMapId = query.uniqueResult(SQLExpressions.nextval(Long.class, "maps_id_seq"));

        map.setId(nextMapId);
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        map.setCreatedAt(now);
        map.setDisplayName("map-with-id-" + mapId);
        map.setUserId(userId);
        new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), maps).populate(map).execute();

        String mapName = null;

        // Create a changeset, providing a map name that isn't unique. A failure
        // should occur and no data in system should be modified.
        try {
            // try to create a changeset from a map name that is linked to multiple map IDs
            mapName = "map-with-id-" + mapId;
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", mapName)
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                         "<osm>" +
                             "<changeset version=\"0.3\" generator=\"iD\">" +
                                 "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                 "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                 "<tag k=\"comment\" v=\"my edit\"/>" +
                             "</changeset>" +
                         "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class)
                    .contains("Multiple maps exist with name: " + mapName + ".  Please specify a single, valid map."));

            throw e;
        }
        finally {
            new SQLDeleteClause(conn, DbUtils.getConfiguration(), maps).where(maps.id.eq(nextMapId)).execute();
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateMissingMapParam() throws Exception {
        // Try to create a changeset without specifying its map. A failure
        // should occur and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID"));
            Assert.assertFalse(DbUtils.changesetDataExistsInServicesDb(conn));
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateEmptyMapId() throws Exception {
        // Try to create a changeset, specifying an empty map ID string. A
        // failure should occur and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", "")
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID"));
            Assert.assertFalse(DbUtils.changesetDataExistsInServicesDb(conn));
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateInvalidMapId() throws Exception {
        // Try to create a changeset, specifying an ID of a map that doesn't
        // exist. A failure should occur and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", "-1")
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID"));
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateBadXml() throws Exception {
        // Try to create a changeset with malformed XML. A failure should occur
        // and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm" + // missing closing tag
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing changeset XML"));
            Assert.assertFalse(DbUtils.changesetDataExistsInServicesDb(conn));
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateEmptyTag() throws Exception {
        // Try to create a changeset with a tag that has no attributes. A
        // failure should occur and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag />" + // tag with no attributes
                                "<tag k=\"created_by\" v=\"iD 1.1.6\"/>" +
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error inserting tags"));
            Assert.assertFalse(DbUtils.changesetDataExistsInServicesDb(conn));
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testCreateMissingTagValue() throws Exception {
        // Try to create a changeset with a tag missing its attribute value. A
        // failure should occur and no data in system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/create")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_PLAIN)
                    .put(String.class,
                        "<osm>" +
                            "<changeset version=\"0.3\" generator=\"iD\">" +
                                "<tag k=\"imagery_used\" v=\"Bing\"/>" +
                                "<tag k=\"created_by\"/>" + // tag with no value attribute
                                "<tag k=\"comment\" v=\"my edit\"/>" +
                            "</changeset>" +
                        "</osm>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error inserting tags"));
            Assert.assertFalse(DbUtils.changesetDataExistsInServicesDb(conn));
            throw e;
        }
    }
}
