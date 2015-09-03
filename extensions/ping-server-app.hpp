/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 University of California, Los Angeles
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
 * Authors: Spyridon (Spyros) Mastorakis <mastorakis@cs.ucla.edu>
 *          Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/ndnSIM-module.h"
#include "ns3/integer.h"
#include "ns3/string.h"

#include "ping-server.hpp"

namespace ns3 {
namespace ndn {

namespace ping = ::ndn::ping;

class PingServerApp : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("PingServerApp")
      .SetParent<Application>()
      .AddConstructor<PingServerApp>()
      .AddAttribute("Prefix", "Prefix for ping interests", StringValue("/"),
                    MakeNameAccessor(&PingServerApp::m_prefix), MakeNameChecker())
      .AddAttribute("nMaxPings", "Maximum number of ping interests to be satisfied", IntegerValue(1),
                    MakeIntegerAccessor(&PingServerApp::m_nMaxPings), MakeIntegerChecker<int32_t>());

    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    m_instance.reset(new ping::PingServer);
    m_instance->setPrefix(m_prefix);
    m_instance->setNMaxPings(m_nMaxPings);
    m_instance->run();
  }

  virtual void
  StopApplication()
  {
    m_instance.reset();
  }

private:
  std::unique_ptr<ping::PingServer> m_instance;
  std::uint32_t m_nMaxPings;
  Name m_prefix;
};

} // namespace ndn
} // namespace ns3
