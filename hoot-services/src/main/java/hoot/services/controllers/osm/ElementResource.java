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

import java.sql.Connection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.regex.Pattern;

import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.OsmResponseHeaderGenerator;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.transform.dom.DOMSource;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.w3c.dom.Document;

import com.mysema.query.Tuple;

/**
 * Service endpoint for retrieving elements by ID
 */
@Path("/api/0.6")
public class ElementResource
{
  private static final Logger log = LoggerFactory.getLogger(ElementResource.class);
  
  //<map id>_<first letter of the element type>_<element id>
  private static final String UNIQUE_ELEMENT_ID_REGEX = "\\d+_(n|w|r)_\\d+";
  private static final Pattern UNIQUE_ELEMENT_ID_PATTERN = Pattern.compile(UNIQUE_ELEMENT_ID_REGEX);
  
  @SuppressWarnings("unused")
  private ClassPathXmlApplicationContext appContext;
  
  public ElementResource()
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
  }
  
  /**
	 * <NAME>Element Service - Get Element By ID </NAME>
	 * <DESCRIPTION>
	 * 	Allows for retrieving a node, way, or relation by numeric OSM element ID. Child element of ways
	 * and relations are not added to the output (use the "full" method for that functionality).
	 * The ID of the map owning the element must be specified in the query string.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <mapId>
	 *  string; ID or name of the map the requested element belongs to
	 *  </mapId>
	 *  <elementId>
	 *   long; OSM ID of the requested element
	 *  </elementId>
	 *  <elementType>
	 *  string; OSM type of the requested element; valid values are "node", "way", or "relation"
	 *  </elementType>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the requested element
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/node/1?mapId=1</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  OSM XML
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a single element item's XML for a given map without its element children
   * 
   * @param mapId ID of the map the element belongs to
   * @param elementId OSM element ID of the element to retrieve
   * @param elementType OSM element type of the element to retrieve; valid values are: node, way, 
   * or relation
   * @return element XML document
   * @throws Exception
   */
  @GET
  @Path("{elementType: node|way|relation}/{elementId}")
  @Consumes({MediaType.TEXT_PLAIN})
  @Produces({MediaType.TEXT_XML})
  public Response getElement(
    @QueryParam("mapId")
    String mapId,
    @PathParam("elementId") 
    final long elementId,
    @PathParam("elementType") 
    final String elementType) 
    throws Exception
  {
    final ElementType elementTypeVal = Element.elementTypeFromString(elementType);
    if (elementTypeVal == null)
    {
      ResourceErrorHandler.handleError(
        "Invalid element type: " + elementType, Status.BAD_REQUEST, log); 
    }
    
    Connection conn = DbUtils.createConnection();
    Document elementDoc = null;
    try
    {
      log.debug("Initializing database connection...");
      
      elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, false, conn);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    
    log.debug("Returning response: " + 
      StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
    return 
      Response
        .ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
        .header("Content-type", MediaType.APPLICATION_XML)
        .build();
  }
  
  /**
	 * <NAME>Element Service - Get Element By ID Get Element By Unique ID </NAME>
	 * <DESCRIPTION>
	 * 	Convenience method which allows for retrieving a node, way, or relation by an OSM unique element ID.
	 * Child element of ways and relations are not added to the output (use the "full" method for that functionality). The ID of
	 * the map owning the element does not need to be specified in the query string because the information already exists in the
	 * element ID. This method is not part of the OSM API.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <elementId>
	 *   long; OSM ID of the requested element
	 *  </elementId>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the requested element
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/element/1_n_1</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  OSM XML
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a single element item's XML for a given map without its element children
   * 
   * @param elementId a "hoot" formatted element id of the format: 
   * <map id>_<first letter of the element type>_<element id>; valid element types include: node, 
   * way, or relation
   * @return element XML document
   * @throws Exception
   */
  @GET
  @Path("/element/{elementId}")
  @Consumes({MediaType.TEXT_PLAIN})
  @Produces({MediaType.TEXT_XML})
  public Response getElementByUniqueId(
    @PathParam("elementId") 
    final String elementId) 
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    Document elementDoc = null;
    try
    {
      log.debug("Initializing database connection...");
      
      if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches())
      {
        ResourceErrorHandler.handleError(
          "Invalid element ID: " + elementId, Status.BAD_REQUEST, log);
      }
      final String[] elementIdParts = elementId.split("_");
      final ElementType elementTypeVal = Element.elementTypeFromString(elementIdParts[1]);
      if (elementTypeVal == null)
      {
        ResourceErrorHandler.handleError(
          "Invalid element type: " + elementIdParts[1], Status.BAD_REQUEST, log); 
      }
      elementDoc = 
        getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementTypeVal, 
          true, false, conn);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    
    log.debug("Returning response: " + 
      StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
    return 
      Response
        .ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
        .header("Content-type", MediaType.APPLICATION_XML)
        .build();
  }
  
  /**
	 * <NAME>Element Service - Get Full Element By ID </NAME>
	 * <DESCRIPTION>
	 * 	Convenience method which allows for retrieving a way or relation and all of its child elements
	 *  (way nodes or relation members) by numeric OSM element ID. The ID of the map owning the element
	 *   must be specified in the query string.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <mapId>
	 *  string; ID or name of the map the requested element belongs to
	 *  </mapId>
	 *  <elementId>
	 *   long; OSM ID of the requested element
	 *  </elementId>
	 *  <elementType>
	 *  string; OSM type of the requested element; valid values are "node", "way", or "relation"
	 *  </elementType>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the requested element
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/node/1/full?mapId=1</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  OSM XML
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a single element item's XML for a given map with all of its element children
   * 
   * @param mapId ID of the map the element belongs to
   * @param elementId OSM element ID of the element to retrieve
   * @param elementType OSM element type of the element to retrieve; valid values are: way
   * or relation
   * @return element XML document
   * @throws Exception
   */
  @GET
  @Path("/{elementType: way|relation}/{elementId}/full")
  @Consumes({MediaType.TEXT_PLAIN})
  @Produces({MediaType.TEXT_XML})
  public Response getFullElement(
    @QueryParam("mapId")
    String mapId,
    @PathParam("elementId") 
    final long elementId,
    @PathParam("elementType") 
    final String elementType) 
    throws Exception
  {
    final ElementType elementTypeVal = Element.elementTypeFromString(elementType);
    if (elementTypeVal == null)
    {
      ResourceErrorHandler.handleError(
        "Invalid element type: " + elementType, Status.BAD_REQUEST, log); 
    }
    
    Connection conn = DbUtils.createConnection();
    Document elementDoc = null;
    try
    {
      log.debug("Initializing database connection...");
      
      elementDoc = getElementXml(mapId, elementId, elementTypeVal, false, true, conn);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    
    log.debug("Returning response: " + 
      StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
    return 
      Response
        .ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
        .header("Content-type", MediaType.APPLICATION_XML)
        .build();
  }
  
  /**
	 * <NAME>Element Service - Get Full Element By Unique ID </NAME>
	 * <DESCRIPTION>
	 * 	Convenience method which allows for retrieving a way or relation and all of its child
	 * elements (way nodes or relation members) by an OSM unique element ID. The ID of the map owning
	 * the element does not need to be specified in the query string because the information already exists
	 * in the element ID. This method is not part of the OSM API.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <elementId>
	 *   string; ID for the requested element unique across all maps of the form:
	 *   <map id>_<first letter of the element type>_<element id>; valid values for
	 *    <first letter of the element type> are: "w" or "r"
	 *  </elementId>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the requested element
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/element/1_n_1/full</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  OSM XML
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a single element item's XML for a given map with all of its element children
   * 
   * @param elementId a "hoot" formatted element id of the format: 
   * <map id>_<first letter of the element type>_<element id>; valid element types include: 
   * way or relation
   * @return element XML document
   * @throws Exception
   */
  @GET
  @Path("/element/{elementId}/full")
  @Consumes({MediaType.TEXT_PLAIN})
  @Produces({MediaType.TEXT_XML})
  public Response getFullElementByUniqueId(
    @PathParam("elementId") 
    final String elementId) 
    throws Exception
  {
    if (!UNIQUE_ELEMENT_ID_PATTERN.matcher(elementId).matches())
    {
      ResourceErrorHandler.handleError(
        "Invalid element ID: " + elementId, Status.BAD_REQUEST, log);
    }
    final String[] elementIdParts = elementId.split("_");
    final ElementType elementType = Element.elementTypeFromString(elementIdParts[1]);
    if (elementType == null)
    {
      ResourceErrorHandler.handleError(
        "Invalid element type: " + elementType, Status.BAD_REQUEST, log); 
    }
    else if (elementType.equals(ElementType.Node))
    {
      ResourceErrorHandler.handleError(
        "Get Full Element Request Invalid for type = Node", Status.BAD_REQUEST, log);
    }
    
    Connection conn = DbUtils.createConnection();
    Document elementDoc = null;
    try
    {
      log.debug("Initializing database connection...");
      
      elementDoc = 
        getElementXml(elementIdParts[0], Long.parseLong(elementIdParts[2]), elementType, true,
          true, conn);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    
    log.debug("Returning response: " + 
      StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 100) + " ...");
    return 
      Response
        .ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
        .header("Content-type", MediaType.APPLICATION_XML)
        .build();
  }
  
  private Document getElementXml(final String mapId, final long elementId, 
    final ElementType elementType, final boolean multiLayerUniqueElementIds, 
    final boolean addChildren, Connection  dbConn) throws Exception
  {
    long mapIdNum = -1;
    try
    {
    	QMaps maps = QMaps.maps;
      //input mapId may be a map ID or a map name
      mapIdNum = 
        ModelDaoUtils.getRecordIdForInputString(mapId, dbConn, 
        		maps, maps.id, maps.displayName);
    }
    catch (Exception e)
    {
      if (e.getMessage().startsWith("Multiple records exist"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      else if (e.getMessage().startsWith("No record exists"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      ResourceErrorHandler.handleError(
        "Error requesting map with ID: " + mapId + " (" + e.getMessage() + ")", 
        Status.BAD_REQUEST,
        log);
    }
    
    Document elementDoc = null;
    Set<Long> elementIds = new HashSet<Long>();
    elementIds.add(elementId);
    @SuppressWarnings("unchecked")
	final List<Tuple> elementRecords = 
      (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType, elementIds, dbConn);
    if (elementRecords == null || elementRecords.size() == 0)
    {
      ResourceErrorHandler.handleError(
        "Element with ID: " + elementId + " and type: " + elementType + " does not exist.", 
        Status.NOT_FOUND,
        log);
    }
    assert(elementRecords.size() == 1);
    

		
    final Element element = 
      ElementFactory.getInstance().create(elementType, elementRecords.get(0), dbConn, Long.parseLong(mapId));
    
    Users usersTable = elementRecords.get(0).get(QUsers.users);
    elementDoc = XmlDocumentBuilder.create();
    org.w3c.dom.Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(elementDoc);
    elementDoc.appendChild(elementRootXml);
    org.w3c.dom.Element elementXml = 
      element.toXml(
        elementRootXml,
        usersTable.getId(), 
        usersTable.getDisplayName(),
        multiLayerUniqueElementIds,
        addChildren);
    elementRootXml.appendChild(elementXml);
    
    return elementDoc;
  }
  
  /**
	 * <NAME>Element Service - Get Elements By IDs </NAME>
	 * <DESCRIPTION>
	 * 	Allows for retrieving multiple nodes, ways, or relations by numeric OSM element ID. Child element of ways
	 * and relations are not added to the output (use the "full" method for that functionality).
	 * The ID of the map owning the element must be specified in the query string.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 *  <mapId>
	 *  string; ID or name of the map the requested element belongs to
	 *  </mapId>
	 *  <elementIds>
	 *   string; OSM IDs of the requested elements
	 *  </elementIds>
	 *  <elementType>
	 *  string; OSM type of the requested element; valid values are "node", "way", or "relation"
	 *  </elementType>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	XML representation of the requested element
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/osm/api/0.6/node/1?mapId=1</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 *  OSM XML
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Returns a single element item's XML for a given map without its element children
   * 
   * @param mapId ID of the map the element belongs to
   * @param elementId OSM element ID of the element to retrieve
   * @param elementType OSM element type of the element to retrieve; valid values are: node, way, 
   * or relation
   * @return element XML document
   * @throws Exception
   */
  @GET
  @Path("{elementType: nodes|ways|relations}")
  @Consumes({MediaType.TEXT_PLAIN})
  @Produces({MediaType.TEXT_XML})
  public Response getElements(
    @QueryParam("mapId")
    String mapId,
    @QueryParam("elementIds") 
    final String elementIds,
    @PathParam("elementType") 
    final String elemType) 
    throws Exception
  {
  	String elementType = elemType.substring(0, elemType.length()-1);
  	String[] elemIds = elementIds.split(",");
  	
    final ElementType elementTypeVal = Element.elementTypeFromString(elementType);
    if (elementTypeVal == null)
    {
      ResourceErrorHandler.handleError(
        "Invalid element type: " + elementType, Status.BAD_REQUEST, log); 
    }
    
    Connection conn = DbUtils.createConnection();
    Document elementDoc = null;
    try
    {
      log.debug("Initializing database connection...");
      
      elementDoc = getElementsXml(mapId, elemIds, elementTypeVal, false, true, conn);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
    
    log.debug("Returning response: " + 
      StringUtils.abbreviate(XmlDocumentBuilder.toString(elementDoc), 10000) + " ...");
    return 
      Response
        .ok(new DOMSource(elementDoc), MediaType.APPLICATION_XML)
        .header("Content-type", MediaType.APPLICATION_XML)
        .build();
  }


  private Document getElementsXml(final String mapId, final String[] elementIdsStr, 
    final ElementType elementType, final boolean multiLayerUniqueElementIds, 
    final boolean addChildren, Connection  dbConn) throws Exception
  {
    long mapIdNum = -1;
    try
    {
    	QMaps maps = QMaps.maps;
      //input mapId may be a map ID or a map name
      mapIdNum = 
        ModelDaoUtils.getRecordIdForInputString(mapId, dbConn, maps, maps.id, maps.displayName);
    }
    catch (Exception e)
    {
      if (e.getMessage().startsWith("Multiple records exist"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      else if (e.getMessage().startsWith("No record exists"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      ResourceErrorHandler.handleError(
        "Error requesting map with ID: " + mapId + " (" + e.getMessage() + ")", 
        Status.BAD_REQUEST,
        log);
    }
    
    Document elementDoc = null;
    Set<Long> elementIds = new HashSet<Long>();
    for(String elemId : elementIdsStr)
    {
    	long elementId = Long.parseLong(elemId);
    	elementIds.add(elementId);
    }
    
    @SuppressWarnings("unchecked")
    final List<Tuple> elementRecords = 
      (List<Tuple>) Element.getElementRecordsWithUserInfo(mapIdNum, elementType, elementIds, dbConn);
    if (elementRecords == null || elementRecords.size() == 0)
    {
      ResourceErrorHandler.handleError(
        "Elements with IDs: " + StringUtils.join(elementIdsStr, ",") + " and type: " + 
          elementType + " does not exist.", 
        Status.NOT_FOUND,
        log);
    }
    assert(elementRecords.size() == 1);
    
    elementDoc = XmlDocumentBuilder.create();
    org.w3c.dom.Element elementRootXml = OsmResponseHeaderGenerator.getOsmDataHeader(elementDoc);
    elementDoc.appendChild(elementRootXml);
    
    for(int i=0; i<elementRecords.size(); i++)
    {
    	final Element element = 
          ElementFactory.getInstance().create(elementType, elementRecords.get(i), dbConn, Long.parseLong(mapId));      
        Users usersTable = elementRecords.get(i).get(QUsers.users);
        
        org.w3c.dom.Element elementXml = 
            element.toXml(
              elementRootXml,
              usersTable.getId(), 
              usersTable.getDisplayName(),
              multiLayerUniqueElementIds,
              addChildren);
          elementRootXml.appendChild(elementXml);
    }
    
    return elementDoc;
  }
}
