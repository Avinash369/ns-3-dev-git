NS_LOG="TcpSocketBase=warn" ./waf --run "scratch/PRR-example --recoveryType=ns3::TcpClassicRecovery" > result 2>&1
awk -f formatResult.awk result > ClassicResult

NS_LOG="TcpSocketBase=warn" ./waf --run "scratch/PRR-example --recoveryType=ns3::TcpPrrRecovery --reductionBound=SSRB" > result 2>&1
awk -f formatResult.awk result > PrrSSRBResult

NS_LOG="TcpSocketBase=warn" ./waf --run "scratch/PRR-example --recoveryType=ns3::TcpPrrRecovery --reductionBound=CRB" > result 2>&1
awk -f formatResult.awk result > PrrCRBResult

NS_LOG="TcpSocketBase=warn" ./waf --run "scratch/PRR-example --recoveryType=ns3::TcpRateHalvingRecovery" > result 2>&1
awk -f formatResult.awk result > RateHalvingResult

rm -rf result
