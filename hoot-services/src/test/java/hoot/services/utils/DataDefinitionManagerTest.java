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
package hoot.services.utils;

import static hoot.services.HootProperties.DB_NAME;

import java.util.List;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class DataDefinitionManagerTest {
    @Test
    @Category(UnitTest.class)
    public void testDb() throws Exception {
        boolean exists = DataDefinitionManager.checkDbExists("wfdbtest");
        if (exists) {
            DataDefinitionManager.deleteDb("wfdbtest", true);
        }

        DataDefinitionManager.createDb("wfdbtest");
        exists = DataDefinitionManager.checkDbExists("wfdbtest");
        Assert.assertTrue(exists);
        DataDefinitionManager.deleteDb("wfdbtest", true);

        exists = DataDefinitionManager.checkDbExists("wfdbtest");
        Assert.assertTrue(!exists);
    }

    @Test
    @Category(UnitTest.class)
    public void testTable() throws Exception {
        boolean exists = DataDefinitionManager.checkDbExists("wfdbtest");
        if (exists) {
            DataDefinitionManager.deleteDb("wfdbtest", true);
        }

        DataDefinitionManager.createDb("wfdbtest");
        exists = DataDefinitionManager.checkDbExists("wfdbtest");
        Assert.assertTrue(exists);

        String createTblSql = "CREATE TABLE test_TABLE " + "(id INTEGER not NULL, " + " first VARCHAR(255), "
                + " last VARCHAR(255), " + " age INTEGER, " + " PRIMARY KEY ( id ))";
        DataDefinitionManager.createTable(createTblSql, "wfdbtest");

        List<String> tbls = DataDefinitionManager.getTablesList("wfdbtest", "test");
        Assert.assertTrue(!tbls.isEmpty());

        DataDefinitionManager.deleteTables(tbls, "wfdbtest");

        tbls = DataDefinitionManager.getTablesList("wfdbtest", "TEST");
        Assert.assertTrue(tbls.isEmpty());

        DataDefinitionManager.deleteDb("wfdbtest", true);

        exists = DataDefinitionManager.checkDbExists("wfdbtest");
        Assert.assertTrue(!exists);
    }

    @Test
    @Category(UnitTest.class)
    public void createMap() throws Exception {
        String dbname = DB_NAME;

        try {
            try {
                // just in case the tables exist.
                DbUtils.deleteMapRelatedTablesByMapId(1234);
            }
            catch (Exception ignored) {
                // exception can be currently thrown while trying to delete non-existent tables.
            }

            MapUtils.createMap(1234);

            List<String> tbls = DataDefinitionManager.getTablesList(dbname, "changesets");
            Assert.assertTrue(!tbls.isEmpty());

            tbls = DataDefinitionManager.getTablesList(dbname, "current_nodes");
            Assert.assertTrue(!tbls.isEmpty());

            tbls = DataDefinitionManager.getTablesList(dbname, "current_relation_members");
            Assert.assertTrue(!tbls.isEmpty());

            tbls = DataDefinitionManager.getTablesList(dbname, "current_relations");
            Assert.assertTrue(!tbls.isEmpty());

            tbls = DataDefinitionManager.getTablesList(dbname, "current_way_nodes");
            Assert.assertTrue(!tbls.isEmpty());

            tbls = DataDefinitionManager.getTablesList(dbname, "current_ways");
            Assert.assertTrue(!tbls.isEmpty());
        }
        finally {
            DbUtils.deleteMapRelatedTablesByMapId(1234);
        }
    }
}
