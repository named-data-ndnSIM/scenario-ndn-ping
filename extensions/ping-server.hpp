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

#include "tools/ping/server/ping-server.hpp"
#include "tools/ping/server/tracer.hpp"

#include <ndn-cxx/face.hpp>

namespace ndn {
namespace ping {

namespace pingServer = ::ndn::ping::server;

class PingServer
{
public:
  PingServer()
  {
    m_options.freshnessPeriod = time::milliseconds(1000);
    m_options.shouldLimitSatisfied = false;
    m_options.shouldPrintTimestamp = false;
    m_options.payloadSize = 1000;

    m_pingServer = std::make_shared<pingServer::PingServer>(m_face, ns3::ndn::StackHelper::getKeyChain(),
                                                        m_options);
    m_tracer = std::make_shared<pingServer::Tracer>(*m_pingServer, m_options);

    m_pingServer->afterReceive.connect([this] (const Name& name) {
        std::cout << "\n--- ping server " << m_options.prefix << " ---" << std::endl;
        std::cout << "--- ping received! --- (" << name << ")" << std::endl;
        std::cout << m_pingServer->getNPings() << " packets processed" << std::endl;
      });
  }

  ~PingServer()
  {
  }

  void
  setPrefix(Name name)
  {
    m_options.prefix = name;
  }

  void
  setNMaxPings(int nPings)
  {
    m_options.nMaxPings = nPings;
  }

  void
  run()
  {
    m_pingServer->start();
  }

private:
  Face m_face;
  server::Options m_options;
  shared_ptr<server::PingServer> m_pingServer;
  shared_ptr<server::Tracer> m_tracer;
};

} // namespace ping
} // namespace ndn
