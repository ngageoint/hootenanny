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
 * @copyright Copyright (C) 2016, 2017, 2018 Maxar (http://www.maxar.com/)
 */

package hoot.services.jerseyframework;

import javax.sql.DataSource;

import org.apache.commons.dbcp2.BasicDataSource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.DependsOn;
import org.springframework.context.annotation.FilterType;
import org.springframework.context.annotation.Primary;
import org.springframework.context.annotation.PropertySource;
import org.springframework.core.env.Environment;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.security.oauth.consumer.BaseProtectedResourceDetails;
import org.springframework.security.oauth.consumer.client.OAuthRestTemplate;
import org.springframework.security.oauth.consumer.token.HttpSessionBasedTokenServices;
import org.springframework.security.oauth.consumer.token.OAuthConsumerTokenServices;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import hoot.services.ApplicationContextUtils;
import hoot.services.HootServicesSpringConfig;
import hoot.services.command.ExternalCommandManager;
import hoot.services.command.ExternalCommandManagerImplStub;
import hoot.services.job.JobProcessorImplStub;


@Configuration
@EnableTransactionManagement
@ComponentScan(basePackages = {"hoot.services"},
excludeFilters = @ComponentScan.Filter(value = HootServicesSpringConfig.class, type = FilterType.ASSIGNABLE_TYPE))
@PropertySource("classpath:db/db.properties")
@ActiveProfiles("test")
public class HootServicesSpringTestConfig {

    @Autowired
    private Environment env;

    public HootServicesSpringTestConfig() {}

    @Bean
    public ApplicationContextUtils applicationContextUtils() {
        return new ApplicationContextUtils();
    }

    @Bean(name = "dataSource")
    public DataSource dataSource() {
        BasicDataSource dataSource = new BasicDataSource();
        dataSource.setDriverClassName("org.postgresql.Driver");
        dataSource.setUrl("jdbc:postgresql://" + env.getProperty("HOOTAPI_DB_HOST") + ":" +
                env.getProperty("HOOTAPI_DB_PORT") + "/" +
                env.getProperty("HOOTAPI_DB_NAME"));
        dataSource.setUsername(env.getProperty("HOOTAPI_DB_USER"));
        dataSource.setPassword(env.getProperty("HOOTAPI_DB_PASSWORD"));
        dataSource.setInitialSize(5);
        dataSource.setMaxTotal(10);
        dataSource.setMaxIdle(2);
        dataSource.setDefaultAutoCommit(false);
        dataSource.setRemoveAbandonedOnBorrow(true);
        dataSource.setRemoveAbandonedOnMaintenance(true);
        dataSource.setLogAbandoned(true);
        return dataSource;
    }

    @Bean(name = "transactionManager")
    @Autowired
    @DependsOn("dataSource")
    public PlatformTransactionManager transactionManager(DataSource dataSource) {
        return new DataSourceTransactionManager(dataSource);
    }

    @Bean
    @Autowired
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }

    @Primary
    @Bean
    public JobProcessorImplStub jobProcessor() {
        return new JobProcessorImplStub();
    }

    @Primary
    @Bean
    public ExternalCommandManager externalCommandManager() {
        return new ExternalCommandManagerImplStub();
    }

    @Primary
    @Bean
    public OAuthRestTemplate oauthRestTemplate() {
        BaseProtectedResourceDetails r = new BaseProtectedResourceDetails();
        OAuthRestTemplate restTemplate = new OAuthRestTemplate(r);
        return restTemplate;
    }

    @Primary
    @Bean
    public OAuthConsumerTokenServices tokenServices() {
        return new HttpSessionBasedTokenServices();
    }
}
