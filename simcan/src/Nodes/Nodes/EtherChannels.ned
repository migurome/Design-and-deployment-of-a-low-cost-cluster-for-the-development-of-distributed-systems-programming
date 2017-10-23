// Channels for defining conections between Ethernet hosts
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.Nodes.Nodes;
import inet.nodes.ethernet.EtherLink;

channelinterface IEthChannel{
}

//
// 10 megabit/sec Ethernet link
//
channel Eth10M_channel extends EtherLink like IEthChannel
{
    datarate = 10Mbps;
}

//
// 100 megabit/sec Ethernet link
//
channel Eth100M_channel extends EtherLink like IEthChannel
{
    datarate = 100Mbps;
}

//
// 1 gigabit/sec Ethernet link
//
channel Eth1G_channel extends EtherLink like IEthChannel
{
    datarate = 1Gbps;
}

//
// 10 gigabit/sec Ethernet link
//
channel Eth10G_channel extends EtherLink like IEthChannel
{
    datarate = 10Gbps;
}

//
// 40 gigabit/sec Ethernet link
//
channel Eth40G_channel extends EtherLink like IEthChannel
{
    datarate = 40Gbps;
}

//
// 100 gigabit/sec Ethernet link
//
channel Eth100G_channel extends EtherLink like IEthChannel
{
    datarate = 100Gbps;
}