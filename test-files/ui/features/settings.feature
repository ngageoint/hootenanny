@javascript
Feature: Hootenanny Settings Test
    As a software tester
    I want test the manage tab
    So I can ensure that it is working properly

    Scenario: Starting Hootenanny
        Given I am on Hootenanny
        And I click Get Started

    Scenario: Validating Hootenanny Version
        And I click the "Hootenanny" link
        Then I wait 5 "seconds" to see "Hootenanny Core - Version:"
        And I should see "Hootenanny Web Services - Version:"
        And I should see "Hootenanny iD "
        #When I click the "Download User Guide" link
        #Then I should download the User Guide
        Then I select the "x" div
