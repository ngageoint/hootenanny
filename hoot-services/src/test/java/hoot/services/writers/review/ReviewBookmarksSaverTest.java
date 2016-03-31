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
package hoot.services.writers.review;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.List;
import java.util.Random;

import org.json.simple.JSONObject;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.dml.SQLInsertClause;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkSaveRequest;
import hoot.services.readers.review.ReviewBookmarkRetriever;

public class ReviewBookmarksSaverTest {

  @Test
  @Category(UnitTest.class)
  public void testInsert() throws Exception
  {
    Connection conn = null;
    ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, null, -1);
    ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
    SQLInsertClause cl = tagsSaver._createInsertClause(req);
    String actual = cl.toString();

    String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n" +
        "values (?, ?, ?, ?, '')";

    org.junit.Assert.assertEquals(expected, actual);
  }

  @Test
  @Category(UnitTest.class)
  public void testInsertDetailWithConn() throws Exception
  {
    Connection conn = DbUtils.createConnection();
    JSONObject o = new JSONObject();
    o.put("test1", "val1");
    o.put("test2", "val2");
    ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(getMapId(conn), 1, o, -1);
    ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);

    long saved = tagsSaver.save(req);

    org.junit.Assert.assertEquals(1, saved);
  }

  @Test
  @Category(UnitTest.class)
  public void testInsertWithDetail() throws Exception
  {
    Connection conn = null;
    JSONObject o = new JSONObject();
    o.put("test1", "val1");
    o.put("test2", "val2");
    ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(1, 1, o, -1);
    ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
    SQLInsertClause cl = tagsSaver._createInsertClause(req);
    String actual = cl.toString();

    String expected = "insert into \"review_bookmarks\" (\"map_id\", \"relation_id\", \"created_at\", \"created_by\", \"detail\")\n" +
        "values (?, ?, ?, ?, '\"test1\"=>\"val1\",\"test2\"=>\"val2\"')";

    org.junit.Assert.assertEquals(expected, actual);
  }


  @Test
  @Category(UnitTest.class)
  public void testUpdateWithDetail() throws Exception
  {
    Connection conn = DbUtils.createConnection();
    long mapId = getMapId(conn);
    long generatedLong = new Random().nextLong();

    JSONObject o = new JSONObject();
    o.put("utest1", "val1");
    o.put("utest2", "val2");
    ReviewBookmarkSaveRequest req = new ReviewBookmarkSaveRequest(mapId, generatedLong, o, -1);
    ReviewBookmarksSaver tagsSaver = new ReviewBookmarksSaver(conn);
    tagsSaver.save(req);

    ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
    List<ReviewBookmarks> bookmarks = retriever.retrieve(mapId, generatedLong);

    o = new JSONObject();
    o.put("utest1", "new_val1");
    o.put("utest3", "val3");
    o.put("utest4", "val4");
    req = new ReviewBookmarkSaveRequest(getMapId(conn), generatedLong, o, -1);

    org.junit.Assert.assertEquals(1, bookmarks.size());
    org.junit.Assert.assertEquals(1, tagsSaver.update(req, bookmarks.get(0)));
  }

  protected long getMapId(Connection conn) throws Exception
  {
    long mapId = 0;
    Statement stmt = null;
    ResultSet rs = null;

    String sql = "select min(id) as id from maps";
    try
    {
      stmt = conn.createStatement();
      rs = stmt.executeQuery(sql);
      while (rs.next())
      {
        mapId = rs.getLong("id");
      }
    }
    finally
    {
      if (rs != null)
      {
        rs.close();
      }
      if (stmt != null)
      {
        stmt.close();
      }
    }
    return mapId;
  }
}
