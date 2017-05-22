/******************************************************************************
* Copyright (C) 2017, Divideon. All rights reserved.
* No part of this code may be reproduced in any form
* without the written permission of the copyright holder.
******************************************************************************/

#ifndef XVC_ENC_LIB_ENCODER_SETTINGS_H_
#define XVC_ENC_LIB_ENCODER_SETTINGS_H_

#include "xvc_common_lib/restrictions.h"

namespace xvc {

enum struct SpeedMode {
  kPlacebo = 0,
  kSlow = 1,
  kTotalNumber = 2,
};

enum struct TuneMode {
  kDefault = 0,
  kPsnr = 1,
  kTotalNumber = 2,
};

struct EncoderSettings {
  // Initialize based on speed mode setting
  void Initialize(SpeedMode speed_mode) {
    switch (speed_mode) {
      case SpeedMode::kPlacebo:
        fast_intra_mode_eval_level = 0;
        fast_merge_eval = 0;
        bipred_refinement_iterations = 4;
        always_evaluate_intra_in_inter = 1;
        default_num_ref_pics = 3;
        max_binary_split_depth = 3;
        break;
      case SpeedMode::kSlow:
        fast_intra_mode_eval_level = 1;
        fast_merge_eval = 1;
        bipred_refinement_iterations = 1;
        always_evaluate_intra_in_inter = 0;
        default_num_ref_pics = 2;
        max_binary_split_depth = 2;
        break;
      default:
        assert(0);
        break;
    }
  }

  // Initialize based on restricted mode setting
  void Initialize(RestrictedMode restricted_mode) {
    switch (restricted_mode) {
      case RestrictedMode::kModeA:
        eval_prev_mv_search_result = 1;
        fast_intra_mode_eval_level = 1;
        fast_inter_pred_bits = 1;
        fast_merge_eval = 0;
        bipred_refinement_iterations = 1;
        always_evaluate_intra_in_inter = 0;
        smooth_lambda_scaling = 0;
        default_num_ref_pics = 2;
        max_binary_split_depth = 0;
        adaptive_qp = 0;
        structural_ssd = 0;
        break;
      case RestrictedMode::kModeB:
        eval_prev_mv_search_result = 0;
        fast_intra_mode_eval_level = 2;
        fast_inter_pred_bits = 1;
        fast_merge_eval = 1;
        bipred_refinement_iterations = 1;
        always_evaluate_intra_in_inter = 0;
        smooth_lambda_scaling = 0;
        default_num_ref_pics = 2;
        max_binary_split_depth = 2;
        adaptive_qp = 0;
        structural_ssd = 0;
        break;
      default:
        assert(0);
        break;
    }
  }

  void Tune(TuneMode tune_mode) {
    switch (tune_mode) {
      case TuneMode::kDefault:
        // No settings are changed in default mode.
        break;
      case TuneMode::kPsnr:
        adaptive_qp = 0;
        structural_ssd = 0;
        break;
      default:
        assert(0);
        break;
    }
  }

  // Encoder rdo behavior
  static constexpr bool kEncoderStrictRdoBitCounting = false;
  static constexpr bool kEncoderCountActualWrittenBits = true;
  static_assert(EncoderSettings::kEncoderCountActualWrittenBits ||
                EncoderSettings::kEncoderStrictRdoBitCounting,
                "Fast bit counting should use strict rdo bit signaling");

  // Fast encoder decisions (always used)
  static const bool fast_quad_split_based_on_binary_split = true;
  static const bool fast_cu_split_based_on_full_cu = true;
  static const bool fast_mode_selection_for_cached_cu = true;
  static const bool skip_mode_decision_for_identical_cu = false;
  static const bool fast_inter_cbf_dist = true;  // not really any impact
  static const bool fast_inter_root_cbf_zero_bits = true;  // very small loss

  // Speed mode dependent settings
  int fast_intra_mode_eval_level = -1;
  int fast_merge_eval = -1;
  int bipred_refinement_iterations = -1;
  int always_evaluate_intra_in_inter = -1;
  int default_num_ref_pics = -1;
  int max_binary_split_depth = -1;

  // Setting with default values used in all speed modes
  int eval_prev_mv_search_result = 1;
  int fast_inter_pred_bits = 0;
  int smooth_lambda_scaling = 1;
  int adaptive_qp = 1;
  double aqp_strength = 1.0;
  int structural_ssd = 1;
};

}   // namespace xvc

#endif  // XVC_ENC_LIB_ENCODER_SETTINGS_H_