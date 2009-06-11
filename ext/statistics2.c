/* 
   statistics2.c

   distributions of statistics2             
   by Shin-ichiro HARA

   2003.09.25

   Ref:
   [1] http://www.matsusaka-u.ac.jp/~okumura/algo/
   [2] http://www5.airnet.ne.jp/tomy/cpro/sslib11.htm
*/

#include "ruby.h"
#include <math.h>
#include <errno.h>

#ifndef RUBY_19
#ifndef RFLOAT_VALUE
#define RFLOAT_VALUE(v) (RFLOAT(v)->value)
#endif
#endif

#define PI 3.14159265358979324
#define Need_Float(x) (x) = rb_Float(x)

VALUE rb_mStatistics2;

/* normal distribution ([1]) */
/* P( (-\infty, z] ) */
static double p_nor(double z)
{
    int i, e;
    double z2, prev, p, t;

    if (z < -12) {return 0.0;}
    if (z > 12) {return 1.0;}
    if (z == 0.0) {return 0.5;}

    if (z > 0) {
        e = 1;
    } else if (z == 0) {
        return 0.5;
    } else {
        e = 0;
        z = -z;
    }
                
    z2 = z * z;
    t = p = z * exp(-0.5 * z2) / sqrt(2 * PI);
    for (i = 3; i < 200; i += 2) {
        prev = p;  t *= z2 / i;  p += t;
        if (p <= prev) return(e ? 0.5 + p : 0.5 - p);
    }
    return (e ? 1.0 : 0.0);
}

/* inverse of normal distribution ([2]) */
/* P( (-\infty, z] ) = qn -> z a*/
static double pnorm(double qn)
{
    static double b[11] = {1.570796288,     0.03706987906,  -0.8364353589e-3,
                           -0.2250947176e-3, 0.6841218299e-5, 0.5824238515e-5,
                           -0.104527497e-5,  0.8360937017e-7,-0.3231081277e-8,
                           0.3657763036e-10,0.6936233982e-12};
	double w1, w3;
	int i;

	if(qn < 0. || 1. < qn)
	{
		fprintf(stderr, "Error : qn <= 0 or qn >= 1 in pnorm()!\n");
		return 0.;
	}
	if(qn == 0.5)	return 0.;

	w1 = qn;
	if(qn > 0.5)	w1 = 1. - w1;
	w3 = -log(4. * w1 * (1. - w1));
	w1 = b[0];
	for(i = 1; i < 11; i++)	w1 += (b[i] * pow(w3, (double)i));
	if(qn > 0.5)	return sqrt(w1 * w3);
	return -sqrt(w1 * w3);
}


/* normal-distribution interface */
static double normaldist(z)
    double z;
{
    return p_nor(z);
}

static double pnormaldist(qn)
    double qn;
{
    return pnorm(qn);
}


/* chi-square distribution ([1]) */
/* [x, \infty) */
static double q_chi2(int df, double chi2)
{
    int k;
    double s, t, chi;

    if (df & 1) {
        chi = sqrt(chi2);
        if (df == 1) return 2 * (1.0 - normaldist(chi));
        s = t = chi * exp(-0.5 * chi2) / sqrt(2 * PI);
        for (k = 3; k < df; k += 2) {
            t *= chi2 / k;  s += t;
        }
        return 2 * (1.0 - normaldist(chi) + s);
    } else {
        s = t = exp(-0.5 * chi2);
        for (k = 2; k < df; k += 2) {
            t *= chi2 / k;  s += t;
        }
        return s;
    }
}

/* inverse of chi-square distribution */
/*
static double chi2dens(n, x)
    int n;
double x;
{
    double n2 = ((double) n)/2.0;
    return 1.0 / pow(2, n2) / gamma(n2) * pow(x,(n2 - 1.0)) * exp(-x/2.0);
}
*/
/*
  static double newton_chi(n, y, ini)
  int n;
  double y, ini;
  {
  double epsilon = 1.0e-6, x = ini, prev, df, f;
  int limit = 30, i;
  for (i = 0; i < 30; i++) {
  prev = x;
  f = q_chi2(n, prev);
  df = - chi2dens(n, prev);
  x = (y - f)/df + prev;
  if (fabs(x - prev) < epsilon) return x;
  }
  fprintf(stderr, "Warning(newton approximation): over limit\n");
  return x;
  }
*/

/* [x, \infty) */
static double pchi2(y, n)
    int n;
double y;
{
    double v, s, qe, eps, w;
    if (n == 1) {
        w = pnorm(1.0 - y/2);
        return(w * w);
    } else if (n == 2) {
        /*        v = (1.0 / y - 1.0) / 33.0;
                  return newton_chi(n, y, v); */
        return(-2.0 * log(y));
    }
    else {
        eps = 1.0e-5;
        v = 0.0;
        s = 10.0;
        for (;;) {
            v += s;
            if (s <= eps) break;
            if ((qe = q_chi2(n, v) - y) == 0.0) break;
            if (qe < 0.0) {
                v -= s;
                s /= 10.0;
            }
        }
        return v;
    }
}

/* chi-square-distribution interface */

static double chi2dist(df, chi2)
    int df;
double chi2;
{
    return 1.0 - q_chi2(df, chi2);
}

static double pchi2dist(n, y)
    double y;
int n;
{
    return pchi2(1.0 - y, n);
}


/* t-distribution ([1]) */
/* (-\infty, x] */
double p_t(int df, double t)
{
    int i;
    double c2, p, s;

    c2 = df / (df + t * t);
    s = sqrt(1 - c2);  if (t < 0) s = -s;
    p = 0.0;
    for (i = df % 2 + 2; i <= df; i += 2) {
        p += s;  s *= (i - 1) * c2 / i;
    }
    if (df & 1)
        return 0.5+(p*sqrt(c2)+atan(t/sqrt(df)))/PI;
    else
        return (1.0 + p) / 2.0;
}


double ptsub(double q, int n)
{
	double eps, qe, s, w;

	if(n == 1 && 0.001 <= q && q < 0.01) eps = 1.e-4;
	else if (n == 2 && q < 0.0001) eps = 1.e-4;
	else if (n == 1 && q < 0.001) eps = 1.e-2;
	else eps = 1.e-5;
	s = 10000.;
	w = 0.;
	for(;;)
	{
		w += s;
		if(s <= eps)	return w;
		if((qe = 2.0 - p_t(n, w)*2.0 - q) == 0.)	return w;
		if(qe < 0.)
		{
			w -= s;
			s /= 10.;
		}
	}
} 

/* inverse of t-distribution ([2]) */
/* (-\infty, -q/2] + [q/2, \infty) */
double pt(double q, int n)
{
	double f, f1, f2, f3, f4, f5, u, u2, w, w0, w1, w2, w3, w4;

	if(q < 1.e-5 || q > 1. || n < 1)
	{
		fprintf(stderr,"Error : Illigal parameter  in pt()!\n");
		return 0.;
	}

	if(n <= 5)	return ptsub(q, n);

	if(q <= 5.e-3 && n <= 13)	return ptsub(q, n);

	f1 = 4. * (f = (double)n);
	f5 = (f4 = (f3 = (f2 = f * f) * f) * f) * f;
	f2 *= 96.;
	f3 *= 384.;
	f4 *= 92160.;
	f5 *= 368640.;
	u = pnormaldist(1. - q / 2.);

	w0 = (u2 = u * u) * u;
	w1 = w0 * u2;
	w2 = w1 * u2;
	w3 = w2 * u2;
	w4 = w3 * u2;
	w = ((w0 + u) / f1);
	w += ((5. * w1 + 16. * w0 + 3. * u) / f2);
	w += ((3. * w2 + 19. * w1 + 17. * w0 - 15. * u) / f3);
	w += ((79. * w3 + 776. * w2 + 1482. * w1 - 1920. * w0 - 945. * u) / f4);
	w += ((27. * w4 + 339. * w3 + 930. * w2 - 1782. * w1 - 765. * w0
           + 17955. * u) / f5);
	return u + w;
}


/* t-distribution interface */
static double tdist(n, t)
    int n;
double t;
{
    return p_t(n, t);
}

static double ptdist(n, y)
    double y;
int n;
{
    return (y > 0.5 ? pt(2.0 - y*2.0, n) : -pt(y*2.0, n));
}


/* F-distribution ([1]) */
/* [x, \infty) */
double q_f(int df1, int df2, double f)
{
    int i;
    double cos2, sin2, prob, temp;

    if (f <= 0) return 1;
    if (df1 % 2 != 0 && df2 % 2 == 0)
        return 1 - q_f(df2, df1, 1 / f);
    cos2 = 1 / (1 + df1 * f / df2);  sin2 = 1 - cos2;
    if (df1 % 2 == 0) {
        prob = pow(cos2, df2 / 2.0);  temp = prob;
        for (i = 2; i < df1; i += 2) {
            temp *= (df2 + i - 2) * sin2 / i;
            prob += temp;
        }
        return prob;
    }
    prob = atan(sqrt(df2 / (df1 * f)));
    temp = sqrt(sin2 * cos2);
    for (i = 3; i <= df1; i += 2) {
        prob += temp;  temp *= (i - 1) * sin2 / i;
    }
    temp *= df1;
    for (i = 3; i <= df2; i += 2) {
        prob -= temp;
        temp *= (df1 + i - 2) * cos2 / i;
    }
    return prob * 2 / PI;
}

/* inverse of F-distribution ([2]) */
double pfsub(double x, double y, double z)
{
	return (sqrt(z) - y) / x / 2.;
}

/* [x, \infty) */
double pf(double q, int n1, int n2)
{
	double a, b, c, d, eps, fw, qe, qn, s, u, u2, w1, w2, w3, w4;

	if(q < 0. || q > 1. || n1 < 1 || n2 < 1)
	{
		fprintf(stderr,"Error : Illegal parameter  in pf()!\n");
		return 0.;
	}

	if(n1 <= 240 || n2 <= 240)
	{
		eps = 1.e-5;
		if(n2 == 1)	eps = 1.e-4;
		s = 1000.;
		fw = 0.;
		for(;;)
		{
            fw += s;
            if(s <= eps)	return fw;
            if((qe = q_f(n1, n2, fw) - q) == 0.) return fw;
            if(qe < 0.)
			{
                fw -= s;
                s /= 10.;
			}
		}
	}

	eps = 1.e-6;
	qn = q;
	if(q < 0.5)	qn = 1. - q;
	u = pnormaldist(qn);
	w1 = 2. / (double)n1 / 9.;
	w2 = 2. / (double)n2 / 9.;
	w3 = 1. - w1;
	w4 = 1. - w2;
	u2 = u * u;
	a = w4 * w4 - u2 * w2;
	b = -2. * w3 * w4;
	c = w3 * w3 - u2 * w1;
	d = b * b - 4 * a * c;
	if(d < 0.)	fw = pfsub(a, b, 0.);
	else
	{
		if(fabs(a) > eps)	fw = pfsub(a, b, d);
		else
		{
			if(fabs(b) > eps)	return -c / b;
			fw = pfsub(a, b, 0.);
		}
	}
	return fw * fw * fw;
}

/* F-distribution interface */
static double fdist(n1, n2, f)
    int n1, n2;
double f;
{
    return 1.0 - q_f(n1, n2, f);
}

static double pfdist(n1, n2, y)
    int n1, n2;
double y;
{
    return pf(1.0 - y, n1, n2);
}


/* discrete distributions */
static int perm(n, x)
    int n, x;
{
    int r = 1;
    if (n < 0 || x < 0) rb_raise(rb_eRangeError, "parameters should be positive");
    while (x >= 1) {
        r *= n;
        n -= 1;
        x -= 1;
    }
    return r;
}
  
static int combi(n, x)
    int n, x;
{
    if (n < 0 || x < 0) rb_raise(rb_eRangeError, "parameters should be positive");
    if (x*2 > n) x = n - x;
    return perm(n, x) / perm(x, x);
}

static float bindens(n, p, x)
    int n, x;
float p;
{
    float q = 1.0 - p;
    return combi(n, x) * pow(p, x) * pow(q, n - x);
}
  
static float bindist(n, p, x)
    int n, x;
float p;
{
    float s = 0.0;
    int k;
    for (k = 0; k <= x; k ++) {
        s += bindens(n, p, k);
    }
    return s;
}

static float poissondens(m, x)
    float m;
int x;
{
    if (x < 0) return 0.0 ;
    return pow(m, x) * exp(-m) / perm(x, x);
}

static float poissondist(m, x)
    float m;
int x;
{
    float s = 0.0;
    int k;
    for (k = 0; k <= x; k ++) {
        s += poissondens(m, k);
    }
    return s;
}

/* normal-distribution */
static VALUE rb_normaldist(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(normaldist(RFLOAT_VALUE(x)));
}

static VALUE rb_normalxXX_(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(normaldist(RFLOAT_VALUE(x)));
}

static VALUE rb_normal__X_(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(normaldist(RFLOAT_VALUE(x)) - 0.5);
}

static VALUE rb_normal___x(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(1.0 - normaldist(RFLOAT_VALUE(x)));
}

static VALUE rb_normalx__x(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(2.0 - normaldist(RFLOAT_VALUE(x)) * 2.0);
}

/* inverse of normal-distribution */
static VALUE rb_pnormaldist(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(pnormaldist(RFLOAT_VALUE(x)));
}

static VALUE rb_pnormalxXX_(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(pnormaldist(RFLOAT_VALUE(x)));
}

static VALUE rb_pnormal__X_(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(pnormaldist(RFLOAT_VALUE(x) + 0.5));
}

static VALUE rb_pnormal___x(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(pnormaldist(1.0 - RFLOAT_VALUE(x)));
}

static VALUE rb_pnormalx__x(mod, x)
    VALUE mod, x;
{
    Need_Float(x);
    return rb_float_new(pnormaldist(1.0 - (RFLOAT_VALUE(x))/2.0));
}


/* chi-square-distribution */
static VALUE rb_chi2_x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(1.0 - chi2dist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_pchi2_x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(pchi2dist(FIX2INT(n), 1.0 - (RFLOAT_VALUE(x))));
}

static VALUE rb_chi2X_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(chi2dist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_pchi2X_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(pchi2dist(FIX2INT(n), RFLOAT_VALUE(x)));
}


/* inverse of chi-square-distribution */
static VALUE rb_chi2dist(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(1.0 - (q_chi2(FIX2INT(n), RFLOAT_VALUE(x))));
}

static VALUE rb_pchi2dist(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(pchi2(1.0 - (RFLOAT_VALUE(x)), FIX2INT(n)));
}


/* t-distribution */
static VALUE rb_tdist(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(tdist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_tx__x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(2.0 - tdist(FIX2INT(n), RFLOAT_VALUE(x))*2.0);
}

static VALUE rb_txXX_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(tdist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_t__X_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(tdist(FIX2INT(n), RFLOAT_VALUE(x)) - 0.5);
}

static VALUE rb_t___x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(1.0 - tdist(FIX2INT(n), RFLOAT_VALUE(x)));
}

/* inverse of t-distribution */
static VALUE rb_ptdist(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(ptdist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_ptx__x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(ptdist(FIX2INT(n), 1.0 - (RFLOAT_VALUE(x))/2.0));
}

static VALUE rb_ptxXX_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(ptdist(FIX2INT(n), RFLOAT_VALUE(x)));
}

static VALUE rb_pt__X_(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(ptdist(FIX2INT(n), 0.5 + (RFLOAT_VALUE(x))));
}

static VALUE rb_pt___x(mod, n, x)
    VALUE mod, n, x;
{
    Need_Float(x);
    return rb_float_new(ptdist(FIX2INT(n), 1.0 - (RFLOAT_VALUE(x))));
}

/* F-distribution */
static VALUE rb_fdist(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(fdist(FIX2INT(n1), FIX2INT(n2), RFLOAT_VALUE(x)));
}


static VALUE rb_f_x(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(1.0 - fdist(FIX2INT(n1), FIX2INT(n2), RFLOAT_VALUE(x)));
}

static VALUE rb_fX_(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(fdist(FIX2INT(n1), FIX2INT(n2), RFLOAT_VALUE(x)));
}

/* inverse of F-distribution */
static VALUE rb_pfdist(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(pfdist(FIX2INT(n1), FIX2INT(n2), RFLOAT_VALUE(x)));
}

static VALUE rb_pf_x(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(pfdist(FIX2INT(n1), FIX2INT(n2), 1.0 - (RFLOAT_VALUE(x))));
}

static VALUE rb_pfX_(mod, n1, n2, x)
    VALUE mod, n1, n2, x;
{
    Need_Float(x);
    return rb_float_new(pfdist(FIX2INT(n1), FIX2INT(n2), RFLOAT_VALUE(x)));
}

/* discrete distributions */

static VALUE rb_bindens(mod, n, p, x)
    VALUE n, p, x;
{
    Need_Float(p);
    return rb_float_new(bindens(FIX2INT(n), RFLOAT_VALUE(p), FIX2INT(x)));
}

static VALUE rb_bindist(mod, n, p, x)
    VALUE n, p, x;
{
    Need_Float(p);
    return rb_float_new(bindist(FIX2INT(n), RFLOAT_VALUE(p), FIX2INT(x)));
}

static VALUE rb_binX_(mod, n, p, x)
    VALUE n, p, x;
{
    Need_Float(p);
    return rb_float_new(bindist(FIX2INT(n), RFLOAT_VALUE(p), FIX2INT(x)));
}

static VALUE rb_bin_x(mod, n, p, x)
    VALUE n, p, x;
{
    Need_Float(p);
    return rb_float_new(bindist(FIX2INT(n), 1.0 - (RFLOAT_VALUE(p)), FIX2INT(n) - FIX2INT(x)));
}

static VALUE rb_poissondens(mod, m, x)
    VALUE m, x;
{
    Need_Float(m);
    return rb_float_new(poissondens(RFLOAT_VALUE(m), FIX2INT(x)));
}

static VALUE rb_poissondist(mod, m, x)
    VALUE m, x;
{
    Need_Float(m);
    return rb_float_new(poissondist(RFLOAT_VALUE(m), FIX2INT(x)));
}

static VALUE rb_poissonX_(mod, m, x)
    VALUE m, x;
{
    Need_Float(m);
    return rb_float_new(poissondist(RFLOAT_VALUE(m), FIX2INT(x)));
}

/*
  static VALUE rb_poisson_x(mod, m, x)
  VALUE m, x;
  {
  Need_Float(m);
  return rb_float_new(1.0 - poissondist((RFLOAT_VALUE(m)), FIX2INT(x) - 1));
  }
*/

/* ruby interface */

void
Init_statistics2(void)
{
    int i;
    char *checkMethods[] = { "normaldist", "normalxXX_", "normal__X_", "normal___x", "normalx__x",
                             "pnormaldist", "pnormalxXX_", "pnormal__X_", "pnormal___x", "pnormalx__x",
                             "chi2dist", "chi2X_", "chi2_x", "pchi2dist", "pchi2X_", "pchi2_x",
                             "tdist", "txXX_", "t__X_", "t___x", "tx__x", "ptdist", "ptxXX_", "pt__X_", "pt___x", "ptx__x",
                             "fdist", "fX_", "f_x", "pfdist", "pfX_", "pf_x",
                             "bindens", "bindist", "binX_", "bin_x",
                             "poissondens", "poissondist", "poissonX_", "poisson_x" };
    VALUE methodDefined = rb_intern("private_method_defined?"),
          removeMethod  = rb_intern("remove_method");
    

    rb_mStatistics2 = rb_define_module("Statistics2");    
    /* Remove any preexisting methods before adding the new ones */
    for (i = 0; i < (sizeof(checkMethods) / sizeof(checkMethods[0])); i++) {
        if (RTEST(rb_funcall(rb_mStatistics2, methodDefined, 1, rb_str_new2(checkMethods[i])))) {
            /* Remove instance method */
            rb_funcall(rb_mStatistics2, removeMethod, 1, rb_str_new2(checkMethods[i]));
            /* Remove the singleton method */
            rb_funcall(rb_singleton_class(rb_mStatistics2), removeMethod, 1, rb_str_new2(checkMethods[i]));
        }
    }
    
    rb_define_module_function(rb_mStatistics2, "normaldist", rb_normaldist, 1);
    rb_define_module_function(rb_mStatistics2, "normalxXX_", rb_normalxXX_ , 1);
    rb_define_module_function(rb_mStatistics2, "normal__X_", rb_normal__X_, 1);
    rb_define_module_function(rb_mStatistics2, "normal___x", rb_normal___x, 1);
    rb_define_module_function(rb_mStatistics2, "normalx__x", rb_normalx__x, 1);

    rb_define_module_function(rb_mStatistics2, "pnormaldist", rb_pnormaldist, 1);
    rb_define_module_function(rb_mStatistics2, "pnormalxXX_", rb_pnormalxXX_, 1);
    rb_define_module_function(rb_mStatistics2, "pnormal__X_", rb_pnormal__X_, 1);
    rb_define_module_function(rb_mStatistics2, "pnormal___x", rb_pnormal___x, 1);
    rb_define_module_function(rb_mStatistics2, "pnormalx__x", rb_pnormalx__x, 1);


    rb_define_module_function(rb_mStatistics2, "chi2dist", rb_chi2dist, 2);
    rb_define_module_function(rb_mStatistics2, "chi2X_", rb_chi2X_ , 2);
    rb_define_module_function(rb_mStatistics2, "chi2_x", rb_chi2_x, 2);

    rb_define_module_function(rb_mStatistics2, "pchi2dist", rb_pchi2dist, 2);
    rb_define_module_function(rb_mStatistics2, "pchi2X_", rb_pchi2X_, 2);
    rb_define_module_function(rb_mStatistics2, "pchi2_x", rb_pchi2_x, 2);


    rb_define_module_function(rb_mStatistics2, "tdist", rb_tdist, 2);
    rb_define_module_function(rb_mStatistics2, "txXX_", rb_txXX_ , 2);
    rb_define_module_function(rb_mStatistics2, "t__X_", rb_t__X_, 2);
    rb_define_module_function(rb_mStatistics2, "t___x", rb_t___x, 2);
    rb_define_module_function(rb_mStatistics2, "tx__x", rb_tx__x, 2);

    rb_define_module_function(rb_mStatistics2, "ptdist", rb_ptdist, 2);
    rb_define_module_function(rb_mStatistics2, "ptxXX_", rb_ptxXX_, 2);
    rb_define_module_function(rb_mStatistics2, "pt__X_", rb_pt__X_, 2);
    rb_define_module_function(rb_mStatistics2, "pt___x", rb_pt___x, 2);
    rb_define_module_function(rb_mStatistics2, "ptx__x", rb_ptx__x, 2);


    rb_define_module_function(rb_mStatistics2, "fdist", rb_fdist, 3);
    rb_define_module_function(rb_mStatistics2, "fX_", rb_fX_ , 3);
    rb_define_module_function(rb_mStatistics2, "f_x", rb_f_x, 3);

    rb_define_module_function(rb_mStatistics2, "pfdist", rb_pfdist, 3);
    rb_define_module_function(rb_mStatistics2, "pfX_", rb_pfX_, 3);
    rb_define_module_function(rb_mStatistics2, "pf_x", rb_pf_x, 3);


    rb_define_module_function(rb_mStatistics2, "bindens", rb_bindens, 3);
    rb_define_module_function(rb_mStatistics2, "bindist", rb_bindist, 3);
    rb_define_module_function(rb_mStatistics2, "binX_", rb_binX_, 3);
    rb_define_module_function(rb_mStatistics2, "bin_x", rb_bin_x, 3);

    rb_define_module_function(rb_mStatistics2, "poissondens", rb_poissondens, 2);
    rb_define_module_function(rb_mStatistics2, "poissondist", rb_poissondist, 2);
    rb_define_module_function(rb_mStatistics2, "poissonX_", rb_poissonX_, 2);
    rb_define_module_function(rb_mStatistics2, "poisson_x", rb_bin_x, 2);
}
