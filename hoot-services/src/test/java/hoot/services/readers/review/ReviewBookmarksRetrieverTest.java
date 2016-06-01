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
        SQLQuery q = r._getQuery(1, 2);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"map_id\" = ? and \"review_bookmarks\".\"relation_id\" = ?";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAt() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("createdAt", true, -1, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtDesc() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("createdAt", false, -1, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"created_at\" desc";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByIdLimit100() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("id", true, 100, -1, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"id\" asc\n" + "limit ?";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByIdLimit100Offset123() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("id", true, 100, 123, null, null);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"id\" asc\n" + "limit ?\n" + "offset ?";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithFilterBy() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("createdAt", true, -1, -1, "createdBy", (long) 10);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"created_by\" = ?\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithFilterByNonExistCol() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("createdAt", true, -1, -1, "createdByMe", (long) 10);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "order by \"review_bookmarks\".\"created_at\" asc";

        org.junit.Assert.assertEquals(expected, actual);

    }

    @Test
    @Category(UnitTest.class)
    public void testRetrieveAllOrderByCreatedAtWithMapIdFilter() throws Exception {
        Connection conn = null;

        ReviewBookmarkRetriever r = new ReviewBookmarkRetriever(conn);
        SQLQuery q = r._getAllQuery("createdAt", true, -1, -1, "mapId", (long) 10);

        String actual = q.toString();
        String expected = "from \"review_bookmarks\" \"review_bookmarks\"\n"
                + "where \"review_bookmarks\".\"map_id\" = ?\n" + "order by \"review_bookmarks\".\"created_at\" asc";

        org.junit.Assert.assertEquals(expected, actual);

    }

}
