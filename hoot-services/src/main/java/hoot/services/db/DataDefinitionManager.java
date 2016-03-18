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
package hoot.services.db;

import hoot.services.HootProperties;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class DataDefinitionManager
{
	private static final Logger log = LoggerFactory.getLogger(DataDefinitionManager.class);
	private static final String POSTGRESQL_DRIVER = "org.postgresql.Driver";
	private String DB_URL = null;
	private String userid = null;
	private String pwd = null;

	private String db_name = null;

	public DataDefinitionManager() throws Exception
	{
		try
		{
			db_name = HootProperties.getProperty("dbName");
			userid = HootProperties.getProperty("dbUserId");
			pwd = HootProperties.getProperty("dbPassword");
			String host = HootProperties.getProperty("dbHost");
			DB_URL = "jdbc:postgresql://" + host + "/";
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
	}

	public boolean checkDbExists(String dbname) throws Exception
	{
		boolean exists = false;
		Connection conn = null;
		Statement stmt = null;
		ResultSet rs = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + db_name, userid, pwd);
			stmt = conn.createStatement();
			String sql = "SELECT 1 FROM pg_database WHERE datname = '" + dbname + "'";
			rs = stmt.executeQuery(sql);

			if (rs == null)
			{
				throw new Exception("Error executing checkDbExists");
			}
			
		  // STEP 5: Extract data from result set
			while (rs.next())
			{
				exists = true;
			}
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
		finally
		{
			if (rs != null) rs.close();
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
		return exists;
	}

	public void createDb(String dbname) throws Exception
	{
		Connection conn = null;
		Statement stmt = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + db_name, userid, pwd);

			stmt = conn.createStatement();
			String sql = "CREATE DATABASE \"" + dbname + "\"";
			stmt.executeUpdate(sql);
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
		finally
		{
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
	}

	public void deleteTables(List<String> tables, String dbname) throws Exception
	{
		Connection conn = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + dbname, userid, pwd);
			for (int i = 0; i < tables.size(); i++)
			{
				String tblName = tables.get(i);

				String sql = "DROP TABLE \"" + tblName + "\"";
				PreparedStatement stmt = null;
				try
				{
					stmt = conn.prepareStatement(sql);
					stmt.execute();
				}
				catch (Exception e)
				{
					log.error(e.getMessage());
				}
				finally
				{
				  if (stmt != null)
					{
						stmt.close();
					}
				}
			}
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
		}
		finally
		{
			if (conn != null) conn.close();
		}
	}

	public void deleteDb(String dbname, boolean force) throws Exception
	{
		Connection conn = null;
		Statement stmt = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + db_name, userid, pwd);
			stmt = conn.createStatement();
			if (force)
			{
				String forceSql = "select pg_terminate_backend(procpid) from pg_stat_activity where datname='"
						+ dbname + "'";
				stmt.executeQuery(forceSql);
			}
			String sql = "DROP DATABASE \"" + dbname + "\"";
			stmt.executeUpdate(sql);
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
		finally
		{
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
	}

	public List<String> getTablesList(String dbName, String filter_prefix) throws Exception
	{
		List<String> tblList = new ArrayList<String>();
		Connection conn = null;
		Statement stmt = null;
		ResultSet rs = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + dbName, userid, pwd);
			stmt = conn.createStatement();
			filter_prefix = filter_prefix.replace('-', '_');
			String sql = "SELECT table_name FROM information_schema.tables WHERE table_schema='public' AND table_name LIKE "
					+ "'" + filter_prefix + "_%'";
			rs = stmt.executeQuery(sql);
			
			if (rs == null)
			{
				throw new SQLException("Error executing getTablesListy");
			}
			
			// STEP 5: Extract data from result set
			while (rs.next())
			{
				// Retrieve by column name
				String tblName = rs.getString("table_name");
				tblList.add(tblName);
			}
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
		finally
		{
			if (rs != null)
			{
				rs.close();
			}
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
		return tblList;
	}

	public void createTable(String createTblSql, String dbname) throws Exception
	{
		Connection conn = null;
		Statement stmt = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);
			conn = DriverManager.getConnection(DB_URL + dbname, userid, pwd);
			stmt = conn.createStatement();

			stmt.executeUpdate(createTblSql);
		}
		catch (Exception e)
		{
			log.error(e.getMessage());
			throw e;
		}
		finally
		{
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
	}

}
