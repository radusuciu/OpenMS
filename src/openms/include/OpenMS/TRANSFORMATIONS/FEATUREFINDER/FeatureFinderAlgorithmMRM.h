// Copyright (c) 2002-present, The OpenMS Team -- EKU Tuebingen, ETH Zurich, and FU Berlin
// SPDX-License-Identifier: BSD-3-Clause
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/FeatureFinderAlgorithm.h>

namespace OpenMS
{
  class InterpolationModel;

  /**
    @brief FeatureFinderAlgorithm for MRM experiments.

    @experimental This tool has not been tested thoroughly and might behave not as expected!

    This very simple algorithm has mainly been tested on data generated
    on an ABI/SCIEX QTrap 4000. Maybe this algorithm cannot be directly
    applied on data generated by other mass spectrometers.

    Precursor m/z information (Q1) is stored in the "MZ" metavalue of each feature!
    The feature's m/z itself is the Q3's ion m/z.

    @htmlinclude OpenMS_FeatureFinderAlgorithmMRM.parameters

    @ingroup FeatureFinder
  */
  class OPENMS_DLLAPI FeatureFinderAlgorithmMRM :
    public FeatureFinderAlgorithm,
    public FeatureFinderDefs
  {
public:
    ///@name Type definitions
    //@{
    typedef FeatureFinderAlgorithm::MapType MapType;
    typedef MapType::SpectrumType SpectrumType;
    typedef SpectrumType::FloatDataArrays FloatDataArrays;
    //@}

    using FeatureFinderAlgorithm::param_;
    using FeatureFinderAlgorithm::features_;
    using FeatureFinderAlgorithm::ff_;
    using FeatureFinderAlgorithm::defaults_;
    using FeatureFinderAlgorithm::map_;

public:

    enum
    {
      RT = Peak2D::RT,
      MZ = Peak2D::MZ
    };

    /// default constructor
    FeatureFinderAlgorithmMRM();

    /// Main method for actual FeatureFinder
    void run() override;

    static FeatureFinderAlgorithm* create();

    static const String getProductName();

protected:

    double fitRT_(std::vector<Peak1D>& rt_input_data, std::unique_ptr<InterpolationModel>& model) const;

    //Docu in base class
    void updateMembers_() override;
  };

} // namespace OpenMS

