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

import java.io.IOException;
import java.util.Map;

import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.sun.jersey.api.client.UniformInterfaceException;

import hoot.services.UnitTest;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementInfo;
import hoot.services.models.review.ReviewRef;
import hoot.services.models.review.ReviewRefsRequest;
import hoot.services.models.review.ReviewRefsResponse;
import hoot.services.models.review.ReviewRefsResponses;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.RandomNumberGenerator;


public class ReviewResourceGetReferencesTest extends OsmResourceTestAbstract {
    public ReviewResourceGetReferencesTest() throws NumberFormatException, IOException {
        super(new String[] { "hoot.services.controllers.job" });
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReferences() throws Exception {
        ReviewTestUtils testUtils = new ReviewTestUtils();
        /* final long changesetId = */ testUtils.populateReviewDataForAllDataTypes(mapId, userId);
        final Map<Long, Element> oldNodeIdsToNewNodes = testUtils.parsedElementIdsToElementsByType
                .get(ElementType.Node);

        final ReviewRefsResponses response = resource().path("/review/refs").type(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON).post(ReviewRefsResponses.class,
                        new ReviewRefsRequest(new ElementInfo[] {
                                new ElementInfo(String.valueOf(mapId), oldNodeIdsToNewNodes.get((long) -116).getId(),
                                        "node"),
                                new ElementInfo(String.valueOf(mapId), oldNodeIdsToNewNodes.get((long) -117).getId(),
                                        "node") }));

        final ReviewRefsResponse[] reviewReferenceResponses = response.getReviewRefsResponses();
        Assert.assertEquals(2, reviewReferenceResponses.length);
        for (int i = 0; i < reviewReferenceResponses.length; i++) {
            ReviewRefsResponse refsResponse = reviewReferenceResponses[i];
            ElementInfo queryElementInfo = refsResponse.getQueryElementInfo();

            if (i == 0) {
                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -116).getId(), queryElementInfo.getId());
                Assert.assertEquals("node", queryElementInfo.getType());

                final ReviewRef[] refs = refsResponse.getReviewRefs();
                Assert.assertEquals(4, refs.length);

                // Now we are returning self since in one to many queried
                // element can be involved in
                // many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to
                // handle self)
                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -116).getId(), refs[0].getId());
                Assert.assertEquals("node", refs[0].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
                Assert.assertEquals(2, refs[0].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -46).getId(), refs[1].getId());
                Assert.assertEquals("node", refs[1].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
                Assert.assertEquals(2, refs[1].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -116).getId(), refs[2].getId());
                Assert.assertEquals("node", refs[2].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[2].getMapId());
                Assert.assertEquals(3, refs[2].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -49).getId(), refs[3].getId());
                Assert.assertEquals("node", refs[3].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[3].getMapId());
                Assert.assertEquals(3, refs[3].getReviewRelationId());
            }
            else if (i == 1) {
                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -117).getId(), queryElementInfo.getId());
                Assert.assertEquals("node", queryElementInfo.getType());

                final ReviewRef[] refs = refsResponse.getReviewRefs();
                Assert.assertEquals(4, refs.length);

                // Now we are returning self since in one to many queried
                // element can be involved in
                // many different relations and since we do not know the
                // element's parent relation (or even if there is one)
                // we are forced return all including self. (Client need to
                // handle self)
                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -117).getId(), refs[0].getId());
                Assert.assertEquals("node", refs[0].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[0].getMapId());
                Assert.assertEquals(4, refs[0].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -42).getId(), refs[1].getId());
                Assert.assertEquals("node", refs[1].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[1].getMapId());
                Assert.assertEquals(4, refs[1].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -117).getId(), refs[2].getId());
                Assert.assertEquals("node", refs[2].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[2].getMapId());
                Assert.assertEquals(6, refs[2].getReviewRelationId());

                Assert.assertEquals(oldNodeIdsToNewNodes.get((long) -47).getId(), refs[3].getId());
                Assert.assertEquals("node", refs[3].getType());
                Assert.assertEquals(String.valueOf(mapId), refs[3].getMapId());
                Assert.assertEquals(6, refs[3].getReviewRelationId());
            }
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testGetMapDoesntExist() throws Exception {
        try {
            resource()
                    .path("/review/refs").type(
                            MediaType.APPLICATION_JSON)
                    .accept(MediaType.APPLICATION_JSON).post(ReviewRefsResponses.class,
                            new ReviewRefsRequest(
                                    new ElementInfo[] {
                                            new ElementInfo(
                                                    // map id doesn't exist
                                                    String.valueOf((long) RandomNumberGenerator
                                                            .nextDouble(mapId + 10 ^ 4, Integer.MAX_VALUE)),
                                                    -1, "") }));
        }
        catch (UniformInterfaceException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            // System.out.println(e.getResponse().getEntity(String.class));
            Assert.assertTrue(e.getResponse().getEntity(String.class).contains("No map exists"));

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testGetQueryElementDoesntExist() throws Exception {
        try {
            resource().path("/review/refs").type(MediaType.APPLICATION_JSON).accept(MediaType.APPLICATION_JSON).post(
                    ReviewRefsResponses.class,
                    new ReviewRefsRequest(new ElementInfo[] {
                            // this element doesn't exist
                            new ElementInfo(String.valueOf(mapId), -1, "node") }));
        }
        catch (UniformInterfaceException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            // System.out.println(e.getResponse().getEntity(String.class));
            Assert.assertTrue(e.getResponse().getEntity(String.class).contains("does not exist"));

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testGetQueryElementEmptyElementType() throws Exception {
        try {
            resource().path("/review/refs").type(MediaType.APPLICATION_JSON).accept(MediaType.APPLICATION_JSON).post(
                    ReviewRefsResponses.class,
                    new ReviewRefsRequest(new ElementInfo[] {
                            // this element has the incorrect type
                            new ElementInfo(String.valueOf(mapId), -1, "") }));
        }
        catch (UniformInterfaceException e) {
            Assert.assertEquals(Status.NOT_FOUND.getStatusCode(), e.getResponse().getStatus());
            // System.out.println(e.getResponse().getEntity(String.class));
            Assert.assertTrue(e.getResponse().getEntity(String.class).contains("does not exist"));

            throw e;
        }
    }
}
