/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Magister Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Jani Puttonen <jani.puttonen@magister.fi>
 */


#ifndef SATELLITE_CHANNEL_ESTIMATION_ERROR_H_
#define SATELLITE_CHANNEL_ESTIMATION_ERROR_H_

#include "ns3/object.h"
#include "ns3/random-variable-stream.h"

namespace ns3 {

/**
 * \ingroup satellite
 * Sat
 *
 */
class SatChannelEstimationError : public Object
{
public:
  SatChannelEstimationError ();

  /**
   * Constructor
   * \param filePathName A file containing the gaussian
   * gaussian distribution mean and std.
   */
  SatChannelEstimationError (std::string filePathName);
  virtual ~SatChannelEstimationError ();

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * Add channel estimation error to SINR
   * \param sinrInDb Measured SINR in dB
   * @return double SINR including channel estimation error in dB
   */
  double AddError (double sinrInDb) const;

private:

  /**
   * Read the distribution mean and std values from file
   * @param filePathName File name
   */
  void ReadFile (std::string filePathName);

  /**
   * Last sample index of the containers
   */
  uint32_t m_lastSampleIndex;

  /**
   * Normal random variable used to calculate the
   * channel estimation error.
   */
  Ptr<NormalRandomVariable> m_normalRandomVariable;

  /**
   * SINR values
   */
  std::vector<double> m_sinrsDb;

  /**
   * Mean values
   */
  std::vector<double> m_mueCesDb;

  /**
   * Standard deviation values
   */
  std::vector<double> m_stdCesDb;

};

}



#endif /* SATELLITE_CHANNEL_ESTIMATION_ERROR_H_ */