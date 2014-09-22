/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/satellite-module.h"

using namespace ns3;

/**
 * \file sat-loo-example.cc
 * \ingroup satellite
 *
 * \brief Example for Loo's model fader class. Demonstrates the functionality
 * of the Loo's model fader used by the Markov-fading. The example creates
 * three faders with different parameters, switches the faders in time and
 * outputs the fading values.
 */

NS_LOG_COMPONENT_DEFINE ("sat-loo-example");

int
main (int argc, char *argv[])
{
  /// Enable info logs
  LogComponentEnable ("sat-loo-example", LOG_LEVEL_INFO);
  LogComponentEnable ("SatLooModel", LOG_LEVEL_INFO);

  /// Load default configuration
  Ptr<SatLooConf> looConf = CreateObject<SatLooConf> ();

  /// Create faders
  Ptr<SatLooModel> looFaderLineOfSight = CreateObject<SatLooModel> (looConf,3,0,0);
  Ptr<SatLooModel> looFaderLightShadowing = CreateObject<SatLooModel> (looConf,3,0,1);
  Ptr<SatLooModel> looFaderHeavyShadowing = CreateObject<SatLooModel> (looConf,3,0,2);

  /// Run simulation
  for (uint32_t i = 0; i < 1000; i++)
    {
      Simulator::Schedule (Time (300000 + i*500000), &SatLooModel::GetChannelGainDb, looFaderLineOfSight);
      Simulator::Schedule (Time (500000 + i*500000), &SatLooModel::GetChannelGainDb, looFaderLightShadowing);
      Simulator::Schedule (Time (700000 + i*500000), &SatLooModel::GetChannelGainDb, looFaderHeavyShadowing);
    }

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
