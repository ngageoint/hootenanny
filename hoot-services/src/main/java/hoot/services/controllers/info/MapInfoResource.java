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
package hoot.services.controllers.info;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.utils.ResourceErrorHandler;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Path("/map")
public class MapInfoResource {
	private static final Logger log = LoggerFactory.getLogger(MapInfoResource.class);
	protected static Long conflateThreshold = null;
	protected static Long ingestThreshold = null;
	protected static Long exportThreshold = null;
	
	
	public MapInfoResource ()
	{
		try
		{
			if(conflateThreshold == null)
			{
				String s = HootProperties.getProperty("conflateSizeThreshold");
				conflateThreshold = Long.parseLong(s);
			}
			
			if(ingestThreshold == null)
			{
				String s = HootProperties.getProperty("ingestSizeThreshold");
				ingestThreshold = Long.parseLong(s);
			}
			
			if(exportThreshold == null)
			{
				String s = HootProperties.getProperty("exportSizeThreshold");
				exportThreshold = Long.parseLong(s);
			}
		}
		catch (Exception ex)
		{
			log.error("Failed obtain map info config: " + ex.getMessage());
		}
	}
	
	/**
  * <NAME>Map Physical Size Information Service</NAME>
  * <DESCRIPTION>Service method endpoint for retrieving the physical size of a map record.</DESCRIPTION>
  * <PARAMETERS></PARAMETERS>
	* <OUTPUT>
	* 	JSON containing size information
	* </OUTPUT>
	* <EXAMPLE>
	* 	<URL>http://localhost:8080/hoot-services/info/map/size?mapid=4</URL>
	* 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	* <INPUT>None</INPUT>
  * <OUTPUT>
	* {
  * 	"mapid": "4","size_byte": 172032
  * } 
  * </OUTPUT>
  * </EXAMPLE>
  * 
  */
	@GET
  @Path("/size")
  @Produces(MediaType.TEXT_PLAIN)
  public Response getMapSize(@QueryParam("mapid") final String mapIds)
	{
		long nsize = 0;
		String[] maptables = {"changesets","current_nodes","current_relation_members",
				"current_relations","current_way_nodes","current_ways"};

		try
		{
			String[] mapids = mapIds.split(",");
			for(String mapId :  mapids)
			{
				for(String table : maptables)
				{
					nsize += DbUtils.getTableSizeInByte(table + "_" +  mapId);
				}
			}
			
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
					"Error getting map size: " + ex.toString(),
				    Status.INTERNAL_SERVER_ERROR,
					log);
		}
		JSONObject res = new JSONObject();
		res.put("mapid", mapIds);
		res.put("size_byte", nsize);
		/*res.put("conflate_threshold", conflateThreshold);
		res.put("ingest_threshold", ingestThreshold);
		res.put("export_threshold", exportThreshold);*/
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
	}
	
	
	/**
  * <NAME>Maximum Data Size Information Service</NAME>
  * <DESCRIPTION>Maximum data size for export conflate and ingest. </DESCRIPTION>
  * <PARAMETERS></PARAMETERS>
	* <OUTPUT>
	* 	JSON containing threshold information
	* </OUTPUT>
	* <EXAMPLE>
	* 	<URL>http://localhost:8080/hoot-services/info/map/thresholds</URL>
	* 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	* <INPUT>None</INPUT>
  * <OUTPUT>
	* {
  * 	"export_threshold": 400000000, "conflate_threshold": 200000000, "ingest_threshold": 200000000
  * } 
  * </OUTPUT>
  * </EXAMPLE>
  * 
  */	
	@GET
  @Path("/thresholds")
  @Produces(MediaType.TEXT_PLAIN)
  public Response getThresholds()
	{
		JSONObject res = new JSONObject();
		res.put("conflate_threshold", conflateThreshold);
		res.put("ingest_threshold", ingestThreshold);
		res.put("export_threshold", exportThreshold);
		return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
		
	}

}
