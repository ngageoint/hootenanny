package hoot.services.writers.review;


import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewTags;
import hoot.services.db2.ReviewTags;
import hoot.services.models.review.ReviewTagSaveRequest;
import hoot.services.readers.review.ReviewTagRetriever;

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

public class ReviewTagsSaver {
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewTagsSaver.class);
	
	private Connection _conn;
	
	public ReviewTagsSaver(final Connection cn) 
	{
		_conn = cn;
	}
	public long save(final ReviewTagSaveRequest request) throws Exception
	{
		long nSaved = 0;
		ReviewTagRetriever retriever = new ReviewTagRetriever(_conn);
		
		List<ReviewTags> res = retriever.retrieve(request.getMapId(), request.getRelationId());
		
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
	public long insert(final ReviewTagSaveRequest request) throws Exception
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
	
	protected SQLInsertClause _createInsertClause(final ReviewTagSaveRequest request) throws Exception
	{
		SQLInsertClause cl = null;
		try
		{
			Configuration configuration = DbUtils.getConfiguration();
			
	  	final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
	  	
	  	QReviewTags reviewTags = QReviewTags.reviewTags;
	  	cl = new SQLInsertClause(_conn, configuration,reviewTags)
	  	.columns(reviewTags.mapId, reviewTags.relationId, reviewTags.createdAt, reviewTags.createdBy, reviewTags.detail)
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
	
	public long update(final ReviewTagSaveRequest request, final ReviewTags reviewTagsDto) throws Exception
	{
		long nUpdated = 0;
		
		try
		{
			nUpdated = _getUpdateQuery(request, reviewTagsDto)
	    .execute();		
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return nUpdated;
	}
	
	protected SQLUpdateClause _getUpdateQuery(final ReviewTagSaveRequest request,
			final ReviewTags reviewTagsDto) throws Exception
	{
		SQLUpdateClause res = null;
		
		try
		{
			final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
	
			reviewTagsDto.setLastModifiedAt(now);
			reviewTagsDto.setLastModifiedBy(request.getUserid());
			
			reviewTagsDto.setDetail(_jasonToHStore(request.getDetail()));
			
			Configuration configuration = DbUtils.getConfiguration();
			QReviewTags reviewTags = QReviewTags.reviewTags;
			res = new SQLUpdateClause(_conn, configuration, reviewTags)
	    .populate(reviewTagsDto)
	    .where(reviewTags.id.eq(reviewTagsDto.getId()));
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return res;
	}
	
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
