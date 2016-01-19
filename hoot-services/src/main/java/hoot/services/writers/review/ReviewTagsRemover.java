package hoot.services.writers.review;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewTags;
import hoot.services.models.review.ReviewTagDelRequest;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.dml.SQLDeleteClause;

public class ReviewTagsRemover {
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewTagsRemover.class);
	
	private Connection _conn;
	
	public ReviewTagsRemover(final Connection cn) 
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
	public long remove(final ReviewTagDelRequest request) throws Exception
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
	 * @param request - Request containing mapid and relationid
	 * @return - toal numbers of removed
	 * @throws Exception
	 */
	protected SQLDeleteClause _createDelClause(final ReviewTagDelRequest request) throws Exception
	{
		SQLDeleteClause cl = null;
		
		try
		{
			QReviewTags reviewTags = QReviewTags.reviewTags;
			Configuration configuration = DbUtils.getConfiguration();
			cl = new SQLDeleteClause(_conn, configuration, reviewTags)
			.where(reviewTags.mapId.eq(request.getMapId()), 
					reviewTags.relationId.eq(request.getRelationId()));
		}
		catch (Exception ex)
		{
			
		}
		
		return cl;
	}
}
