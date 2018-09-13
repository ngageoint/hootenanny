#Cucumber provides a number of hooks which allow us to run blocks at various points in the Cucumber test cycle

Before do
  # Do something before each scenario.
end

Before do |scenario|
  # The +scenario+ argument is optional, but if you use it, you can get the title,
  # description, or name (title + description) of the scenario that is about to be
  # executed.
end

After do
  # Do something after each scenario.

  # If running instrumented code, POST the coverage data to istanbul-middleware
  page.execute_script("(function () { if (window.__coverage__) { d3.xhr('http://localhost:8880/coverage/client').header('Content-Type', 'application/json').post(JSON.stringify(window.__coverage__),function(err, data){});}}())");
end

After do |scenario|
  # Do something after each scenario.
  # The +scenario+ argument is optional, but
  # if you use it, you can inspect status with
  # the #failed?, #passed? and #exception methods.
end

#Tagged hooks

Before('@Ex_tag1 or @Ex_tag2') do
  # This will only run before scenarios tagged
  # with @cucumis OR @sativus.
end

AfterStep('@Ex_tag1 or @Ex_tag2') do
  # This will only run after steps within scenarios tagged
  # with @cucumis AND @sativus.
end

scenario_times = {}

Around() do |scenario, block|
  start = Time.now
  block.call
  scenario_times["#{scenario.feature.file}::#{scenario.name}"] = Time.now - start
end

at_exit do
  max_scenarios = scenario_times.size > 20 ? 20 : scenario_times.size
  puts "------------- Top #{max_scenarios} slowest scenarios -------------"
  sorted_times = scenario_times.sort { |a, b| b[1] <=> a[1] }
  sorted_times[0..max_scenarios - 1].each do |key, value|
    puts "#{value.round(2)}  #{key}"
  end
end
