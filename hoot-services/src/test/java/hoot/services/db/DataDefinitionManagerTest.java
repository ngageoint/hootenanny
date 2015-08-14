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

import java.util.ArrayList;
import java.util.List;

import hoot.services.HootProperties;
import hoot.services.UnitTest;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;


public class DataDefinitionManagerTest {
	@Test
	@Category(UnitTest.class)
	public void testDb() throws Exception
	{
		DataDefinitionManager spy = new DataDefinitionManager();
		boolean exists = spy.checkDbExists("wfdbtest");
		if(exists)
		{
			spy.deleteDb("wfdbtest", true);
		}

		spy.createDb("wfdbtest");
		exists = spy.checkDbExists("wfdbtest");
		Assert.assertTrue(exists);
		spy.deleteDb("wfdbtest", true);

		exists = spy.checkDbExists("wfdbtest");
		Assert.assertTrue(!exists);
	}

	@Test
	@Category(UnitTest.class)
	public void testTable() throws Exception
	{
		DataDefinitionManager spy = new DataDefinitionManager();
		boolean exists = spy.checkDbExists("wfdbtest");
		if(exists)
		{
			spy.deleteDb("wfdbtest", true);
		}

		spy.createDb("wfdbtest");
		exists = spy.checkDbExists("wfdbtest");
		Assert.assertTrue(exists);

		String createTblSql = "CREATE TABLE test_TABLE " +
        "(id INTEGER not NULL, " +
        " first VARCHAR(255), " +
        " last VARCHAR(255), " +
        " age INTEGER, " +
        " PRIMARY KEY ( id ))";
		spy.createTable(createTblSql, "wfdbtest");

		List<String> tbls = spy.getTablesList("wfdbtest", "test");
		Assert.assertTrue(tbls.size() > 0);

		spy.deleteTables(tbls, "wfdbtest");

		tbls = spy.getTablesList("wfdbtest", "TEST");
		Assert.assertTrue(tbls.size() == 0);

		spy.deleteDb("wfdbtest", true);

		exists = spy.checkDbExists("wfdbtest");
		Assert.assertTrue(!exists);
	}


	//TODO: This test assumes a map with ID = 1234 doesn't exit, which is no longer true when
	//running the tests.  The test needs to be updated.
	@Ignore
	@Test
	@Category(UnitTest.class)
	public void createMap() throws Exception
	{
		String dbname = HootProperties.getProperty("dbName");
		DataDefinitionManager spy = new DataDefinitionManager();
		List<String> tables = new ArrayList<String>();
		tables.add("changesets_1234");
		tables.add("current_nodes_1234");
		tables.add("current_relation_members_1234");
		tables.add("current_relations_1234");
		tables.add("current_way_nodes_1234");
		tables.add("current_ways_1234");
		spy.deleteTables(tables, dbname);

		DbUtils.createMap(1234);

		List<String> tbls = spy.getTablesList(dbname, "changesets");
		Assert.assertTrue(tbls.size() > 0);

		tbls = spy.getTablesList(dbname, "current_nodes");
		Assert.assertTrue(tbls.size() > 0);

		tbls = spy.getTablesList(dbname, "current_relation_members");
		Assert.assertTrue(tbls.size() > 0);

		tbls = spy.getTablesList(dbname, "current_relations");
		Assert.assertTrue(tbls.size() > 0);

		tbls = spy.getTablesList(dbname, "current_way_nodes");
		Assert.assertTrue(tbls.size() > 0);

		tbls = spy.getTablesList(dbname, "current_ways");
		Assert.assertTrue(tbls.size() > 0);

	}
}
