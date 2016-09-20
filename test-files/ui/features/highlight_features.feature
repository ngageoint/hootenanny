Feature: Highlight features on click

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Add Data
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesMerged" Dataset
        And I press "Add Layer"
        And I accept the alert
        Then I wait 30 "seconds" to see "AllDataTypesMerged"
        When I click the "add-point" button
        And I hover over "#map"
        And I click the "map" at "350","300"

    Scenario: I click on a feature and see a highlight
        When I select a node map feature with OSM id "n-1"
        Then I should see a node element "n-1" with a selected highlight
        And I hover over "#map"
        And I click the "map" at "450","300"
        # Then I click off

    Scenario: I can see a highlight on the active review features
        And I should see an element with class "activeFeature" on the map
        And I should see an element with class "activeFeature2" on the map
    
    Scenario: I can see a highlight on the added feature
        When I click to expand Map Data
        Then I click the "a.hide-toggle" with text "Map Features"
        Then I turn on highlight edited features
        And I hover over "#map"
        And I click the "map" at "450","300"
        And I should see an element with class "edited unsaved" on the map

