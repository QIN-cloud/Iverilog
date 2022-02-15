#include "PUdp.h"

PUdp::PUdp(const string&n, unsigned nports)
: ports(nports), sequential(false), initial(verinum::Vx), name_(n)
{
}