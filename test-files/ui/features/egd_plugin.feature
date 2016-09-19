Feature: DG G-EGD plugin

    Scenario: The image carousel loads tiles
        Given I am on Hootenanny at location "18.00/151.21546/-33.85727"
        And I resize the window
        And I click Get Started
        Given that the EGD plugin is available
        When I click the image carousel button
        Then I wait 10 seconds to see image thumbnails
        When I hover over the first thumbnail
        Then I should see an image footprint on the map
        When I hover over "#map"
        Then I should not see an image footprint on the map
        When I click on the first thumbnail
        Then I should see an image overlay on the map
        When I click on the first thumbnail
        Then I should not see an image overlay on the map
        When I double click on the first thumbnail
        Then I should see "Image Metadata"
        Then I click on "span.carousel-close"
        When I select the "dgarrow.up" div
        When I select the "dgarrow.down" div
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click on "button.zoom-out"
        And I wait 1 seconds
        When I click the image carousel button
        And I wait 1 seconds
        Then I should see "Zoom in to display image metadata"

    Scenario: The image carousel says to zoom in
        Given I am on Hootenanny at location "10.00/151.21546/-33.85727"
        And I resize the window
        And I click Get Started
        Given that the EGD plugin is available
        When I click the image carousel button
        Then I should see "Zoom in to display image metadata"