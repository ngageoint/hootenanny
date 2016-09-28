Feature: Display inset map

    Scenario: I can launch Hootenanny and open the inset map
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        When I click the "/" key
        Then I should see a ".map-in-map" on the map
        When I click the "/" key

    Scenario: I can resize the sidebar
        When I expand the sidebar
        When I click the "/" key
        Then I should see a ".map-in-map" on the map