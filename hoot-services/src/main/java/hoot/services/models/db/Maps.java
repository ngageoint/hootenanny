package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * Maps is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Maps {

    private java.sql.Timestamp createdAt;

    private String displayName;

    private Long id;

    private Boolean publicCol;

    private Object tags;

    private Long userId;

    public java.sql.Timestamp getCreatedAt() {
        return createdAt;
    }

    public void setCreatedAt(java.sql.Timestamp createdAt) {
        this.createdAt = createdAt;
    }

    public String getDisplayName() {
        return displayName;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Boolean getPublicCol() {
        return publicCol;
    }

    public void setPublicCol(Boolean publicCol) {
        this.publicCol = publicCol;
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

