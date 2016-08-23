package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * Users is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Users {

    private String displayName;

    private String email;

    private Long id;

    public String getDisplayName() {
        return displayName;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

}

