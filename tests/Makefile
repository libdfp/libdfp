libdfp_c_tests = test-printf test-amort test-decode test-inexact-exception \
		 test-strtod test-cast-to-overflow test-numdigits test-get_digits \
		 test-fenv test-bfp-conversions test-type-conversions test-wchar \
		 test-getexp test-setexp test-left_justify test-cast-to-underflow \
		 test-fpclassify-d32 test-fpclassify-d64 test-fpclassify-d128 \
		 test-fabs-d32 test-fabs-d64 test-fabs-d128 \
		 test-copysign-d32 test-copysign-d64 test-copysign-d128 \
		 test-quantum-d32 test-quantum-d64 test-quantum-d128 \
		 test-quantize-d32 test-quantize-d64 test-quantize-d128 \
		 test-samequantum-d32 test-samequantum-d64 test-samequantum-d128 \
		 test-logb-d32 test-logb-d64 test-logb-d128 \
		 test-llquantexp-d32 test-llquantexp-d64 test-llquantexp-d128 \
		 test-isnan-d32 test-isnan-d64 test-isnan-d128 \
		 test-isnan_internal-d32 test-isnan_internal-d64 test-isnan_internal-d128 \
		 test-isinf-d32 test-isinf-d64 test-isinf-d128 \
		 test-isfinite-d32 test-isfinite-d64 test-isfinite-d128 \
		 test-issignaling-d32 test-issignaling-d64 test-issignaling-d128 \
		 test-ceil-d32 test-ceil-d64 test-ceil-d128 \
		 test-floor-d32 test-floor-d64 test-floor-d128 \
		 test-nearbyint-d32 test-nearbyint-d64 test-nearbyint-d128 \
		 test-round-d32 test-round-d64 test-round-d128 \
		 test-roundeven-d32 test-roundeven-d64 test-roundeven-d128 \
		 test-trunc-d32 test-trunc-d64 test-trunc-d128 \
		 test-isgreater-d32 test-isgreater-d64 test-isgreater-d128 \
		 test-isgreaterequal-d32 test-isgreaterequal-d64 test-isgreaterequal-d128 \
		 test-isless-d32 test-isless-d64 test-isless-d128 \
		 test-islessequal-d32 test-islessequal-d64 test-islessequal-d128 \
		 test-rint-d32 test-rint-d64 test-rint-d128 \
		 test-lround-d32 test-lround-d64 test-lround-d128 \
		 test-llround-d32 test-llround-d64 test-llround-d128 \
		 test-lrint-d32 test-lrint-d64 test-lrint-d128 \
		 test-llrint-d32 test-llrint-d64 test-llrint-d128 \
		 test-isnormal-d32 test-isnormal-d64 test-isnormal-d128 \
		 test-signbit-d32 test-signbit-d64 test-signbit-d128 \
		 test-fmin-d32 test-fmin-d64 test-fmin-d128 \
		 test-fmax-d32 test-fmax-d64 test-fmax-d128 \
		 test-fdim-d32 test-fdim-d64 test-fdim-d128 \
		 test-isunordered-d32 test-isunordered-d64 test-isunordered-d128 \
		 test-islessgreater-d32 test-islessgreater-d64 test-islessgreater-d128 \
		 test-ldexp-d32 test-ldexp-d64 test-ldexp-d128 \
		 test-fmaxmag-d32 test-fmaxmag-d64 test-fmaxmag-d128 \
		 test-fminmag-d32 test-fminmag-d64 test-fminmag-d128 \
		 test-math-macros test-strfrom \
		 test-nextup-d32 test-nextup-d64 test-nextup-d128 \
		 test-nextdown-d32 test-nextdown-d64 test-nextdown-d128

ifeq ($(libdfp-has-timode),yes)
libdfp_c_tests += test-fix test-float
endif

include $(top_srcdir)/tests/Makefile.autotest

libdfp_c_autotests = $(foreach test, $(libdfp-auto-tests), $(foreach type, d32 d64 d128, test-$(test)-$(type)))

$(addsuffix .out,$(libdfp_c_autotests)) : test-extra-opts += -u $(basename $^).ulp

libdfp_c_tests += $(libdfp_c_autotests)

test-%-d%: $(addsuffix .os,$@)
	$(CC) $(CFLAGS) $(sysdeps-CFLAGS) $(GLIBC_LIBS) -L$(top_builddir)/ -ldfp $(top_builddir)/$^ -o $@
test-%-d32.c: %.input
	$(PYTHON) $(top_srcdir)/tests/gen-libdfp-tests.py -t decimal32 $^ > $(top_builddir)/$@
test-%-d64.c: %.input
	$(PYTHON) $(top_srcdir)/tests/gen-libdfp-tests.py -t decimal64 $^ > $(top_builddir)/$@
test-%-d128.c: %.input
	$(PYTHON) $(top_srcdir)/tests/gen-libdfp-tests.py -t decimal128 $^ > $(top_builddir)/$@

ulps-file = $(shell find $(sysdep_dirs:%=$(top_srcdir)/%/) \
			 -name libdfp-test-ulps | head -1)

libdfp-test-ulps.h: $(ulps-file)
	$(PYTHON) $(top_srcdir)/tests/gen-libdfp-ulps.py $< -o $@

# Add the ULP file generation explicity rule
$(addprefix $(objpfx), $(libdfp_c_tests)): $(top_builddir)/libdfp-test-ulps.h

regen-ulps:
	for i in $(libdfp-auto-tests); do \
	  echo "test $$i"; \
	  cat test-$$i-*.ulp; \
	  echo; \
	done > ulps
.PHONY: regen-ulps
