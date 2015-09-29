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
			dbUserId = HootProperties.getProperty("dbName");
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
	
	
	protected final String  getDbName()
	{
		return _dbName;
	}
	
	protected final String getDbUserId()
	{
		return _dbUserId;
	}
	
	protected final String getDbPassword()
	{
		return _dbPassword;
	}
	
	protected final String getDbHost()
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
   * 
   * @param in
   * @param out
   * @return
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
