Feature: Display inset map

    Scenario: I can launch Hootenanny and open the inset map
        Given I am on Hootenanny
        And I resize the window
        And I click Get Started
        When I click the "/" key
        Then I should see the previously hidden "div.map-in-map" on the page
        When I click the "/" key
        Then I should not see the "div.map-in-map" on the page

    Scenario: I can check minimap and see the inset map
        When I click the Background settings button
        And I should see checkbox "Minimap" unchecked
        And I check the "Minimap" checkbox
        Then I should see the previously hidden "div.map-in-map" on the page
        And I uncheck the "Minimap" checkbox
        Then I should not see the "div.map-in-map" on the page

    Scenario: I can resize the sidebar
        When I expand the sidebar
        When I click the "/" key
        Then I should see the previously hidden "div.map-in-map" on the page
        When I click the "/" key
        Then I should not see the "div.map-in-map" on the page
