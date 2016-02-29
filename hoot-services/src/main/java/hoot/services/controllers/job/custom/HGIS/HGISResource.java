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
package hoot.services.controllers.job.custom.HGIS;

import java.io.IOException;
import java.sql.Connection;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.controllers.job.JobControllerBase;
import hoot.services.db.DbUtils;
import hoot.services.validators.osm.MapValidator;

/**
 * Base code for HGIS specific Rest endpoint.
 * 
 *
 */
public class HGISResource extends JobControllerBase
{
	static private final String _dbName;
	static private final String _dbUserId;
	static private final String _dbPassword;
	static private final String _dbHost;
	
	private static final Logger log = LoggerFactory.getLogger(HGISResource.class);

	// Load just once during class load
	static
	{
		String dbName = null;
		String dbUserId = null;
		String dbPassword = null;
		String dbHost = null;
		try {
			dbName = HootProperties.getProperty("dbName");
			dbUserId = HootProperties.getProperty("dbUserId");
			dbPassword = HootProperties.getProperty("dbPassword");
			dbHost = HootProperties.getProperty("dbHost");
    } catch (IOException e) {
    	log.error("failed to retrieve parameter:" + e.getMessage());
    }
		
		_dbName = dbName;
		_dbUserId = dbUserId;
		_dbPassword = dbPassword;
		_dbHost = dbHost;
	}
	
	
	protected final static String  getDbName()
	{
		return _dbName;
	}
	
	protected final static String getDbUserId()
	{
		return _dbUserId;
	}
	
	protected final static String getDbPassword()
	{
		return _dbPassword;
	}
	
	protected final static String getDbHost()
	{
		return _dbHost;
	}
	
  /**
   * Checks for the existence of map
   * 
   * @param mapName
   * @return returns true when exists else false
   * @throws Exception
   */
  protected boolean _mapExists(final String mapName) throws Exception
  {
  	boolean exists = false;
  	try(Connection conn = DbUtils.createConnection())
  	{
  		MapValidator validator = new MapValidator(conn);
  		exists = (validator.verifyMapExists(mapName) > -1);
  		
  	}
  	catch(Exception ex)
  	{
  		throw ex;
  	}
  	
  	
  	return exists;
  }
  

  /**
   * Creates db conection string based on config settings in hoot-services.conf
   * 
   * 
   * @param mapName
   * @return output looks like postgresql://hoot:hoottest@localhost:5432/hoot1/BrazilOsmPois
   */
  protected final String _generateDbMapParam(final String mapName)
  {
  	return "postgresql://" + getDbUserId() + ":" + getDbPassword() + "@" + getDbHost() + "/" + getDbName() + "/" + mapName;
  }
  

  /**
   * Default parameter creator. Child class probably will override this to meet its need.
   * TODO: We would need to figure out the transport object and replace jsonarray with it
   */
  protected final JSONArray _createParamObj(final String in, final String out)
  {
  	JSONArray commandArgs = new JSONArray();

		JSONObject arg = new JSONObject();
		arg.put("SOURCE", _generateDbMapParam(in));
		commandArgs.add(arg);


		arg = new JSONObject();
		arg.put("OUTPUT", _generateDbMapParam(out));
		commandArgs.add(arg);
		
  	return commandArgs;
  }
}
