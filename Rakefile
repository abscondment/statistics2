task :extension do
  Dir.chdir File.expand_path('../ext', __FILE__) do
    system "ruby extconf.rb"
    system "make"
  end
end

task :test => :extension do
  Dir.chdir File.expand_path('..', __FILE__) do
    system "ruby -w -I lib -I ext -r test/unit -e 'Test::Unit::AutoRunner.run' test"
  end
end

task :default => :test
