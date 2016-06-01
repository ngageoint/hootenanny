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

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;

//import hoot.services.models.review.AllReviewableItems;
//import hoot.services.models.review.ReviewableItemBbox;
//import hoot.services.models.review.ReviewableItemBboxInfo;
//import hoot.services.utils.MockResultSet;
//import hoot.services.utils.MockStatement;
//import org.json.simple.JSONObject;
//import org.mockito.Mockito;

/*import com.mysema.query.QueryFlag;
import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.support.Expressions;
import com.mysema.query.types.Path;
import com.mysema.query.types.Predicate;
import com.mysema.query.types.query.ListSubQuery;*/


//TODO: a lot of commented out tests in here...either fix or remove them
public class AllReviewableItemsQueryTest {
    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithWayMembersCentroidInBboxQuery() throws Exception {
        BoundingBox bbox = new BoundingBox(-77.0520431244303, 38.9025514720831, -77.0120431244303, 38.9925514720831);
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 10, bbox);
        String actual = q._getReviewableRelatioWithWayMembersCentroidInBboxQuery().toString();

        String expected = "from (select \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"relation_id\", \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"needreview\", max(\"currentNodeSubQ\".\"latitude\") as \"maxlat\", min(\"currentNodeSubQ\".\"latitude\") as \"minlat\", max(\"currentNodeSubQ\".\"longitude\") as \"maxlon\", min(\"currentNodeSubQ\".\"longitude\") as \"minlon\", (((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\")) as \"centlat\", (((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\")) as \"centlon\"\n"
                + "from (select \"current_nodes\".\"id\", \"current_nodes\".\"latitude\", \"current_nodes\".\"longitude\"\n"
                + "from \"current_nodes_10\" \"current_nodes\") as \"currentNodeSubQ\"\n"
                + "join (select \"currentWayNodesSubQ\".\"node_id\", \"reviewRelJoinRelMemberSubQ\".\"relation_id\", \"reviewRelJoinRelMemberSubQ\".\"needreview\"\n"
                + "from (select \"current_way_nodes\".\"node_id\", \"current_way_nodes\".\"way_id\"\n"
                + "from \"current_way_nodes_10\" \"current_way_nodes\") as \"currentWayNodesSubQ\"\n"
                + "join (select \"currentRelMembersSubQ\".\"member_id\", \"currentRelMembersSubQ\".\"relation_id\", \"reviewableCurrentRelSubQ\".\"needreview\"\n"
                + "from (select \"current_relation_members\".\"member_id\", \"current_relation_members\".\"relation_id\", \"current_relation_members\".\"member_type\"\n"
                + "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"member_type\" = ?) as \"currentRelMembersSubQ\"\n"
                + "join (select \"current_relations\".\"id\", tags->'hoot:review:needs' as \"needreview\"\n"
                + "from \"current_relations_10\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) as \"reviewableCurrentRelSubQ\"\n"
                + "on \"currentRelMembersSubQ\".\"relation_id\" = \"reviewableCurrentRelSubQ\".\"id\") as \"reviewRelJoinRelMemberSubQ\"\n"
                + "on \"currentWayNodesSubQ\".\"way_id\" = \"reviewRelJoinRelMemberSubQ\".\"member_id\") as \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\"\n"
                + "on \"currentNodeSubQ\".\"id\" = \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"node_id\"\n"
                + "group by \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"relation_id\", \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"needreview\") as \"reviewRelWayMembersCentroidSubQ\"\n"
                + "where \"reviewRelWayMembersCentroidSubQ\".\"centlat\" >= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlat\" <= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlon\" >= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlon\" <= ?\n"
                + "limit ?";
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithWayMembersCentroidInBboxQueryWithEmpytBbox() throws Exception {
        BoundingBox bbox = new BoundingBox();
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 7, bbox);
        String actual = q._getReviewableRelatioWithWayMembersCentroidInBboxQuery().toString();

        String expected = "from (select \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"relation_id\", \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"needreview\", max(\"currentNodeSubQ\".\"latitude\") as \"maxlat\", min(\"currentNodeSubQ\".\"latitude\") as \"minlat\", max(\"currentNodeSubQ\".\"longitude\") as \"maxlon\", min(\"currentNodeSubQ\".\"longitude\") as \"minlon\", (((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\")) as \"centlat\", (((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\")) as \"centlon\"\n"
                + "from (select \"current_nodes\".\"id\", \"current_nodes\".\"latitude\", \"current_nodes\".\"longitude\"\n"
                + "from \"current_nodes_7\" \"current_nodes\") as \"currentNodeSubQ\"\n"
                + "join (select \"currentWayNodesSubQ\".\"node_id\", \"reviewRelJoinRelMemberSubQ\".\"relation_id\", \"reviewRelJoinRelMemberSubQ\".\"needreview\"\n"
                + "from (select \"current_way_nodes\".\"node_id\", \"current_way_nodes\".\"way_id\"\n"
                + "from \"current_way_nodes_7\" \"current_way_nodes\") as \"currentWayNodesSubQ\"\n"
                + "join (select \"currentRelMembersSubQ\".\"member_id\", \"currentRelMembersSubQ\".\"relation_id\", \"reviewableCurrentRelSubQ\".\"needreview\"\n"
                + "from (select \"current_relation_members\".\"member_id\", \"current_relation_members\".\"relation_id\", \"current_relation_members\".\"member_type\"\n"
                + "from \"current_relation_members_7\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"member_type\" = ?) as \"currentRelMembersSubQ\"\n"
                + "join (select \"current_relations\".\"id\", tags->'hoot:review:needs' as \"needreview\"\n"
                + "from \"current_relations_7\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) as \"reviewableCurrentRelSubQ\"\n"
                + "on \"currentRelMembersSubQ\".\"relation_id\" = \"reviewableCurrentRelSubQ\".\"id\") as \"reviewRelJoinRelMemberSubQ\"\n"
                + "on \"currentWayNodesSubQ\".\"way_id\" = \"reviewRelJoinRelMemberSubQ\".\"member_id\") as \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\"\n"
                + "on \"currentNodeSubQ\".\"id\" = \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"node_id\"\n"
                + "group by \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"relation_id\", \"reviewRelJoinRelMemberJoinCurrentWayNodesSubQ\".\"needreview\") as \"reviewRelWayMembersCentroidSubQ\"\n"
                + "where \"reviewRelWayMembersCentroidSubQ\".\"centlat\" >= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlat\" <= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlon\" >= ? and \"reviewRelWayMembersCentroidSubQ\".\"centlon\" <= ?\n"
                + "limit ?";
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithWayMembersCentroidInBboxQueryWithNullBbox() throws Exception {
        BoundingBox bbox = null;
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 10, bbox);
        try {
            q._getReviewableRelatioWithWayMembersCentroidInBboxQuery();
        }
        catch (Exception ex) {
            String expected = "Invalid Bounding box.";
            org.junit.Assert.assertEquals(expected, ex.getMessage());
        }

    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelationMembersCountByTypeQuery() throws Exception {
        BoundingBox bbox = new BoundingBox();
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 10, bbox);
        String actual = q._getReviewableRelationMembersCountByTypeQuery(DbUtils.nwr_enum.node).toString();
        String expected = "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" in (select \"current_relations\".\"id\"\n"
                + "from \"current_relations_10\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) and \"current_relation_members\".\"member_type\" = ?";
        org.junit.Assert.assertEquals(expected, actual);

        actual = q._getReviewableRelationMembersCountByTypeQuery(DbUtils.nwr_enum.way).toString();
        expected = "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" in (select \"current_relations\".\"id\"\n"
                + "from \"current_relations_10\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) and \"current_relation_members\".\"member_type\" = ?";
        org.junit.Assert.assertEquals(expected, actual);

        actual = q._getReviewableRelationMembersCountByTypeQuery(DbUtils.nwr_enum.relation).toString();
        expected = "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"relation_id\" in (select \"current_relations\".\"id\"\n"
                + "from \"current_relations_10\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) and \"current_relation_members\".\"member_type\" = ?";
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithWayMembersCentroidInBbox() throws Exception {
        /*
         * class ReviewMockResultSet extends MockResultSet { private int
         * _currIdx = -1; private List<JSONObject> _mockData = new
         * ArrayList<>();
         * 
         * public ReviewMockResultSet() {
         * 
         * JSONObject d = new JSONObject(); d.put("maxlon", -77.052043);
         * d.put("maxlat", 38.902551); d.put("minlon", -77.052043);
         * d.put("minlat", 38.902551); d.put("relation_id", (long)2);
         * d.put("needreview", "yes"); _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("maxlon", -77.047465); d.put("maxlat",
         * 38.895676); d.put("minlon", -77.047465); d.put("minlat", 38.895676);
         * d.put("relation_id", (long)4); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("maxlon", -77.028118); d.put("maxlat",
         * 38.892085); d.put("minlon", -77.028118); d.put("minlat", 38.892085);
         * d.put("relation_id", (long)5); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * }
         * 
         * @Override public double getDouble(String columnLabel) throws
         * SQLException { return (double)
         * _mockData.get(_currIdx).get(columnLabel); }
         * 
         * @Override public boolean next() throws SQLException { _currIdx++;
         * return _currIdx < _mockData.size(); }
         * 
         * @Override public long getLong(String columnLabel) throws SQLException
         * { return (long) _mockData.get(_currIdx).get(columnLabel); } }
         * 
         * ReviewMockResultSet mockResult = new ReviewMockResultSet();
         * 
         * BoundingBox bbox = new BoundingBox(); AllReviewableItemsQuery real =
         * new AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery
         * spy = Mockito.spy(real);
         * org.mockito.Mockito.doReturn(mockResult).when(spy)
         * ._execReviewableRelatioWithWayMembersCentroidInBboxQuery(org.mockito.
         * Matchers.any(java.sql.Statement.class));
         * 
         * 
         * 
         * 
         * Map<Long, ReviewableItemBboxInfo> res =
         * spy._getReviewableRelatioWithWayMembersCentroidInBbox();
         * 
         * org.junit.Assert.assertEquals(3, res.size());
         * 
         * BoundingBox resBbox = res.get((long)2).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.902551 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.052043 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.902551 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.052043 == resBbox.getMaxLon());
         * 
         * resBbox = res.get((long)4).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.895676 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.047465 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.895676 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.047465 == resBbox.getMaxLon());
         * 
         * resBbox = res.get((long)5).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.892085 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.028118 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.892085 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.028118 == resBbox.getMaxLon());
         * 
         */
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithNodeMembersCentroidInBboxQuery() throws Exception {
        BoundingBox bbox = new BoundingBox(-77.0520431244303, 38.9025514720831, -77.0120431244303, 38.9925514720831);
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 10, bbox);
        String actual = q._getReviewableRelatioWithNodeMembersCentroidInBboxQuery().toString();

        String expected = "from (select \"reviewRelJoinRelMemberSubQ\".\"relation_id\", \"reviewRelJoinRelMemberSubQ\".\"needreview\", max(\"currentNodeSubQ\".\"latitude\") as \"maxlat\", min(\"currentNodeSubQ\".\"latitude\") as \"minlat\", max(\"currentNodeSubQ\".\"longitude\") as \"maxlon\", min(\"currentNodeSubQ\".\"longitude\") as \"minlon\", (((max(\"currentNodeSubQ\".\"latitude\") - min(\"currentNodeSubQ\".\"latitude\"))/2)+min(\"currentNodeSubQ\".\"latitude\")) as \"centlat\", (((max(\"currentNodeSubQ\".\"longitude\") - min(\"currentNodeSubQ\".\"longitude\"))/2)+min(\"currentNodeSubQ\".\"longitude\")) as \"centlon\"\n"
                + "from (select \"current_nodes\".\"id\", \"current_nodes\".\"latitude\", \"current_nodes\".\"longitude\"\n"
                + "from \"current_nodes_10\" \"current_nodes\") as \"currentNodeSubQ\"\n"
                + "join (select \"currentRelMembersSubQ\".\"member_id\", \"currentRelMembersSubQ\".\"relation_id\", \"reviewableCurrentRelSubQ\".\"needreview\"\n"
                + "from (select \"current_relation_members\".\"member_id\", \"current_relation_members\".\"relation_id\", \"current_relation_members\".\"member_type\"\n"
                + "from \"current_relation_members_10\" \"current_relation_members\"\n"
                + "where \"current_relation_members\".\"member_type\" = ?) as \"currentRelMembersSubQ\"\n"
                + "join (select \"current_relations\".\"id\", tags->'hoot:review:needs' as \"needreview\"\n"
                + "from \"current_relations_10\" \"current_relations\"\n"
                + "where exist(tags,'hoot:review:needs')) as \"reviewableCurrentRelSubQ\"\n"
                + "on \"currentRelMembersSubQ\".\"relation_id\" = \"reviewableCurrentRelSubQ\".\"id\") as \"reviewRelJoinRelMemberSubQ\"\n"
                + "on \"currentNodeSubQ\".\"id\" = \"reviewRelJoinRelMemberSubQ\".\"member_id\"\n"
                + "group by \"reviewRelJoinRelMemberSubQ\".\"relation_id\", \"reviewRelJoinRelMemberSubQ\".\"needreview\") as \"reviewRelNodeMembersCentroidSubQ\"\n"
                + "where \"reviewRelNodeMembersCentroidSubQ\".\"centlat\" >= ? and \"reviewRelNodeMembersCentroidSubQ\".\"centlat\" <= ? and \"reviewRelNodeMembersCentroidSubQ\".\"centlon\" >= ? and \"reviewRelNodeMembersCentroidSubQ\".\"centlon\" <= ?\n"
                + "limit ?";
        org.junit.Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithNodeMembersCentroidInBboxQueryWithNullBbox() throws Exception {
        BoundingBox bbox = null;
        AllReviewableItemsQuery q = new AllReviewableItemsQuery(null, 10, bbox);
        try {
            q._getReviewableRelatioWithNodeMembersCentroidInBboxQuery();
        }
        catch (Exception ex) {
            String expected = "Invalid Bounding box.";
            org.junit.Assert.assertEquals(expected, ex.getMessage());
        }

    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelatioWithNodeMembersCentroidInBbox() throws Exception {
        /*
         * class ReviewMockResultSet extends MockResultSet { private int
         * _currIdx = -1; private List<JSONObject> _mockData = new
         * ArrayList<>();
         * 
         * public ReviewMockResultSet() {
         * 
         * JSONObject d = new JSONObject(); d.put("maxlon", -77.052043);
         * d.put("maxlat", 38.902551); d.put("minlon", -77.052043);
         * d.put("minlat", 38.902551); d.put("relation_id", (long)2);
         * d.put("needreview", "yes"); _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("maxlon", -77.047465); d.put("maxlat",
         * 38.895676); d.put("minlon", -77.047465); d.put("minlat", 38.895676);
         * d.put("relation_id", (long)4); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("maxlon", -77.028118); d.put("maxlat",
         * 38.892085); d.put("minlon", -77.028118); d.put("minlat", 38.892085);
         * d.put("relation_id", (long)5); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * }
         * 
         * @Override public double getDouble(String columnLabel) throws
         * SQLException { return (double)
         * _mockData.get(_currIdx).get(columnLabel); }
         * 
         * @Override public boolean next() throws SQLException { _currIdx++;
         * return _currIdx < _mockData.size(); }
         * 
         * @Override public long getLong(String columnLabel) throws SQLException
         * { return (long) _mockData.get(_currIdx).get(columnLabel); } }
         * 
         * ReviewMockResultSet mockResult = new ReviewMockResultSet();
         * 
         * BoundingBox bbox = new BoundingBox(); AllReviewableItemsQuery real =
         * new AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery
         * spy = Mockito.spy(real);
         * org.mockito.Mockito.doReturn(mockResult).when(spy)
         * ._execReviewableRelatioWithNodeMembersCentroidInBboxQuery(org.mockito
         * .Matchers.any(java.sql.Statement.class));
         * 
         * org.mockito.Mockito.doReturn(new MockStatement()).when(spy)
         * ._createStatement();
         * 
         * 
         * 
         * Map<Long, ReviewableItemBboxInfo> res =
         * spy._getReviewableRelatioWithNodeMembersCentroidInBbox();
         * 
         * org.junit.Assert.assertEquals(3, res.size());
         * 
         * BoundingBox resBbox = res.get((long)2).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.902551 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.052043 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.902551 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.052043 == resBbox.getMaxLon());
         * 
         * resBbox = res.get((long)4).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.895676 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.047465 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.895676 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.047465 == resBbox.getMaxLon());
         * 
         * resBbox = res.get((long)5).getBbox();
         * 
         * org.junit.Assert.assertTrue(38.892085 == resBbox.getMinLat());
         * org.junit.Assert.assertTrue(-77.028118 == resBbox.getMinLon());
         * org.junit.Assert.assertTrue(38.892085 == resBbox.getMaxLat());
         * org.junit.Assert.assertTrue(-77.028118 == resBbox.getMaxLon());
         * 
         */
    }

    @Test
    @Category(UnitTest.class)
    public void testGetReviewableRelationMembers() throws Exception {
        /*
         * class ReviewMockResultSet extends MockResultSet { private int
         * _currIdx = -1; private List<JSONObject> _mockData = new
         * ArrayList<>();
         * 
         * public ReviewMockResultSet() {
         * 
         * JSONObject d = new JSONObject(); d.put("member_id", (long)165);
         * d.put("relation_id", (long)3); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("member_id", (long)27);
         * d.put("relation_id", (long)3); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("member_id", (long)769);
         * d.put("relation_id", (long)6); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * d = new JSONObject(); d.put("member_id", (long)628);
         * d.put("relation_id", (long)6); d.put("needreview", "yes");
         * _mockData.add(d);
         * 
         * }
         * 
         * @Override public String getString(String columnLabel) throws
         * SQLException { return (String)
         * _mockData.get(_currIdx).get(columnLabel); }
         * 
         * @Override public boolean next() throws SQLException { _currIdx++;
         * return _currIdx < _mockData.size(); }
         * 
         * @Override public long getLong(String columnLabel) throws SQLException
         * { return (long) _mockData.get(_currIdx).get(columnLabel); } }
         * 
         * ReviewMockResultSet mockResult = new ReviewMockResultSet();
         * 
         * BoundingBox bbox = new BoundingBox(); AllReviewableItemsQuery real =
         * new AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery
         * spy = Mockito.spy(real);
         * 
         * org.mockito.Mockito.doReturn(mockResult).when(spy)
         * ._execReviewableRelationMembersQuery(org.mockito.Matchers.any(java.
         * sql.Statement.class));
         * 
         * org.mockito.Mockito.doReturn(new MockStatement()).when(spy)
         * ._createStatement();
         * 
         * 
         * 
         * List<JSONObject> res = spy._getReviewableRelationMembers();
         * 
         * org.junit.Assert.assertEquals(4, res.size());
         * 
         * JSONObject o = res.get(0); org.junit.Assert.assertTrue((long)165 ==
         * (long)o.get("memberid")); org.junit.Assert.assertTrue((long)3 ==
         * (long)o.get("relationid"));
         * org.junit.Assert.assertTrue(o.get("needreview").toString().equals(
         * "yes"));
         * 
         * o = res.get(1); org.junit.Assert.assertTrue((long)27 ==
         * (long)o.get("memberid")); org.junit.Assert.assertTrue((long)3 ==
         * (long)o.get("relationid"));
         * org.junit.Assert.assertTrue(o.get("needreview").toString().equals(
         * "yes"));
         * 
         * o = res.get(2); org.junit.Assert.assertTrue((long)769 ==
         * (long)o.get("memberid")); org.junit.Assert.assertTrue((long)6 ==
         * (long)o.get("relationid"));
         * org.junit.Assert.assertTrue(o.get("needreview").toString().equals(
         * "yes"));
         * 
         * o = res.get(3); org.junit.Assert.assertTrue((long)628 ==
         * (long)o.get("memberid")); org.junit.Assert.assertTrue((long)6 ==
         * (long)o.get("relationid"));
         * org.junit.Assert.assertTrue(o.get("needreview").toString().equals(
         * "yes"));
         */
    }

    @Test
    @Category(UnitTest.class)
    public void testExecQueryNode() throws Exception {
        /*
         * BoundingBox bbox = new BoundingBox(); AllReviewableItemsQuery real =
         * new AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery
         * spy = Mockito.spy(real);
         * 
         * org.mockito.Mockito.doReturn((long)10).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "node"));
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "way"));
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "relation"));
         * 
         * Map<Long, ReviewableItemBboxInfo> mockNodeResp = new HashMap<>();
         * long relId = 2; String needReview = "yes"; BoundingBox bbx = new
         * BoundingBox(-77.052043, 38.902551, -77.052043, 38.902551);
         * ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbx, 0,
         * relId, needReview); mockNodeResp.put(relId, info);
         * 
         * relId = 4; needReview = "yes"; bbx = new BoundingBox(-77.047465,
         * 38.895676, -77.047465, 38.895676); info = new
         * ReviewableItemBboxInfo(bbx, 0, relId, needReview);
         * mockNodeResp.put(relId, info);
         * 
         * 
         * 
         * org.mockito.Mockito.doReturn(mockNodeResp).when(spy)
         * ._getReviewableRelatioWithNodeMembersCentroidInBbox();
         * 
         * AllReviewableItems resp = (AllReviewableItems) spy.execQuery();
         * org.junit.Assert.assertEquals(2, resp.getReviewableItems().size());
         * 
         * org.junit.Assert.assertEquals((long)0,
         * resp.getReviewableItems().get((long)2).getMapId());
         * org.junit.Assert.assertEquals((long)2,
         * resp.getReviewableItems().get((long)2).getRelationId());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMinLat());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMaxLat());
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMinLon());
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMaxLon());
         * 
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getCenterX());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getCenterY());
         * 
         * 
         * org.junit.Assert.assertEquals((long)0,
         * resp.getReviewableItems().get((long)4).getMapId());
         * org.junit.Assert.assertEquals((long)4,
         * resp.getReviewableItems().get((long)4).getRelationId());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMinLat());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMaxLat());
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMinLon());
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMaxLon());
         * 
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getCenterX());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getCenterY());
         */
    }

    @Test
    @Category(UnitTest.class)
    public void testExecQueryWay() throws Exception {
        /*
         * BoundingBox bbox = new BoundingBox(); AllReviewableItemsQuery real =
         * new AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery
         * spy = Mockito.spy(real);
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "node"));
         * 
         * org.mockito.Mockito.doReturn((long)10).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "way"));
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "relation"));
         * 
         * Map<Long, ReviewableItemBboxInfo> mockWayResp = new HashMap<>(); long
         * relId = 2; String needReview = "yes"; BoundingBox bbx = new
         * BoundingBox(-77.052043, 38.902551, -77.052043, 38.902551);
         * ReviewableItemBboxInfo info = new ReviewableItemBboxInfo(bbx, 0,
         * relId, needReview); mockWayResp.put(relId, info);
         * 
         * relId = 4; needReview = "yes"; bbx = new BoundingBox(-77.047465,
         * 38.895676, -77.047465, 38.895676); info = new
         * ReviewableItemBboxInfo(bbx, 0, relId, needReview);
         * mockWayResp.put(relId, info);
         * 
         * 
         * 
         * org.mockito.Mockito.doReturn(mockWayResp).when(spy)
         * ._getReviewableRelatioWithWayMembersCentroidInBbox();
         * 
         * AllReviewableItems resp = (AllReviewableItems) spy.execQuery();
         * org.junit.Assert.assertEquals(2, resp.getReviewableItems().size());
         * 
         * org.junit.Assert.assertEquals((long)0,
         * resp.getReviewableItems().get((long)2).getMapId());
         * org.junit.Assert.assertEquals((long)2,
         * resp.getReviewableItems().get((long)2).getRelationId());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMinLat());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMaxLat());
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMinLon());
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getMaxLon());
         * 
         * org.junit.Assert.assertTrue(-77.052043 ==
         * resp.getReviewableItems().get((long)2).getBbox().getCenterX());
         * org.junit.Assert.assertTrue(38.902551 ==
         * resp.getReviewableItems().get((long)2).getBbox().getCenterY());
         * 
         * 
         * org.junit.Assert.assertEquals((long)0,
         * resp.getReviewableItems().get((long)4).getMapId());
         * org.junit.Assert.assertEquals((long)4,
         * resp.getReviewableItems().get((long)4).getRelationId());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMinLat());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMaxLat());
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMinLon());
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getMaxLon());
         * 
         * org.junit.Assert.assertTrue(-77.047465 ==
         * resp.getReviewableItems().get((long)4).getBbox().getCenterX());
         * org.junit.Assert.assertTrue(38.895676 ==
         * resp.getReviewableItems().get((long)4).getBbox().getCenterY());
         */
    }

    @Test
    @Category(UnitTest.class)
    public void testExecQueryRelation() throws Exception {
        /*
         * BoundingBox bbox = new
         * BoundingBox(-104.912548,38.857832,-104.719153,38.875195);
         * AllReviewableItemsQuery real = new
         * AllReviewableItemsQuery(null,10,bbox); AllReviewableItemsQuery spy =
         * Mockito.spy(real);
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "node"));
         * 
         * org.mockito.Mockito.doReturn((long)0).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "way"));
         * 
         * org.mockito.Mockito.doReturn((long)4).when(spy)
         * ._getReviewableRelationMembersCount(org.mockito.Matchers.matches(
         * "relation"));
         * 
         * List<JSONObject> mockReviewMems = new ArrayList<>(); JSONObject d =
         * new JSONObject(); d.put("memberid", (long)165); d.put("relationid",
         * (long)3); d.put("needreview", "yes"); mockReviewMems.add(d);
         * 
         * d = new JSONObject(); d.put("memberid", (long)27);
         * d.put("relationid", (long)3); d.put("needreview", "yes");
         * mockReviewMems.add(d);
         * 
         * d = new JSONObject(); d.put("memberid", (long)769);
         * d.put("relationid", (long)6); d.put("needreview", "yes");
         * mockReviewMems.add(d);
         * 
         * d = new JSONObject(); d.put("memberid", (long)628);
         * d.put("relationid", (long)6); d.put("needreview", "yes");
         * mockReviewMems.add(d);
         * 
         * 
         * 
         * org.mockito.Mockito.doReturn(mockReviewMems).when(spy)
         * ._getReviewableRelationMembers();
         * 
         * BoundingBox mockBbox = new BoundingBox(-77.052043, 38.902551,
         * -77.012043, 38.992551); ReviewableItemBbox mockRelBbox = new
         * ReviewableItemBbox(mockBbox,0, 165);
         * org.mockito.Mockito.doReturn(mockRelBbox).when(spy)
         * ._getRelationMemberBbox(org.mockito.Matchers.eq((long)165));
         * 
         * mockBbox = new BoundingBox(-104.720859, 38.874594, -104.719645,
         * 38.874783); mockRelBbox = new ReviewableItemBbox(mockBbox,0, 27);
         * org.mockito.Mockito.doReturn(mockRelBbox).when(spy)
         * ._getRelationMemberBbox(org.mockito.Matchers.eq((long)27));
         * 
         * mockBbox = new BoundingBox(-104.719153, 38.875195, -104.717939,
         * 38.875384); mockRelBbox = new ReviewableItemBbox(mockBbox,0, 769);
         * org.mockito.Mockito.doReturn(mockRelBbox).when(spy)
         * ._getRelationMemberBbox(org.mockito.Matchers.eq((long)769));
         * 
         * mockBbox = new BoundingBox(-104.9197586, 38.857832, -104.912548,
         * 38.859302); mockRelBbox = new ReviewableItemBbox(mockBbox,0, 628);
         * org.mockito.Mockito.doReturn(mockRelBbox).when(spy)
         * ._getRelationMemberBbox(org.mockito.Matchers.eq((long)628));
         * 
         * 
         * 
         * AllReviewableItems resp = (AllReviewableItems) spy.execQuery();
         * org.junit.Assert.assertEquals(2, resp.getReviewableItems().size());
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getMapId() == (long)10);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getRelationId() == (long)3);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getNeedReview().equals("yes"));
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getBbox().getMinLat()==38.874594);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getBbox().getMinLon()==-104.720859);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getBbox().getMaxLat()==38.874783);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)3).
         * getBbox().getMaxLon()==-104.719645);
         * 
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getMapId() == (long)10);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getRelationId() == (long)6);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getNeedReview().equals("yes"));
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getBbox().getMinLat()==38.857832);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getBbox().getMinLon()==-104.9197586);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getBbox().getMaxLat()==38.859302);
         * org.junit.Assert.assertTrue(resp.getReviewableItems().get((long)6).
         * getBbox().getMaxLon()==-104.912548);
         */

    }

}
