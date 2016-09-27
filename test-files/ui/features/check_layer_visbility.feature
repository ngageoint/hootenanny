Feature: Check Layer Visibility

    Scenario: Open Hootenanny and Add Data
        Given I am on Hootenanny
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
        And I press "Add Secondary Dataset"
        And I click the "DcOsmRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcOsmRoadsCucumber"

    Scenario: Check Layer Visbility
        When I click the Background settings button
        And I should see checkbox "DcGisRoadsCucumber" checked
        And I should see checkbox "DcOsmRoadsCucumber" checked
        When I should see a way map feature with OSM id "stroke w220_ tag-hoot-DcGisRoadsCucumber"
        When I should see a way map feature with OSM id "stroke w189_ tag-hoot-DcOsmRoadsCucumber"

    Scenario: Toggle Layers and Check Visibility
        Then I uncheck the "DcGisRoadsCucumber" checkbox
        Then I should see checkbox "DcGisRoadsCucumber" unchecked
        When I should not see a way map feature with OSM id "tag-hoot-DcGisRoadsCucumber"
        When I should see a way map feature with OSM id "stroke w189_ tag-hoot-DcOsmRoadsCucumber"
        Then I uncheck the "DcOsmRoadsCucumber" checkbox
        Then I should see checkbox "DcOsmRoadsCucumber" unchecked
        When I should not see a way map feature with OSM id "tag-hoot-DcOsmRoadsCucumber"
        Then I check the "DcGisRoadsCucumber" checkbox
        When I should see a way map feature with OSM id "stroke w220_ tag-hoot-DcGisRoadsCucumber"
