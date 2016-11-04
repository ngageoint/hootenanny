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

import java.util.Map;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.testsupport.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;


public class ReviewResourceGetReferencesTest extends OSMResourceTestAbstract {

    @Test
    @Category(UnitTest.class)
    public void testGetReferences() throws Exception {
        ReviewTestUtils testUtils = new ReviewTestUtils();
        /* final long changesetId = */ testUtils.populateReviewDataForAllDataTypes(mapId, userId);
        Map<Long, Element> oldNodeIdsToNewNodes = testUtils.parsedElementIdsToElementsByType.get(ElementType.Node);

        ReviewRefsResponses response = target("/review/refs")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(
                    new ReviewRefsRequest(new ElementInfo[] {
                        new ElementInfo(String.valueOf(mapId), oldNodeIdsToNewNodes.get(-116L).getId(), "node"),
                        new ElementInfo(String.valueOf(mapId), oldNodeIdsToNewNodes.get(-117L).getId(), "node") })),
                    ReviewRefsResponses.class);

        ReviewRefsResponse[] reviewReferenceResponses = response.getReviewRefsResponses();
        Assert.assertEquals(2, reviewReferenceResponses.length);
        for (int i = 0; i < reviewReferenceResponses.length; i++) {
            ReviewRefsResponse refsResponse = reviewReferenceResponses[i];
            ElementInfo queryElementInfo = refsResponse.getQueryElementInfo();

            if (i == 0) {
                Assert.assertEquals(oldNodeIdsToNewNodes.get(-116L).getId(), queryElementInfo.getId());
                Assert.assertEquals("node", queryElementInfo.getType());

                ReviewRef[] refs = refsResponse.getReviewRefs();
                Assert.assertEquals(4, refs.length);

                // Now we are returning self since in one to many queried
                // element can be involved in
                // many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to
                // handle self)
                Assert.assertEquals(oldNodeIdsToNewNodes.get(-116L).getId(), refs[0].getId());
                Assert.assertEquals("node", refs[0].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
                Assert.assertEquals(2, refs[0].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-46L).getId(), refs[1].getId());
                Assert.assertEquals("node", refs[1].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
                Assert.assertEquals(2, refs[1].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-116L).getId(), refs[2].getId());
                Assert.assertEquals("node", refs[2].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[2].getMapId());
                Assert.assertEquals(3, refs[2].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-49L).getId(), refs[3].getId());
                Assert.assertEquals("node", refs[3].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[3].getMapId());
                Assert.assertEquals(3, refs[3].getReviewRelationId());
            }
            else if (i == 1) {
                Assert.assertEquals(oldNodeIdsToNewNodes.get(-117L).getId(), queryElementInfo.getId());
                Assert.assertEquals("node", queryElementInfo.getType());

                ReviewRef[] refs = refsResponse.getReviewRefs();
                Assert.assertEquals(4, refs.length);

                // Now we are returning self since in one to many queried
                // element can be involved in many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to handle self)
                Assert.assertEquals(oldNodeIdsToNewNodes.get(-117L).getId(), refs[0].getId());
                Assert.assertEquals("node", refs[0].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
                Assert.assertEquals(4, refs[0].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-42L).getId(), refs[1].getId());
                Assert.assertEquals("node", refs[1].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
                Assert.assertEquals(4, refs[1].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-117L).getId(), refs[2].getId());
                Assert.assertEquals("node", refs[2].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[2].getMapId());
                Assert.assertEquals(6, refs[2].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get(-47L).getId(), refs[3].getId());
                Assert.assertEquals("node", refs[3].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[3].getMapId());
                Assert.assertEquals(6, refs[3].getReviewRelationId());
            }
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetMapDoesntExist() throws Exception {
        try {
            target("/review/refs")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(
                            new ReviewRefsRequest(
                                    new ElementInfo[] {
                                            new ElementInfo(
                                                    // map id doesn't exist
                                                    String.valueOf((long) RandomNumberGenerator
                                                            .nextDouble((mapId + 10) ^ 4, Integer.MAX_VALUE)),
                                                    -1, "") })), ReviewRefsResponses.class);
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().readEntity(String.class).contains("No map exists"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetQueryElementDoesntExist() throws Exception {
        try {
            target("/review/refs")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.json(
                    new ReviewRefsRequest(new ElementInfo[] {
                            // this element doesn't exist
                            new ElementInfo(String.valueOf(mapId), -1, "node") })), ReviewRefsResponses.class);
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().readEntity(String.class).contains("does not exist"));
            throw e;
        }
    }

    @Test(expected = WebApplicationException.class)
    @Category(UnitTest.class)
    public void testGetQueryElementEmptyElementType() throws Exception {
        try {
            target("/review/refs")
                .request(MediaType.APPLICATION_JSON).post(
                    Entity.json(
                    new ReviewRefsRequest(new ElementInfo[] {
                            // this element has the incorrect type
                            new ElementInfo(String.valueOf(mapId), -1, "") })), ReviewRefsResponses.class);
        }
        catch (WebApplicationException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            Assert.assertTrue(e.getResponse().readEntity(String.class).contains("does not exist"));
            throw e;
        }
    }
}
