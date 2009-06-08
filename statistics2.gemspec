# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{statistics2}
  s.version = "0.54"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Brendan Ribera", "Shin-ichiro Hara"]
  s.date = %q{2009-06-08}
  s.description = %q{}
  s.email = ["brendan.ribera@gmail.com", "sinara@blade.nagaokaut.ac.jp"]
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = ["Manifest.txt"]
  s.files = ["History.markdown", "Manifest.txt", "README.markdown", "Rakefile", "ext/extconf.rb", "ext/show.rb", "ext/statistics2.c", "lib/statistics2.rb", "mklist.rb", "test/sample-tbl.rb", "test/test_ext.rb", "test/test_inv.rb"]
  s.homepage = %q{http://github.com/abscondment/statistics2}
  s.rdoc_options = ["--main", "README.markdown"]
  s.require_paths = ["lib", "ext"]
  s.rubyforge_project = %q{statistics2}
  s.rubygems_version = %q{1.3.4}
  s.summary = %q{}
  s.test_files = ["test/test_inv.rb", "test/test_ext.rb"]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<hoe>, [">= 2.0.0"])
    else
      s.add_dependency(%q<hoe>, [">= 2.0.0"])
    end
  else
    s.add_dependency(%q<hoe>, [">= 2.0.0"])
  end
end
