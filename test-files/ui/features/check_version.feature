Feature: Check Version

    Scenario: Check version popup
        Given I am on Hootenanny
        And I click Get Started
        When I select the "hoot_label" div
        Then I should see "About Hootenanny"
        Then I should see "Main Versions:"

    Scenario: Check version types
        Then I should see "Hootenanny Core - Version:"
        Then I should see "Hootenanny Web Services - Version:"
        Then I should see "Hootenanny iD Editor - Version:"

    Scenario: I can download Hoot User Guide
        Then I should see "Download User Guide"
        When I click on "a.dark.fr.button.loud.pad2x"
        Then I wait 5 seconds
        Then the download file "Hootenanny - User Interface Guide.pdf" should exist
