package hoot.services.models.osm;

import javax.xml.bind.annotation.XmlRootElement;

/**
 * Element information
 */
@XmlRootElement
public class ElementInfo
{
	private long id = -1;
  public long getId() { return id; }
  public void setId(long id) { this.id = id; }
  
  //can be map name or id
  private String mapId;
  public String getMapId() { return mapId; }
  public void setMapId(String id) { this.mapId = id; }
  
  private String type;
  public String getType() { return type; }
  public void setType(String type) { this.type = type; }
  
  public ElementInfo()
  {
  }
  
  public ElementInfo(final String mapId, final long elementId, final String elementType)
  {
  	this.mapId = mapId;
  	this.id = elementId;
  	this.type = elementType;
  }
  
  @Override
  public String toString()
  {
  	return "map id: " + mapId + ", element id: " + id + ", element type: " + type;
  }
}
