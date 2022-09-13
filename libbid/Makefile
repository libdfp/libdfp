libdfp_files += _addsub_dd _dd_to_sf _dd_to_xf _di_to_td _eq_td _gt_td \
				_le_td _mul_td _sd_to_di _sd_to_usi _si_to_sd _td_to_sf \
				_tf_to_dd _unord_dd _xf_to_dd _addsub_sd _dd_to_si \
				_df_to_dd _div_dd _ge_dd _lt_dd _ne_dd _sd_to_sf _sd_to_xf \
				_si_to_td _td_to_si _tf_to_sd _unord_sd _xf_to_sd _addsub_td \
				_dd_to_td _df_to_sd _div_sd _ge_sd _lt_sd _ne_sd _sd_to_si \
				_sf_to_dd _td_to_dd _td_to_tf _tf_to_td _unord_td _xf_to_td \
				_dd_to_df _dd_to_tf _df_to_td _div_td _ge_td _lt_td _ne_td \
				_sd_to_td _sf_to_sd _td_to_df _td_to_udi _udi_to_dd _usi_to_dd \
				_dd_to_di _dd_to_udi _di_to_dd _eq_dd _gt_dd _le_dd _mul_dd \
				_sd_to_dd _sd_to_tf _sf_to_td _td_to_di _td_to_usi _udi_to_sd \
				_usi_to_sd _dd_to_sd _dd_to_usi _di_to_sd _eq_sd _gt_sd _le_sd \
				_mul_sd _sd_to_df _sd_to_udi _si_to_dd _td_to_sd _td_to_xf \
				_udi_to_td _usi_to_td _dd_to_hf _hf_to_dd _hf_to_sd _hf_to_sd \
				_hf_to_td _sd_to_hf _td_to_hf

libdfp_files += bid_decimal_globals bid_decimal_data \
         bid_binarydecimal bid_convert_data \
         _isinfd32 _isinfd64 _isinfd128 bid64_noncomp \
         bid128_noncomp bid128_fma bid_round bid_from_int \
         bid64_add bid128_add bid64_div bid128_div \
         bid64_mul bid128_mul bid64_compare bid128_compare \
         bid128 bid32_to_bid64 bid32_to_bid128 bid64_to_bid128 \
         bid64_to_int32 bid64_to_int64 \
         bid64_to_uint32 bid64_to_uint64 \
         bid128_to_int32 bid128_to_int64 \
         bid128_to_uint32 bid128_to_uint64

# Ensure rounding mode is per-thread.
# TODO: Should this be via autoconf instead?
sysdeps-CFLAGS += -DUSE_TLS -DHAVE_CC_TLS

# TODO: TImode and TFmode support are missing from the above.
# for now, use BID -> decnumber
ifeq ($(libdfp-has-timode),yes)
libdfp_files += fixsdti fixddti \
		   fixunssdti fixunsddti \
		   fixtdti fixunstdti \
		   floattitd floatunstitd \
		   floattidd floatunstidd \
		   floattisd floatunstisd
endif
