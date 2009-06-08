 = Statistics2 -- Statistical Distributions for Ruby

 Authors: Shin-ichiro HARA(sinara@blade.nagaokaut.ac.jp), Brendan Ribera (brendan.ribera@gmail.com)

 == REQUIREMENTS:
 
 * ruby-1.8

 == INSTALL:

 * TODO

 === Installation of extension module (if you need):
 
 This package also contains the C extension module which has the same functions in `statistics2.rb'. Enter ext/ directory of the package. Then type:
    $ ruby extconf.rb
    $ make
    $ make test
    $ make install

 == USAGE:
 
 * Example:
    require "statistics2"
    puts Statistics2.normaldist(0.27) #=> 0.606419873198039

 == LICENSE:
 
 Ruby's (see http://www.ruby-lang.org/en/LICENSE.txt)
