package hoot.services.models.review.custom.HGIS;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class PrepareForValidationRequest {
	private String sourceMap;
	public String getSourceMap()
	{
		return this.sourceMap;
	}
	
	public void setSourceMap(String srcMap)
	{
		this.sourceMap = srcMap;
	}
	private String outputMap;
	
	public String getOutputMap()
	{
		return this.outputMap;
	}
	
	public void setOutputMap(String outMap)
	{
		this.outputMap = outMap;
	}
}
