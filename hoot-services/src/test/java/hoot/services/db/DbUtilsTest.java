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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.db;

import hoot.services.db.DbUtils;
import hoot.services.UnitTest;

import java.sql.Connection;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class DbUtilsTest {

    @Test
    @Category(UnitTest.class)
    public void testUpdateMapsTableTags() throws Exception
    {
        Connection conn = DbUtils.createConnection();
        long userId = 1;
        long mapId = DbUtils.insertMap(userId, conn);
        try {
            Map<String, String> tags = new HashMap<String, String>();
            String k1 = "input1";
            String k2 = "input2";
            String v1 = "layer1";
            String v2 = "layer2";
            tags.put(k1, v1);
            tags.put(k2, v2);
            // Test tag set
            long result = DbUtils.updateMapsTableTags(tags, mapId, conn);
            Assert.assertTrue(result > -1);
            Map<String, String> checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.get(k1).equals(v1));
            Assert.assertTrue(checkTags.get(k2).equals(v2));

            // Test tag append
            Map<String, String> tagsAppend = new HashMap<String, String>();
            String k3 = "stats";
            String v3 = "value";
            tagsAppend.put(k3, v3);
            result = DbUtils.updateMapsTableTags(tagsAppend, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k3));
            Assert.assertTrue(checkTags.get(k1).equals(v1));
            Assert.assertTrue(checkTags.get(k2).equals(v2));
            Assert.assertTrue(checkTags.get(k3).equals(v3));


            // Test tag update
            Map<String, String> tagsUpdate = new HashMap<String, String>();
            String k4 = "stats";
            String v4 = "change";
            tagsAppend.put(k4, v4);
            result = DbUtils.updateMapsTableTags(tagsAppend, mapId, conn);
            Assert.assertTrue(result > -1);
            checkTags = DbUtils.getMapsTableTags(mapId, conn);
            Assert.assertTrue(checkTags.containsKey(k1));
            Assert.assertTrue(checkTags.containsKey(k2));
            Assert.assertTrue(checkTags.containsKey(k4));
            Assert.assertTrue(checkTags.get(k1).equals(v1));
            Assert.assertTrue(checkTags.get(k2).equals(v2));
            Assert.assertTrue(checkTags.get(k4).equals(v4));


        // } catch (Exception ex) {
        //     throw(ex);
        } finally {
            DbUtils.deleteOSMRecord(conn, mapId);
        }
    }

}
