package hoot.services.models.review.custom.HGIS;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class PrepareForValidationResponse {
	private String jobId;
	
	public String getJobId()
	{
		return this.jobId;
	}
	
	public void setJobId(String id)
	{
		this.jobId = id;
	}
}
