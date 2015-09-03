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

#include "tools/ping/client/ping.hpp"
#include "tools/ping/client/statistics-collector.hpp"
#include "tools/ping/client/tracer.hpp"

#include <ndn-cxx/face.hpp>

namespace ndn {
namespace ping {

namespace pingClient = ::ndn::ping::client;

class PingClient
{
public:
  PingClient()
  {
    m_options.shouldAllowStaleData = false;
    m_options.interval = time::milliseconds(1000);
    m_options.timeout = time::milliseconds(4000);
    m_options.startSeq = 0;
    m_options.shouldGenerateRandomSeq = false;
    m_options.shouldPrintTimestamp = false;

    m_pingClient = std::make_shared<pingClient::Ping>(m_face, m_options);
    m_statisticsCollector = std::make_shared<pingClient::StatisticsCollector>(*m_pingClient, m_options);
    m_tracer = std::make_shared<pingClient::Tracer>(*m_pingClient, m_options);

    m_pingClient->afterFinish.connect([this] {
        m_statisticsCollector->computeStatistics().printSummary(std::cout);
      });
  }

  void
  setPrefix(Name name)
  {
    m_options.prefix = name;
  }

  void
  setNPings(int nPings)
  {
    m_options.nPings = nPings;
  }

  void
  run()
  {
    m_pingClient->start();
  }

private:
  Face m_face;
  client::Options m_options;
  shared_ptr<client::Ping> m_pingClient;
  shared_ptr<client::StatisticsCollector> m_statisticsCollector;
  shared_ptr<client::Tracer> m_tracer;
};

} // namespace ping
} // namespace ndn
