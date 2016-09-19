Feature: Highlight features on click

    Scenario: Start Hootenanny
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started

    Scenario: Add Data
        And I press "Add Reference Dataset"
        And I click the "AllDataTypesACucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "AllDataTypesACucumber"
        And I press "Add Secondary Dataset"
        And I click the "AllDataTypesBCucumber" Dataset
        And I press "Add Layer"
        Then I wait 30 "seconds" to see "AllDataTypesBCucumber"

    Scenario: I click on a feature and see a highlight
        When I select a node map feature with OSM id "n15_14"
        Then I should see element "n15_14" with a highlight