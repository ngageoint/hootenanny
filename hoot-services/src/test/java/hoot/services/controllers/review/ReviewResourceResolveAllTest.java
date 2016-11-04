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
package hoot.services.controllers.review;

import static hoot.services.controllers.osm.OSMTestUtils.getMapId;
import static hoot.services.models.db.QCurrentRelations.currentRelations;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.assertEquals;

import javax.ws.rs.NotFoundException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.querydsl.core.types.dsl.Expressions;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.testsupport.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;


public class ReviewResourceResolveAllTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testSetAllReviewsResolved() throws Exception {
        ReviewTestUtils testUtils = new ReviewTestUtils();
        testUtils.populateReviewDataForAllDataTypes(mapId, userId);

        ReviewResolverResponse response =
            target("/review/resolveall")
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.json(new ReviewResolverRequest(String.valueOf(mapId))),
                     ReviewResolverResponse.class);

        long count = createQuery(getMapId())
                .select()
                .from(currentRelations)
                .where(Expressions.booleanTemplate("tags->'hoot:review:needs' = 'yes'"))
                .fetchCount();

        // all review relations should have been set to resolved
        assertEquals(0, count);

        count = createQuery(getMapId())
                .select()
                .from(currentRelations)
                .where(Expressions.booleanTemplate("tags->'type' = 'review' and tags->'hoot:review:needs' = 'no'")
                        .and(currentRelations.changesetId.eq(response.getChangesetId())))
                .fetchCount();

        // all review relations should have the incremented changeset id
        assertEquals(5, count);

        count = createQuery(getMapId())
                .select()
                .from(currentRelations)
                .where(Expressions.booleanTemplate("tags->'type' = 'review' and tags->'hoot:review:needs' = 'no' and version = 2")
                        .and(currentRelations.changesetId.eq(response.getChangesetId())))
                .fetchCount();

        // all review relations should have had their version incremented by one
        assertEquals(5, count);
    }

    @Test(expected = NotFoundException.class)
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
        catch (NotFoundException e) {
            assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().readEntity(String.class).contains("No map exists"));
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testSetAllReviewsResolvedMissingMapIdParam() throws Exception {
        target("/review/resolveall")
                .request(MediaType.APPLICATION_JSON)
                .put(Entity.json(new ReviewResolverRequest()), ReviewResolverResponse.class);
    }
}
