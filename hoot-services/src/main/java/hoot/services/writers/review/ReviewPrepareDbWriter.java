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
package hoot.services.writers.review;

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.json.simple.JSONObject;
import org.postgresql.util.PGobject;
import org.postgresql.util.PSQLException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.ListMultimap;
import com.mysema.query.sql.ColumnMetadata;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

import hoot.services.HootProperties;
import hoot.services.db.DbClientAbstract;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewItems;
import hoot.services.db2.ReviewMap;
import hoot.services.job.Executable;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.review.ReviewUtils;

/**
 * Writes review data to the services database
 * 
 * Assumes that data is always prepared once and once only, so doesn't check to see if any existing
 * reviews are already in the database before parsing new ones.
 */
public class ReviewPrepareDbWriter extends DbClientAbstract implements Executable
{
	private static final Logger log = LoggerFactory.getLogger(ReviewPrepareDbWriter.class);
	private static final QReviewMap reviewMap = QReviewMap.reviewMap;
	private static final QReviewItems reviewItems = QReviewItems.reviewItems;
	private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
	private Connection conn;

	private int maxWarningsDisplayed = 10;
	private int warnMessagesDisplayed = 0;
	
	private long mapId;
	private long uniqueIdsParsed = 0;
	private boolean idMappingRecordWritten = false;
	private long reviewRecordsParsed = 0;
	private boolean reviewRecordWritten = false;
	private long totalParseableRecords = 0;
	private long totalReviewableRecords = 0;
	private int maxRecordBatchSize;
	private boolean cleanReviewTags = true;
	private ListMultimap<String, String> reviewableItemIdToReviewAgainstItemIds;

	private String finalStatusDetail;

	public String getFinalStatusDetail()
	{
		return finalStatusDetail;
	}

	public ReviewPrepareDbWriter() throws Exception
	{
		super();

		maxRecordBatchSize = 
			Integer.parseInt(
				HootProperties.getInstance()
		      .getProperty("maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
		maxWarningsDisplayed = 
			Integer.parseInt(
				HootProperties.getInstance()
		      .getProperty("maxWarningsDisplayed", HootProperties.getDefault("maxWarningsDisplayed")));
		cleanReviewTags = 
			Boolean.parseBoolean(
				HootProperties.getInstance()
		      .getProperty("cleanReviewTags", HootProperties.getDefault("cleanReviewTags")));
	}

	/**
	 * See CoreServiceContext.xml
	 */
	public void init() // NO_UCD (unused code)
	{
	}

	/**
	 * See CoreServiceContext.xml
	 */
	public void destroy() // NO_UCD (unused code)
	{
	}

	/**
	 * Prepares a map's conflated data for review
	 *
	 * @param command
	 *          input parameters containing a map and job ID
	 * @throws Exception
	 */
	public void exec(JSONObject command) throws Exception
	{
		if (command.containsKey("testDelayMilliseconds"))
		{
			Thread.sleep((Long) command.get("testDelayMilliseconds"));
		}
		if (command.containsKey("simulateFailure"))
		{
			simulateFailure = (Boolean) command.get("simulateFailure");
		}

		mapId = (Long) command.get("mapId");
		final String jobId = (String) command.get("jobId");
		log.debug("Executing review prepare job with ID: " + jobId
		    + " for map with ID: " + String.valueOf(mapId) + " ...");

		// TODO: verify that no other writes are seen during this transaction

		try
		{
			log.debug("Initializing database driver...");
			conn = DbUtils.createConnection();

			log.debug("Intializing ReviewDbPreparer transaction...");
			TransactionStatus transactionStatus = 
				transactionManager.getTransaction(
					new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
			conn.setAutoCommit(false);

			try
			{
				// see if map review info for this map already exists
				final ReviewMap mapReviewInfo = 
					new SQLQuery(conn, DbUtils.getConfiguration(mapId))
				    .from(reviewMap)
				    .where(reviewMap.mapId.eq(mapId))
				    .singleResult(reviewMap);

				if (mapReviewInfo == null)
				{
					// if not, add new entry to review_map table
					ReviewMap mapReviewInfoRecord = new ReviewMap();
					mapReviewInfoRecord.setMapId(mapId);
					mapReviewInfoRecord.setReviewPrepareJobId((String)command.get("jobId"));
					/*
					 * if (mapReviewInfoRecord.insert() != 1) { throw new
					 * Exception("Error inserting map review info."); }
					 */

					SQLInsertClause insert = new SQLInsertClause(conn,
					    DbUtils.getConfiguration(mapId), reviewMap);
					if (insert.populate(mapReviewInfoRecord).execute() != 1)
					{
						throw new Exception("Error inserting map review info.");
					}
				}
				else
				{
					// if so, overwrite any existing unreviewed data
					deleteExistingUnreviewedItems(mapId);

					// and then write new job id to the review map table
					SQLUpdateClause update = 
						new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), reviewMap);
					if (update
							  .where(reviewMap.mapId.eq(mapId))
					      .set(reviewMap.reviewPrepareJobId, jobId)
					      .execute() != 1)
					{
						throw new Exception("Error updating job entry in review table.");
					}
				}

				//two passes through the data have to be made; one to create the mappings from the 
				//unique IDs to the osm element IDs and then a second to parse the review tags; 
				//there might be a way to do this in one pass...
				
				//The logic here got a little redundant...but no point in cleaning up now, since this class 
				//will soon be gone.

				boolean reviewableItemsExist = false;
				final String noRecordsParsedMessage = 
					"No records available for review for map with ID: " + mapId;
				
				totalReviewableRecords = getTotalReviewableRecords(mapId);
				if (totalReviewableRecords > 0)
				{
					totalParseableRecords = getTotalParseableRecords(mapId);
					final boolean uuidsExist = parseElementUniqueIdTags(mapId);
					if (!uuidsExist)
					{
						log.info("Parsing unique element IDs: " + noRecordsParsedMessage);
					}
					else
					{
						// read the review tags from the OSM elements and write their data to services db review
						// tables
						reviewableItemsExist = parseElementReviewTags(mapId);
						if (!reviewableItemsExist)
						{
							log.info("Parsing review tags: " + noRecordsParsedMessage);
						}
					}
					if (!uuidsExist || !reviewableItemsExist)
					{
						finalStatusDetail = noRecordsParsedMessage;
					}
					else
					{
						finalStatusDetail = "Reviewable records successfully prepared.";
					}
				}
				else
				{
					finalStatusDetail = noRecordsParsedMessage;
				}
			}
			catch (Exception e)
			{
				log.error("Error preparing review items: " + e.getMessage());
				log.debug("Rolling back transaction for ReviewDbPreparer...");
				transactionManager.rollback(transactionStatus);
				conn.rollback();
				throw e;
			}

			log.debug("Committing ReviewDbPreparer database transaction...");
			try
			{
				transactionManager.commit(transactionStatus);
				conn.commit();
				log.info("Review item preparation complete.");
			}
			catch (Exception e)
			{
				log.error("Error committing transaction: " + e.getMessage());
				if (e instanceof PSQLException)
				{
					log.error("SQL error: " + ((PSQLException)e).getServerErrorMessage().getDetail());
				}
				throw e;
			}
		}
		finally
		{
			conn.setAutoCommit(true);
			DbUtils.closeConnection(conn);
		}
		
		boolean errorDeletingTempRecords = false;
		if (totalReviewableRecords > 0 && 
				Boolean.parseBoolean(
          HootProperties.getInstance().getProperty(
	          "reviewPrepareCleanup", HootProperties.getDefault("reviewPrepareCleanup"))))
		{
			try
			{
				log.debug("Initializing database driver...");
				conn = DbUtils.createConnection();

				log.debug("Intializing ReviewDbPreparer cleanup transaction...");
				TransactionStatus transactionStatus = 
					transactionManager.getTransaction(
						new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
				conn.setAutoCommit(false);
				
			  // There may be element id's written as a result of parseElementUniqueIdTags that don't
				// have associated review item records, because of the two pass write operation. They
				// aren't currently hurting anything by existing and will be ignored, but its a good idea
				// to clean them out.
				removeUnusedElementIdMappings();
				
				log.debug("Committing ReviewDbPreparer database cleanup transaction...");
				transactionManager.commit(transactionStatus);
				conn.commit();
				log.info("Review element ID temp record cleanup complete.");
			}
		  //It's not the end of the world if these don't get cleaned out, but you should eventually 
			//try to figure out why they couldn't be cleaned if an error occurs.
			catch (Exception e)
			{
				log.warn("Caught error while cleaning out unused element ID mappings.  " +
			    "Skipping temp record cleanup.  Error: " + e.getMessage());
				if (e instanceof PSQLException)
				{
					log.warn("SQL error: " + ((PSQLException)e).getServerErrorMessage().getDetail());
				}
				errorDeletingTempRecords = true;
			}
			finally
			{
				if (!errorDeletingTempRecords)
				{
					conn.setAutoCommit(true);
				}
				DbUtils.closeConnection(conn);
			}
		}
		else
		{
			log.debug("Review record UUID's not cleaned up.");
		}
	}

	private long getTotalParseableRecords(final long mapId)
	  throws InstantiationException, IllegalAccessException, ClassNotFoundException, 
	  NoSuchMethodException, Exception
	{
		long count = 0;
		for (ElementType elementType : ElementType.values())
		{
			if (!elementType.equals(ElementType.Changeset))
			{
				final Element prototype = 
					ElementFactory.getInstance().create(mapId, elementType, conn);
				String tableName = prototype.getElementTable().getTableName();

				String POSTGRESQL_DRIVER = "org.postgresql.Driver";
				Statement stmt = null;
				try
				{
					Class.forName(POSTGRESQL_DRIVER);

					stmt = conn.createStatement();

					String sql = 
						"select count(*) from " + tableName + "_" + mapId + 
						" where " + "EXIST(tags, 'uuid') = TRUE";
					stmt = conn.createStatement();
					ResultSet rs = stmt.executeQuery(sql);
					rs.next();
					count += rs.getInt(1);
					rs.close();
				}
				finally
				{
					// finally block used to close resources
					try
					{
						if (stmt != null) stmt.close();
					}
					catch (SQLException se2)
					{
					}// nothing we can do

				}// end try
			}
		}
		return count;
	}

	private long getTotalReviewableRecords(final long mapId)
	   throws InstantiationException, IllegalAccessException, ClassNotFoundException, 
	   NoSuchMethodException, InvocationTargetException, SQLException
	{
		long count = 0;
		for (ElementType elementType : ElementType.values())
		{
			if (!elementType.equals(ElementType.Changeset))
			{
				final Element prototype = 
					ElementFactory.getInstance().create(mapId, elementType, conn);

				String tableName = prototype.getElementTable().getTableName();

				String POSTGRESQL_DRIVER = "org.postgresql.Driver";
				Statement stmt = null;
				try
				{
					Class.forName(POSTGRESQL_DRIVER);

					stmt = conn.createStatement();

					String sql = 
						"select count(*) from " + tableName + "_" + mapId + 
						" where " + " tags->'hoot:review:needs' = 'yes'";
					stmt = conn.createStatement();
					ResultSet rs = stmt.executeQuery(sql);
					rs.next();
					count += rs.getInt(1);
					rs.close();
				}
				finally
				{
					// finally block used to close resources
					try
					{
						if (stmt != null) stmt.close();
					}
					catch (SQLException se2)
					{
					}// nothing we can do
				}
			}
		}
		return count;
	}

	private Map<Long, Object> getParseableElementRecords(final long mapId,
	  final ElementType elementType, final int limit, final int offset)
	  throws InstantiationException, IllegalAccessException, ClassNotFoundException, 
	  NoSuchMethodException, InvocationTargetException, SQLException
	{
		Map<Long, Object> retMap = new LinkedHashMap<Long, Object>();
		final Element prototype = 
			ElementFactory.getInstance().create(mapId, elementType, conn);
		String tableName = prototype.getElementTable().getTableName();
		String idFieldName = 
			ColumnMetadata.getColumnMetadata(prototype.getElementIdField()).getName();

		String POSTGRESQL_DRIVER = "org.postgresql.Driver";
		Statement stmt = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);

			stmt = conn.createStatement();

			String sql = "select * from " + tableName + "_" + mapId + " where " + 
			  "EXIST(tags, 'uuid') = TRUE " + " order by " + idFieldName + " limit " + limit + 
			  " offset " + offset;
			stmt = conn.createStatement();
			ResultSet rs = stmt.executeQuery(sql);

			//TODO: Is there any way to change this back to the original element generic code?
			while (rs.next())
			{
				if (elementType == ElementType.Node)
				{
					CurrentNodes nodes = new CurrentNodes();
					nodes.setId(rs.getLong("id"));

					nodes.setLatitude(rs.getDouble("latitude"));
					nodes.setLongitude(rs.getDouble("longitude"));
					nodes.setChangesetId(rs.getLong("changeset_id"));
					nodes.setVisible(rs.getBoolean("visible"));
					nodes.setTimestamp(rs.getTimestamp("timestamp"));
					nodes.setTile(rs.getLong("tile"));
					nodes.setVersion(rs.getLong("version"));
					nodes.setTags(rs.getObject("tags"));
					retMap.put(nodes.getId(), nodes);
				}
				else if (elementType == ElementType.Way)
				{
					CurrentWays ways = new CurrentWays();
					ways.setId(rs.getLong("id"));

					ways.setChangesetId(rs.getLong("changeset_id"));
					ways.setVisible(rs.getBoolean("visible"));
					ways.setTimestamp(rs.getTimestamp("timestamp"));
					ways.setVersion(rs.getLong("version"));
					ways.setTags(rs.getObject("tags"));
					retMap.put(ways.getId(), ways);
				}
				else if (elementType == ElementType.Relation)
				{
					CurrentRelations rel = new CurrentRelations();
					rel.setId(rs.getLong("id"));

					rel.setChangesetId(rs.getLong("changeset_id"));
					rel.setVisible(rs.getBoolean("visible"));
					rel.setTimestamp(rs.getTimestamp("timestamp"));
					rel.setVersion(rs.getLong("version"));
					rel.setTags(rs.getObject("tags"));
					retMap.put(rel.getId(), rel);
				}
			}
			rs.close();
		}
		finally
		{
			// finally block used to close resources
			try
			{
				if (stmt != null) stmt.close();
			}
			catch (SQLException se2)
			{
			}// nothing we can do

		}

		return retMap;
	}

	private Map<Long, Object> getReviewableElementRecords(final long mapId,
	  final ElementType elementType, final int limit, final int offset) throws InstantiationException, 
	  IllegalAccessException, ClassNotFoundException, NoSuchMethodException, InvocationTargetException, 
	  SQLException
	{
		Map<Long, Object> retMap = new LinkedHashMap<Long, Object>();
		final Element prototype = 
			ElementFactory.getInstance().create(mapId, elementType, conn);

		String tableName = prototype.getElementTable().getTableName();
		String idFieldName = 
			ColumnMetadata.getColumnMetadata(prototype.getElementIdField()).getName();

		String POSTGRESQL_DRIVER = "org.postgresql.Driver";
		Statement stmt = null;
		try
		{
			Class.forName(POSTGRESQL_DRIVER);

			stmt = conn.createStatement();

			String sql = 
				"select * from " + tableName + "_" + mapId + " where tags->'hoot:review:needs' = 'yes' " + 
			  " order by " + idFieldName + " limit " + limit + " offset " + offset;
			stmt = conn.createStatement();
			ResultSet rs = stmt.executeQuery(sql);

			while (rs.next())
			{
				final Object obj = DbUtils.resultToObj(rs, elementType);
				final long id = (Long)MethodUtils.invokeMethod(obj, "getId", new Object[]{});
				retMap.put(id, obj);
			}
			rs.close();
		}
		finally
		{
			try
			{
				if (stmt != null)
					stmt.close();
			}
			catch (SQLException se2)
			{

			}// nothing we can do

		}

		return retMap;
	}
	
	/*
   * Extract all unique ID's from both reviewable and review against items referenced in the
   * recordset
   */
  private Set<String> parseAllUuidsFromElementRecords(final Map<Long, Object> elementRecords) 
  	throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, Exception
	{
  	Set<String> allUuids = new HashSet<String>();
  	for (Map.Entry<Long, Object> elementRecordEntry : elementRecords.entrySet())
    {
      final Object elementRecord = elementRecordEntry.getValue();
      final Map<String, String> tags =
        PostgresUtils.postgresObjToHStore(
      	  (PGobject)MethodUtils.invokeMethod(elementRecord, "getTags", new Object[]{}));
      assert(tags != null);
      final String reviewableItemId = StringUtils.trimToNull(tags.get("uuid"));
      if (reviewableItemId != null)
      {
      	allUuids.add(reviewableItemId);
      }
      
      final String itemsToReviewAgainstStr = StringUtils.trimToNull(tags.get("hoot:review:uuid"));
      if (itemsToReviewAgainstStr != null)
      {
      	String[] reviewAgainstItemIds = null;
        //see related note in parseElementReviewTags
        if (!itemsToReviewAgainstStr.contains(";"))
        {
          reviewAgainstItemIds = new String[] { itemsToReviewAgainstStr };
        }
        else
        {
          reviewAgainstItemIds = itemsToReviewAgainstStr.split(";");
        }
        for (int i = 0; i < reviewAgainstItemIds.length; i++)
        {
        	final String id = reviewAgainstItemIds[i];
        	if (id != null)
        	{
        		allUuids.add(id);
        	}
        }
      }
    }
  	return allUuids;
	}

  /*
   * logging records with invalid uuids and skipping; if errors should be thrown, then the
   * unit tests will have to reworked
   */
  private boolean parseElementUniqueIdTags(final long mapId) throws Exception
  {
    final String logMsgStart =
      "Parsing element unique ID tags for map with ID: " + mapId + ".  Step 2 of 4.";
    log.info(logMsgStart);

    uniqueIdsParsed = 0;
    idMappingRecordWritten = false;
    List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
    //create this outside of the batch read loop, since we need to maintain a list of unique
    //IDs parsed over the entire map's set of reviewable records
    Set<String> elementIds = new HashSet<String>();

    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        int numElementsReturned = Integer.MAX_VALUE;
        int elementIndex = 0;
        while (numElementsReturned > 0)
        {
          //get all elements with with a uuid tag
          final Map<Long, Object> parseableElementRecords =
            getParseableElementRecords(mapId, elementType, maxRecordBatchSize, elementIndex);
          
          final Set<String> allUuids = parseAllUuidsFromElementRecords(parseableElementRecords);
          Set<String> existingIdMappings = null;
          if (allUuids.size() > 0)
          {
          	log.debug("allUuids length: " + allUuids.size());
            existingIdMappings =  
            	Element.filterOutNonExistingElementMappingUniqueIds(
          			mapId, allUuids.toArray(new String[]{}), elementType, conn);
            log.debug("existingIdMappings length: " + existingIdMappings.size());
          }
          
          numElementsReturned = parseableElementRecords.size();
          elementIndex += numElementsReturned;
          
          for (Map.Entry<Long, Object> parseableElementRecordEntry :
          	   parseableElementRecords.entrySet())
          {
            final long osmElementId = parseableElementRecordEntry.getKey();
            final Object reviewableElementRecord = parseableElementRecordEntry.getValue();
            final Map<String, String> tags =
              PostgresUtils.postgresObjToHStore(
              	(PGobject)MethodUtils.invokeMethod(
              		reviewableElementRecord, "getTags", new Object[]{}));
            final String uniqueElementIdStr = StringUtils.trimToNull(tags.get("uuid"));
            
            if (uniqueElementIdStr == null)
            {
            	final String msg = 
            		"Null or empty UUID for map with ID: " + mapId +
                " and OSM record with ID: " + osmElementId + ".  Skipping adding unique ID record...";
            	if (warnMessagesDisplayed <= maxWarningsDisplayed)
            	{
            		log.warn(msg);
            		warnMessagesDisplayed++;
            	}
            	else
            	{
            		log.debug(msg);
            	}
            }
            else
            {
            	//There actually can be more than one unique element ID for the same element, which is
          		//counter-intuitive.  This is b/c we store element ID's by both breaking up 
            	//concatenated uuid's and storing them whole.  This may not be the best way to handle 
            	//storing the unique element ID's, but was necessary to avoid the client sending ID's 
            	//from reviews that the server knew nothing about.  This shouldn't, however, result 
            	//in any duplicated review records, since we don't break up ID's in the same way when
            	//writing them.  
            	List<String> uniqueElementIds = new ArrayList<String>();
          		uniqueElementIds.add(uniqueElementIdStr);
              if (uniqueElementIdStr.contains(";"))
              {
                String[] uniqueElementIdsArr = uniqueElementIdStr.split(";");
                for (String id : uniqueElementIdsArr)
                {
                	uniqueElementIds.add(id);
                }
              }
                
              for (String uniqueElementId : uniqueElementIds)
              {
                if (existingIdMappings.contains(uniqueElementId))
                {
                	final String msg = 
                		"UUID: " + uniqueElementId + " for map with ID: " + mapId + 
                    " and OSM record ID: " + osmElementId + " already exists.  " +
                    "Skipping adding unique ID record...";
                	if (warnMessagesDisplayed <= maxWarningsDisplayed)
                	{
                		log.warn(msg);
                		warnMessagesDisplayed++;
                	}
                	else
                	{
                		log.debug(msg);
                	}
                }
                else
                {
                  if (elementIds.add(uniqueElementId))  //don't add duplicates
                  {
                    log.debug("Adding UUID: " + uniqueElementId);
                    elementIdMappingRecordsToInsert.add(
                      ReviewUtils.createElementIdMappingRecord(
                      	uniqueElementId, osmElementId, elementType, mapId));
                    flushIdMappingRecords(
                      elementIdMappingRecordsToInsert, maxRecordBatchSize, logMsgStart);
                  }
                  else
                  {
                    log.debug(
                      "Duplicate element ID: " + uniqueElementId.toString() + " for map with ID: " +
                      mapId + ".  Skipping adding unique ID record...");
                  }
                }
              }
            }
          }
        }
      }
    }

    //final flush
    flushIdMappingRecords(elementIdMappingRecordsToInsert, 0, logMsgStart);

    log.debug("Wrote " + elementIds.size() + " ID mappings.");

    return idMappingRecordWritten;
  }
  
  /*
   * find all elements who have a hoot:review:uuid tag id which doesn't correspond to the uuid
   * of any existing element and remove that uuid from the tag so that the invalid uuid isn't passed
   * back in via a changeset at a later time
   */
  private void removeInvalidReviewAgainstTagsFromElements(final ElementType elementType, 
  	final Set<String> invalidUuids) throws Exception
  {
  	if (invalidUuids.size() > 0)
  	{
  		/*if (invalidUuids.contains("{8f23026f-27c1-5e03-8ae4-0d229d996c9b}"))
  		{
  			int test = 1;
  		}*/
  		
  		final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);
  		String tableName = prototype.getElementTable().getTableName();

  		String POSTGRESQL_DRIVER = "org.postgresql.Driver";
  		Statement stmt = null;
  		try
  		{
  			Class.forName(POSTGRESQL_DRIVER);
  			stmt = conn.createStatement();

  			String sql = 
  				"select * from " + tableName + "_" + mapId + " where";
  			final String[] invalidUuidsArr = invalidUuids.toArray(new String[]{});
  			for (int i = 0; i < invalidUuidsArr.length; i++)
  			{
  				sql += " tags->'hoot:review:uuid' like '%" + invalidUuidsArr[i] + "%'";
  				if (i < invalidUuidsArr.length - 1)
  				{
  					sql += " or";
  				}
  			}
  			stmt = conn.createStatement();
  			ResultSet rs = stmt.executeQuery(sql);
  			List<Object> recordsToModify = new ArrayList<Object>();
  			while (rs.next())
  			{
  				final Object obj = DbUtils.resultToObj(rs, elementType);
  				Map<String, String> tags =
		        PostgresUtils.postgresObjToHStore(
		      	  (PGobject)MethodUtils.invokeMethod(obj, "getTags", new Object[]{}));
  				if (tags.containsKey("hoot:review:uuid"))
  				{
  				  final String originalReviewAgainstIdsStr = tags.get("hoot:review:uuid");
  				  String[] reviewAgainstIdsArr = null;
  				  if (originalReviewAgainstIdsStr.contains(";"))
  				  {
  				  	reviewAgainstIdsArr = originalReviewAgainstIdsStr.split(";");
  				  }
  				  else
  				  {
  				  	reviewAgainstIdsArr = new String[1];
  				  	reviewAgainstIdsArr[0] = originalReviewAgainstIdsStr;
  				  }
  				  Set<String> reviewAgainstIds = new HashSet(Arrays.asList(reviewAgainstIdsArr));
  				  
  				  /*if (reviewAgainstIds.contains("{8f23026f-27c1-5e03-8ae4-0d229d996c9b}"))
  		  		{
  		  			int test = 1;
  		  		}*/
  				  
  				  reviewAgainstIds.removeAll(invalidUuids);
  				  
  				  /*if (reviewAgainstIds.contains("{8f23026f-27c1-5e03-8ae4-0d229d996c9b}"))
  		  		{
  		  			int test = 1;
  		  		}*/
  				  
  				  final String newReviewAgainstIdsStr = StringUtils.join(reviewAgainstIds, ";");
  				  if (!newReviewAgainstIdsStr.equals(originalReviewAgainstIdsStr))
  				  {
  				  	if (StringUtils.trimToNull(newReviewAgainstIdsStr) != null)
    				  {
    				  	tags.put("hoot:review:uuid", newReviewAgainstIdsStr);
    				  } 
    				  else
    				  {
    				  	//no review left, so drop all review tags
    				  	Map<String, String> tagsCopy = new HashMap(tags);
    				  	for (Map.Entry<String, String> tagEntry : tags.entrySet())
    			      {
    			      	if (tagEntry.getKey().startsWith("hoot:review"))
    			      	{
    			      		tagsCopy.remove(tagEntry.getKey());
    			      	}
    			      }
    				  	tags = tagsCopy;
    				  }
  				  	MethodUtils.invokeMethod(obj, "setTags", tags);
  				  	recordsToModify.add(obj);
  				  }
  				}
  			}
  			rs.close();
  			
  		  //TODO: make this element generic
  			long updateCount = 0;
  			if (elementType == ElementType.Node)
        {
  				updateCount += 
  					DbUtils.batchRecordsDirectNodes(mapId, recordsToModify, RecordBatchType.UPDATE, conn,
  						recordsToModify.size());
        }
        else if (elementType == ElementType.Way)
        {
        	updateCount += 
            DbUtils.batchRecordsDirectWays(mapId, recordsToModify, RecordBatchType.UPDATE, conn,
            	recordsToModify.size());
        }
        else if (elementType == ElementType.Relation)
        {
        	updateCount += 
            DbUtils.batchRecordsDirectRelations(mapId, recordsToModify, RecordBatchType.UPDATE, conn,
            	recordsToModify.size());
        }
  			log.debug(
  				updateCount + " element records updated with review against ID's " +
  				"that don't have corresponding uuid tag entries (" + recordsToModify.size() + 
  				" records sent in udpate query)");
  		}
  		finally
  		{
  			try
  			{
  				if (stmt != null) stmt.close();
  			}
  			catch (SQLException se2)
  			{
  			}
  		}
  	}
  }

  /*
   * logging records with invalid tag values and skipping; if errors should be thrown, then the
   * unit tests will have to reworked
   */
  private boolean parseElementReviewTags(final long mapId) throws Exception
  {
    final String logMsgStart =
      "Parsing element review tags for map with ID: " + mapId + ".  Step 3 of 4.";
    log.info(logMsgStart);
    //parse review tags for all nodes, ways, and relations from the OSM element tables for the given
    //map
    reviewRecordsParsed = 0;
    reviewRecordWritten = false;
    List<ReviewItems> reviewRecordsToInsert = new ArrayList<ReviewItems>();
    //create this outside of the batch read loop, since we need to maintain a list of unique
    //IDs parsed over the entire map's set of reviewable records
    reviewableItemIdToReviewAgainstItemIds = ArrayListMultimap.create();

    int numReviewItemsAdded = 0;
    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        int numElementsReturned = Integer.MAX_VALUE;
        int elementIndex = 0;
        while (numElementsReturned > 0)
        {
          //get a batch of reviewable elements
          final Map<Long, Object> reviewableElementRecords =
            getReviewableElementRecords(mapId, elementType, maxRecordBatchSize, elementIndex);
          
          final Set<String> allUuids = parseAllUuidsFromElementRecords(reviewableElementRecords);
          Set<String> existingIdMappings = null;
          Set<String> validUuids = null;
          Set<String> invalidUuids = null;
          if (allUuids.size() > 0)
          {
          	/*if (allUuids.contains("{8f23026f-27c1-5e03-8ae4-0d229d996c9b}"))
          	{
          		int test = 1;
          	}*/
          	
          	log.debug("allUuids length: " + allUuids.size());
            existingIdMappings =  
          		Element.filterOutNonExistingElementMappingUniqueIds(
          			mapId, allUuids.toArray(new String[]{}), elementType, conn);
            log.debug("existingIdMappings length: " + existingIdMappings.size());
            validUuids =
              Element.filterOutNonExistingUuids(
              	mapId, allUuids.toArray(new String[]{}), elementType, conn);
            log.debug("validUuids length: " + validUuids.size());
            
            /*if (validUuids.contains("{8f23026f-27c1-5e03-8ae4-0d229d996c9b}"))
          	{
          		int test = 1;
          	}*/
            
            invalidUuids = new HashSet<String>(allUuids);
            invalidUuids.removeAll(validUuids);
            log.debug("invalidUuids length: " + invalidUuids.size());
            if (cleanReviewTags)
            {
            	removeInvalidReviewAgainstTagsFromElements(elementType, invalidUuids);
            }
          } 
          
          numElementsReturned = reviewableElementRecords.size();
          elementIndex += numElementsReturned;
          
          for (Map.Entry<Long, Object> reviewableElementRecordEntry : 
          	   reviewableElementRecords.entrySet())
          {
          	final long osmElementId = reviewableElementRecordEntry.getKey();
            final Object reviewableElementRecord = reviewableElementRecordEntry.getValue();
            final Map<String, String> tags =
              PostgresUtils.postgresObjToHStore(
              	(PGobject)MethodUtils.invokeMethod(reviewableElementRecord, "getTags", new Object[]{}));
            final String reviewableItemId = StringUtils.trimToNull(tags.get("uuid"));
            
            if (StringUtils.isEmpty(reviewableItemId))
            {
            	final String msg = 
            		"Invalid UUID: " + reviewableItemId + " for OSM record with ID: " + osmElementId +
            		" for map with ID: " + mapId + "Skipping adding review record...";
            	if (warnMessagesDisplayed <= maxWarningsDisplayed)
            	{
            		log.warn(msg);
            		warnMessagesDisplayed++;
            	}
            	else
            	{
            		log.debug(msg);
            	}
            }
            else if (!validUuids.contains(reviewableItemId))
            {
            	final String msg = 
            		"No feature exists with UUID: " + reviewableItemId + " for OSM record with ID: " + 
            	  osmElementId + " for map with ID: " + mapId + " Skipping adding review record...";
            	if (warnMessagesDisplayed <= maxWarningsDisplayed)
            	{
            		log.warn(msg);
            		warnMessagesDisplayed++;
            	}
            	else
            	{
            		log.debug(msg);
            	}
            }
            else
            {
          	  //some items won't have a review score tag; For now, the way this is being handled
              //is that items missing a tag get a review score = 1.0; items with an empty string
              //or invalid string for a review tag get a review score of -1.0, which invalidates
              //the review pair.  The case could be argued that invalid/empty score strings should
              //also result in a review score = 1.0.
              double reviewScore = -1.0;
              if (tags.containsKey("hoot:review:score"))
              {
                try
                {
                  reviewScore = Double.parseDouble(tags.get("hoot:review:score"));
                }
                catch (NumberFormatException e)
                {
                }
              }
              else
              {
                reviewScore = 1.0;
              }
              
              //final String source = StringUtils.trimToNull(tags.get("hoot:review:source"));
            	//assert(source != null);
              
              //paired item review
              if (tags.containsKey("hoot:review:uuid") && 
              		StringUtils.trimToNull(tags.get("hoot:review:uuid")) != null)
              {
              	final String itemsToReviewAgainstStr = 
                  StringUtils.trimToNull(tags.get("hoot:review:uuid"));
              	assert(itemsToReviewAgainstStr != null);
              	String[] reviewAgainstItemIds = null;
                //We are parsing pairwise comparisons and don't want duplicates, so ignore one
                //to many reviewable item to review against item relationships.  They are always
                //represented with a duplicated one to one relationship in the data.
                if (!itemsToReviewAgainstStr.contains(";"))
                {
                  reviewAgainstItemIds = new String[] { itemsToReviewAgainstStr };
                }
                else
                {
                  reviewAgainstItemIds = itemsToReviewAgainstStr.split(";");
                }

                for (int i = 0; i < reviewAgainstItemIds.length; i++)
                {
                  final String reviewAgainstItemId = 
                  	StringUtils.trimToNull(reviewAgainstItemIds[i]);
                  if (reviewAgainstItemId != null)
                  {
                  	if (!validUuids.contains(reviewAgainstItemId))
                    {
                    	final String msg = "No feature exists with review against UUID: " + 
                        reviewAgainstItemId + " for reviewable OSM record with ID: " + osmElementId + 
                        " for map with ID: " + mapId + " Skipping adding review record...";
                    	if (warnMessagesDisplayed <= maxWarningsDisplayed)
                    	{
                    		log.warn(msg);
                    		warnMessagesDisplayed++;
                    	}
                    	else
                    	{
                    		log.debug(msg);
                    	}
                    }
                    else if (!existingIdMappings.contains(reviewAgainstItemId))
                    {
                    	final String msg = 
                    		"No element ID mapping exists for review against item with ID: " +
                        reviewAgainstItemId + " for reviewable OSM record with ID: " + osmElementId +
                        " for map with ID: " + mapId + ".  Skipping adding review record...";
                    	if (warnMessagesDisplayed <= maxWarningsDisplayed)
                    	{
                    		log.warn(msg);
                    		warnMessagesDisplayed++;
                    	}
                    	else
                    	{
                    		log.debug(msg);
                    	}
                    }
                    else
                    {
                      if (!reviewPairAlreadyParsed(reviewableItemId, reviewAgainstItemId))
                      {
                      	//TODO: Had to take this source check out, b/c it was resulting in bad review
                      	//data being written.  See #6320
                      	/*if (source.equals("2"))
                      	{
                        	//For paired reviews, we want the reviewableItem to always come from 
                      		//source 1 and the reviewAgainstItems to always come from source 2.  
                      		//So, re-ordering here is necessary for some records.
                      		log.debug(
                            "Adding review item with reviewable item ID: " + reviewAgainstItemId + 
                            ", review against item ID: " + reviewableItemId + ", and source: " + 
                            source);
                          reviewRecordsToInsert.add(
                            ReviewUtils.createReviewItemRecord(
                            	reviewAgainstItemId, reviewScore, reviewableItemId, mapId));
                          reviewableItemIdToReviewAgainstItemIds.put(
                          	reviewAgainstItemId, reviewableItemId);
                      	}
                      	else
                      	{*/
                      		log.debug(
                            "Adding review item with reviewable item ID: " +  reviewableItemId + 
                            ", review against item ID: " + reviewAgainstItemId); //+ ", and source: " + 
                            //source);
                          reviewRecordsToInsert.add(
                          	ReviewUtils.createReviewItemRecord(
                            	reviewableItemId, reviewScore, reviewAgainstItemId, mapId));
                          reviewableItemIdToReviewAgainstItemIds.put(
                          	reviewableItemId, reviewAgainstItemId);
                      	//}
                        
                        flushReviewRecords(reviewRecordsToInsert, maxRecordBatchSize, logMsgStart);
                        numReviewItemsAdded++;
                      }
                    }
                  }
                }
              }
              //single item review (non-pair)
              else if (!tags.containsKey("hoot:review:uuid") ||
                       StringUtils.trimToNull(tags.get("hoot:review:uuid")) == null)
              {
              	//TODO: this description invalid given #6320 revert changes made above; The one 
              	//case where the reviewableItem can be from source = 2 is for a single item review, 
              	//hence the source check done for paired reviews is not done here. 
              	
              	if (!reviewPairAlreadyParsed(reviewableItemId, reviewableItemId))
                {
                  log.debug(
                    "Adding review item with reviewable item ID: " +  reviewableItemId + " and " +
                    "review against item ID: " + reviewableItemId);
                  reviewRecordsToInsert.add(
                  	ReviewUtils.createReviewItemRecord(
                      reviewableItemId, reviewScore, reviewableItemId, mapId));
                  reviewableItemIdToReviewAgainstItemIds.put(reviewableItemId, reviewableItemId);
                  flushReviewRecords(reviewRecordsToInsert, maxRecordBatchSize, logMsgStart);
                  numReviewItemsAdded++;
                }
              }
            }
          }
        }
      }
    }

    //final flush
    flushReviewRecords(reviewRecordsToInsert, 0, logMsgStart);

    log.debug("Wrote " + numReviewItemsAdded + " review items.");

    if (simulateFailure)
    {
      throw new Exception("Simulated test review data parse failure.");
    }

    return reviewRecordWritten;
  }

  private void flushIdMappingRecords(List<ElementIdMappings> elementIdMappingRecordsToInsert,
	  final int threshold, final String logMsgStart) throws Exception
	{
		if (elementIdMappingRecordsToInsert.size() > 0 && 
				elementIdMappingRecordsToInsert.size() >= threshold)
		{
			// flush
			uniqueIdsParsed += elementIdMappingRecordsToInsert.size();
			writeIdMappingRecords(elementIdMappingRecordsToInsert);
			idMappingRecordWritten = true;
			// the best that can be done here is a log statement, I think, for now...trying to update the 
			// job status causes issues
			if (totalParseableRecords > 0 && uniqueIdsParsed > 0) // sanity check
			{
				// the progress percentage complete message is incorrect
				log.info(logMsgStart + " - "
				    + String.valueOf((uniqueIdsParsed / totalParseableRecords) * 100)
				    + "% complete.");
			}
		}
	}

  private boolean reviewPairAlreadyParsed(final String reviewableItemId, 
		final String reviewAgainstItemId)
	{
		if (reviewAgainstItemId != null)
		{
			return (reviewableItemIdToReviewAgainstItemIds .containsKey(reviewableItemId) && 
			        reviewableItemIdToReviewAgainstItemIds
			          .get(reviewableItemId).contains(reviewAgainstItemId)) || 
			       (reviewableItemIdToReviewAgainstItemIds.containsKey(reviewAgainstItemId) && 
			      	reviewableItemIdToReviewAgainstItemIds
			          .get(reviewAgainstItemId).contains(reviewableItemId));
		}
		else
		{
			return reviewableItemIdToReviewAgainstItemIds.containsKey(reviewableItemId);
		}
	}

  private void flushReviewRecords(List<ReviewItems> reviewRecordsToInsert,
	  final int threshold, final String logMsgStart) throws Exception
	{
		if (reviewRecordsToInsert.size() > 0 && reviewRecordsToInsert.size() >= threshold)
		{
			// flush
			reviewRecordsParsed += reviewRecordsToInsert.size();
			writeReviewRecords(reviewRecordsToInsert);
			reviewRecordWritten = true;
			// the best that can be done here is a log statement, I think, for
			// now...trying
			// to update the job status causes issues
			if (totalReviewableRecords > 0 && reviewRecordsParsed > 0) // sanity check
			{
				// the progress percentage complete message is incorrect
				log.info(logMsgStart
				    + " - "
				    + String
				        .valueOf((reviewRecordsParsed / totalReviewableRecords) / 100)
				    + "% complete.");
			}
		}
	}

	private void writeIdMappingRecords(List<ElementIdMappings> elementIdMappingRecordsToInsert) 
	  throws Exception
	{
		DbUtils.batchRecords(
			mapId, elementIdMappingRecordsToInsert, elementIdMappings, null, RecordBatchType.INSERT, conn,
		  maxRecordBatchSize);
		elementIdMappingRecordsToInsert.clear();
	}

	private void writeReviewRecords(List<ReviewItems> reviewRecordsToInsert) throws Exception
	{
		DbUtils.batchRecords(
			mapId, reviewRecordsToInsert, reviewItems, null, RecordBatchType.INSERT, conn, 
			maxRecordBatchSize);
		reviewRecordsToInsert.clear();
	}

	private void deleteExistingUnreviewedItems(final long mapId) throws Exception
	{
		final String logMsgStart = 
			"Deleting existing unreviewed records for map with ID: " + mapId + ".  Step 1 of 4.";
		log.info(logMsgStart);

		// clear out the unreviewed records from review_items table first, since it depends on 
		// element_id_mappings
		final long count = 
			new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)
		    .where(
		      reviewItems.mapId.eq(mapId).and(
		        reviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)))
		    .count();
		// TODO: does this need to be a buffered query ??
		final long result = 
		  new SQLDeleteClause(conn,
		    DbUtils.getConfiguration(mapId), reviewItems)
		    .where(
		        reviewItems.mapId.eq(mapId).and(
		          reviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed))).execute();

		if (result != count)
		{
			throw new Exception(
			  "Error deleting existing unreviewed data from review items table during review prepare job.");
		}
	}

	private void removeUnusedElementIdMappings() throws Exception
	{
		final String logMsgStart = 
		"Removing unused ID mappings for map with ID: " + mapId + ".  Step 4 of 4.";
		log.info(logMsgStart);

		// get all unique reviewable item ids in review_items
		// TODO: these need to be buffered queries
		log.debug("Getting reviewable item IDs...");
		final Set<String> reviewableItemIds = 
			new HashSet<String>(
			  new SQLQuery(conn, DbUtils.getConfiguration(mapId))
			    .from(reviewItems)
		      .where(reviewItems.mapId.eq(mapId))
		      .list(reviewItems.reviewableItemId));

		log.debug("Getting review against item IDs...");
		final Set<String> reviewAgainstItemIds = 
		  new HashSet<String>(
		    new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		      .from(reviewItems)
		      .where(reviewItems.mapId.eq(mapId))
		      .list(reviewItems.reviewAgainstItemId));

		log.debug("Getting element unique IDs...");
		final Set<String> elementUniqueIds = 
			new HashSet<String>(
		    new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		      .from(elementIdMappings)
		      .where(elementIdMappings.mapId.eq(mapId))
		      .list(elementIdMappings.elementId));
		
		// anything in elementUniqueIds that's not in reviewableItemIds or reviewAgainstItemIds, 
		// regardless of what map it belongs to, must be a unique id not being used and should be
		// deleted
		log.debug("Determining unused element IDs...");
		Set<String> uniqueIdsNotInReviewItems = new HashSet<String>(elementUniqueIds);
		uniqueIdsNotInReviewItems.removeAll(reviewableItemIds);
		uniqueIdsNotInReviewItems.removeAll(reviewAgainstItemIds);
		log.debug("Unused element IDs determined.");
		if (uniqueIdsNotInReviewItems.size() > 0)
		{
			log.debug("Retrieving unique IDs not in review items...");
			final long result =
			  new SQLDeleteClause(conn, DbUtils.getConfiguration(mapId), elementIdMappings)
			    .where(elementIdMappings.elementId
			      .in(uniqueIdsNotInReviewItems)
			      .and(elementIdMappings.mapId.eq(mapId)))
			    .execute();
			if (result != uniqueIdsNotInReviewItems.size())
			{
				log.warn("The number of unique IDs not in review items calculated does not " +
				  "match the number deleted from the database.");
			}
			log.debug(result + " redundant element ID mappings deleted.");
		}
		else
		{
			log.debug("No redundant element ID mappings found.");
		}
	}
}
