package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * Databasechangelog is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Databasechangelog {

    private String author;

    private String comments;

    private java.sql.Timestamp dateexecuted;

    private String description;

    private String exectype;

    private String filename;

    private String id;

    private String liquibase;

    private String md5sum;

    private Integer orderexecuted;

    private String tag;

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getComments() {
        return comments;
    }

    public void setComments(String comments) {
        this.comments = comments;
    }

    public java.sql.Timestamp getDateexecuted() {
        return dateexecuted;
    }

    public void setDateexecuted(java.sql.Timestamp dateexecuted) {
        this.dateexecuted = dateexecuted;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getExectype() {
        return exectype;
    }

    public void setExectype(String exectype) {
        this.exectype = exectype;
    }

    public String getFilename() {
        return filename;
    }

    public void setFilename(String filename) {
        this.filename = filename;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getLiquibase() {
        return liquibase;
    }

    public void setLiquibase(String liquibase) {
        this.liquibase = liquibase;
    }

    public String getMd5sum() {
        return md5sum;
    }

    public void setMd5sum(String md5sum) {
        this.md5sum = md5sum;
    }

    public Integer getOrderexecuted() {
        return orderexecuted;
    }

    public void setOrderexecuted(Integer orderexecuted) {
        this.orderexecuted = orderexecuted;
    }

    public String getTag() {
        return tag;
    }

    public void setTag(String tag) {
        this.tag = tag;
    }

}

