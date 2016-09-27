Feature: Check Layer Visibility

    Scenario: Open Hootenanny and add data
        Given I am on Hootenanny
        And I click Get Started
        And I press "Add Reference Dataset"
        And I click the "DcGisRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcGisRoadsCucumber"
        And I press "Add Secondary Dataseet"
        And I click the "DcOsmRoadsCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "DcOsmRoadsCucumber"

    Scenario: Check Layer Visbility
        When I click to expand Background settings
        And I should see checkbox "DcGisRoadsCucumber" checked
        And I should see checkbox "DcOsmRoadsCucumber" checked
        And I hover over "#{map}"