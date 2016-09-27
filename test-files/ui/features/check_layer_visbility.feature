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
        And I hover over "#map"
        When I select a way map feature with class "w28_53"
        Then I wait 10 "seconds" to see "Edit feature:"
        When I select a way map feature with class "w189_67"
        Then I wait 10 "seconds" to see "Edit feature:" 

    Scenario: Toggle Layers and Check Visibility
        When I click the Background settings button
        Then I uncheck the "DcGisRoadsCucumber" checkbox
        Then I should see checkbox "DcGisRoadsCucumber" unchecked
        And I hover over "#map"
        And I should not see an element "w28_53"
        When I select a way map feature with class "w189_67"
        Then I wait 10 "seconds" to see "Edit feature:"
        When I click the Background settings button
        Then I uncheck the "DcOsmRoadsCucumber" checkbox
        Then I should see checkbox "DcOsmRoadsCucumber" unchecked
        And I hover over "#map"
        And I should not see an element "w189_67"