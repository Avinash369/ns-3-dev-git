/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
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
 * Author: Avinash <avinash.kumar369@gmail.com>
 *         Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/tcp-congestion-ops.h"
#include "ns3/tcp-recovery-ops.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/tcp-rate-halving-recovery.h"
#include "ns3/string.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpRateHalvingRecoveryTestSuite");

/**
 * \brief RateHalving Recovery algorithm test
 */
class RateHalvingRecoveryTest : public TestCase
{
public:
  /**
   * \brief Constructor.
   * \param cWnd Congestion window.
   * \param segmentSize Segment size.
   * \param ssThresh Slow Start Threshold.
   * \param unAckDataCount Unacknowledged data at the start of recovery.
   * \param bytesInFlight Current bytes in flight.
   * \para m_lastSackedBytes Bytes SACKed on last acknowledgment.
   * \para bytesSent Bytes sent while in recovery phase.
   * \param name Test description.
   */
  RateHalvingRecoveryTest (uint32_t cWnd,
                           uint32_t segmentSize,
                           uint32_t ssThresh,
                           uint32_t unAckDataCount,
                           uint32_t bytesInFlight,
                           uint32_t m_lastSackedBytes,
                           uint32_t bytesSent,
                           const std::string &name);

private:
  virtual void DoRun (void);

  uint32_t m_cWnd;                    //!< Congestion window.
  uint32_t m_segmentSize;             //!< Segment size.
  uint32_t m_ssThresh;                //!< Slow Start Threshold.
  uint32_t m_unAckDataCount;          //!< Unacknowledged data at the start of recovery.
  uint32_t m_bytesInFlight;           //!< Current bytes in flight.
  uint32_t m_lastSackedBytes;         //!< Bytes SACKed on last acknowledgment.
  uint32_t m_bytesSent;               //!< Bytes sent while in recovery phase.

  Ptr<TcpSocketState> m_state;  //!< TCP socket state.
};

RateHalvingRecoveryTest::RateHalvingRecoveryTest (uint32_t cWnd,
                                                  uint32_t segmentSize,
                                                  uint32_t ssThresh,
                                                  uint32_t unAckDataCount,
                                                  uint32_t bytesInFlight,
                                                  uint32_t lastSackedBytes,
                                                  uint32_t bytesSent,
                                                  const std::string &name)
  : TestCase (name),
  m_cWnd (cWnd),
  m_segmentSize (segmentSize),
  m_ssThresh (ssThresh),
  m_unAckDataCount (unAckDataCount),
  m_bytesInFlight (bytesInFlight),
  m_lastSackedBytes (lastSackedBytes),
  m_bytesSent (bytesSent)
{
}

void
RateHalvingRecoveryTest::DoRun ()
{
  m_state = CreateObject<TcpSocketState> ();

  m_state->m_cWnd = m_cWnd;
  m_state->m_cWndInfl = m_cWnd;
  m_state->m_segmentSize = m_segmentSize;
  m_state->m_ssThresh = m_ssThresh;
  m_state->m_bytesInFlight = m_bytesInFlight;

  Ptr<TcpRateHalvingRecovery> recovery = CreateObject <TcpRateHalvingRecovery> ();

  recovery->EnterRecovery (m_state, 3, m_unAckDataCount, 0);

  NS_TEST_ASSERT_MSG_LT_OR_EQ (m_state->m_cWnd.Get (), m_cWnd + m_segmentSize,
                               "There should be at least one transmission on entering recovery");

  for (uint32_t iterator = 0; iterator < m_bytesSent; )
    {
      recovery->UpdateBytesSent (m_segmentSize);
      iterator += m_segmentSize;
    }

  m_bytesInFlight += m_state->m_cWnd.Get () - m_cWnd;
  m_state->m_bytesInFlight = m_bytesInFlight;
  m_cWnd = m_state->m_cWnd.Get ();
  recovery->DoRecovery (m_state, 0, m_lastSackedBytes);

  if (m_bytesInFlight > m_state->m_ssThresh)
    {
      NS_TEST_ASSERT_MSG_GT_OR_EQ (m_state->m_cWnd.Get (), m_cWnd,
                                   "Updated cwnd should be greater than or equal to the existing cwnd");
    }
  else
    {
      NS_TEST_ASSERT_MSG_LT_OR_EQ (m_state->m_cWnd.Get (), m_cWnd,
                                   "Updated cwnd should be less than or equal to the existing cwnd");
    }
}

/**
 * \ingroup internet-test
 * \ingroup tests
 *
 * \brief Rate Halving Recovery TestSuite
 */
class RateHalvingRecoveryTestSuite : public TestSuite
{
public:
  RateHalvingRecoveryTestSuite () : TestSuite ("tcp-rate-halving-recovery-test", UNIT)
  {
    AddTestCase (new RateHalvingRecoveryTest (3000, 500, 2500, 3000, 3000, 500, 1000,
                                              "RateHalving test on cWnd when bytesInFlight is greater than ssThresh"),
                 TestCase::QUICK);
    AddTestCase (new RateHalvingRecoveryTest (1000, 500, 2500, 3000, 1000, 500, 1000,
                                              "RateHalving test on cWnd when bytesInFlight is lower than ssThresh"),
                 TestCase::QUICK);
  }
};

static RateHalvingRecoveryTestSuite g_TcpRateHalvingRecoveryTest; //!< Static variable for test initialization
