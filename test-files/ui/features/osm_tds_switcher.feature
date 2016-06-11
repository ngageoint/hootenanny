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
