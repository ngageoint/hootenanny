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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.osm;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import org.junit.Test;

import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.controllers.osm.OSMTestUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element.ElementType;

public class MapTest extends OSMResourceTestAbstract {

    @Test
    public void test() throws Exception {

        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // create some nodes that are completely outside the query bounds
        Set<Long> oobNodeIds = OSMTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
        Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[oobNodeIds.size()]);

        // create a way completely outside the query bounds
        long oobWayId = OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(oobNodeIds), null);

        // Create a way with some nodes inside the query bounds and some
        // outside; The way and the out of bounds nodes it owns should be returned by the query since
        // at least one of the added way nodes is within the bounds.
        Set<Long> partiallyOobNodeIds = new LinkedHashSet<>();
        partiallyOobNodeIds.add(nodeIdsArr[0]);
        partiallyOobNodeIds.add(oobNodeIdsArr[0]);
        wayIds.add(OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(partiallyOobNodeIds), null));
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // create a relation where all members are completely outside of the query bounds
        List<RelationMember> members = new ArrayList<>();

        // Only oobNodeIdsArr[1] will make this relation out of bounds, b/c
        // oobNodeIdsArr[0] is used by a way which is partially in bounds and will be returned by the
        // query and, thus, any relations which reference that way and/or its nodes will also be returned.
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        OSMTestUtils.insertNewRelation(changesetId, mapId, members, null);

        // create a relation where some members are inside the query bounds and some are not
        members = new ArrayList<>();
        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));

        // see note above for why oobNodeIdsArr[1] is used here
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        relationIds.add(OSMTestUtils.insertNewRelation(changesetId, mapId, members, null));

        Map theMap = new Map(mapId);

        assertEquals(7, theMap.getNodesCount(null));
        assertEquals(5, theMap.getNodesCount(originalBounds));
        assertEquals(4, theMap.getNodesCount(queryBounds));
        assertEquals(2, theMap.retrieveNodesOutsideBounds(originalBounds).size());
        assertEquals(3, theMap.retrieveNodesOutsideBounds(queryBounds).size());

        assertFalse(theMap.getBounds().toServicesString().equalsIgnoreCase(originalBounds.toServicesString()));
        assertFalse(theMap.getBounds().toServicesString().equalsIgnoreCase(queryBounds.toServicesString()));

    }

}
