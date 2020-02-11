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

#pragma once

#include "ns3/tcp-recovery-ops.h"

namespace ns3 {

/**
 * \ingroup recoveryOps
 *
 * \brief An implementation of Rate Halving
 *
 * Rate Halving is an algorithm that adjusts the congestion window 
 * by spacing transmissions at the rate of one data segment per two 
 * segments acknowledged over the entire recovery period, thereby 
 * sustaining the self-clocking of TCP and avoiding a burst.
 */
class TcpRateHalvingRecovery : public TcpClassicRecovery
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  /**
   * Create an unbound tcp socket.
   */
  TcpRateHalvingRecovery (void);

  /**
   * \brief Copy constructor
   * \param sock the object to copy
   */
  TcpRateHalvingRecovery (const TcpRateHalvingRecovery& sock);

  virtual ~TcpRateHalvingRecovery (void) override;

  std::string GetName () const override;

  virtual void EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount,
                              uint32_t unAckDataCount, uint32_t deliveredBytes) override;

  virtual void DoRecovery (Ptr<TcpSocketState> tcb, uint32_t deliveredBytes) override;

  virtual void ExitRecovery (Ptr<TcpSocketState> tcb) override;

  virtual Ptr<TcpRecoveryOps> Fork () override;

private:
  bool m_isAlternateAck                    {true};   //!< Boolean variable to sent a data packet at every alternate ACK.
};
} // namespace ns3

