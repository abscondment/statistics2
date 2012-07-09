require File.expand_path('../lib/statistics2/version', __FILE__)

Gem::Specification.new do |s|
  s.name = %q{statistics2}
  s.version = Statistics2::VERSION
  s.authors = ["Brendan Ribera", "Shin-ichiro Hara"]
  s.date = %q{2010-01-26}
  s.description = %q{Statistics2 is a module that provides normal, Chi-square, t- and F- probability distributions for Ruby. It is a fork/continuation of Shin-ichiro Hara's original code. It provides a native, compiled extension and a pure Ruby implementation.}
  s.email = ["brendan.ribera@gmail.com", "sinara@blade.nagaokaut.ac.jp"]
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["README.rdoc", "History.rdoc"]
  s.files = Dir["lib/**/*.rb", "ext/**/*.c", "ext/extconf.rb", "examples/**/*.rb"]
  s.homepage = %q{http://github.com/abscondment/statistics2}
  s.rdoc_options = ["--main", "README.rdoc"]
  s.require_paths = ["lib", "ext"]
  s.rubyforge_project = %q{statistics2}
  s.rubygems_version = %q{1.3.5}
  s.summary = %q{Statistical Distributions for Ruby. Based on Shin-ichiro Hara's original library, updated for Ruby 1.9}
  s.test_files = Dir["test/**/*.rb"]
end
