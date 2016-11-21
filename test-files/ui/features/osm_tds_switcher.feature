Feature: OSM/TDS Switcher

    Scenario: I can add the DC GIS data to the map
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"

    Scenario: I can select a feature and switch between attribute schemas
        When I select a way map feature with OSM id "w309"
        Then I wait 10 "seconds" to see "Edit feature:"
        Then I should see element "#preset-input-name" with value "23RD ST NW"
        Then I select the "TDSv61" option labelled "Tag Schema"
        Then I wait 10 "seconds" to see "label" element with text "Feature Code"
        Then I should see element "#preset-input-TDSv61\/F_CODE" with value "AP030"
        Then I select the "TDSv40" option labelled "Tag Schema"
        Then I wait 10 "seconds" to see "label" element with text "Geographic Name Information : Full Name"
        Then I wait 5 seconds
        Then I should see element "#preset-input-TDSv40\/ZI005_FNA" with value "23RD ST NW"
        Then I select the "MGCP" option labelled "Tag Schema"
        Then I wait 10 "seconds" to see "label" element with text "Thoroughfare Class"
        Then I should see element "#preset-input-MGCP\/HCT" with value "Unknown"

    Scenario: I can add a new tag using an MGCP field
        Then I select the "Secondary Route" option labelled "Thoroughfare Class"
        # Then I select the "Median Present" option labelled "Add field:"
        # Then I select the "Without Median" option labelled "Median Present"

    Scenario: I can add a new feature and switch between attribute schemas
        # Add a point with OSM schema
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "350","300"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "OSM" option labelled "Tag Schema"
        And I click the "div.label" with text "Park"
        And I should see a "g.tag-leisure-park" on the map
        And I click the "map" at "400","400"

        # Translate this point to TDSv61 and get Feature out of spec error
        When I select a node map feature with class "tag-leisure-park"
        And I should see "Edit feature: "
        And I select the "TDSv61" option labelled "Tag Schema"
        Then I wait 10 "seconds" to see "Feature out of spec"

        # Add a point with TDSv61 schema
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "50","120"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "TDSv61" option labelled "Tag Schema"

        # Use the preset search feature
        And I append "preset-search-input" input with "b"
        And I append "preset-search-input" input with "u"
        And I append "preset-search-input" input with "i"
        And I append "preset-search-input" input with "l"
        And I append "preset-search-input" input with "d"
        And I wait 10 "seconds" to see "Building Superstructure (AL018)"
        # Use the searched preset here
        And I click the "div.label" with text "Building Superstructure (AL018)"
        # I can add a new tag using a TDSv61 field
        Then I wait 5 seconds
        Then I select the "Building Superstructure Type" option labelled "Add field:"
        Then I select the "Dome" option labelled "Building Superstructure Type"
        And I click the "map" at "440","425"

        # Add another point with TDSv61 schema
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "50","270"
        Then I wait 5 "seconds" to see "Select feature type"
        And I click the "div.label" with text "Building (AL013)"
        And I should see a "g.tag-building" on the map
        # I can add a new tag using a TDSv61 field
        Then I wait 5 seconds
        Then I select the "Floor Count" option labelled "Add field:"
        Then I fill "#preset-input-TDSv61\/BNF" with "3"
        And I click the "map" at "440","425"

        # Translate this point to MGCP and get valid translation
        When I select a node map feature with class "tag-building"
        And I should see "Edit feature: "
        And I select the "MGCP" option labelled "Tag Schema"
        Then I wait 5 seconds
        Then I should see element "[id='preset-input-MGCP/FCODE']" with value "AL015"

        # Add a point with MGCP schema
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "200","200"
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "MGCP" option labelled "Tag Schema"
        And I click the "div.label" with text "Settlement (AL105)"
        And I should see a "g.tag-place-settlement" on the map
        And I click the "map" at "400","400"

        # Add a line with TDSv40 schema
        When I click the "add-line" button
        And I hover over "#map"
        And I click the "map" at "300","200"
        And I click the "map" at "200","300"
        And I double-click the "map" at "200","400"
        Then I press the escape key
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "TDSv40" option labelled "Tag Schema"
        And I click the "div.label" with text "Trail (AP050)"
        And I should see a "path.tag-highway-path" on the map

        # Add an area with MGCP schema
        When I click the "add-area" button
        And I hover over "#map"
        And I click the "map" at "150","150"
        And I click the "map" at "450","450"
        And I double-click the "map" at "150","450"
        Then I press the escape key
        Then I wait 5 "seconds" to see "Select feature type"
        Then I select the "MGCP" option labelled "Tag Schema"
        And I click the "div.label" with text "Built-up Area (AL020)"
        And I should see a "path.tag-landuse-built_up_area" on the map

        # Save the features
        When I press "Save"
        Then I click the "save-button" button
        Then I click the "trash" button
        And I accept the alert
