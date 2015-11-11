package hoot.services.readers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.validators.review.ReviewMapValidator;

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
	private long mapId;
	
  public ReviewReferencesRetriever(final Connection conn, final String mapId) throws Exception
  {
  	this.conn = conn;
  	this.mapId = (new ReviewMapValidator(conn).verifyMapPrepared(mapId));
  }
	
  /**
   * Retrieves all other element references to reviews for an element
   * 
   * @param elementUniqueId unique ID of the element whose review references are to be retrieved
   * @return a list containing a list of all features the input feature was to be reviewed against
   * in the first position and a list of all features that were to be reviewed against the feature
   * in the second position
   */
	public List<List<ReviewAgainstItem>> getReferences(final String elementUniqueId)
  {
		log.debug("mapId: " + mapId);
  	log.debug("elementUniqueId: " + elementUniqueId);
		List<List<ReviewAgainstItem>> references = new ArrayList<List<ReviewAgainstItem>>();
		List<String> idsParsed = new ArrayList<String>();
		
		List<ReviewAgainstItem> reviewAgainstItems = new ArrayList<ReviewAgainstItem>();
		//get all review against items that are referenced by the input elements; since
		//the review table data breaks up one to many reviewable uuid to review against uuid
		//relationships, we don't have to break up the review against uuid's list here
		final List<ElementIdMappings> reviewAgainstItemRecords =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)
		    .join(elementIdMappings)
		    .on(
		    	reviewItems.reviewAgainstItemId.eq(elementIdMappings.elementId)
      			.and(elementIdMappings.mapId.eq(mapId)))
  		  .where(
  		  	reviewItems.reviewableItemId.eq(elementUniqueId)
  		  	.and(reviewItems.mapId.eq(mapId)))
  		  .orderBy(reviewItems.reviewId.asc())
  		  .list(elementIdMappings);
		for (ElementIdMappings itemRecord : reviewAgainstItemRecords)
		{
			if (!idsParsed.contains(itemRecord.getElementId()))
			{
				ReviewAgainstItem item = new ReviewAgainstItem();
				item.setId(itemRecord.getOsmElementId());
				item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
				item.setUuid(itemRecord.getElementId());
				reviewAgainstItems.add(item);
				idsParsed.add(itemRecord.getElementId());
			}	
		}
		idsParsed.clear();
		references.add(reviewAgainstItems);
		
		List<ReviewAgainstItem> reviewableItems = new ArrayList<ReviewAgainstItem>();
    //add all items which reference the input features as a review against item
		final List<ElementIdMappings> reviewableItemRecords =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
		    .from(reviewItems)  //was using distinct here but ordering was giving me problems when using it
		    .join(elementIdMappings)
		    .on(
		    	reviewItems.reviewableItemId.eq(elementIdMappings.elementId)
      			.and(elementIdMappings.mapId.eq(mapId)))
  		  .where(
  		  	reviewItems.reviewAgainstItemId.eq(elementUniqueId)
  		  	  .and(reviewItems.mapId.eq(mapId)))
  		  .orderBy(reviewItems.reviewId.asc())
  		  .list(elementIdMappings);
		for (ElementIdMappings itemRecord : reviewableItemRecords)
		{
			if (!idsParsed.contains(itemRecord.getElementId()))
			{
				ReviewAgainstItem item = new ReviewAgainstItem();
				item.setId(itemRecord.getOsmElementId());
				item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
				item.setUuid(itemRecord.getElementId());
				reviewableItems.add(item);
				idsParsed.add(itemRecord.getElementId());
			}
		}
		idsParsed.clear();
		references.add(reviewableItems);
		
		return references;
  }
}
