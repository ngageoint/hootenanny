Feature: Measure Tool

    Scenario: Open the web app
        Given I am on Hootenanny
        And I click Get Started

    Scenario: Measure Length
        Given I press "Tools"
        And I hover over the "span" with text "Measurement Tools"
        Then I click the "span" with text "Measure Length"
        And I click the "map" at "100","100"
        And I double-click the "map" at "300","300"
        Then I should see a measurement line
        Then I press "Clear"

    Scenario: Measure Area
        Given I press "Tools"
        And I hover over the "span" with text "Measurement Tools"
        Then I click the "span" with text "Measure Area"
        And I click the "map" at "100","100"
        And I click the "map" at "200","200"
        And I double-click the "map" at "100","200"
        Then I should see a measurement area
        Then I press "Clear"

    Scenario: Measure Help
        Given I press "Tools"
        And I hover over the "span" with text "Measurement Tools"
        Then I click the "span" with text "Measure Help"
        Then I should see "Click anywhere on the map to start measuring"
