package hoot.services.writers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkDelRequest;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.dml.SQLDeleteClause;

public class ReviewBookmarksRemover {
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewBookmarksRemover.class);
	
	private Connection _conn;
	
	public ReviewBookmarksRemover(final Connection cn) 
	{
		_conn = cn;
	}
	
	/**
	 * Removes review tag.
	 * 
	 * @param request - Request containing mapid and relationid
	 * @return - total numbers of removed
	 * @throws Exception
	 */
	public long remove(final ReviewBookmarkDelRequest request) throws Exception
	{
		long nDeleted = 0;
		
		try
		{
			SQLDeleteClause cl = _createDelClause(request);
			nDeleted = cl.execute();
		}
		catch (Exception ex)
		{
			
		}
		return nDeleted;
	}
	
	
	/**
	 * Delete clause
	 * 
	 * @param request - Request containing bookmarkid
	 * @return - toal numbers of removed
	 * @throws Exception
	 */
	protected SQLDeleteClause _createDelClause(final ReviewBookmarkDelRequest request) throws Exception
	{
		SQLDeleteClause cl = null;
		
		try
		{
			QReviewBookmarks reviewBookmarks = QReviewBookmarks.reviewBookmarks;
			Configuration configuration = DbUtils.getConfiguration();
			cl = new SQLDeleteClause(_conn, configuration, reviewBookmarks)
			.where(reviewBookmarks.id.eq(request.getBookmarkId()));
		}
		catch (Exception ex)
		{
			
		}
		
		return cl;
	}
}
