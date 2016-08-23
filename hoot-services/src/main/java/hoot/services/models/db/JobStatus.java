package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * JobStatus is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class JobStatus {

    private java.sql.Timestamp end;

    private String jobId;

    private Double percentComplete;

    private java.sql.Timestamp start;

    private Integer status;

    private String statusDetail;

    public java.sql.Timestamp getEnd() {
        return end;
    }

    public void setEnd(java.sql.Timestamp end) {
        this.end = end;
    }

    public String getJobId() {
        return jobId;
    }

    public void setJobId(String jobId) {
        this.jobId = jobId;
    }

    public Double getPercentComplete() {
        return percentComplete;
    }

    public void setPercentComplete(Double percentComplete) {
        this.percentComplete = percentComplete;
    }

    public java.sql.Timestamp getStart() {
        return start;
    }

    public void setStart(java.sql.Timestamp start) {
        this.start = start;
    }

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }

    public String getStatusDetail() {
        return statusDetail;
    }

    public void setStatusDetail(String statusDetail) {
        this.statusDetail = statusDetail;
    }

}

