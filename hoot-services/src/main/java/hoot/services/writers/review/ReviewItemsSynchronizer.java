package hoot.services.writers.review;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.ReviewItems;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.review.ReviewUtils;
import hoot.services.utils.XmlUtils;
import hoot.services.validators.review.ReviewMapValidator;

import java.lang.reflect.InvocationTargetException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.xml.transform.TransformerException;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.DOMException;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.ListMultimap;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.types.expr.BooleanExpression;

/**
 * Responsible for maintaining review SQL data consistency based on saved OSM changesets
 */
public class ReviewItemsSynchronizer
{
  private static final Logger log = LoggerFactory.getLogger(ReviewItemsSynchronizer.class);
  
  private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
  
  private long mapId;
  private Connection conn;
  private int maxRecordBatchSize;
  private String[] reviewTags;
	
  public ReviewItemsSynchronizer(final Connection conn, final String mapId) throws Exception
  {
    this.conn = conn;
    this.mapId = Long.parseLong(mapId);
    maxRecordBatchSize = 
  		Integer.parseInt(HootProperties.getInstance()
  		  .getProperty("maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
    String reviewTagsStr = 
  		HootProperties.getInstance()
  		  .getProperty("reviewTags", HootProperties.getDefault("reviewTags"));
    if (reviewTagsStr.contains(";"))
    {
    	reviewTags = reviewTagsStr.split(";");
    }
    else
    {
    	reviewTags = new String[1];
    	reviewTags[0] = reviewTagsStr;
    }
  }
  
  private String[] reviewAgainstUuidsFromChangesetElement(final org.w3c.dom.Node elementXml) 
  	throws DOMException, TransformerException
  {
  	String[] reviewAgainstUuids = null;
  	final org.w3c.dom.Node nodeXml = 
  		XPathAPI.selectSingleNode(elementXml, "tag[@k = 'hoot:review:uuid']/@v");
  	if (nodeXml != null)
  	{
  		final String reviewAgainstUuidStr = nodeXml.getNodeValue();
    	if (StringUtils.trimToNull(reviewAgainstUuidStr) != null)
    	{
    		if (reviewAgainstUuidStr.contains(";"))
    		{
    			reviewAgainstUuids = reviewAgainstUuidStr.split(";");
    		}
    		else
    		{
    			reviewAgainstUuids = new String[1];
    			reviewAgainstUuids[0] = reviewAgainstUuidStr;
    		}
    	}
  	}
  	return reviewAgainstUuids;
  }
  
  private void updateReviewRecordsFromCreateChangeset(final Document changesetDoc,
    final Map<ElementType, HashMap<Long, Element>> parsedElementIdsToElementsByType) 
    throws Exception
  {
  	log.debug("updateReviewRecordsFromCreateChangeset");
  	
  	int numReviewItemsUpdated = 0;
  	long numElementIdMappingRecordsInserted = 0;
  	List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
    List<ReviewItems> reviewItemRecordsToInsert = new ArrayList<ReviewItems>();
  	
    //We're only concerned with elements having a uuid tag.  If they don't have that tag they aren't
    //involved in a review.
  	final NodeList createdElements = 
  		XPathAPI.selectNodeList(changesetDoc, "//osmChange/create/*/tag[@k = 'uuid']/..");
  	log.debug("createdElements: " + createdElements.getLength());
  	if (createdElements.getLength() > 0)
  	{
  		log.debug(XmlUtils.nodeListToString(createdElements));
  	  //Create element id mappings for every record, regardless if its involved in a review.  This
    	//will result in unused records, but makes the code simpler overall.  We're making a big 
    	//assumption here that any review against items already have an element id mapping record entry...
    	//which should be the case.
    	for (int i = 0; i < createdElements.getLength(); i++)
    	{
    		final org.w3c.dom.Node elementXml = createdElements.item(i);
    		final String uuid = 
    			XPathAPI.selectSingleNode(elementXml, "tag[@k = 'uuid']/@v").getNodeValue();
    		assert(StringUtils.trimToNull(uuid) != null);
    		final long changesetOsmElementId = 
    			Long.parseLong(elementXml.getAttributes().getNamedItem("id").getNodeValue());
    		final ElementType elementType = Element.elementTypeFromString(elementXml.getNodeName());
    		final long actualOsmElementId = 
    			parsedElementIdsToElementsByType.get(elementType).get(changesetOsmElementId).getId();
    		elementIdMappingRecordsToInsert.add(
    			ReviewUtils.createElementIdMappingRecord(
    				uuid, 
    				actualOsmElementId, 
    				elementType, 
    				mapId));
    	}
    	
    	//check create changeset for any newly created reviewable items
    	final NodeList createdReviewItems = 
    	  XPathAPI.selectNodeList(
    	    changesetDoc, 
          "//osmChange/create/*/tag[@k = 'hoot:review:needs' and @v = 'yes']/..");
    	log.debug("created review items: " + String.valueOf(createdReviewItems.getLength()));
    	log.debug(StringUtils.abbreviate(XmlUtils.nodeListToString(createdReviewItems), 1000));
    	for (int i = 0; i < createdReviewItems.getLength(); i++)
    	{
    		//add the associated review data; not checking to see if the element already exists in the
    		//review data, b/c it shouldn't
    		final org.w3c.dom.Node elementXml = createdReviewItems.item(i);
    		final String uuid = 
    			XPathAPI.selectSingleNode(elementXml, "tag[@k = 'uuid']/@v").getNodeValue();
    		assert(StringUtils.trimToNull(uuid) != null);
    		final String[] reviewAgainstUuids = reviewAgainstUuidsFromChangesetElement(elementXml);
    		if (reviewAgainstUuids != null)
    		{
      		for (int j = 0; j < reviewAgainstUuids.length; j++)
      		{
      			final String reviewAgainstUuid = reviewAgainstUuids[j];
      			if (StringUtils.trimToNull(reviewAgainstUuid) != null)
      			{
      				reviewItemRecordsToInsert.add(
        	  		ReviewUtils.createReviewItemRecord(
        	  			uuid, 
        	  		  //TODO: the way to retrieve the correct score would be to trace back the elements 
        	  			//that made up this (presumably) merged element...although you could argue that the 
        	  			//old score is obsolete after a merge; this is possibly complicated, so holding off on 
        	  			//doing fixing this for now
        	  			1.0,
        	  			reviewAgainstUuid, 
        	  			mapId));
      			}
      		}
    		}
    		else
    		{
    			//adding the item here as reviewed if it has no review against id's left for bookkeeping 
    			//purposes
    			ReviewItems reviewItemRecord = 
    				ReviewUtils.createReviewItemRecord(
  	  			  uuid, 
  	  			  1.0, //TODO: see explanation above
  	  			  null, 
  	  			  mapId);
    			reviewItemRecord.setReviewStatus(DbUtils.review_status_enum.reviewed);
    			reviewItemRecordsToInsert.add(reviewItemRecord);
    		}
    	}
    	
    	numElementIdMappingRecordsInserted =
    		DbUtils.batchRecords(
      	  mapId, elementIdMappingRecordsToInsert, elementIdMappings, null, RecordBatchType.INSERT, 
      	  conn, maxRecordBatchSize);
    	numReviewItemsUpdated += 
    	  DbUtils.batchRecords(
      	  mapId, reviewItemRecordsToInsert, reviewItems, null, RecordBatchType.INSERT, conn, 
      	  maxRecordBatchSize);
  	}
  	
  	log.debug(numElementIdMappingRecordsInserted + 
  		" element id mappings records inserted as a result of the create changeset.");
  	log.debug(numReviewItemsUpdated + " review records inserted as a result of the create changeset.");
  }
  
  private void updateReviewRecordsFromModifyChangeset(final Document changesetDoc) throws Exception
  {
    log.debug("updateReviewRecordsFromModifyChangeset");
    
    int numReviewItemsInserted = 0;
    int numReviewItemsUpdated = 0;
    
    //get a list of all the uuid's in the modify changeset (both types: reviewable and review 
    //against); We're only concerned with elements having a uuid tag.  If they don't have that tag 
    //they were never involved in a review at any time.
    final NodeList modifiedItems =
      XPathAPI.selectNodeList(changesetDoc, "//osmChange/modify/*/tag[@k = 'uuid']/..");
    log.debug("modifiedItems: " + modifiedItems.getLength());
    if (modifiedItems.getLength() > 0)
    {
    	List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
      List<ReviewItems> reviewRecordsToInsert = new ArrayList<ReviewItems>();
      List<ReviewItems> reviewRecordsToUpdate = new ArrayList<ReviewItems>();
    	
    	Map<String, org.w3c.dom.Node> changesetReviewableUuidsToXml = 
      	new HashMap<String, org.w3c.dom.Node>();
      Set<String> allChangesetUniqueIds = new HashSet<String>();
      for (int i = 0; i < modifiedItems.getLength(); i++)
      {
        final org.w3c.dom.Node elementXml = modifiedItems.item(i);
        final String uuid =
          XPathAPI.selectSingleNode(elementXml, "tag[@k = 'uuid']/@v").getNodeValue();
        assert(StringUtils.trimToNull(uuid) != null);
        changesetReviewableUuidsToXml.put(uuid, elementXml);
        allChangesetUniqueIds.add(uuid);
        final String[] reviewAgainstUuids = reviewAgainstUuidsFromChangesetElement(elementXml);
        if (reviewAgainstUuids != null)
        {
          for (int j = 0; j < reviewAgainstUuids.length; j++)
          {
          	final String reviewAgainstUuid = reviewAgainstUuids[j];
          	if (StringUtils.trimToNull(reviewAgainstUuid) != null)
          	{
          		allChangesetUniqueIds.add(reviewAgainstUuid);
          	}
          }
        }
      }
      
      //query out all review records corresponding to the changeset elements; store in a map
      //not inefficient, b/c we're only retrieving the info for records in the modify
      //changeset, which should typically not be too large
      List<ReviewItems> allExistingReviewRecordsFromChangeset = new ArrayList<ReviewItems>();
      if (changesetReviewableUuidsToXml.size() > 0)
      {
        //Need to make queries in both directions here (reviewable/review against), since the review
      	//tags are store reflexively (tags specify both "review A against B" and "review B against 
      	//A", while the database records are not reflexive to avoid showing duplicate reviews to
      	//users.  The client may be passing in multiple tag updates for the
    		//same review (reviewable and review against item review tags), and we don't want that to 
      	//cause a new redundant review record to be added.
      	allExistingReviewRecordsFromChangeset = 
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId))
            .from(reviewItems)
            .where(
            	reviewItems.mapId.eq(mapId)
            	.and(
        	      reviewItems.reviewableItemId.in(changesetReviewableUuidsToXml.keySet())
        	        .or(reviewItems.reviewAgainstItemId.in(changesetReviewableUuidsToXml.keySet()))))
            .list(reviewItems);
      }
      Map<String, ReviewItems> dbReviewableUuidsToReviewRecords = new HashMap<String, ReviewItems>();
      ListMultimap<String, String> dbReviewableUuidsToReviewAgainstUuids = ArrayListMultimap.create();
      Map<String, String> dbReviewAgainstUuidsToReviewableUuids = new HashMap<String, String>();
      for (ReviewItems reviewItem : allExistingReviewRecordsFromChangeset)
      {
      	if (StringUtils.trimToNull(reviewItem.getReviewableItemId()) != null &&
      			StringUtils.trimToNull(reviewItem.getReviewAgainstItemId()) != null)
      	{
      		dbReviewableUuidsToReviewRecords.put(
        		reviewItem.getReviewableItemId() + ";" + reviewItem.getReviewAgainstItemId(), reviewItem);
      	  //see note above for why a map is kept both reviewable id --> review against --> id
      		//and vice versa
        	dbReviewableUuidsToReviewAgainstUuids.put(
        		reviewItem.getReviewableItemId(), reviewItem.getReviewAgainstItemId());
        	dbReviewAgainstUuidsToReviewableUuids.put(
          	reviewItem.getReviewAgainstItemId(), reviewItem.getReviewableItemId());
      	}
      }
      
      //we need this to know whether a new element id mapping record needs to be created or not; 
      //not inefficient for the reasons listed above regarding allExistingReviewRecordsFromChangeset
      final List<String> allExistingElementIdMappingUuidsFromChangeset =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
          .from(elementIdMappings)
          .where(
          	elementIdMappings.elementId.in(allChangesetUniqueIds)
              .and(elementIdMappings.mapId.eq(mapId)))
          .list(elementIdMappings.elementId);
      
      //for each reviewable element in changeset
      for (Map.Entry<String, org.w3c.dom.Node> nodeEntry : changesetReviewableUuidsToXml.entrySet())
      {
      	final org.w3c.dom.Node elementXml = nodeEntry.getValue();
      	final String uuid =
          XPathAPI.selectSingleNode(elementXml, "tag[@k = 'uuid']/@v").getNodeValue();
      	assert(StringUtils.trimToNull(uuid) != null);
      	
      	//create element id mapping for reviewable in case it doesn't already exist (possible if it
      	//was created by a post conflate db dump and wasn't involved in a review at that time)
      	if (!allExistingElementIdMappingUuidsFromChangeset.contains(uuid))
      	{
      		elementIdMappingRecordsToInsert.add(
      			ReviewUtils.createElementIdMappingRecord(
      				uuid, 
      				Long.parseLong(elementXml.getAttributes().getNamedItem("id").getNodeValue()), 
      				Element.elementTypeFromString(elementXml.getNodeName()), 
      				mapId));
      	}
      	
        final String[] reviewAgainstUuids = reviewAgainstUuidsFromChangesetElement(elementXml);
        if (reviewAgainstUuids != null)
        {
        	//for each review against id it has
          for (int j = 0; j < reviewAgainstUuids.length; j++)
          {
          	final String reviewAgainstUuid = reviewAgainstUuids[j];
          	//if a corresponding record for the reviewable/review against doesn't exist AND its 
          	//inverse record doesn't exist, create a new record; if either exists, do nothing 
          	//(leave existing records unreviewed)
          	if (StringUtils.trimToNull(reviewAgainstUuid) != null && 
          			!dbReviewableUuidsToReviewRecords.containsKey(uuid + ";" + reviewAgainstUuid) &&
          			!dbReviewableUuidsToReviewRecords.containsKey(reviewAgainstUuid + ";" + uuid))
          	{
          		//create an insert record
          		reviewRecordsToInsert.add(
                ReviewUtils.createReviewItemRecord(
                  uuid,
                  1.0, //see comment in updateCreatedReviewItems
                  reviewAgainstUuid,
                  mapId));
          		
          		//TODO: what about review against records that may not have an element id mapping yet?
          		//i.e. they were created during initial conflate db dump and weren't part of a review at
          		//that time, but now are involved in one. - The way the client logic is set up, the only
          		//way this could possibly happen is if a user mucked with the review tags, b/c neither 
          		//deleting, merging, nor resolving reviews should result in that happening.  There is
          		//a ticket to prevent this from happening.  I would just go ahead add the mapping here, 
          		//but its potentially going to take extra querying to get the review against items osm 
          		//id's.  Will just add an assert for this instead.
          		assert(allExistingElementIdMappingUuidsFromChangeset.contains(reviewAgainstUuid));
          	}
          }
          
          //for each record where the record has an existing db review against uuid that isn't in 
          //the set of parsed review against id's from the changeset, create an update record for 
          //it marked as reviewed
          List<String> existingReviewAgainstIds = dbReviewableUuidsToReviewAgainstUuids.get(uuid);
          List<String> reviewAgainstUuidsList = Arrays.asList(reviewAgainstUuids);
          for (String id : existingReviewAgainstIds)
          {
          	if (!reviewAgainstUuidsList.contains(id) && 
          			//If the record doesn't already exist the we can't update it here.
          			dbReviewableUuidsToReviewRecords.containsKey(uuid + ";" + id))
          	{

          		ReviewItems reviewRecord = 
            		ReviewUtils.createReviewItemRecord(
                  uuid,
                  1.0, //TODO: see comment in updateCreatedReviewItems
                  id,
                  mapId);
            	reviewRecord.setReviewStatus(DbUtils.review_status_enum.reviewed);
            	reviewRecordsToUpdate.add(reviewRecord);
          	}
          }
        }
        else
        {
        	//all reviews are gone, so update all records for the reviewable as reviewed
        	List<String> existingReviewAgainstIds = dbReviewableUuidsToReviewAgainstUuids.get(uuid);
          for (String id : existingReviewAgainstIds)
          {
            //If the record doesn't already exist the we can't update it here.
      			if (dbReviewableUuidsToReviewRecords.containsKey(uuid + ";" + id))
          	{
          		ReviewItems reviewRecord = 
            		ReviewUtils.createReviewItemRecord(
                  uuid,
                  1.0, //TODO: see comment in updateCreatedReviewItems
                  id,
                  mapId);
            	reviewRecord.setReviewStatus(DbUtils.review_status_enum.reviewed);
            	reviewRecordsToUpdate.add(reviewRecord);
          	}
          }
        }
      }
      
      //Technically, we also should go through and clean element ID mappings records that are no longer
      //in use b/c they aren't involved in reviews, but that seems difficult and they aren't hurting
      //anything by being in the database and not being used...
      
      numReviewItemsInserted += 
    	  DbUtils.batchRecords(
      	  mapId, reviewRecordsToInsert, reviewItems, null, RecordBatchType.INSERT, 
      	  conn, maxRecordBatchSize);
      
      List<List<BooleanExpression>> predicatelist = new ArrayList<List<BooleanExpression>>();
      for (int i = 0; i < reviewRecordsToUpdate.size(); i++)
      {
        List<BooleanExpression> predicates = new ArrayList<BooleanExpression>();
        predicates.add(reviewItems.mapId.eq(mapId));
        predicates.add(
          reviewItems.reviewableItemId.eq(
            reviewRecordsToUpdate.get(i).getReviewableItemId()));
        predicates.add(
          reviewItems.reviewAgainstItemId.eq(
          	reviewRecordsToUpdate.get(i).getReviewAgainstItemId()));
        predicatelist.add(predicates);
      }
    	numReviewItemsUpdated += 
    	  DbUtils.batchRecords(
      	  mapId, reviewRecordsToUpdate, reviewItems, predicatelist, RecordBatchType.UPDATE, 
      	  conn, maxRecordBatchSize);
    }
    
    log.debug(
    	numReviewItemsInserted + " review records inserted as a result of the modify changeset.");
    log.debug(
    	numReviewItemsUpdated + " review records updated as a result of the modify changeset.");
  }
  
  //This code is heavily tailored for the POI merge use case and has only been tested against that
  //so far.  Not sure what affect this code would have on other types of reviews yet...
  private void updateReviewRecordsFromDeleteChangeset(final Document changesetDoc) throws Exception
  {
  	int numReviewItemsUpdated = 0;
  	int numReviewItemsDeleted = 0;
  	
  	final NodeList deletedItems = 
  	  XPathAPI.selectNodeList(changesetDoc, "//osmChange/delete/*/tag[@k = 'uuid']/@v");
  	List<String> deletedItemUuids = new ArrayList<String>();
  	for (int i = 0; i < deletedItems.getLength(); i++)
  	{
  		final String deletedItemUuid = deletedItems.item(i).getNodeValue();
  		if (StringUtils.trimToNull(deletedItemUuid) != null)
  		{
  			deletedItemUuids.add(deletedItems.item(i).getNodeValue());
  		}
  	}

  	if (deletedItemUuids.size() > 0)
  	{
  	  //If a record is made up entirely of deleted items (reviewable and review against)...like 
  		//when two are deleted as the result of a merge...then mark the record as reviewed.  This is 
  		//kind of a really big assumption.  Just b/c two features that were part of the same review 
  		//were deleted doesn't mean they were ever actually reviewed, but there doesn't seem to be 
  		//any better way to handle this.
  		numReviewItemsUpdated += 
  			new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), reviewItems)
		      .where(
		      	reviewItems.mapId.eq(mapId)
		      	  .and(reviewItems.reviewableItemId.in(deletedItemUuids)
		  	        .and(reviewItems.reviewAgainstItemId.in(deletedItemUuids))))
		  	  .set(reviewItems.reviewStatus, DbUtils.review_status_enum.reviewed)
		      .execute();
  	  //Otherwise, delete the review record, b/c the client should have handled fixing the 
  		//orphaned record links already.  In other words, the review never happened due to it being
  		//OBE by the poi merge, record links were updated to reflect that with new review records,
  		//and these records should then be deleted.  Here we're deleting all records where one of the 
  		//uuid's is in the reviewableItemId or the reviewAgainstItemId, but not both.  This is also
  		//a fairly big assumption, but once again, not sure of a better way to handle it right now.
  		numReviewItemsDeleted += 
  			new SQLDeleteClause(conn, DbUtils.getConfiguration(mapId), reviewItems)
		      .where(
		      	reviewItems.mapId.eq(mapId)
		      	  .and(
		      	  	reviewItems.reviewableItemId.in(deletedItemUuids)
		  	          .and(reviewItems.reviewAgainstItemId.notIn(deletedItemUuids))
		  	        .or(
		  	      	  reviewItems.reviewAgainstItemId.in(deletedItemUuids)
		  	      	    .and(reviewItems.reviewableItemId.notIn(deletedItemUuids)))))
		      .execute();
  	}  	
  	
  	log.debug(numReviewItemsUpdated + " review records updated as a result of the delete changeset.");
  	log.debug(numReviewItemsDeleted + " review records deleted as a result of the delete changeset.");
  }
   
  /**
   * Synchronizes review table data based on OSM changeset input
   * 
   * @param changesetDoc OSM changeset
   * @param parsedElementIdsToElementsByType mapping of element ID's passed in the changeset to
   * actual element ID's stored in the database
   * @return the number of review records updated
   * @throws Exception 
   */
  public void updateReviewItems(final Document changesetDoc, 
    final Map<ElementType, HashMap<Long, Element>> parsedElementIdsToElementsByType) 
    throws Exception
  { 
  	if ((new ReviewMapValidator(conn)).mapIsPrepared(String.valueOf(mapId)))
  	{
      log.debug("Updating review items for changeset...");
      
      //Items in the create changeset are passed in with temporary id's that the server replaces.  
      //So, we need to pass the id mapping data structure to this method so it has the correct ID's 
      //without having to do extra database queries for them.
    	updateReviewRecordsFromCreateChangeset(changesetDoc, parsedElementIdsToElementsByType);
    	updateReviewRecordsFromModifyChangeset(changesetDoc);
    	updateReviewRecordsFromDeleteChangeset(changesetDoc);
  	}
  	else
  	{
  		log.debug(
  		  "No review data prepared for map with ID: " + String.valueOf(mapId) + ".  Skipping " +
  		  "review record synchronization.");
  	}
  }
  
  /**
   * Updates all features for the given map as reviewed
   * 
   * @throws SQLException
   * @throws InstantiationException
   * @throws IllegalAccessException
   * @throws ClassNotFoundException
   * @throws NoSuchMethodException
   * @throws InvocationTargetException
   */
  public void setAllItemsReviewed() throws SQLException, InstantiationException, 
    IllegalAccessException, ClassNotFoundException, NoSuchMethodException, InvocationTargetException
  {
    //set all review records for this map ID to reviewed
  	new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), reviewItems)
      .set(reviewItems.reviewStatus, DbUtils.review_status_enum.reviewed)
      .execute();
  	
  	//drop all review tags from all elements in the osm table
  	long numReviewTagsDropped = 0;
  	long numElementVersionIncrements = 0;
		for (ElementType elementType : ElementType.values())
		{
			if (!elementType.equals(ElementType.Changeset))
			{
				final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);
				final String tableName = prototype.getElementTable().getTableName() + "_" + mapId;
				
			  //The element version update sql makes the assumption that all elements have the same set
				//of review keys, which should always be true.
				final String elementVersionIncrementSql = 
					"update " + tableName + 
					" set version = version + 1" +
					" where " + "EXIST(tags, 'hoot:review:needs') = TRUE";
				final String reviewTagDropSql = 
					"update " + tableName + 
					" set tags = delete(tags, (?))" +
					" where " + "EXIST(tags, (?)) = TRUE";
				Statement elementVersionIncrementStatement = null;
				PreparedStatement reviewTagDropStatement = null;
				try
				{
					elementVersionIncrementStatement = conn.createStatement();
					numElementVersionIncrements += 
						elementVersionIncrementStatement.executeUpdate(elementVersionIncrementSql);
					log.debug("numElementVersionIncrements: " + numElementVersionIncrements);
					
					reviewTagDropStatement = conn.prepareStatement(reviewTagDropSql);
					for (int i = 0; i < reviewTags.length; i++)
					{
						final String reviewTag = reviewTags[i];
						Class.forName("org.postgresql.Driver");
						reviewTagDropStatement.setString(1, reviewTag);
						reviewTagDropStatement.setString(2, reviewTag);
						reviewTagDropStatement.addBatch();
					}
					final int[] resultCounts = reviewTagDropStatement.executeBatch();
					for (int i = 0; i < resultCounts.length; i++)
					{
						final int resultCount = resultCounts[i];
						log.debug("numReviewTagsDropped: " + resultCount);
						numReviewTagsDropped += resultCount;
					}
				}
				catch (Exception e)
				{
					log.error("Error setting review tags reviewed: " + e.getMessage());
					throw e;
				}
				finally
				{
					try
					{
						if (reviewTagDropStatement != null) reviewTagDropStatement.close();
						if (elementVersionIncrementStatement != null) elementVersionIncrementStatement.close();
					}
					catch (SQLException se2)
					{
						log.error(se2.getMessage());
					}
				}
			}
		}
		
		log.debug("total numReviewTagsDropped: " + numReviewTagsDropped);
		log.debug("total numElementVersionIncrements: " + numElementVersionIncrements);
  }
}
