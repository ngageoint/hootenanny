package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * Folders is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Folders {

    private java.sql.Timestamp createdAt;

    private String displayName;

    private Long id;

    private Long parentId;

    private Boolean publicCol;

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

    public Long getParentId() {
        return parentId;
    }

    public void setParentId(Long parentId) {
        this.parentId = parentId;
    }

    public Boolean getPublicCol() {
        return publicCol;
    }

    public void setPublicCol(Boolean publicCol) {
        this.publicCol = publicCol;
    }

    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

}

