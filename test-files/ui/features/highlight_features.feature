Feature: Highlight features on click

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Add Data
        And I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "DcGisRoadsCucumber"
        And I press "Add Secondary Dataset"
        And I click the "DcOsmRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "DcOsmRoadsCucumber"

    Scenario: I click on a feature and see a highlight
        When I select a way map feature with OSM id "w250_16"
        Then I should see element "w250_16" with a highlight