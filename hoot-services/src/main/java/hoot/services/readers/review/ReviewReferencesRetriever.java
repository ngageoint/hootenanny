package hoot.services.readers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.models.osm.Element;
import hoot.services.models.review.ReviewAgainstItem;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

/**
 * Retrieves all other element references to reviews for an element
 */
public class ReviewReferencesRetriever
{
	private static final Logger log = LoggerFactory.getLogger(ReviewReferencesRetriever.class);
	
	private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
	
	private Connection conn;
	
  public ReviewReferencesRetriever(final Connection conn)
  {
  	this.conn = conn;
  }
	
  /**
   * Retrieves all other element references to reviews for an element
   * 
   * @param mapId map owning the feature passed in
   * @param elementId OSM ID of the element to be retrieved
   * @param elementType OSM type of the element to be retrieved
   * @return a list containing a list of all features the input feature was to be reviewed against
   * in the first position and a list of all features that were to be reviewed against the feature
   * in the second position
   */
	public List<List<ReviewAgainstItem>> getReferences(final long mapId, final long elementId, 
	  	final String elementType)
  {
  	log.debug("mapId: " + mapId);
		log.debug("elementId: " + elementId);
		log.debug("elementType: " + elementType);
		List<List<ReviewAgainstItem>> references = new ArrayList<List<ReviewAgainstItem>>();
		
		final String elementUniqueId = 
			new SQLQuery(conn, DbUtils.getConfiguration(mapId))
        .from(elementIdMappings)
	      .where(
	    	  elementIdMappings.mapId.eq(mapId)
	    	  .and(elementIdMappings.osmElementId.eq(elementId))
	    	  .and(elementIdMappings.osmElementType.eq(Element.elementEnumForString(elementType))))
	      .singleResult(elementIdMappings.elementId);
		log.debug("elementUniqueId: " + elementUniqueId);
		
		List<ReviewAgainstItem> reviewAgainstItems = new ArrayList<ReviewAgainstItem>();
		//get all review against items that are referenced by the input elements; since
		//the review table data breaks up one to many reviewable uuid to review against uuid
		//relationships, we don't have to break up the review against uuid's list here
		final List<String> reviewAgainstItemUniqueIds =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)
  		  .where(
  		  	reviewItems.reviewableItemId.eq(elementUniqueId)
  		  	  .and(reviewItems.mapId.eq(mapId)))
  		  .list(reviewItems.reviewAgainstItemId);
		if (reviewAgainstItemUniqueIds.size() > 0)
		{
			final List<ElementIdMappings> reviewAgainstItemRecords =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    		  .from(elementIdMappings)
      		.where(
      			elementIdMappings.elementId.in(reviewAgainstItemUniqueIds)
      			  .and(elementIdMappings.mapId.eq(mapId)))
      		.list(elementIdMappings);
  		for (ElementIdMappings itemRecord : reviewAgainstItemRecords)
  		{
  			ReviewAgainstItem item = new ReviewAgainstItem();
  			item.setId(itemRecord.getOsmElementId());
  			item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
  			item.setUuid(itemRecord.getElementId());
  			reviewAgainstItems.add(item);
  		}
		}
		references.add(reviewAgainstItems);
		
		List<ReviewAgainstItem> reviewableItems = new ArrayList<ReviewAgainstItem>();
    //add all items which reference the input features as a review against item
		final List<String> reviewableItemUniqueIds =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)
  		  .where(
  		  	reviewItems.reviewAgainstItemId.eq(elementUniqueId)
  		  	  .and(reviewItems.mapId.eq(mapId)))
  		  .list(reviewItems.reviewableItemId);
		if (reviewableItemUniqueIds.size() > 0)
		{
			final List<ElementIdMappings> reviewableItemRecords =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    		  .from(elementIdMappings)
      		.where(
      			elementIdMappings.elementId.in(reviewableItemUniqueIds)
      			  .and(elementIdMappings.mapId.eq(mapId)))
      		.list(elementIdMappings);
  		for (ElementIdMappings itemRecord : reviewableItemRecords)
  		{
  			//returning a review against item here, instead of a reviewable item, b/c the review against
  			//item has all of the info that's needed to return the references, whereas ReviewableItem
  			//has a lot of extra unneeded info...client won't know the different between the two anyway
  			ReviewAgainstItem item = new ReviewAgainstItem();
  			item.setId(itemRecord.getOsmElementId());
  			item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
  			item.setUuid(itemRecord.getElementId());
  			reviewableItems.add(item);
  		}
		}
		references.add(reviewableItems);
		
		return references;
  }
	
}
