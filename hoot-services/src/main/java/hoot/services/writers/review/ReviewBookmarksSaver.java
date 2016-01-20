package hoot.services.writers.review;


import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.db2.ReviewBookmarks;
import hoot.services.models.review.ReviewBookmarkSaveRequest;
import hoot.services.readers.review.ReviewBookmarkRetriever;

import java.sql.Connection;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.support.Expressions;

public class ReviewBookmarksSaver {
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewBookmarksSaver.class);
	
	private Connection _conn;
	
	public ReviewBookmarksSaver(final Connection cn) 
	{
		_conn = cn;
	}
	
	/**
	 * Saves review tags. It first checks to see if exists and if not insert else update
	 * 
	 * @param request - request object containing inserted/updated fields
	 * @return - numbers of saved tags
	 * @throws Exception
	 */
	public long save(final ReviewBookmarkSaveRequest request) throws Exception
	{
		long nSaved = 0;
		ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(_conn);
		
		List<ReviewBookmarks> res = retriever.retrieve(request.getMapId(), request.getRelationId());
		
		if(res.size() == 0)
		{
			// insert
			nSaved = insert(request);
		}
		else
		{
			// update
			nSaved = update(request, res.get(0));
		}
		
		return nSaved;
	}
	
	/**
	 * Inserts review tag into database
	 * 
	 * @param request - request object containing inserted fields
	 * @return - total numbers of inserted
	 * @throws Exception
	 */
	public long insert(final ReviewBookmarkSaveRequest request) throws Exception
	{
		long nInserted = 0;
		try
		{
			SQLInsertClause cl = _createInsertClause(request);
			if(cl != null)
			{
				nInserted = cl.execute();
			}
			else 
			{
				throw new Exception("Invalid insert clause.");
			}

		}
		catch(Exception ex)
		{
			String err = ex.getMessage();
			log.error(err);
			throw ex;
		}
		return nInserted;

	}
	
	/**
	 * Updates review tag.
	 * 
	 * @param request - request object containing updated fields
	 * @param reviewBookmarksDto - Current review tag
	 * @return- total numbers of updated
	 * @throws Exception
	 */
	public long update(final ReviewBookmarkSaveRequest request, final ReviewBookmarks reviewBookmarksDto) throws Exception
	{
		long nUpdated = 0;
		
		try
		{
			nUpdated = _getUpdateQuery(request, reviewBookmarksDto)
	    .execute();		
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return nUpdated;
	}
	
	
	/**
	 * Creates insert clause
	 * 
	 * @param request - request object containing inserted fields
	 * @return - SQLInsertClause
	 * @throws Exception
	 */
	protected SQLInsertClause _createInsertClause(final ReviewBookmarkSaveRequest request) throws Exception
	{
		SQLInsertClause cl = null;
		try
		{
			Configuration configuration = DbUtils.getConfiguration();
			
	  	final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
	  	
	  	QReviewBookmarks reviewBookmarks = QReviewBookmarks.reviewBookmarks;
	  	cl = new SQLInsertClause(_conn, configuration,reviewBookmarks)
	  	.columns(reviewBookmarks.mapId, reviewBookmarks.relationId, reviewBookmarks.createdAt, reviewBookmarks.createdBy, reviewBookmarks.detail)
	  	.values(request.getMapId(), request.getRelationId(), now, request.getUserid(),
	  			_jasonToHStore(request.getDetail()));
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return cl;
	}
	
	
	/**
	 * Creates update clause
	 * 
	 * @param request - request object containing updated fields
	 * @param reviewBookmarksDto - Current review tag
	 * @return - SQLUpdateClause
	 * @throws Exception
	 */
	protected SQLUpdateClause _getUpdateQuery(final ReviewBookmarkSaveRequest request,
			final ReviewBookmarks reviewBookmarksDto) throws Exception
	{
		SQLUpdateClause res = null;
		
		try
		{
			final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
	
			reviewBookmarksDto.setLastModifiedAt(now);
			reviewBookmarksDto.setLastModifiedBy(request.getUserid());
			
			reviewBookmarksDto.setDetail(_jasonToHStore(request.getDetail()));
			
			Configuration configuration = DbUtils.getConfiguration();
			QReviewBookmarks reviewBookmarks = QReviewBookmarks.reviewBookmarks;
			res = new SQLUpdateClause(_conn, configuration, reviewBookmarks)
	    .populate(reviewBookmarksDto)
	    .where(reviewBookmarks.id.eq(reviewBookmarksDto.getId()));
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return res;
	}
	
	/**
	 * Since QueryDSL does not support hstore this function creates QueryDSL Expression object
	 * form tags json.
	 * 
	 * @param tags - json containing tags kv
	 * @return - Expression Object for QueryDSL consumption
	 * @throws Exception
	 */
	protected Object _jasonToHStore (final JSONObject tags) throws Exception
	{
  	String hstoreStr = "";
		
  	if(tags != null)
  	{
	  	Iterator it = tags.entrySet().iterator();
			while (it.hasNext())
			{
				Map.Entry pairs = (Map.Entry) it.next();
				if (hstoreStr.length() > 0)
				{
					hstoreStr += ",";
				}
				hstoreStr += "\"" + pairs.getKey() + "\"=>\"" + pairs.getValue()
				    + "\"";
			}
  	}
		hstoreStr = "'" + hstoreStr + "'";
		return Expressions.template(Object.class, hstoreStr);
	}
	
}
