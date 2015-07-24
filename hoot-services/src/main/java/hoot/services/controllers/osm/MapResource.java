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
package hoot.services.controllers.osm;

import hoot.services.db.DbUtils;
import hoot.services.db2.Maps;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobExecutioner;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayers;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.writers.osm.MapQueryResponseWriter;

import java.io.StringWriter;
import java.io.Writer;
import java.net.SocketException;
import java.sql.Connection;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.UUID;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;

/**
 * Service endpoint for maps containing OSM data
 */
@Path("/api/0.6/map")
public class MapResource
{
  private static final Logger log = LoggerFactory.getLogger(MapResource.class);

  @SuppressWarnings("unused")
  private ClassPathXmlApplicationContext appContext;

  public MapResource()
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
  }

	/**
	 * <NAME>Map Service - List Layers </NAME>
	 * <DESCRIPTION>
	 * 	Returns a list of all map layers in the services database
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	a JSON object containing a list of map layers
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/map/layers</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  {
	 *    "layers":
	 *    [
	 *      {
	 *        "id": 1,
 	 *       "name": "layer 1",
	 *      },
	 *      {
	 *        "id": 2,
	 *        "name": "layer 2",
	 *      }
	 *    ]
	 *  }
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a list of all map layers in the services database
   *
   * @return a JSON object containing a list of map layers
   * @throws Exception
   * @see https
   *      ://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/User_
   *      -_OsmMapService#List-Layers
   */
  @GET
  @Path("/layers")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public MapLayers getLayers() throws Exception
  {
    Connection conn = DbUtils.createConnection();
    MapLayers mapLayers = null;
    try
    {
      log.info("Retrieving map layers list...");

      log.debug("Initializing database connection...");

      QMaps maps = QMaps.maps;
      SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

      final List<Maps> mapLayerRecords = query.from(maps).orderBy(maps.displayName.asc()).list(maps);

      mapLayers = Map.mapLayerRecordsToLayers(mapLayerRecords);
    }
    catch (Exception e)
    {
      handleError(e, null, null);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    String message = "Returning map layers response";
    if (mapLayers != null && mapLayers.getLayers() != null)
    {
      message += " of size: " + mapLayers.getLayers().length;
    }
    log.debug(message);
    return mapLayers;
  }

  private Document _generateExtentOSM(String maxlon, String maxlat, String minlon, String minlat)
    throws Exception
  {
    SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss'Z'");
    Date now = new Date();
    String strDate = sdfDate.format(now);

    DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
    dbf.setValidating(false);
    DocumentBuilder db = dbf.newDocumentBuilder();
    Document doc = db.newDocument();

    Element osmElem = doc.createElement("osm");
    osmElem.setAttribute("version", "0.6");
    osmElem.setAttribute("generator", "hootenanny");
    doc.appendChild(osmElem);

    Element boundsElem = doc.createElement("bounds");
    boundsElem.setAttribute("minlat", minlat);
    boundsElem.setAttribute("minlon", minlon);
    boundsElem.setAttribute("maxlat", maxlat);
    boundsElem.setAttribute("maxlon", maxlon);
    osmElem.appendChild(boundsElem);

    Element nodeElem = doc.createElement("node");
    nodeElem.setAttribute("id", "0");
    nodeElem.setAttribute("timestamp", strDate);
    nodeElem.setAttribute("user", "hootenannyuser");
    nodeElem.setAttribute("visible", "true");
    nodeElem.setAttribute("version", "1");
    nodeElem.setAttribute("lat", maxlat);
    nodeElem.setAttribute("lon", minlon);
    osmElem.appendChild(nodeElem);

    nodeElem = doc.createElement("node");
    nodeElem.setAttribute("id", "1");
    nodeElem.setAttribute("timestamp", strDate);
    nodeElem.setAttribute("user", "hootenannyuser");
    nodeElem.setAttribute("visible", "true");
    nodeElem.setAttribute("version", "1");
    nodeElem.setAttribute("lat", maxlat);
    nodeElem.setAttribute("lon", maxlon);
    osmElem.appendChild(nodeElem);

    nodeElem = doc.createElement("node");
    nodeElem.setAttribute("id", "2");
    nodeElem.setAttribute("timestamp", strDate);
    nodeElem.setAttribute("user", "hootenannyuser");
    nodeElem.setAttribute("visible", "true");
    nodeElem.setAttribute("version", "1");
    nodeElem.setAttribute("lat", minlat);
    nodeElem.setAttribute("lon", maxlon);
    osmElem.appendChild(nodeElem);

    nodeElem = doc.createElement("node");
    nodeElem.setAttribute("id", "3");
    nodeElem.setAttribute("timestamp", strDate);
    nodeElem.setAttribute("user", "hootenannyuser");
    nodeElem.setAttribute("visible", "true");
    nodeElem.setAttribute("version", "1");
    nodeElem.setAttribute("lat", minlat);
    nodeElem.setAttribute("lon", minlon);
    osmElem.appendChild(nodeElem);

    Element wayElem = doc.createElement("way");
    wayElem.setAttribute("id", "0");
    wayElem.setAttribute("timestamp", strDate);
    wayElem.setAttribute("user", "hootenannyuser");
    wayElem.setAttribute("visible", "true");
    wayElem.setAttribute("version", "1");

    Element ndElem = doc.createElement("nd");
    ndElem.setAttribute("ref", "0");
    wayElem.appendChild(ndElem);

    ndElem = doc.createElement("nd");
    ndElem.setAttribute("ref", "1");
    wayElem.appendChild(ndElem);

    ndElem = doc.createElement("nd");
    ndElem.setAttribute("ref", "2");
    wayElem.appendChild(ndElem);

    ndElem = doc.createElement("nd");
    ndElem.setAttribute("ref", "3");
    wayElem.appendChild(ndElem);

    ndElem = doc.createElement("nd");
    ndElem.setAttribute("ref", "0");
    wayElem.appendChild(ndElem);

    /*
     * ndElem = doc.createElement("tag"); ndElem.setAttribute("k", "area");
     * ndElem.setAttribute("v", "yes"); wayElem.appendChild(ndElem);
     */

    osmElem.appendChild(wayElem);

    Transformer tf = TransformerFactory.newInstance().newTransformer();
    tf.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
    tf.setOutputProperty(OutputKeys.INDENT, "yes");
    Writer out = new StringWriter();
    tf.transform(new DOMSource(doc), new StreamResult(out));

    // System.out.println(out.toString());
    log.debug("Layer Extent OSM: " + out.toString());

    return doc;
  }

	/**
	 * <NAME>Map Service - Query </NAME>
	 * <DESCRIPTION>
	 * 	The Hootenanny Map Service is similar to the query portion of the OSM Map Service design,
	 *  except that a map ID must also be specified when using it. The Map service first attempts to parse
	 *  the request map identifier as a numerical user ID, and then if unsuccessful, attempts to parse it as a map name string.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <mapId>
	 *  string; ID or name of the map to query
	 *  </mapId>
	 *  <bbox>
	 *  string; geographic bounding box to restrict the map query to of the form: minimum longitude, minimum latitude,
	 *  maximum longitude, maximum latitude; in WGS84 degrees
	 *  </bbox>
	 *  <multiLayerUniqueElementIds>
	 *  boolean; if true, returned element ID's are prepended with [map id]_[first letter of the element type]_
	 *  </multiLayerUniqueElementIds>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of each element which satisfied the map query
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/map?
	 * mapId=dc-admin&bbox=-77.09655761718749,38.89958342598271,-77.09106445312499,38.90385833966776&multiLayerUniqueElementIds=false</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * OSM XML
	 * see https
   *      ://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/User_
   *      -_OsmMapService#Query
	 * </OUTPUT>
	 * </EXAMPLE>
   * Service method endpoint for retrieving OSM entity data for a given map
   *
   * @param mapId
   *          ID of the map to query
   * @param bbox
   *          geographic bounding box the requested entities should reside in
   * @param multiLayerUniqueElementIds
   *          if true, returned element ID's are prepended with <map id>_<first
   *          letter of the element type>_; this setting activated is not
   *          compatible with standard OSM clients (specific to Hootenanny iD);
   *          defaults to false
   * @return response containing the data of the requested elements
   * @throws Exception
   * @see https
   *      ://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/User_
   *      -_OsmMapService#Query
   */
  @GET
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.TEXT_XML)
  public Response get(
      @QueryParam("mapId") final String mapId,
      @QueryParam("bbox") final String BBox,
      @QueryParam("extent") final String extent,
      @QueryParam("autoextent") final String auto,
      @DefaultValue("false") @QueryParam("multiLayerUniqueElementIds") final boolean multiLayerUniqueElementIds)
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    Document responseDoc = null;
    try
    {
      log.info("Retrieving map data for map with ID: " + mapId + " and bounds " + BBox + " ...");

      log.debug("Initializing database connection...");
      String bbox = BBox;
      String[] Coords = bbox.split(",");
      if (Coords.length == 4)
      {
        String sMinX = Coords[0];
        String sMinY = Coords[1];
        String sMaxX = Coords[2];
        String sMaxY = Coords[3];

        double minX = Double.parseDouble(sMinX);
        double minY = Double.parseDouble(sMinY);
        double maxX = Double.parseDouble(sMaxX);
        double maxY = Double.parseDouble(sMaxY);

        minX = minX > 180 ? 180 : minX;
        minX = minX < -180 ? -180 : minX;

        maxX = maxX > 180 ? 180 : maxX;
        maxX = maxX < -180 ? -180 : maxX;

        minY = minY > 90 ? 90 : minY;
        minY = minY < -90 ? -90 : minY;

        maxY = maxY > 90 ? 90 : maxY;
        maxY = maxY < -90 ? -90 : maxY;

        bbox = "" + minX + "," + minY + "," + maxX + "," + maxY;

      }

      QMaps maps = QMaps.maps;
      long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id,
          maps.displayName);
      assert (mapIdNum != -1);

      BoundingBox queryBounds = null;
      try
      {
        queryBounds = new BoundingBox(bbox);
        log.debug("Query bounds area: " + queryBounds.getArea());
      }
      catch (Exception e)
      {
        throw new Exception("Error parsing bounding box from bbox param: " + bbox + " ("
            + e.getMessage() + ")");
      }

      boolean doDefault = true;
      if (auto != null && extent != null)
      {

        if (auto.equalsIgnoreCase("manual"))
        {
          if (extent.length() > 0)
          {
            String[] coords = extent.split(",");
            if (coords.length == 4)
            {
              String maxlon = coords[0].trim();
              String maxlat = coords[1].trim();
              String minlon = coords[2].trim();
              String minlat = coords[3].trim();
              responseDoc = _generateExtentOSM(maxlon, maxlat, minlon, minlat);
              doDefault = false;
            }
          }

        }
      }

      if (doDefault)
      {
        final java.util.Map<ElementType, java.util.Map<Long, Tuple>> results = (new Map(mapIdNum,
            conn)).query(queryBounds);

        responseDoc = (new MapQueryResponseWriter(mapIdNum, conn)).writeResponse(results,
            queryBounds, multiLayerUniqueElementIds);

      }

    }
    catch (Exception e)
    {
      handleError(e, mapId, BBox);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }

    return Response.ok(new DOMSource(responseDoc), MediaType.TEXT_XML)
        .header("Content-type", MediaType.TEXT_XML)
        // TODO: what's the point of setting this header?...taken directly from
        // the rails port code
        .header("Content-Disposition", "attachment; filename=\"map.osm\"").build();
  }

  @POST
  @Path("/nodescount")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public Response getTileNodesCounts(String params) throws Exception
  {
    Connection conn = DbUtils.createConnection();
    JSONObject ret = new JSONObject();
    String mapId = "";
    String bbox = "";
    long nodeCnt = 0;
    try
    {
      log.info("Retrieving map data for map with ID: " + mapId + " ...");

      log.debug("Initializing database connection...");
      JSONParser parser = new JSONParser();
      JSONArray paramsArray = (JSONArray) parser.parse(params);

      for (int i = 0; i < paramsArray.size(); i++)
      {
        JSONObject param = (JSONObject) paramsArray.get(i);
        mapId = (String) param.get("mapId");
        bbox = (String) param.get("tile");
        String[] coords = bbox.split(",");
        if (coords.length == 4)
        {
          String sMinX = coords[0];
          String sMinY = coords[1];
          String sMaxX = coords[2];
          String sMaxY = coords[3];

          double minX = Double.parseDouble(sMinX);
          double minY = Double.parseDouble(sMinY);
          double maxX = Double.parseDouble(sMaxX);
          double maxY = Double.parseDouble(sMaxY);

          minX = minX > 180 ? 180 : minX;
          minX = minX < -180 ? -180 : minX;

          maxX = maxX > 180 ? 180 : maxX;
          maxX = maxX < -180 ? -180 : maxX;

          minY = minY > 90 ? 90 : minY;
          minY = minY < -90 ? -90 : minY;

          maxY = maxY > 90 ? 90 : maxY;
          maxY = maxY < -90 ? -90 : maxY;

          bbox = "" + minX + "," + minY + "," + maxX + "," + maxY;

        }

        QMaps maps = QMaps.maps;
        long mapIdNum =
          ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id, maps.displayName);
        assert (mapIdNum != -1);

        BoundingBox queryBounds = null;
        try
        {
          queryBounds = new BoundingBox(bbox);
          log.debug("Query bounds area: " + queryBounds.getArea());
        }
        catch (Exception e)
        {
          throw new Exception("Error parsing bounding box from bbox param: " + bbox + " ("
              + e.getMessage() + ")");
        }
        Map currMap = new Map(mapIdNum, conn);
        nodeCnt += currMap.getNodesCount(queryBounds);

      }

      ret.put("nodescount", nodeCnt);
    }
    catch (Exception e)
    {
      handleError(e, mapId, bbox);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }

    return Response.ok(ret.toString(), MediaType.APPLICATION_JSON).build();
  }

  @GET
  @Path("/mbr")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public Response getMBR(@QueryParam("mapId") final String mapId) throws Exception
  {
    Connection conn = DbUtils.createConnection();
    JSONObject ret = new JSONObject();
    try
    {
      log.info("Retrieving map data for map with ID: " + mapId + " ...");

      log.debug("Initializing database connection...");

      QMaps maps = QMaps.maps;
      long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn, maps, maps.id,
          maps.displayName);
      assert (mapIdNum != -1);

      BoundingBox queryBounds = null;
      try
      {
        queryBounds = new BoundingBox("-180,-90,180,90");
        log.debug("Query bounds area: " + queryBounds.getArea());
      }
      catch (Exception e)
      {
        throw new Exception("Error parsing bounding box from bbox param: " + "-180,-90,180,90"
            + " (" + e.getMessage() + ")");
      }

      Map currMap = new Map(mapIdNum, conn);
      final JSONObject extents = currMap.retrieveNodesMBR(queryBounds);

      final JSONObject anode = currMap.retrieveANode(queryBounds);
      long nodeCnt = currMap.getNodesCount(queryBounds);

      double dMinLon = DbUtils.fromDbCoordValue((Long) extents.get("minlon"));
      double dMaxLon = DbUtils.fromDbCoordValue((Long) extents.get("maxlon"));
      double dMinLat = DbUtils.fromDbCoordValue((Long) extents.get("minlat"));
      double dMaxLat = DbUtils.fromDbCoordValue((Long) extents.get("maxlat"));

      double dFirstLon = DbUtils.fromDbCoordValue((Long) anode.get("lon"));
      double dFirstLat = DbUtils.fromDbCoordValue((Long) anode.get("lat"));

      ret.put("minlon", dMinLon);
      ret.put("maxlon", dMaxLon);
      ret.put("minlat", dMinLat);
      ret.put("maxlat", dMaxLat);
      ret.put("firstlon", dFirstLon);
      ret.put("firstlat", dFirstLat);
      ret.put("nodescount", nodeCnt);
    }
    catch (Exception e)
    {
      handleError(e, mapId, "-180,-90,180,90");
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }

    return Response.ok(ret.toString(), MediaType.APPLICATION_JSON).build();
  }

  private void handleError(final Exception e, final String mapId, final String requestSnippet)
  {
    if (e instanceof SocketException && e.getMessage().toLowerCase().contains("broken pipe"))
    {
      //This occurs when iD aborts a tile request before it is finished.  This happens quite
      //frequently but is acceptable, so let's catch this and just log as debug rather than an
      //error to make the logs cleaner.
      log.debug(e.getMessage());
    }
    else if (e.getMessage().startsWith("Multiple records exist"))
    {
      ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"),
          Status.NOT_FOUND, log);
    }
    else if (e.getMessage().startsWith("No record exists"))
    {
      ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"),
          Status.NOT_FOUND, log);
    }
    else if (e.getMessage().startsWith("Error parsing bounding box from bbox param"))
    {
      ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
    }
    else if (e.getMessage().contains("The maximum bbox size is"))
    {
      ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
    }
    else if (e.getMessage().contains(
        "The maximum number of nodes that may be returned in a map query"))
    {
      ResourceErrorHandler.handleError(e.getMessage(), Status.BAD_REQUEST, log);
    }
    else
    {
      if (mapId != null)
      {
        ResourceErrorHandler.handleError(
            "Error querying map with ID: " + mapId + " - data: (" + e.getMessage() + ") "
                + requestSnippet, Status.INTERNAL_SERVER_ERROR, log);
      }
      else
      {
        ResourceErrorHandler.handleError("Error listing layers for map - data: (" + e.getMessage()
            + ") " + requestSnippet, Status.INTERNAL_SERVER_ERROR, log);
      }
    }
  }

  /**
	 * <NAME>Clean Map Data Service</NAME>
	 * <DESCRIPTION>
	 * Clean map data service provides the ability to remove map associated record.
	 *  It removes from CURRENT_WAY_NODES,CURRENT_RELATION_MEMBERS,CURRENT_NODES,CURRENT_WAYS,
	 *  CURRENT_RELATIONS, CHANGESET_TAGS,CHANGESETS,MAPS,REVIEW_ITEMS,ELEMENT_ID_MAPPINGS and REVIEW_MAP.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapId>
	 * 	ID of map record to be deleted
	 * http://localhost:8080/hoot-services/osm/api/0.6/map/delete?mapId={Map ID}
	 * </mapId>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	Job ID
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/map/delete?mapId=123456</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>{"jobId": "b9462277-73bc-41ea-94ec-c7819137b00b" }</OUTPUT>
	 * </EXAMPLE>
   * @param mapId
   * @return
   * @throws Exception
   */
  @POST
  @Path("/delete")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.TEXT_PLAIN)
  public Response deleteLayers(@QueryParam("mapId") final String mapId) throws Exception
  {
    JSONObject command = new JSONObject();
    command.put("mapId", mapId);
    command.put("execImpl", "ResourcesCleanUtil");

    final String jobId = UUID.randomUUID().toString();

    (new JobExecutioner(jobId, command)).start();

    JSONObject res = new JSONObject();
    res.put("jobId", jobId);
    return Response.ok(res.toJSONString(), MediaType.APPLICATION_JSON).build();
  }
}
