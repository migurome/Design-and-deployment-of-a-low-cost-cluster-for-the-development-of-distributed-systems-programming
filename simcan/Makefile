all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile

makefiles:
	cd src && opp_makemake -f --deep -I$$\(INET_HOME\)/src/linklayer/ieee80211/radio -I$$\(INET_HOME\)/src/networklayer/routing/aodv -I$$\(INET_HOME\)/src/networklayer/common -I$$\(INET_HOME\)/src -I$$\(INET_HOME\)/src/networklayer/icmpv6 -I$$\(INET_HOME\)/src/world/obstacles -I$$\(INET_HOME\)/src/networklayer/xmipv6 -I$$\(INET_HOME\)/src/networklayer/contract -I$$\(INET_HOME\)/src/networklayer/autorouting/ipv4 -I$$\(INET_HOME\)/src/util -I$$\(INET_HOME\)/src/linklayer/common -I$$\(INET_HOME\)/src/transport/contract -I$$\(INET_HOME\)/src/status -I$$\(INET_HOME\)/src/linklayer/radio/propagation -I$$\(INET_HOME\)/src/linklayer/ieee80211/radio/errormodel -I$$\(INET_HOME\)/src/linklayer/radio -I$$\(INET_HOME\)/src/util/headerserializers/tcp -I$$\(INET_HOME\)/src/networklayer/ipv4 -I$$\(INET_HOME\)/src/mobility/contract -I$$\(INET_HOME\)/src/util/headerserializers/ipv4 -I$$\(INET_HOME\)/src/base -I$$\(INET_HOME\)/src/util/headerserializers -I$$\(INET_HOME\)/src/world/radio -I$$\(INET_HOME\)/src/linklayer/ieee80211/mac -I$$\(INET_HOME\)/src/transport/sctp -I$$\(INET_HOME\)/src/networklayer/ipv6 -I$$\(INET_HOME\)/src/util/headerserializers/udp -I$$\(INET_HOME\)/src/networklayer/ipv6tunneling -I$$\(INET_HOME\)/src/applications/pingapp -I$$\(INET_HOME\)/src/battery/models -I$$\(INET_HOME\)/src/util/headerserializers/ipv6 -I$$\(INET_HOME\)/src/util/headerserializers/sctp -I$$\(INET_HOME\)/src/linklayer/contract -I$$\(INET_HOME\)/src/transport/tcp_common -I$$\(INET_HOME\)/src/networklayer/arp -I$$\(INET_HOME\)/src/transport/udp -L../../inet/out/$$\(CONFIGNAME\)/src -linet -DINET_IMPORT -KINET_PROJ=../../inet

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
