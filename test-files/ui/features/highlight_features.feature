Feature: Highlight features on click

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Add Data
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesMergedCucumber" Dataset
        And I press "Add Layer"
        And I accept the alert
        Then I wait 30 "seconds" to see "AllDataTypesMergedCucumber"
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "350","300"

    Scenario: I select a feature and see an orange highlight
        When I select a node map feature with OSM id "n-1"
        Then I should see a node element "n-1" with a selected highlight
        # Then I de-select the feature
        And I click the "map" at "450","300"

    Scenario: I can see a red and blue highlight on the active review features
        Then I should see a ".activeReviewFeature" on the map
        And I should see a ".activeReviewFeature2" on the map
        # I zoom the map
        When I click the "zoom-out" button
        And I click the "zoom-out" button
        And I should see a "g.activeReviewFeature" on the map
        And I should see a "g.activeReviewFeature2" on the map
        And I hover over "#map"

    Scenario: I can see a yellow highlight on the edited feature
        When I click to expand Map Data
        Then I click the "a.hide-toggle" with text "Map Features"
        Then I turn on highlight edited features
        And I hover over "#map"
        And I click the "map" at "450","300"
        Then I should see element "n-1" with a yellow highlight
        Then I wait 5 seconds
        Then I click the "trash.map-button" icon
        And I accept the alert

