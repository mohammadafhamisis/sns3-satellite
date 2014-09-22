/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/satellite-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-module.h"
#include "ns3/config-store-module.h"


using namespace ns3;

/**
 * \file sat-ra-sim-tn9-comparison.cc
 * \ingroup satellite
 *
 * \brief Simulation script to run example simulation results related to satellite RTN
 * link performance. Currently only one beam is simulated with variable amount of users
 * and RA-DAMA configuration. This example is a CRDSA decoder test for providing comparison
 * results with the following parameters:
 * - 1 unique payload per frame per UT
 * - 1 beam simulated (Beam ID 18)
 * - 0% backoff probability
 * - DAMA and CRA disabled
 * - Fading disabled
 * - CBR with 1 burst per frame
 * - 30, 60, 90, 120, 150, 180, 210, 240 UTs
 * - 2 and 3 replicas
 * - infinite max num of consecutive blocks accessed
 * - dynamic load control off
 *
 * As output, the example provides statistics about RA collision and error rate,
 * throughput, packet delay, SINR, resources granted, frame load and waveform usage.
 * The random access CRDSA decoder comparison results for TN9 were obtained by using this script.
 *
 * execute command -> ./waf --run "sat-ra-sim-tn9-comparison --PrintHelp"
 */

NS_LOG_COMPONENT_DEFINE ("sat-ra-sim-tn9-comparison");

int
main (int argc, char *argv[])
{
  uint32_t beamId (18);
  uint32_t endUsersPerUt (1);
  uint32_t utsPerBeam (1);
  uint32_t packetSize (1);
  uint32_t numOfInstances (3);
  double intervalInSeconds (0.1);
  bool isNoisy (false);

  double simLength (60.0); // in seconds
  Time appStartTime = Seconds (0.1);

  // To read attributes from file
  std::string inputFileNameWithPath = Singleton<SatEnvVariables>::Get ()->LocateDirectory ("src/satellite/examples") + "/tn9-ra-input-attributes.xml";
  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue (inputFileNameWithPath));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Load"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));
  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // read command line parameters given by user
  CommandLine cmd;
  cmd.AddValue ("utsPerBeam", "Number of UTs per spot-beam", utsPerBeam);
  cmd.AddValue ("numOfInstances", "Number of CRDSA packet instances", numOfInstances);
  cmd.AddValue ("isNoisy", "If true, may print some logging messages", isNoisy);
  cmd.Parse (argc, argv);

  if (isNoisy)
    {
      LogComponentEnable ("sat-ra-sim-tn9", LOG_INFO);
    }

  // Enable Random Access with all available modules
  Config::SetDefault ("ns3::SatBeamHelper::RandomAccessModel",EnumValue (SatEnums::RA_MODEL_RCS2_SPECIFICATION));

  // Set Random Access interference model
  Config::SetDefault ("ns3::SatBeamHelper::RaInterferenceModel",EnumValue (SatPhyRxCarrierConf::IF_PER_PACKET));

  // Set Random Access collision model
  Config::SetDefault ("ns3::SatBeamHelper::RaCollisionModel",EnumValue (SatPhyRxCarrierConf::RA_COLLISION_CHECK_AGAINST_SINR));

  // Disable fading
  Config::SetDefault ("ns3::SatBeamHelper::FadingModel",EnumValue (SatEnums::FADING_OFF));

  // Disable C/NO reporting
  Config::SetDefault ("ns3::SatRequestManager::CnoReportInterval",TimeValue (Seconds (60.0)));

  // Disable periodic control slots
  Config::SetDefault ("ns3::SatBeamScheduler::ControlSlotsEnabled", BooleanValue (false));

  // Set dynamic load control parameters
  Config::SetDefault ("ns3::SatPhyRxCarrierConf::EnableRandomAccessDynamicLoadControl", BooleanValue (false));
  Config::SetDefault ("ns3::SatPhyRxCarrierConf::RandomAccessAverageNormalizedOfferedLoadMeasurementWindowSize", UintegerValue (10));

  // Set random access parameters
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_MaximumUniquePayloadPerBlock", UintegerValue (1));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_MaximumConsecutiveBlockAccessed", UintegerValue (6));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_MinimumIdleBlock", UintegerValue (0));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_BackOffTimeInMilliSeconds", UintegerValue (50));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_BackOffProbability", UintegerValue (1));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_HighLoadBackOffProbability", UintegerValue (1));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_AverageNormalizedOfferedLoadThreshold", DoubleValue (0.99));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::RaService0_NumberOfInstances", UintegerValue (numOfInstances));

  // Disable CRA and DAMA
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService0_ConstantAssignmentProvided", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService1_ConstantAssignmentProvided", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService2_ConstantAssignmentProvided", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService3_ConstantAssignmentProvided", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService0_RbdcAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService1_RbdcAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService2_RbdcAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService3_RbdcAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService0_VolumeAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService1_VolumeAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService2_VolumeAllowed", BooleanValue (false));
  Config::SetDefault ("ns3::SatLowerLayerServiceConf::DaService3_VolumeAllowed", BooleanValue (false));

  Config::SetDefault ("ns3::SatBeamHelper::CtrlMsgStoreTimeInRtnLink", TimeValue (MilliSeconds (600)));

  // Creating the reference system. Note, currently the satellite module supports
  // only one reference system, which is named as "Scenario72". The string is utilized
  // in mapping the scenario to the needed reference system configuration files. Arbitrary
  // scenario name results in fatal error.
  std::string scenarioName = "Scenario72";

  Ptr<SatHelper> helper = CreateObject<SatHelper> (scenarioName);

  // create user defined scenario
  SatBeamUserInfo beamInfo = SatBeamUserInfo (utsPerBeam, endUsersPerUt);
  std::map<uint32_t, SatBeamUserInfo > beamMap;
  beamMap[beamId] = beamInfo;
  helper->CreateUserDefinedScenario (beamMap);

  // get users
  NodeContainer utUsers = helper->GetUtUsers();
  NodeContainer gwUsers = helper->GetGwUsers();

  // port used for packet delivering
  uint16_t port = 9; // Discard port (RFC 863)
  const std::string protocol = "ns3::UdpSocketFactory";

  /**
   * Set-up CBR traffic
   */
  const SatIdMapper *satIdMapper = Singleton<SatIdMapper>::Get ();
  const InetSocketAddress gwAddr = InetSocketAddress (helper->GetUserAddress (gwUsers.Get (0)), port);

  for (NodeContainer::Iterator itUt = utUsers.Begin ();
      itUt != utUsers.End ();
      ++itUt)
    {
      appStartTime += MilliSeconds (10);

      // return link
      Ptr<CbrApplication> rtnApp = CreateObject<CbrApplication> ();
      rtnApp->SetAttribute ("Protocol", StringValue (protocol));
      rtnApp->SetAttribute ("Remote", AddressValue (gwAddr));
      rtnApp->SetAttribute ("PacketSize", UintegerValue (packetSize));

      if (isNoisy)
        {
          const Address addr = satIdMapper->GetUtUserMacWithNode (*itUt);
          const int32_t utUserId = satIdMapper->GetUtUserIdWithMac (addr);
          const double kbps = packetSize / intervalInSeconds / 125.0;
          std::cout << "UT User " << utUserId
                    << " offers bandwidth of " << kbps << " kbps" << std::endl;
        }
      rtnApp->SetAttribute ("Interval", TimeValue (Seconds (intervalInSeconds)));
      rtnApp->SetStartTime (appStartTime);
      (*itUt)->AddApplication (rtnApp);
    }

  // setup packet sinks at all users
  Ptr<PacketSink> ps = CreateObject<PacketSink> ();
  ps->SetAttribute ("Protocol", StringValue (protocol));
  ps->SetAttribute ("Local", AddressValue (gwAddr));
  gwUsers.Get (0)->AddApplication (ps);

  /**
   * Set-up statistics
   */
  Ptr<SatStatsHelperContainer> s = CreateObject<SatStatsHelperContainer> (helper);

  s->AddPerBeamRtnAppThroughput (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnDevThroughput (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnMacThroughput (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnPhyThroughput (SatStatsHelper::OUTPUT_SCALAR_FILE);

  s->AddPerBeamRtnAppDelay (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnDevDelay (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnPhyDelay (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamRtnMacDelay (SatStatsHelper::OUTPUT_SCALAR_FILE);

  s->AddPerBeamRtnAppDelay (SatStatsHelper::OUTPUT_CDF_FILE);
  s->AddPerBeamRtnDevDelay (SatStatsHelper::OUTPUT_CDF_FILE);
  s->AddPerBeamRtnPhyDelay (SatStatsHelper::OUTPUT_CDF_FILE);
  s->AddPerBeamRtnMacDelay (SatStatsHelper::OUTPUT_CDF_FILE);

  //s->AddPerUtUserRtnAppThroughput (SatStatsHelper::OUTPUT_SCALAR_FILE);
  //s->AddPerUtUserRtnAppThroughput (SatStatsHelper::OUTPUT_SCATTER_FILE);
  //s->AddPerUtUserRtnAppThroughput (SatStatsHelper::OUTPUT_SCATTER_PLOT);
  //s->AddPerUtRtnDevThroughput (SatStatsHelper::OUTPUT_SCATTER_FILE);
  //s->AddPerUtRtnDevThroughput (SatStatsHelper::OUTPUT_SCATTER_PLOT);

  //s->AddPerUtUserRtnAppDelay (SatStatsHelper::OUTPUT_CDF_FILE);
  //s->AddPerUtUserRtnAppDelay (SatStatsHelper::OUTPUT_CDF_PLOT);
  //s->AddPerUtRtnDevDelay (SatStatsHelper::OUTPUT_CDF_FILE);
  //s->AddPerUtRtnDevDelay (SatStatsHelper::OUTPUT_CDF_PLOT);

  s->AddPerBeamRtnCompositeSinr (SatStatsHelper::OUTPUT_CDF_FILE);
  s->AddPerBeamRtnCompositeSinr (SatStatsHelper::OUTPUT_CDF_PLOT);

  s->AddPerBeamResourcesGranted (SatStatsHelper::OUTPUT_CDF_FILE);
  s->AddPerBeamResourcesGranted (SatStatsHelper::OUTPUT_CDF_PLOT);

  s->AddPerBeamFrameSymbolLoad (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamWaveformUsage (SatStatsHelper::OUTPUT_SCALAR_FILE);

  s->AddPerBeamRtnDaPacketError (SatStatsHelper::OUTPUT_SCALAR_FILE);

  s->AddPerBeamCrdsaPacketCollision (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamCrdsaPacketError (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamSlottedAlohaPacketCollision (SatStatsHelper::OUTPUT_SCALAR_FILE);
  s->AddPerBeamSlottedAlohaPacketError (SatStatsHelper::OUTPUT_SCALAR_FILE);

  //s->AddPerUtCrdsaPacketCollision (SatStatsHelper::OUTPUT_SCALAR_FILE);
  //s->AddPerUtCrdsaPacketError (SatStatsHelper::OUTPUT_SCALAR_FILE);
  //s->AddPerUtSlottedAlohaPacketCollision (SatStatsHelper::OUTPUT_SCALAR_FILE);
  //s->AddPerUtSlottedAlohaPacketError (SatStatsHelper::OUTPUT_SCALAR_FILE);

  NS_LOG_INFO("--- sat-ra-sim-tn9-comparison ---");
  NS_LOG_INFO("  Packet size: " << packetSize);
  NS_LOG_INFO("  Simulation length: " << simLength);
  NS_LOG_INFO("  Number of UTs: " << utsPerBeam);
  NS_LOG_INFO("  Number of end users per UT: " << endUsersPerUt);
  NS_LOG_INFO("  ");

  /**
   * Store attributes into XML output
   */
  std::stringstream filename;
  filename << "tn9-ra-comparison-output-attributes-uts-" << utsPerBeam << "-instances-" << numOfInstances << ".xml";

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue (filename.str ()));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  ConfigStore outputConfig;
  outputConfig.ConfigureDefaults ();
  outputConfig.ConfigureAttributes ();

  /**
   * Run simulation
   */
  Simulator::Stop (Seconds (simLength));
  Simulator::Run ();

  Simulator::Destroy ();

  return 0;
}
