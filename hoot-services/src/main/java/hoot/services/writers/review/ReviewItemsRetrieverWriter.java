package hoot.services.writers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewItems;
import hoot.services.exceptions.writer.review.ReviewItemsWriterException;

import java.sql.Connection;
import java.sql.Timestamp;

import com.mysema.query.sql.dml.SQLUpdateClause;

public class ReviewItemsRetrieverWriter 
{
	private long _mapId;
  private Connection _conn;
  
  public ReviewItemsRetrieverWriter(final Connection cn, final long mapid)
  {
  	_mapId = mapid;
  	_conn = cn;
  }
  
  
  
  protected final SQLUpdateClause _getLastAccessUpdateClause(final String reviewItemId, 
      final Timestamp newLastAccessTime, final String reviewAgainst) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLUpdateClause q = new SQLUpdateClause(_conn, DbUtils.getConfiguration(_mapId), rm)
    .set(rm.lastAccessed, newLastAccessTime)
    .where(rm.mapId.eq(_mapId).and(rm.reviewableItemId.eq(reviewItemId)));
    
    if(reviewAgainst != null)
    {
       q = new SQLUpdateClause(_conn, DbUtils.getConfiguration(_mapId), rm)
        .set(rm.lastAccessed, newLastAccessTime)
        .where(rm.mapId.eq(_mapId).and(rm.reviewableItemId.eq(reviewItemId))
            .and(rm.reviewAgainstItemId.eq(reviewAgainst)));
    }
    
    return q;
  }
  
//Update Review LastAccess column
 public final long updateReviewLastAccessTime(final String reviewItemId, final Timestamp newLastAccessTime,
     final String reviewAgainst) throws ReviewItemsWriterException,Exception
 {
	 @SuppressWarnings("unused")
  long updated = 0;
	 SQLUpdateClause cls = _getLastAccessUpdateClause(reviewItemId, newLastAccessTime, reviewAgainst);
	 try
	 {
		 updated = cls.execute(); 
	 }
	 catch  (Exception ex) {
		 throw new ReviewItemsWriterException(cls.toString(), ex.getMessage());
	 }
   return cls.execute(); 
 }
 
 
 protected final SQLUpdateClause _updateLastAccessWithReviewId(final Timestamp now, 
     final long reviewId) throws Exception
 {
   QReviewItems rm = QReviewItems.reviewItems;
   
   SQLUpdateClause q = new SQLUpdateClause(_conn, DbUtils.getConfiguration(_mapId), rm)
   .set(rm.lastAccessed, now)
   .where(rm.mapId.eq(_mapId).and(rm.reviewId.eq(reviewId)));
   
   return q;
 } 
 
 
 public final long updateReviewLastAccessTimeWithReviewId(final Timestamp tmpStamp, 
     final long reviewId) throws ReviewItemsWriterException, Exception
 {
	 long updated = 0;
	 SQLUpdateClause cls = _updateLastAccessWithReviewId(tmpStamp, reviewId);
	 try
	 {
		 updated = cls.execute(); 
	 }
	 catch (Exception ex) {
		 throw new ReviewItemsWriterException(cls.toString(), ex.getMessage());
	 }
   return updated;
 }
 
 
  
}
