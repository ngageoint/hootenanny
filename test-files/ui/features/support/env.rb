require 'capybara'  
require 'capybara/cucumber'  
require 'capybara/webkit'
require 'rspec'  
require 'selenium/webdriver'  
  
# Use headless webkit, can cause issues
#DRIVER=:webkit

# Use chrome
DRIVER=:chrome
$driver = Selenium::WebDriver.for(:chrome)

Capybara.register_driver :javascript do |app|
  Capybara::Selenium::Driver.new(app, :browser => DRIVER)
end
Capybara.register_driver :chrome do |app|
  Capybara::Selenium::Driver.new(app, :browser => DRIVER)
end
Capybara.javascript_driver = DRIVER
Capybara.default_driver = DRIVER
Capybara.default_max_wait_time = 60
#Capybara.ignore_hidden_elements = false

if DRIVER == :webkit
end
