$test = true

require 'test/unit'
require 'sample_tbl'

require 'statistics2'

class TestStatistics2 < Test::Unit::TestCase
  module Base
    extend Statistics2::Base
  end

  module Extension
    extend Statistics2::Extension
  end

  def test_normal
    norm_tbl(0.0, 3.1) do |x|
      a, b = Base.normal___x(x), Extension.normal___x(x)
      assert_in_delta a, b, 0.000001
    end
  end

  def test_chi
    chi2_tbl() do |n, x|
      a, b = Base.pchi2_x(n, x), Extension.pchi2_x(n, x)
      assert_in_delta a/b, 1.0, 0.01
    end
  end

  def test_t
    t_tbl() do |n, x|
      a, b = Base.ptx__x(n, x), Extension.ptx__x(n, x)
      assert_in_delta a, b, 0.001
    end
  end

  def test_f
    [0.01, 0.025, 0.05].each do |opt|
      f_tbl(opt) do |n1, n2, y|
        a, b = Base.pf_x(n1, n2, y), Extension.pf_x(n1, n2, y)
        assert_in_delta a/b, 1.0, 0.01
      end
    end
  end
end
