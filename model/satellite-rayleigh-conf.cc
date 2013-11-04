/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd
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
 * Author: Frans Laakso <frans.laakso@magister.fi>
 */

#include "satellite-rayleigh-conf.h"
#include "satellite-markov-conf.h"
#include <map>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatRayleighConf);
NS_LOG_COMPONENT_DEFINE ("SatRayleighConf");

static const double g_RayleighParameters[SatMarkovConf::DEFAULT_ELEVATION_COUNT][SatMarkovConf::DEFAULT_STATE_COUNT][SatRayleighConf::DEFAULT_RAYLEIGH_PARAMETER_COUNT] =
    {
     /* Elevation 40 */  // TODO: add Ka-band parameters
     {{10,10},
      {10,10},
      {10,10}},

     /* Elevation 60 */
     {{10,10},
      {10,10},
      {10,10}},

     /* Elevation 70 */
     {{10,10},
      {10,10},
      {10,10}},

     /* Elevation 80 */
     {{10,10},
      {10,10},
      {10,10}}
    };

TypeId
SatRayleighConf::GetTypeId (void) //TODO: add attribute m_rayleighParameters
{
  static TypeId tid = TypeId ("ns3::SatRayleighConf")
      .SetParent<SatFaderConf> ()
      .AddConstructor<SatRayleighConf> ()
      .AddAttribute ("ElevationCount", "Number of elevation sets in the Markov model.",
                     UintegerValue (SatMarkovConf::DEFAULT_ELEVATION_COUNT),
                     MakeUintegerAccessor (&SatRayleighConf::m_elevationCount),
                     MakeUintegerChecker<uint32_t> ())
      .AddAttribute ("StateCount", "Number of states in the Markov model.",
                     UintegerValue (SatMarkovConf::DEFAULT_STATE_COUNT),
                     MakeUintegerAccessor (&SatRayleighConf::m_stateCount),
                     MakeUintegerChecker<uint32_t> ());
  return tid;
}

SatRayleighConf::SatRayleighConf () :
    m_elevationCount (SatMarkovConf::DEFAULT_ELEVATION_COUNT),
    m_stateCount (SatMarkovConf::DEFAULT_STATE_COUNT)
{
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO ("Time " << Now ().GetSeconds () << " SatRayleighConf - Creating SatRayleighConf...");

  for (uint32_t i = 0; i < m_elevationCount; i++)
    {
      std::vector<std::vector<double> > states;

      for (uint32_t j = 0; j < m_stateCount; j++)
        {
          std::vector<double> parameters;

          for (uint32_t k = 0; k < SatRayleighConf::DEFAULT_RAYLEIGH_PARAMETER_COUNT; k++)
            {
              parameters.push_back (g_RayleighParameters[i][j][k]);
            }
          states.push_back (parameters);
        }
      m_rayleighParameters.push_back (states);
    }
}

std::vector<std::vector<double> >
SatRayleighConf::GetParameters (uint32_t set)
{
  NS_LOG_FUNCTION (this << set);

  NS_ASSERT ( (set >= 0) && (set < m_elevationCount));
  NS_LOG_INFO ("Time " << Now ().GetSeconds () << " SatRayleighConf - Getting Rayleigh parameters for set ID " << set);
  return m_rayleighParameters[set];
}

} // namespace ns3