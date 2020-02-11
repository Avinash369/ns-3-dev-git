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

#include "tcp-rate-halving-recovery.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/tcp-congestion-ops.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRateHalvingRecovery");
NS_OBJECT_ENSURE_REGISTERED (TcpRateHalvingRecovery);

TypeId
TcpRateHalvingRecovery::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRateHalvingRecovery")
    .SetParent<TcpClassicRecovery> ()
    .AddConstructor<TcpRateHalvingRecovery> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

TcpRateHalvingRecovery::TcpRateHalvingRecovery (void)
  : TcpClassicRecovery ()
{
  NS_LOG_FUNCTION (this);
}

TcpRateHalvingRecovery::TcpRateHalvingRecovery (const TcpRateHalvingRecovery& recovery)
  : TcpClassicRecovery (recovery),
  m_isAlternateAck (recovery.m_isAlternateAck)
{
  NS_LOG_FUNCTION (this);
}

TcpRateHalvingRecovery::~TcpRateHalvingRecovery (void)
{
  NS_LOG_FUNCTION (this);
}

void
TcpRateHalvingRecovery::EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount,
                               uint32_t unAckDataCount, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << dupAckCount << unAckDataCount);
  NS_UNUSED (dupAckCount);
  NS_UNUSED (unAckDataCount);

  if(tcb->m_bytesInFlight < tcb->m_ssThresh){
     tcb->m_cWnd = tcb->m_ssThresh;
     tcb->m_cWndInfl = tcb->m_ssThresh + (dupAckCount * tcb->m_segmentSize);
  }

  else
     DoRecovery (tcb, deliveredBytes);

  m_isAlternateAck = false;
    
}

void
TcpRateHalvingRecovery::DoRecovery (Ptr<TcpSocketState> tcb, uint32_t deliveredBytes)
{
  NS_LOG_FUNCTION (this << tcb << deliveredBytes);

  int sendCount = 0;
  if (tcb->m_bytesInFlight > tcb->m_ssThresh)
    {
      if (m_isAlternateAck)     //Check for Alternate Ack's
        {
          m_isAlternateAck = false;
          sendCount = 1 * tcb->m_segmentSize;          //Setting the byte size to be sent to 1 segment size for each Alternate ACK's
        }
      else 
        {
          m_isAlternateAck = true;
          sendCount = 0;
        }
    }

  else
    {
      tcb->m_cWnd = tcb->m_ssThresh;    //Setting cwnd to ssThresh
    }
  tcb->m_cWnd = tcb->m_bytesInFlight + sendCount;       //Updating cwnd with the sent byte
  tcb->m_cWndInfl = tcb->m_cWnd;        //Inflating the cwnd
}

void 
TcpRateHalvingRecovery::ExitRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  tcb->m_cWnd = tcb->m_ssThresh.Get ();
  tcb->m_cWndInfl = tcb->m_cWnd;
}

Ptr<TcpRecoveryOps>
TcpRateHalvingRecovery::Fork (void)
{
  return CopyObject<TcpRateHalvingRecovery> (this);
}

std::string
TcpRateHalvingRecovery::GetName () const
{
  return "RateHalvingRecovery";
}

} // namespace ns3
