Feature: Highlight features on click

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Add Data
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 15 "seconds" to see "span.strong" element with text "AllDataTypesBCucumber"
        Then I should see "Conflate"
        And I press "Conflate"
        And I fill "saveAs" input with "AllDataTypes_HighlightCucumber"
        And I scroll element into view and press "conflate2"
        Then I wait 30 "seconds" to see "Conflating …"
        Then I wait 3 "minutes" to see "AllDataTypes_HighlightCucumber"
        And I wait 30 "seconds" to not see "Please wait while panning to review item."

    Scenario: I add a point to the map
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "350","300"

    Scenario: I select a feature and see an orange highlight
        When I select a node map feature with OSM id "n-1"
        Then I should see a node element "n-1" with a selected highlight
        # Then I de-select the feature
        And I click the "map" at "450","300"
        # Then I click off

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
