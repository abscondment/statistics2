require 'statistics2/version'

#   distributions of statistics
#     by Shin-ichiro HARA
#
#   Ref:
#     [1] http://www.matsusaka-u.ac.jp/~okumura/algo/
#     [2] http://www5.airnet.ne.jp/tomy/cpro/sslib11.htm

module Statistics2
  require 'statistics2/base'
  extend Base

  if not defined? Statistics2::NO_EXT
    begin
      require '_statistics2'
      extend Extension
    rescue LoadError
    end
  end
end
