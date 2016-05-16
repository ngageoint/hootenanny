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
package hoot.services.readers.review;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLQuery;

import hoot.services.UnitTest;


public class ReviewableBboxQueryTest {
    @Test
    @Category(UnitTest.class)
    public void testGetRelationMembersQuery() throws Exception {
        ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);

        SQLQuery q = bbxQ._getRelationMembersQuery(101);
        String expected = "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?";
        String actual = q.toString();
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetRelationNodeMembersBboxQuery() throws Exception {
        ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);
        SQLQuery q = bbxQ._getRelationNodeMembersBboxQuery(102);
        String expected = "from \"current_nodes_10\" \"current_nodes\"\n"
                + "where \"current_nodes\".\"id\" in (select \"current_relation_members\".\"member_id\"\n"
                + "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?)";
        String actual = q.toString();
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetRelationWayMembersBboxQuery() throws Exception {
        ReviewableBboxQuery bbxQ = new ReviewableBboxQuery(null, 10, 899);
        SQLQuery q = bbxQ._getRelationWayMembersBboxQuery(103);
        String expected = "from \"current_nodes_10\" \"current_nodes\"\n"
                + "where \"current_nodes\".\"id\" in (select \"current_way_nodes\".\"node_id\"\n"
                + "from \"current_way_nodes_10\" \"current_way_nodes\"\n"
                + "where \"current_way_nodes\".\"way_id\" in (select \"current_relation_members\".\"member_id\"\n"
                + "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" = ? and \"current_relation_members\".\"member_type\" = ?))";
        String actual = q.toString();
        org.junit.Assert.assertEquals(expected, actual);
    }
}
