Please read the Hootenanny Developer's Guide for details on setting up an environment, coding standards, and 
development process. Hootenanny developers use a customization of the 
[Gitflow workflow](https://www.atlassian.com/git/tutorials/comparing-workflows#gitflow-workflow).
## Workflow Summary
* Open a GitHub issue for the new feature to be worked on.
* Perform work for the feature on a new git feature branch derived from the master branch and named with the number of the GitHub issue that was opened.
* Ensure your changes are adequately tested via CPPUnit, conflate case, or command line tests and that they are properly documented.
via code comments and updates to the User Guide.
* Open a pull request to merge the feature branch into the master branch and assign at least one individual to review the changes before merging.
* Correct test failures as needed.
* Check the quality of your changes using [Sonar](https://sonarcloud.io/dashboard?id=hoot) and improve them as necessary before merging.
* Once the pull request is approved, all tests are passing, and all Sonar issues have been dealt with, merge the pull request.
* Check the status of the nightly regression tests the following day for test failures and fix as needed.
