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
package hoot.services.controllers.job;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.models.review.ReviewResolverRequest;
import hoot.services.models.review.ReviewResolverResponse;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.DbUtils;
import hoot.services.utils.RandomNumberGenerator;


public class ReviewResourceResolveAllTest extends OsmResourceTestAbstract {
    public ReviewResourceResolveAllTest() {}

    @Test
    @Category(UnitTest.class)
    public void testSetAllReviewsResolved() throws Exception {
        Connection conn = DbUtils.getConnection();

        ReviewTestUtils testUtils = new ReviewTestUtils();
        /* final long changesetId = */ testUtils.populateReviewDataForAllDataTypes(mapId, userId);

        ReviewResolverResponse response =
            target("/review/resolveall")
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.json(new ReviewResolverRequest(String.valueOf(mapId))),
                     ReviewResolverResponse.class);

        Statement stmt = null;
        ResultSet rs = null;

        // all review relations should have been set to resolved
        String sql = "select count(*) from current_relations_" + mapId;
        sql += " where tags->'hoot:review:needs' = 'yes'";
        try {
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            rs.next();

            Assert.assertEquals(0, rs.getInt(1));
        }
        finally {
            if (rs != null) {
                rs.close();
            }
            if (stmt != null) {
                stmt.close();
            }
        }

        // all review relations should have the incremented changeset id
        sql = "select count(*) from current_relations_" + mapId;
        // don't think this is right...
        sql += " where tags->'type' = 'review' and tags->'hoot:review:needs' = 'no' and " + "changeset_id = "
                + response.getChangesetId();
        try {
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            rs.next();
            Assert.assertEquals(5, rs.getInt(1));
        }
        finally {
            if (rs != null) {
                rs.close();
            }
            if (stmt != null) {
                stmt.close();
            }
        }

        // all review relations should have had their version incremented by one
        sql = "select count(*) from current_relations_" + mapId;
        sql += " where tags->'type' = 'review' and tags->'hoot:review:needs' = 'no' and version = 2";
        try {
            stmt = conn.createStatement();
            rs = stmt.executeQuery(sql);
            rs.next();
            Assert.assertEquals(5, rs.getInt(1));
        }
        finally {
            if (rs != null) {
                rs.close();
            }
            if (stmt != null) {
                stmt.close();
            }
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testSetAllReviewsResolvedMapDoesntExist() throws Exception {
        try {
            target("/review/resolveall")
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.json(
                        new ReviewResolverRequest(String.valueOf(
                                (long) RandomNumberGenerator.nextDouble((mapId + 10) ^ 4, Integer.MAX_VALUE)))),
                        ReviewResolverResponse.class);
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().readEntity(String.class).contains("No map exists"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testSetAllReviewsResolvedMissingMapIdParam() throws Exception {
        try {
            target("/review/resolveall")
                    .request(MediaType.APPLICATION_JSON)
                    .put(Entity.json(new ReviewResolverRequest()),
                            ReviewResolverResponse.class);
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            throw e;
        }
    }
}
