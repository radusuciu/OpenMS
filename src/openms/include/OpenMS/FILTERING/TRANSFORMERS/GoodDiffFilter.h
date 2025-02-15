// Copyright (c) 2002-present, The OpenMS Team -- EKU Tuebingen, ETH Zurich, and FU Berlin
// SPDX-License-Identifier: BSD-3-Clause
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: $
// --------------------------------------------------------------------------
//

#pragma once

#include <OpenMS/FILTERING/TRANSFORMERS/FilterFunctor.h>

#include <map>
#include <string>
#include <cmath>

namespace OpenMS
{
  /**
    @brief GoodDiffFilter counts the number ob peak pairs whose m/z difference can be explained by a amino acid loss

        @htmlinclude OpenMS_GoodDiffFilter.parameters

        @ingroup SpectraFilter
  */
  class OPENMS_DLLAPI GoodDiffFilter :
    public FilterFunctor
  {
public:

    // @name Constructors and Destructors
    // @{
    /// default constructor
    GoodDiffFilter();

    /// copy constructor
    GoodDiffFilter(const GoodDiffFilter & source);

    /// destructor
    ~GoodDiffFilter() override;
    // @}

    // @name Operators
    // @{
    /// assignment operator
    GoodDiffFilter & operator=(const GoodDiffFilter & source);
    // @}

    // @name Accessors
    // @{
    ///
    static FilterFunctor * create() { return new GoodDiffFilter(); }

    ///
    template <typename SpectrumType>
    double apply(SpectrumType & spectrum)
    {
      double tolerance = (double)param_.getValue("tolerance");
      double gooddiff = 0;
      //iterate over all peaks
      double totaldiff = 0;
      for (Size i = 0; i < spectrum.size(); ++i)
      {
        //look for each peakdifference that is in range of aa residuemasses (56/187), if it could be a aa (aamass)
        for (Size j = i; i + j < spectrum.size(); ++j)
        {
          double diff =  spectrum[i + j].getPosition()[0] - spectrum[i].getPosition()[0];
          if (diff < 56)
          {
            continue;
          }

          if (diff > 187)
          {
            j = spectrum.size();
          }
          else
          {
            totaldiff += spectrum[i + j].getIntensity() + spectrum[i].getIntensity();
            std::map<double, char>::const_iterator aait = aamass_.lower_bound(diff);
            if (aait == aamass_.end())
            {
              continue;
            }
            //look for aamasses that fit diff
            if (fabs(aait->first - diff) <= tolerance)
            {
              gooddiff += spectrum[i + j].getIntensity()  + spectrum[i].getIntensity();
            }
            else
            {
              ++aait;
              if ((aait) != aamass_.end() && fabs((aait)->first - diff) <= tolerance)
              {
                gooddiff += spectrum[i + j].getIntensity() + spectrum[i].getIntensity();
              }
            }
          }
        }
      }

      return gooddiff / totaldiff;
    }

    ///
    static const String getProductName()
    {
      return "GoodDiffFilter";
    }

    // @}


private:

    /// list of unique amino acid masses
    std::map<double, char> aamass_;
  };
}

