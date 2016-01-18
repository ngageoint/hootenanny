package hoot.services.writers.review;


import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewTags;
import hoot.services.db2.ReviewTags;
import hoot.services.models.review.ReviewTagSaveRequest;

import java.sql.Connection;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.Calendar;
import java.util.Iterator;
import java.util.Map;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.support.Expressions;

public class ReviewTagsSaver {
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewTagsSaver.class);
	
	private Connection _conn;
	
	public ReviewTagsSaver(final Connection cn) 
	{
		_conn = cn;
	}
	
	public void insert(final ReviewTagSaveRequest request) throws Exception
	{
		try
		{
			SQLInsertClause cl = _createInsertClause(request);
			if(cl != null)
			{
				cl.execute();
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

	}
	
	protected SQLInsertClause _createInsertClause(final ReviewTagSaveRequest request) throws Exception
	{
		SQLInsertClause cl = null;
		try
		{
			Configuration configuration = DbUtils.getConfiguration();
			
	  	SQLQuery query = new SQLQuery(_conn, configuration);
	  	final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
	
	  	JSONObject tags = request.getDetail();
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
	  	
	  	QReviewTags reviewTags = QReviewTags.reviewTags;
	  	cl = new SQLInsertClause(_conn, configuration,reviewTags)
	  	.columns(reviewTags.mapId, reviewTags.relationId, reviewTags.createdAt, reviewTags.createdBy, reviewTags.detail)
	  	.values(request.getMapId(), request.getRelationId(), now, request.getUserid(),
	  			Expressions.template(Object.class, hstoreStr));
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return cl;
	}
	
	public void update(final ReviewTagSaveRequest request)
	{
		final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
		ReviewTags reviewTagsDto = new ReviewTags();
		reviewTagsDto.setLastModifiedAt(now);
		//reviewTagsDto.setLastModifiedBy(request);
		
		
	}
}
