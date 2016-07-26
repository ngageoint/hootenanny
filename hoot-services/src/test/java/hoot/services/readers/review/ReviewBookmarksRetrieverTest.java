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
package hoot.services.readers.review;

import java.sql.Connection;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLQuery;

import hoot.services.UnitTest;


public class ReviewBookmarksRetrieverTest {

    @Test
    @Category(UnitTest.class)
    public void testRetrieve() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getQuery(1, 2);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"map_id\" = ? and \"review_bookmarks\".\"relation_id\" = ?";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAt() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("createdAt", true, -1, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtDesc() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("createdAt", false, -1, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"created_at\" desc";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByIdLimit100() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("id", true, 100, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"id\" asc\n" + "limit ?";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByIdLimit100Offset123() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("id", true, 100, 123, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"id\" asc\n" + "limit ?\n" + "offset ?";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithFilterByCreator() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("createdAt", true, -1, -1, new Long[]{(long) 1,(long) 2} ,null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"created_by\" in (?, ?)\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithMapIdFilter() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("createdAt", true, -1, -1, null, new Long[]{1L,2L});

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"map_id\" in (?, ?)\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        Assert.assertEquals(expected, actual);
    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithCreatorAndMapIdFilter() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r.getAllQuery("createdAt", true, -1, -1, new Long[]{1L,2L}, new Long[]{1L,2L});

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"created_by\" in (?, ?) and \"review_bookmarks\".\"map_id\" in (?, ?)\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        Assert.assertEquals(expected, actual);
    }
}
