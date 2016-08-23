package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * CurrentNodes is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class CurrentNodes {

    private Long changesetId;

    private Long id;

    private Double latitude;

    private Double longitude;

    private Object tags;

    private Long tile;

    private java.sql.Timestamp timestamp;

    private Long version;

    private Boolean visible;

    public Long getChangesetId() {
        return changesetId;
    }

    public void setChangesetId(Long changesetId) {
        this.changesetId = changesetId;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Double getLatitude() {
        return latitude;
    }

    public void setLatitude(Double latitude) {
        this.latitude = latitude;
    }

    public Double getLongitude() {
        return longitude;
    }

    public void setLongitude(Double longitude) {
        this.longitude = longitude;
    }

    public Object getTags() {
        return tags;
    }

    public void setTags(Object tags) {
        this.tags = tags;
    }

    public Long getTile() {
        return tile;
    }

    public void setTile(Long tile) {
        this.tile = tile;
    }

    public java.sql.Timestamp getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(java.sql.Timestamp timestamp) {
        this.timestamp = timestamp;
    }

    public Long getVersion() {
        return version;
    }

    public void setVersion(Long version) {
        this.version = version;
    }

    public Boolean getVisible() {
        return visible;
    }

    public void setVisible(Boolean visible) {
        this.visible = visible;
    }

}

