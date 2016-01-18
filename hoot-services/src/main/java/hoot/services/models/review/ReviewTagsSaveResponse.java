package hoot.services.models.review;

public class ReviewTagsSaveResponse {
	private long tagId;
	
	public void setTagId(final long newTagId) 
	{
		this.tagId = newTagId;
	}
	
	public long getTagid()
	{
		return this.tagId;
	}
}
