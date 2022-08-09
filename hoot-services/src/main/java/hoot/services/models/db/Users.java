/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.models.db;

import java.sql.Timestamp;
import java.util.Date;

import javax.annotation.Generated;
import javax.servlet.http.HttpServletRequest;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;

import hoot.services.HootUserRequestFilter;


/**
 * Users is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
@JsonInclude(JsonInclude.Include.NON_NULL)
public class Users {

    public static final Users TEST_USER = Users.generateTestUser();

    public Users() {
        //The db column default should handle this
        //this.hootservices_created_at = new Timestamp(System.currentTimeMillis());
    }

    private String displayName;

    private String email;

    private Long id;

    @JsonIgnore
    private String provider_access_key;

    @JsonIgnore
    private String provider_access_token;

    private Timestamp hootservices_last_authorize;

    private Timestamp hootservices_created_at;

    private Timestamp provider_created_at;

    private Object privileges;

    public Object favoriteOpts;

    @JsonProperty(value = "display_name")
    public String getDisplayName() {
        return displayName;
    }

    public void setDisplayName(String displayName) {
        this.displayName = displayName;
    }

    // @JsonIgnore
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

    @JsonIgnore
    public String getProviderAccessKey() {
        return provider_access_key;
    }

    public void setProviderAccessKey(String provider_access_key) {
        this.provider_access_key = provider_access_key;
    }

    @JsonIgnore
    public String getProviderAccessToken() {
        return provider_access_token;
    }

    public void setProviderAccessToken(String provider_access_token) {
        this.provider_access_token = provider_access_token;
    }

    @JsonProperty(value = "last_authorized")
    public Timestamp getHootservicesLastAuthorize() {
        return hootservices_last_authorize;
    }

    public void setHootservicesLastAuthorize(Timestamp hootservices_last_authorize) {
        this.hootservices_last_authorize = hootservices_last_authorize;
    }

    @JsonProperty(value = "provider_created_at")
    public Timestamp getProviderCreatedAt() {
        return provider_created_at;
    }

    @JsonIgnore
    public void setProviderCreatedAt(Timestamp provider_created_at) {
        this.provider_created_at = provider_created_at;
    }

    @JsonIgnore
    public void setProviderCreatedAt(String provider_created_at) {
        Timestamp t = Timestamp.valueOf(provider_created_at);
        setProviderCreatedAt(t);
    }

    @JsonProperty(value = "created_at")
    public Timestamp getHootservicesCreatedAt() {
        return hootservices_created_at;
    }

    public void setHootservicesCreatedAt(Timestamp hootservices_created_at) {
        this.hootservices_created_at = hootservices_created_at;
    }

    @JsonProperty(value = "privileges")
    public Object getPrivileges() {
        return privileges;
    }

    public void setPrivileges(Object privileges) {
        this.privileges = privileges;
    }

    @JsonProperty(value = "members")
    public Object getFavoriteOpts() {
        return favoriteOpts;
    }

    public void setFavoriteOpts( Object favoriteOpts ) {
        this.favoriteOpts = favoriteOpts;
    }

    @Override
    public String toString() {
        return this.displayName + "//" + this.id;
    }

    public static Users fromRequest(HttpServletRequest request) {
        if(request == null) {
            return TEST_USER;  //Not sure this shouldn't be null
        }
        Users user = (Users) request.getAttribute(HootUserRequestFilter.HOOT_USER_ATTRIBUTE);
        if (user == null) {
            return TEST_USER;
        }
        return user;
    }

    private static Users generateTestUser() {
        Users u = new Users();
        u.setId(-1L);
        u.setDisplayName("Test User");
        u.setEmail("test@hootenanny");
        u.setProviderAccessKey("provider_access_key");
        u.setProviderAccessToken("provider_access_token");

        u.setHootservicesCreatedAt(new Timestamp(new Date().getTime()));
        u.setHootservicesLastAuthorize(u.getHootservicesCreatedAt());
        u.setProviderCreatedAt(u.getHootservicesCreatedAt());

        return u;
    }
}
