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
package hoot.services.controllers.job;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Calendar;

import org.json.simple.JSONObject;
import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;
import hoot.services.models.db.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkSaveRequest;


public class ReviewBookmarksSaverTest {

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testInsert() throws Exception {
        Connection conn = null;
        ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, null, -1);
        ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver();
        //QLInsertClause cl = tagsSaver.createInsertClause(req);
        //String actual = cl.toString();

        String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n"
                + "values (?, ?, ?, ?, '')";

        //Assert.assertEquals(expected, actual);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testInsertWithDetail() throws Exception {
        JSONObject o = new JSONObject();
        o.put("test1", "val1");
        o.put("test2", "val2");
        ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, o, -1);
        Connection conn = null;
        ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver();
        //SQLInsertClause cl = tagsSaver.createInsertClause(req);
        //String actual = cl.toString();

        String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n"
                + "values (?, ?, ?, ?, '\"test1\"=>\"val1\",\"test2\"=>\"val2\"')";

        //Assert.assertEquals(expected, actual);
    }

    @Ignore
    @Test
    @Category(UnitTest.class)
    public void testUpdateWithDetail() throws Exception {
        Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
        ReviewBookmarks dto = new ReviewBookmarks();
        dto.setId(10L);
        dto.setMapId(1L);
        dto.setRelationId(2L);
        dto.setDetail("\"\"test1\"=>\"val1\", \"test2\"=>\"val2\"\"");
        dto.setCreatedAt(now);
        dto.setCreatedBy(-1L);

        JSONObject o = new JSONObject();
        o.put("test3", "val3");
        o.put("test4", "val4");
        ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 2, o, -2);
        Connection conn = null;
        ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver();

        //tagsSaver.getUpdateQuery(req, dto);

        String expected = "update \"review_bookmarks\"\n"
                + "set \"last_modified_by\" = ?, \"id\" = ?, \"created_by\" = ?, \"created_at\" = ?, \"detail\" = '\"test4\"=>\"val4\",\"test3\"=>\"val3\"', \"last_modified_at\" = ?\n"
                + "where \"review_bookmarks\".\"id\" = ?";

        Assert.assertTrue(expected.indexOf("\"last_modified_by\" = ?") > 0);
        Assert.assertTrue(expected.indexOf("\"id\" = ?") > 0);
        Assert.assertTrue(expected.indexOf("\"created_by\" = ?") > 0);
        Assert.assertTrue(expected.indexOf("\"created_at\" = ?") > 0);
        Assert.assertTrue(expected.indexOf("\"detail\" = '\"test4\"=>\"val4\",\"test3\"=>\"val3\"'") > 0);
        Assert.assertTrue(expected.indexOf("\"last_modified_at\" = ?") > 0);
    }
}
