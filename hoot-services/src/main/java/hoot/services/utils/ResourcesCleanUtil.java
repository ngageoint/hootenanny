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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.utils;

import java.sql.Connection;

import hoot.services.db.DbUtils;
import hoot.services.job.Executable;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class ResourcesCleanUtil implements Executable {

	// remove data
	// remove file store
	private static final Logger log = LoggerFactory.getLogger(ResourcesCleanUtil.class);
  @SuppressWarnings("unused")
  private ClassPathXmlApplicationContext appContext;
	
	private String finalStatusDetail;
  public String getFinalStatusDetail() { return finalStatusDetail; }
	public ResourcesCleanUtil()
	{
		appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
	}
	
	public void exec(JSONObject command) throws Exception
	{
		JSONObject res = deleteLayers(command.get("mapId").toString());
		finalStatusDetail = res.toJSONString();
	}
	public JSONObject deleteLayers(final String mapId) throws Exception
	{
		JSONObject res = new JSONObject();
		res.put("mapId", mapId);
		res.put("result", "success");
	  Connection conn = DbUtils.createConnection();
	  try
	  {     
	    log.debug("Initializing database connection...");
	    
	    DbUtils.deleteOSMRecordByName(conn, mapId);
	  }
	  catch (Exception e)
	  {
	  	log.error(e.getMessage());
			throw e;
	  }
	  finally
	  {
	    DbUtils.closeConnection(conn);
	  }
	  
		return res;
	}
	
	/**
	 * see CoreServiceContext.xml
	 */
	public void init(){

	}

	/**
	 * see CoreServiceContext.xml
	 */
	public void destroy(){

	}
	
}
