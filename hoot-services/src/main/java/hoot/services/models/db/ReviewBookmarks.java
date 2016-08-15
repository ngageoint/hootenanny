package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * ReviewBookmarks is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class ReviewBookmarks {

    private java.sql.Timestamp createdAt;

    private Long createdBy;

    private Object detail;

    private Long id;

    private java.sql.Timestamp lastModifiedAt;

    private Long lastModifiedBy;

    private Long mapId;

    private Long relationId;

    public java.sql.Timestamp getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(java.sql.Timestamp createdAt) {
        this.createdAt = createdAt;
    }

    public Long getCreatedBy() {
        return createdBy;
    }

    public void setCreatedBy(Long createdBy) {
        this.createdBy = createdBy;
    }

    public Object getDetail() {
        return detail;
    }

    public void setDetail(Object detail) {
        this.detail = detail;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public java.sql.Timestamp getLastModifiedAt() {
        return lastModifiedAt;
    }

    public void setLastModifiedAt(java.sql.Timestamp lastModifiedAt) {
        this.lastModifiedAt = lastModifiedAt;
    }

    public Long getLastModifiedBy() {
        return lastModifiedBy;
    }

    public void setLastModifiedBy(Long lastModifiedBy) {
        this.lastModifiedBy = lastModifiedBy;
    }

    public Long getMapId() {
        return mapId;
    }

    public void setMapId(Long mapId) {
        this.mapId = mapId;
    }

    public Long getRelationId() {
        return relationId;
    }

    public void setRelationId(Long relationId) {
        this.relationId = relationId;
    }

}

