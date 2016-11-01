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

package hoot.services.testsupport;

import javax.sql.DataSource;

import org.apache.commons.dbcp.BasicDataSource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.DependsOn;
import org.springframework.context.annotation.FilterType;
import org.springframework.context.annotation.Profile;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DataSourceTransactionManager;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.annotation.EnableTransactionManagement;

import hoot.services.ApplicationContextUtils;
import hoot.services.HootServicesSpringConfig;
import hoot.services.controllers.info.NativeInterfaceStubImpl;
import hoot.services.nativeinterfaces.NativeInterface;


@Configuration
@EnableTransactionManagement
@ComponentScan(basePackages = {"hoot.services"},
        excludeFilters = @ComponentScan.Filter(value = HootServicesSpringConfig.class, type = FilterType.ASSIGNABLE_TYPE))
@Profile("test")
public class HootServicesSpringTestConfig {

    @Bean
    public ApplicationContextUtils applicationContextUtils() {
        return new ApplicationContextUtils();
    }

    @Bean(name = "dataSource")
    public DataSource dataSource() {
        BasicDataSource dataSource = new BasicDataSource();
        dataSource.setDriverClassName("org.postgresql.Driver");
        dataSource.setUrl("jdbc:postgresql://localhost:5432/hoot");
        dataSource.setUsername("hoot");
        dataSource.setPassword("hoottest");
        dataSource.setInitialSize(5);
        dataSource.setMaxActive(10);
        dataSource.setMaxIdle(2);
        dataSource.setDefaultAutoCommit(false);
        dataSource.setRemoveAbandoned(true);
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
    public NativeInterface nativeInterface() {
        return new NativeInterfaceStubImpl();
    }

    @Bean
    @Autowired
    public JdbcTemplate jdbcTemplate(DataSource dataSource) {
        return new JdbcTemplate(dataSource);
    }
}
