package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * Changesets is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Changesets {

    private java.sql.Timestamp closedAt;

    private java.sql.Timestamp createdAt;

    private Long id;

    private Double maxLat;

    private Double maxLon;

    private Double minLat;

    private Double minLon;

    private Integer numChanges;

    private Object tags;

    private Long userId;

    public java.sql.Timestamp getClosedAt() {
        return closedAt;
    }

    public void setClosedAt(java.sql.Timestamp closedAt) {
        this.closedAt = closedAt;
    }

    public java.sql.Timestamp getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(java.sql.Timestamp createdAt) {
        this.createdAt = createdAt;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Double getMaxLat() {
        return maxLat;
    }

    public void setMaxLat(Double maxLat) {
        this.maxLat = maxLat;
    }

    public Double getMaxLon() {
        return maxLon;
    }

    public void setMaxLon(Double maxLon) {
        this.maxLon = maxLon;
    }

    public Double getMinLat() {
        return minLat;
    }

    public void setMinLat(Double minLat) {
        this.minLat = minLat;
    }

    public Double getMinLon() {
        return minLon;
    }

    public void setMinLon(Double minLon) {
        this.minLon = minLon;
    }

    public Integer getNumChanges() {
        return numChanges;
    }

    public void setNumChanges(Integer numChanges) {
        this.numChanges = numChanges;
    }

    public Object getTags() {
        return tags;
    }

    public void setTags(Object tags) {
        this.tags = tags;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

}

