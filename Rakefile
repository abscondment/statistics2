require 'rubygems'
require 'hoe'
require './lib/statistics2/version'

EXT = "ext/statistics2.so"

Hoe.spec 'statistics2' do
  developer 'Brendan Ribera', 'brendan.ribera@gmail.com'
  developer 'Shin-ichiro Hara', 'sinara@blade.nagaokaut.ac.jp'
  
  self.version      = Statistics2::VERSION
  self.readme_file  = 'README.rdoc'
  self.history_file = 'History.rdoc'
  self.url          = 'http://github.com/abscondment/statistics2'
  self.summary      = 'Statistical Distributions for Ruby. Based on Shin-ichiro Hara\'s original library, updated for Ruby 1.9'
  self.description  = 'Statistics2 is a module that provides normal, Chi-square, t- and F- probability distributions for Ruby. It is a fork/continuation of Shin-ichiro Hara\'s original code. It provides a native, compiled extension and a pure Ruby implementation.'
  # C extension goodness
  self.spec_extras[:extensions] = "ext/extconf.rb"
  self.clean_globs << EXT << 'ext/*.o' << 'ext/Makefile'
end

desc "Compile extensions"
task :compile => EXT
task :test => :compile

file EXT => ['ext/extconf.rb', 'ext/statistics2.c'] do
  Dir.chdir 'ext' do
    ruby 'extconf.rb'
    sh 'make'
  end
end

desc "Prepare for github upload"
task :github do
  system "git ls-files | egrep -v \"\\.gitignore\" > Manifest.txt"
  system "rake debug_gem | egrep -v \"^\\(in\" > statistics2.gemspec"
end

task :gem => :github

task :default => [:test]
