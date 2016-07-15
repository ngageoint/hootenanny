Feature: OSM/TDS Switcher

    Scenario: I can add the DC GIS data to the map
        Given I am on Hootenanny
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "DcGisRoadsCucumber"

    Scenario: I can select a feature and switch between attribute schemas
        When I select a way map feature with id "w309"
        Then I wait 10 "seconds" to see "Edit feature:"
        Then I should see element "#preset-input-name" with value "23RD ST NW"
        Then I select the "TDSv61" option labelled "Filter By Type"
        Then I wait 10 "seconds" to see "label" element with text "Feature Code"
        Then I should see element "#preset-input-TDSv61\/F_CODE" with value "AP030:Road"
        Then I select the "TDSv40" option labelled "Filter By Type"
        Then I wait 10 "seconds" to see "label" element with text "Geographic Name Information : Full Name"
        Then I should see element "#preset-input-TDSv40\/ZI005_FNA" with value "23RD ST NW"
        Then I select the "MGCP" option labelled "Filter By Type"
        Then I wait 10 "seconds" to see "label" element with text "Horizontal Accuracy Category"
        Then I should see element "#preset-input-MGCP\/ACC" with value "Accurate"

    Scenario: I can add a new feature and switch between attribute schemas
        # Add a point with OSM schema
        When I click the "add-point" button
        And I click the "map" at "300","300"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "OSM" option labelled "Filter By Type"
        And I click the "div.label" with text "Park"
        And I should see a "g.tag-leisure-park" on the map
        And I click the "map" at "400","400"
        # Add a point with TDSv61 schema
        When I click the "add-point" button
        And I click the "map" at "100","100"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "TDSv61" option labelled "Filter By Type"
        And I click the "div.label" with text "Building (AL013)"
        And I should see a "g.tag-building" on the map
        And I click the "map" at "400","400"
        # Add a point with MGCP schema
        When I click the "add-point" button
        And I click the "map" at "200","200"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "MGCP" option labelled "Filter By Type"
        And I click the "div.label" with text "Settlement (AL105)"
        And I should see a "g.tag-place-settlement" on the map
        And I click the "map" at "400","400"

        # Add a line with TDSv40 schema
        When I click the "add-line" button
        And I click the "map" at "300","200"
        And I click the "map" at "200","300"
        And I double-click the "map" at "200","400"
        Then I press the escape key
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "TDSv40" option labelled "Filter By Type"
        And I click the "div.label" with text "Trail (AP050)"
        And I should see a "path.tag-highway-path" on the map

        # Add an area with MGCP schema
        When I click the "add-area" button
        And I click the "map" at "150","150"
        And I click the "map" at "450","450"
        And I double-click the "map" at "150","450"
        Then I press the escape key
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "MGCP" option labelled "Filter By Type"
        And I click the "div.label" with text "Built-up Area (AL020)"
        And I should see a "path.tag-landuse-built_up_area" on the map
