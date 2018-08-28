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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.models.db;

import java.sql.Timestamp;

import javax.annotation.Generated;

import org.springframework.security.oauth.consumer.OAuthConsumerToken;

/**
 * Users is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class Users {

    private String displayName;

    private String email;

    private Long id;

    private String provider_access_key;
    private String provider_access_token;
    private String session_id;
    private Timestamp hootservices_last_authorize;
    private Timestamp hootservices_created_at;
    private Timestamp provider_created_at;

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

    public String getProviderAccessKey() {
        return provider_access_key;
    }

    public void setProviderAccessKey(String provider_access_key) {
        this.provider_access_key = provider_access_key;
    }

    public String getProviderAccessToken() {
        return provider_access_token;
    }

    public void setProviderAccessToken(String provider_access_token) {
        this.provider_access_token = provider_access_token;
    }

    public Timestamp getHootservicesLastAuthorize() {
        return hootservices_last_authorize;
    }

    public void setHootservicesLastAuthorize(Timestamp hootservices_last_authorize) {
        this.hootservices_last_authorize = hootservices_last_authorize;
    }

    public String getSessionId() {
        return session_id;
    }

    public void setSessionId(String session_id) {
        this.session_id = session_id;
    }

    public Timestamp getProviderCreatedAt() {
        return provider_created_at;
    }

    public void setProviderCreatedAt(Timestamp provider_created_at) {
        this.provider_created_at = provider_created_at;
    }

    public void setProviderCreatedAt(String provider_created_at) {
        Timestamp t = Timestamp.valueOf(provider_created_at);
        setProviderCreatedAt(t);
    }

    public Timestamp getHootservicesCreatedAt() {
        return hootservices_created_at;
    }

    public void setHootservicesCreatedAt(Timestamp hootservices_created_at) {
        this.hootservices_created_at = hootservices_created_at;
    }

    public void setProviderAccessToken(OAuthConsumerToken token) {
        this.setProviderAccessKey(token.getValue());
        this.setProviderAccessToken(token.getSecret());
    }

    @Override
    public String toString() {
        return this.displayName + " // " + this.session_id;
    }

}
