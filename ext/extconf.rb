require "mkmf"
create_makefile("statistics2")
open("Makefile", "a") do |f|
  f << "test:\n\truby -I./lib:. c_tests.rb\n"
  f << "\n"
  f << "uninstall:\n"
  f << "\t@echo rm $(RUBYLIBDIR)/statistics2.rb\n"
  f << "\t@$(RM) $(RUBYLIBDIR)/statistics2.rb\n"
  f << "\t@echo rm $(RUBYARCHDIR)/statistics2.{o,so}\n"
  f << "\t@$(RM) $(RUBYARCHDIR)/statistics2.{o,so}\n"
end
