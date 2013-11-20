/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Magister Solutions Ltd.
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
#include "satellite-interference-output-trace-container.h"

NS_LOG_COMPONENT_DEFINE ("SatInterferenceOutputTraceContainer");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (SatInterferenceOutputTraceContainer);

TypeId 
SatInterferenceOutputTraceContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SatInterferenceOutputTraceContainer")
    .SetParent<SatBaseTraceContainer> ();
  return tid;
}

SatInterferenceOutputTraceContainer::SatInterferenceOutputTraceContainer () :
  m_index (0)
{
  NS_LOG_FUNCTION (this);
}

SatInterferenceOutputTraceContainer::~SatInterferenceOutputTraceContainer ()
{
  NS_LOG_FUNCTION (this);

  Reset ();
}

void
SatInterferenceOutputTraceContainer::DoDispose ()
{
  NS_LOG_FUNCTION (this);

  Reset ();

  SatBaseTraceContainer::DoDispose();
}

void
SatInterferenceOutputTraceContainer::Reset ()
{
  if ( !m_container.empty() )
    {
      m_container.clear();
    }
  m_index = 0;
}

void
SatInterferenceOutputTraceContainer::AddNode (key_t key)
{
  NS_LOG_FUNCTION (this);

  std::stringstream filename;

  /// TODO: add simulator root folder to the path!

  filename << "data/interference_trace/input/nodeId_" << m_index << "_channelType_" + key.second;

  std::pair <container_t::iterator, bool> result = m_container.insert (std::make_pair(key, CreateObject<SatOutputFileStreamDoubleContainer> (filename.str(), std::ios::in, DEFAULT_NUMBER_OF_COLUMNS)));

  if (result.second == false)
    {
      NS_FATAL_ERROR ("SatInterferenceInputTraceContainer::AddNode failed");
    }

  m_index++;
}

Ptr<SatOutputFileStreamDoubleContainer>
SatInterferenceOutputTraceContainer::FindNode (key_t key)
{
  NS_LOG_FUNCTION (this);

  return m_container.at (key);
}

void
SatInterferenceOutputTraceContainer::WriteToFile (key_t key)
{
  FindNode (key)->WriteContainerToFile ();
}

void
SatInterferenceOutputTraceContainer::AddToContainer (key_t key, std::vector<double> newItem)
{
  FindNode (key)->AddToContainer (newItem);
}

} // namespace ns3