#
# OMNeT++/OMNEST Makefile for simcan
#
# This file was generated with the command:
#  opp_makemake -f --deep -I$(INET_HOME)/src/linklayer/ieee80211/radio -I$(INET_HOME)/src/networklayer/routing/aodv -I$(INET_HOME)/src/networklayer/common -I$(INET_HOME)/src -I$(INET_HOME)/src/networklayer/icmpv6 -I$(INET_HOME)/src/world/obstacles -I$(INET_HOME)/src/networklayer/xmipv6 -I$(INET_HOME)/src/networklayer/contract -I$(INET_HOME)/src/networklayer/autorouting/ipv4 -I$(INET_HOME)/src/util -I$(INET_HOME)/src/linklayer/common -I$(INET_HOME)/src/transport/contract -I$(INET_HOME)/src/status -I$(INET_HOME)/src/linklayer/radio/propagation -I$(INET_HOME)/src/linklayer/ieee80211/radio/errormodel -I$(INET_HOME)/src/linklayer/radio -I$(INET_HOME)/src/util/headerserializers/tcp -I$(INET_HOME)/src/networklayer/ipv4 -I$(INET_HOME)/src/mobility/contract -I$(INET_HOME)/src/util/headerserializers/ipv4 -I$(INET_HOME)/src/base -I$(INET_HOME)/src/util/headerserializers -I$(INET_HOME)/src/world/radio -I$(INET_HOME)/src/linklayer/ieee80211/mac -I$(INET_HOME)/src/transport/sctp -I$(INET_HOME)/src/networklayer/ipv6 -I$(INET_HOME)/src/util/headerserializers/udp -I$(INET_HOME)/src/networklayer/ipv6tunneling -I$(INET_HOME)/src/applications/pingapp -I$(INET_HOME)/src/battery/models -I$(INET_HOME)/src/util/headerserializers/ipv6 -I$(INET_HOME)/src/util/headerserializers/sctp -I$(INET_HOME)/src/linklayer/contract -I$(INET_HOME)/src/transport/tcp_common -I$(INET_HOME)/src/networklayer/arp -I$(INET_HOME)/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -DINET_IMPORT -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = simcan$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(INET_HOME)/src/linklayer/ieee80211/radio \
    -I$(INET_HOME)/src/networklayer/routing/aodv \
    -I$(INET_HOME)/src/networklayer/common \
    -I$(INET_HOME)/src \
    -I$(INET_HOME)/src/networklayer/icmpv6 \
    -I$(INET_HOME)/src/world/obstacles \
    -I$(INET_HOME)/src/networklayer/xmipv6 \
    -I$(INET_HOME)/src/networklayer/contract \
    -I$(INET_HOME)/src/networklayer/autorouting/ipv4 \
    -I$(INET_HOME)/src/util \
    -I$(INET_HOME)/src/linklayer/common \
    -I$(INET_HOME)/src/transport/contract \
    -I$(INET_HOME)/src/status \
    -I$(INET_HOME)/src/linklayer/radio/propagation \
    -I$(INET_HOME)/src/linklayer/ieee80211/radio/errormodel \
    -I$(INET_HOME)/src/linklayer/radio \
    -I$(INET_HOME)/src/util/headerserializers/tcp \
    -I$(INET_HOME)/src/networklayer/ipv4 \
    -I$(INET_HOME)/src/mobility/contract \
    -I$(INET_HOME)/src/util/headerserializers/ipv4 \
    -I$(INET_HOME)/src/base \
    -I$(INET_HOME)/src/util/headerserializers \
    -I$(INET_HOME)/src/world/radio \
    -I$(INET_HOME)/src/linklayer/ieee80211/mac \
    -I$(INET_HOME)/src/transport/sctp \
    -I$(INET_HOME)/src/networklayer/ipv6 \
    -I$(INET_HOME)/src/util/headerserializers/udp \
    -I$(INET_HOME)/src/networklayer/ipv6tunneling \
    -I$(INET_HOME)/src/applications/pingapp \
    -I$(INET_HOME)/src/battery/models \
    -I$(INET_HOME)/src/util/headerserializers/ipv6 \
    -I$(INET_HOME)/src/util/headerserializers/sctp \
    -I$(INET_HOME)/src/linklayer/contract \
    -I$(INET_HOME)/src/transport/tcp_common \
    -I$(INET_HOME)/src/networklayer/arp \
    -I$(INET_HOME)/src/transport/udp \
    -I. \
    -IApplications \
    -IApplications/AppMPI_Base \
    -IApplications/Apps \
    -IApplications/Apps/ApplicationHPC \
    -IApplications/Apps/FileTransfer \
    -IApplications/Apps/LocalApplication \
    -IApplications/SimcanAPI \
    -IApplications/SystemApps \
    -IApplications/SystemApps/NFS_Client \
    -IApplications/SystemApps/NFS_Server \
    -IApplications/SystemApps/PFS_Client \
    -IBase \
    -IBase/include \
    -IIO \
    -IIO/BlockServers \
    -IIO/BlockServers/BlockServerModules \
    -IIO/Disks \
    -IIO/Disks/BasicDisk \
    -IIO/Disks/Disk_400GB_LI \
    -IMessages \
    -INodes \
    -INodes/Nodes \
    -INodes/Racks \
    -IOperatingSystems \
    -IOperatingSystems/CPU_Schedulers \
    -IOperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO \
    -IOperatingSystems/CPU_Schedulers/CPU_Scheduler_RR \
    -IOperatingSystems/CPUs \
    -IOperatingSystems/CPUs/CPU_Modules \
    -IOperatingSystems/CPUs/CPUcores \
    -IOperatingSystems/CPUs/CPUcores/CPUcore \
    -IOperatingSystems/FileSystems \
    -IOperatingSystems/FileSystems/BasicFileSystem \
    -IOperatingSystems/FileSystems/GeneralPurposeFS \
    -IOperatingSystems/FileSystems/GeneralPurposeFS/weibull \
    -IOperatingSystems/FileSystems/NFS_FileSystem \
    -IOperatingSystems/FileSystems/ParallelFileSystem \
    -IOperatingSystems/Memories \
    -IOperatingSystems/Memories/Caches \
    -IOperatingSystems/Memories/Caches/BlockCache \
    -IOperatingSystems/Memories/Caches/NullCache \
    -IOperatingSystems/Memories/MainMemories \
    -IOperatingSystems/Memories/MainMemories/BasicMainMemory \
    -IOperatingSystems/Memories/MainMemories/MainMemory \
    -IOperatingSystems/Memories/MainMemories/MainMemoryBlockCache \
    -IOperatingSystems/Memories/MainMemories/NullMemory \
    -IOperatingSystems/NetworkServices \
    -IOperatingSystems/NetworkServices/NetworkService \
    -IOperatingSystems/NetworkServices/NullNetworkService \
    -IOperatingSystems/NetworkServices/TCP_Services \
    -IOperatingSystems/OperatingSystemModules \
    -IOperatingSystems/ServiceRedirectors \
    -IOperatingSystems/ServiceRedirectors/ServiceRedirector \
    -IOperatingSystems/VirtualFileSystems \
    -IOperatingSystems/VirtualFileSystems/VirtualFileSystem \
    -IOperatingSystems/VolumeManagers \
    -IOperatingSystems/VolumeManagers/BlockManagers \
    -IOperatingSystems/VolumeManagers/BlockManagers/DataBlockManager \
    -IOperatingSystems/VolumeManagers/BlockManagers/NullBlockManager \
    -IOperatingSystems/VolumeManagers/BlockSchedulers \
    -IOperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler \
    -IOperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler \
    -Ioutput \
    -ISMS \
    -IUtil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = /src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/Applications/AppMPI_Base/AppMPI_Base.o \
    $O/Applications/AppMPI_Base/MPI_Call.o \
    $O/Applications/Apps/ApplicationHPC/ApplicationHPC.o \
    $O/Applications/Apps/FileTransfer/FileTransfer.o \
    $O/Applications/Apps/LocalApplication/LocalApplication.o \
    $O/Applications/SimcanAPI/SimcanAPI.o \
    $O/Applications/SystemApps/NFS_Client/NFS_Client.o \
    $O/Applications/SystemApps/NFS_Server/NFS_Server.o \
    $O/Applications/SystemApps/PFS_Client/PFS_Client.o \
    $O/Base/cfgMPI.o \
    $O/Base/cfgPreloadFS.o \
    $O/Base/cfgServers.o \
    $O/Base/cfgVFS.o \
    $O/Base/cSIMCAN_Base.o \
    $O/Base/FileConfigManager.o \
    $O/Base/SIMCAN_MemoryBlock.o \
    $O/IO/Disks/BasicDisk/BasicDisk.o \
    $O/IO/Disks/Disk_400GB_LI/Disk_400GB_LI.o \
    $O/Messages/SIMCAN_App_CPU_Message.o \
    $O/Messages/SIMCAN_App_IO_Message.o \
    $O/Messages/SIMCAN_App_MEM_Message.o \
    $O/Messages/SIMCAN_App_NET_Message.o \
    $O/Messages/SIMCAN_BlockList_Message.o \
    $O/Messages/SIMCAN_File.o \
    $O/Messages/SIMCAN_Message.o \
    $O/Messages/SIMCAN_MPI_Message.o \
    $O/OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/CPU_Scheduler_FIFO.o \
    $O/OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/CPU_Scheduler_RR.o \
    $O/OperatingSystems/CPUs/CPUcores/CPUcore/CPUcore.o \
    $O/OperatingSystems/FileSystems/BasicFileSystem/BasicFileSystem.o \
    $O/OperatingSystems/FileSystems/GeneralPurposeFS/GeneralPurposeFS.o \
    $O/OperatingSystems/FileSystems/NFS_FileSystem/NFS_FileSystem.o \
    $O/OperatingSystems/FileSystems/ParallelFileSystem/ParallelFileSystem.o \
    $O/OperatingSystems/Memories/Caches/BlockCache/BlockCache.o \
    $O/OperatingSystems/Memories/Caches/NullCache/NullCache.o \
    $O/OperatingSystems/Memories/MainMemories/BasicMainMemory/BasicMainMemory.o \
    $O/OperatingSystems/Memories/MainMemories/MainMemory/MainMemory.o \
    $O/OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/MainMemoryBlockCache.o \
    $O/OperatingSystems/Memories/MainMemories/NullMemory/NullMemory.o \
    $O/OperatingSystems/NetworkServices/NetworkService/NetworkService.o \
    $O/OperatingSystems/NetworkServices/NullNetworkService/NullNetworkService.o \
    $O/OperatingSystems/NetworkServices/TCP_Services/TCP_ClientSideService.o \
    $O/OperatingSystems/NetworkServices/TCP_Services/TCP_ServerSideService.o \
    $O/OperatingSystems/ServiceRedirectors/ServiceRedirector/ServiceRedirector.o \
    $O/OperatingSystems/VirtualFileSystems/VirtualFileSystem/VirtualFileSystem.o \
    $O/OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/DataBlockManager.o \
    $O/OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/NullBlockManager.o \
    $O/OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/DataBlockScheduler.o \
    $O/OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/NullBlockScheduler.o \
    $O/SMS/SIMCAN_Request.o \
    $O/SMS/SMS_Branch.o \
    $O/SMS/SMS_MainMemory.o \
    $O/SMS/SMS_NFS.o \
    $O/SMS/SMS_PFS.o \
    $O/SMS/SMS_RAID_0.o \
    $O/SMS/SplittingMessageSystem.o \
    $O/Util/ResourcesMeter.o \
    $O/Messages/SIMCAN_App_CPU_Message_m.o \
    $O/Messages/SIMCAN_App_IO_Message_m.o \
    $O/Messages/SIMCAN_App_MEM_Message_m.o \
    $O/Messages/SIMCAN_App_NET_Message_m.o \
    $O/Messages/SIMCAN_BlockList_Message_m.o \
    $O/Messages/SIMCAN_Message_m.o \
    $O/Messages/SIMCAN_MPI_Message_m.o

# Message files
MSGFILES = \
    Messages/SIMCAN_App_CPU_Message.msg \
    Messages/SIMCAN_App_IO_Message.msg \
    Messages/SIMCAN_App_MEM_Message.msg \
    Messages/SIMCAN_App_NET_Message.msg \
    Messages/SIMCAN_BlockList_Message.msg \
    Messages/SIMCAN_Message.msg \
    Messages/SIMCAN_MPI_Message.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f simcan simcan.exe libsimcan.so libsimcan.a libsimcan.dll libsimcan.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f Applications/*_m.cc Applications/*_m.h
	$(Q)-rm -f Applications/AppMPI_Base/*_m.cc Applications/AppMPI_Base/*_m.h
	$(Q)-rm -f Applications/Apps/*_m.cc Applications/Apps/*_m.h
	$(Q)-rm -f Applications/Apps/ApplicationHPC/*_m.cc Applications/Apps/ApplicationHPC/*_m.h
	$(Q)-rm -f Applications/Apps/FileTransfer/*_m.cc Applications/Apps/FileTransfer/*_m.h
	$(Q)-rm -f Applications/Apps/LocalApplication/*_m.cc Applications/Apps/LocalApplication/*_m.h
	$(Q)-rm -f Applications/SimcanAPI/*_m.cc Applications/SimcanAPI/*_m.h
	$(Q)-rm -f Applications/SystemApps/*_m.cc Applications/SystemApps/*_m.h
	$(Q)-rm -f Applications/SystemApps/NFS_Client/*_m.cc Applications/SystemApps/NFS_Client/*_m.h
	$(Q)-rm -f Applications/SystemApps/NFS_Server/*_m.cc Applications/SystemApps/NFS_Server/*_m.h
	$(Q)-rm -f Applications/SystemApps/PFS_Client/*_m.cc Applications/SystemApps/PFS_Client/*_m.h
	$(Q)-rm -f Base/*_m.cc Base/*_m.h
	$(Q)-rm -f Base/include/*_m.cc Base/include/*_m.h
	$(Q)-rm -f IO/*_m.cc IO/*_m.h
	$(Q)-rm -f IO/BlockServers/*_m.cc IO/BlockServers/*_m.h
	$(Q)-rm -f IO/BlockServers/BlockServerModules/*_m.cc IO/BlockServers/BlockServerModules/*_m.h
	$(Q)-rm -f IO/Disks/*_m.cc IO/Disks/*_m.h
	$(Q)-rm -f IO/Disks/BasicDisk/*_m.cc IO/Disks/BasicDisk/*_m.h
	$(Q)-rm -f IO/Disks/Disk_400GB_LI/*_m.cc IO/Disks/Disk_400GB_LI/*_m.h
	$(Q)-rm -f Messages/*_m.cc Messages/*_m.h
	$(Q)-rm -f Nodes/*_m.cc Nodes/*_m.h
	$(Q)-rm -f Nodes/Nodes/*_m.cc Nodes/Nodes/*_m.h
	$(Q)-rm -f Nodes/Racks/*_m.cc Nodes/Racks/*_m.h
	$(Q)-rm -f OperatingSystems/*_m.cc OperatingSystems/*_m.h
	$(Q)-rm -f OperatingSystems/CPU_Schedulers/*_m.cc OperatingSystems/CPU_Schedulers/*_m.h
	$(Q)-rm -f OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/*_m.cc OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/*_m.h
	$(Q)-rm -f OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/*_m.cc OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/*_m.h
	$(Q)-rm -f OperatingSystems/CPUs/*_m.cc OperatingSystems/CPUs/*_m.h
	$(Q)-rm -f OperatingSystems/CPUs/CPU_Modules/*_m.cc OperatingSystems/CPUs/CPU_Modules/*_m.h
	$(Q)-rm -f OperatingSystems/CPUs/CPUcores/*_m.cc OperatingSystems/CPUs/CPUcores/*_m.h
	$(Q)-rm -f OperatingSystems/CPUs/CPUcores/CPUcore/*_m.cc OperatingSystems/CPUs/CPUcores/CPUcore/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/*_m.cc OperatingSystems/FileSystems/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/BasicFileSystem/*_m.cc OperatingSystems/FileSystems/BasicFileSystem/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/GeneralPurposeFS/*_m.cc OperatingSystems/FileSystems/GeneralPurposeFS/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/GeneralPurposeFS/weibull/*_m.cc OperatingSystems/FileSystems/GeneralPurposeFS/weibull/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/NFS_FileSystem/*_m.cc OperatingSystems/FileSystems/NFS_FileSystem/*_m.h
	$(Q)-rm -f OperatingSystems/FileSystems/ParallelFileSystem/*_m.cc OperatingSystems/FileSystems/ParallelFileSystem/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/*_m.cc OperatingSystems/Memories/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/Caches/*_m.cc OperatingSystems/Memories/Caches/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/Caches/BlockCache/*_m.cc OperatingSystems/Memories/Caches/BlockCache/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/Caches/NullCache/*_m.cc OperatingSystems/Memories/Caches/NullCache/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/MainMemories/*_m.cc OperatingSystems/Memories/MainMemories/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/MainMemories/BasicMainMemory/*_m.cc OperatingSystems/Memories/MainMemories/BasicMainMemory/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/MainMemories/MainMemory/*_m.cc OperatingSystems/Memories/MainMemories/MainMemory/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/*_m.cc OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/*_m.h
	$(Q)-rm -f OperatingSystems/Memories/MainMemories/NullMemory/*_m.cc OperatingSystems/Memories/MainMemories/NullMemory/*_m.h
	$(Q)-rm -f OperatingSystems/NetworkServices/*_m.cc OperatingSystems/NetworkServices/*_m.h
	$(Q)-rm -f OperatingSystems/NetworkServices/NetworkService/*_m.cc OperatingSystems/NetworkServices/NetworkService/*_m.h
	$(Q)-rm -f OperatingSystems/NetworkServices/NullNetworkService/*_m.cc OperatingSystems/NetworkServices/NullNetworkService/*_m.h
	$(Q)-rm -f OperatingSystems/NetworkServices/TCP_Services/*_m.cc OperatingSystems/NetworkServices/TCP_Services/*_m.h
	$(Q)-rm -f OperatingSystems/OperatingSystemModules/*_m.cc OperatingSystems/OperatingSystemModules/*_m.h
	$(Q)-rm -f OperatingSystems/ServiceRedirectors/*_m.cc OperatingSystems/ServiceRedirectors/*_m.h
	$(Q)-rm -f OperatingSystems/ServiceRedirectors/ServiceRedirector/*_m.cc OperatingSystems/ServiceRedirectors/ServiceRedirector/*_m.h
	$(Q)-rm -f OperatingSystems/VirtualFileSystems/*_m.cc OperatingSystems/VirtualFileSystems/*_m.h
	$(Q)-rm -f OperatingSystems/VirtualFileSystems/VirtualFileSystem/*_m.cc OperatingSystems/VirtualFileSystems/VirtualFileSystem/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/*_m.cc OperatingSystems/VolumeManagers/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockManagers/*_m.cc OperatingSystems/VolumeManagers/BlockManagers/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/*_m.cc OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/*_m.cc OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockSchedulers/*_m.cc OperatingSystems/VolumeManagers/BlockSchedulers/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/*_m.cc OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/*_m.h
	$(Q)-rm -f OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/*_m.cc OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/*_m.h
	$(Q)-rm -f output/*_m.cc output/*_m.h
	$(Q)-rm -f SMS/*_m.cc SMS/*_m.h
	$(Q)-rm -f Util/*_m.cc Util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc Applications/*.cc Applications/AppMPI_Base/*.cc Applications/Apps/*.cc Applications/Apps/ApplicationHPC/*.cc Applications/Apps/FileTransfer/*.cc Applications/Apps/LocalApplication/*.cc Applications/SimcanAPI/*.cc Applications/SystemApps/*.cc Applications/SystemApps/NFS_Client/*.cc Applications/SystemApps/NFS_Server/*.cc Applications/SystemApps/PFS_Client/*.cc Base/*.cc Base/include/*.cc IO/*.cc IO/BlockServers/*.cc IO/BlockServers/BlockServerModules/*.cc IO/Disks/*.cc IO/Disks/BasicDisk/*.cc IO/Disks/Disk_400GB_LI/*.cc Messages/*.cc Nodes/*.cc Nodes/Nodes/*.cc Nodes/Racks/*.cc OperatingSystems/*.cc OperatingSystems/CPU_Schedulers/*.cc OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/*.cc OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/*.cc OperatingSystems/CPUs/*.cc OperatingSystems/CPUs/CPU_Modules/*.cc OperatingSystems/CPUs/CPUcores/*.cc OperatingSystems/CPUs/CPUcores/CPUcore/*.cc OperatingSystems/FileSystems/*.cc OperatingSystems/FileSystems/BasicFileSystem/*.cc OperatingSystems/FileSystems/GeneralPurposeFS/*.cc OperatingSystems/FileSystems/GeneralPurposeFS/weibull/*.cc OperatingSystems/FileSystems/NFS_FileSystem/*.cc OperatingSystems/FileSystems/ParallelFileSystem/*.cc OperatingSystems/Memories/*.cc OperatingSystems/Memories/Caches/*.cc OperatingSystems/Memories/Caches/BlockCache/*.cc OperatingSystems/Memories/Caches/NullCache/*.cc OperatingSystems/Memories/MainMemories/*.cc OperatingSystems/Memories/MainMemories/BasicMainMemory/*.cc OperatingSystems/Memories/MainMemories/MainMemory/*.cc OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/*.cc OperatingSystems/Memories/MainMemories/NullMemory/*.cc OperatingSystems/NetworkServices/*.cc OperatingSystems/NetworkServices/NetworkService/*.cc OperatingSystems/NetworkServices/NullNetworkService/*.cc OperatingSystems/NetworkServices/TCP_Services/*.cc OperatingSystems/OperatingSystemModules/*.cc OperatingSystems/ServiceRedirectors/*.cc OperatingSystems/ServiceRedirectors/ServiceRedirector/*.cc OperatingSystems/VirtualFileSystems/*.cc OperatingSystems/VirtualFileSystems/VirtualFileSystem/*.cc OperatingSystems/VolumeManagers/*.cc OperatingSystems/VolumeManagers/BlockManagers/*.cc OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/*.cc OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/*.cc OperatingSystems/VolumeManagers/BlockSchedulers/*.cc OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/*.cc OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/*.cc output/*.cc SMS/*.cc Util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Applications/AppMPI_Base/AppMPI_Base.o: Applications/AppMPI_Base/AppMPI_Base.cc \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message_m.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Base/FileConfigManager.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Applications/AppMPI_Base/AppMPI_Base.h \
  Messages/SIMCAN_Message.h \
  Base/cfgVFS.h \
  Messages/IPv4Address.h \
  Messages/IPv6Address.h \
  Applications/AppMPI_Base/MPI_Call.h \
  Messages/TCPSocketMap.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h
$O/Applications/AppMPI_Base/MPI_Call.o: Applications/AppMPI_Base/MPI_Call.cc \
  Messages/SIMCAN_Message.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/IPvXAddress.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/Compat.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Messages/TCPSocketMap.h \
  Applications/AppMPI_Base/MPI_Call.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message_m.h
$O/Applications/Apps/ApplicationHPC/ApplicationHPC.o: Applications/Apps/ApplicationHPC/ApplicationHPC.cc \
  Messages/SIMCAN_MPI_Message.h \
  Messages/TCPSocketMap.h \
  Applications/AppMPI_Base/MPI_Call.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/include/SIMCAN_debug.h \
  Messages/INETDefs.h \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/TCPSocket.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_Message_m.h \
  Applications/Apps/ApplicationHPC/ApplicationHPC.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Applications/AppMPI_Base/AppMPI_Base.h \
  Messages/SIMCAN_Message.h \
  Base/cfgVFS.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Base/cfgPreloadFS.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Base/FileConfigManager.h
$O/Applications/Apps/FileTransfer/FileTransfer.o: Applications/Apps/FileTransfer/FileTransfer.cc \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Applications/Apps/FileTransfer/FileTransfer.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/include/SIMCAN_debug.h \
  Messages/TCPSocketMap.h \
  Messages/IPv6Address.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h
$O/Applications/Apps/LocalApplication/LocalApplication.o: Applications/Apps/LocalApplication/LocalApplication.cc \
  Messages/TCPSocketMap.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Applications/Apps/LocalApplication/LocalApplication.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPv4Address.h
$O/Applications/SimcanAPI/SimcanAPI.o: Applications/SimcanAPI/SimcanAPI.cc \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/TCPSocketMap.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/include/SIMCAN_debug.h
$O/Applications/SystemApps/NFS_Client/NFS_Client.o: Applications/SystemApps/NFS_Client/NFS_Client.cc \
  Base/include/SIMCAN_types.h \
  Base/cfgServers.h \
  Messages/INETDefs.h \
  SMS/SplittingMessageSystem.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  SMS/SMS_NFS.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Applications/SystemApps/NFS_Client/NFS_Client.h \
  SMS/SIMCAN_Request.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/IPv4Address.h \
  Base/cfgVFS.h \
  Base/FileConfigManager.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h
$O/Applications/SystemApps/NFS_Server/NFS_Server.o: Applications/SystemApps/NFS_Server/NFS_Server.cc \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Applications/SystemApps/NFS_Server/NFS_Server.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h
$O/Applications/SystemApps/PFS_Client/PFS_Client.o: Applications/SystemApps/PFS_Client/PFS_Client.cc \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Applications/SystemApps/PFS_Client/PFS_Client.h \
  Messages/SIMCAN_MPI_Message.h \
  SMS/SplittingMessageSystem.h \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  SMS/SMS_NFS.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  SMS/SIMCAN_Request.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Applications/SimcanAPI/SimcanAPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_Message_m.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Base/FileConfigManager.h \
  Base/cfgVFS.h \
  Messages/IPv4Address.h
$O/Base/FileConfigManager.o: Base/FileConfigManager.cc \
  Base/FileConfigManager.h \
  Messages/IPv4Address.h \
  Base/cfgVFS.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Base/cfgMPI.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/TCPSocket.h \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h
$O/Base/SIMCAN_MemoryBlock.o: Base/SIMCAN_MemoryBlock.cc \
  Base/SIMCAN_MemoryBlock.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h
$O/Base/cSIMCAN_Base.o: Base/cSIMCAN_Base.cc \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/IPv4Address.h
$O/Base/cfgMPI.o: Base/cfgMPI.cc \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Base/cfgMPI.h
$O/Base/cfgPreloadFS.o: Base/cfgPreloadFS.cc \
  Base/cfgPreloadFS.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h
$O/Base/cfgServers.o: Base/cfgServers.cc \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h
$O/Base/cfgVFS.o: Base/cfgVFS.cc \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Base/cfgVFS.h
$O/IO/Disks/BasicDisk/BasicDisk.o: IO/Disks/BasicDisk/BasicDisk.cc \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  IO/Disks/BasicDisk/BasicDisk.h \
  Messages/TCPSocket.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_File.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h
$O/IO/Disks/Disk_400GB_LI/Disk_400GB_LI.o: IO/Disks/Disk_400GB_LI/Disk_400GB_LI.cc \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocket.h \
  IO/Disks/Disk_400GB_LI/Disk_400GB_LI.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_File.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_BlockList_Message_m.h
$O/Messages/SIMCAN_App_CPU_Message.o: Messages/SIMCAN_App_CPU_Message.cc \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/TCPSocketMap.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_CPU_Message_m.h
$O/Messages/SIMCAN_App_IO_Message.o: Messages/SIMCAN_App_IO_Message.cc \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/Compat.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h
$O/Messages/SIMCAN_App_MEM_Message.o: Messages/SIMCAN_App_MEM_Message.cc \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/TCPSocketMap.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h
$O/Messages/SIMCAN_App_NET_Message.o: Messages/SIMCAN_App_NET_Message.cc \
  Messages/TCPSocketMap.h \
  Messages/Compat.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h
$O/Messages/SIMCAN_BlockList_Message.o: Messages/SIMCAN_BlockList_Message.cc \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_File.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/Compat.h
$O/Messages/SIMCAN_File.o: Messages/SIMCAN_File.cc \
  Messages/SIMCAN_File.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h
$O/Messages/SIMCAN_MPI_Message.o: Messages/SIMCAN_MPI_Message.cc \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/Compat.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h
$O/Messages/SIMCAN_Message.o: Messages/SIMCAN_Message.cc \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/TCPSocketMap.h \
  Messages/Compat.h
$O/OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/CPU_Scheduler_FIFO.o: OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/CPU_Scheduler_FIFO.cc \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  OperatingSystems/CPU_Schedulers/CPU_Scheduler_FIFO/CPU_Scheduler_FIFO.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h
$O/OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/CPU_Scheduler_RR.o: OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/CPU_Scheduler_RR.cc \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  OperatingSystems/CPU_Schedulers/CPU_Scheduler_RR/CPU_Scheduler_RR.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h
$O/OperatingSystems/CPUs/CPUcores/CPUcore/CPUcore.o: OperatingSystems/CPUs/CPUcores/CPUcore/CPUcore.cc \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  OperatingSystems/CPUs/CPUcores/CPUcore/CPUcore.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h
$O/OperatingSystems/FileSystems/BasicFileSystem/BasicFileSystem.o: OperatingSystems/FileSystems/BasicFileSystem/BasicFileSystem.cc \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Base/include/SIMCAN_types.h \
  Base/cfgServers.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/IPv4Address.h \
  Base/cfgVFS.h \
  Messages/SIMCAN_File.h \
  Base/FileConfigManager.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  OperatingSystems/FileSystems/BasicFileSystem/BasicFileSystem.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h
$O/OperatingSystems/FileSystems/GeneralPurposeFS/GeneralPurposeFS.o: OperatingSystems/FileSystems/GeneralPurposeFS/GeneralPurposeFS.cc \
  Base/cfgMPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Base/cfgVFS.h \
  Base/FileConfigManager.h \
  Messages/SIMCAN_File.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_BlockList_Message.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  OperatingSystems/FileSystems/GeneralPurposeFS/GeneralPurposeFS.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/cfgServers.h
$O/OperatingSystems/FileSystems/NFS_FileSystem/NFS_FileSystem.o: OperatingSystems/FileSystems/NFS_FileSystem/NFS_FileSystem.cc \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  OperatingSystems/FileSystems/NFS_FileSystem/NFS_FileSystem.h \
  Base/cfgServers.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Base/cfgPreloadFS.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/FileConfigManager.h \
  Base/cfgVFS.h \
  Messages/IPv4Address.h \
  Base/cfgMPI.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h
$O/OperatingSystems/FileSystems/ParallelFileSystem/ParallelFileSystem.o: OperatingSystems/FileSystems/ParallelFileSystem/ParallelFileSystem.cc \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Base/cfgMPI.h \
  Base/cfgVFS.h \
  Messages/IPv4Address.h \
  Base/FileConfigManager.h \
  SMS/SIMCAN_Request.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  OperatingSystems/FileSystems/ParallelFileSystem/ParallelFileSystem.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  SMS/SMS_PFS.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Base/cfgPreloadFS.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/cfgServers.h \
  SMS/SplittingMessageSystem.h
$O/OperatingSystems/Memories/Caches/BlockCache/BlockCache.o: OperatingSystems/Memories/Caches/BlockCache/BlockCache.cc \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  OperatingSystems/Memories/Caches/BlockCache/BlockCache.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h
$O/OperatingSystems/Memories/Caches/NullCache/NullCache.o: OperatingSystems/Memories/Caches/NullCache/NullCache.cc \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  OperatingSystems/Memories/Caches/NullCache/NullCache.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/IPv4Address.h
$O/OperatingSystems/Memories/MainMemories/BasicMainMemory/BasicMainMemory.o: OperatingSystems/Memories/MainMemories/BasicMainMemory/BasicMainMemory.cc \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  OperatingSystems/Memories/MainMemories/BasicMainMemory/BasicMainMemory.h \
  Messages/Compat.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h
$O/OperatingSystems/Memories/MainMemories/MainMemory/MainMemory.o: OperatingSystems/Memories/MainMemories/MainMemory/MainMemory.cc \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  SMS/SplittingMessageSystem.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  SMS/SMS_MainMemory.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  SMS/SIMCAN_Request.h \
  Messages/TCPSocketMap.h \
  Base/SIMCAN_MemoryBlock.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  OperatingSystems/Memories/MainMemories/MainMemory/MainMemory.h
$O/OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/MainMemoryBlockCache.o: OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/MainMemoryBlockCache.cc \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  SMS/SIMCAN_Request.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Base/SIMCAN_MemoryBlock.h \
  Messages/TCPSocketMap.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  SMS/SplittingMessageSystem.h \
  Messages/SIMCAN_MPI_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message.h \
  OperatingSystems/Memories/MainMemories/MainMemoryBlockCache/MainMemoryBlockCache.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  SMS/SMS_MainMemory.h
$O/OperatingSystems/Memories/MainMemories/NullMemory/NullMemory.o: OperatingSystems/Memories/MainMemories/NullMemory/NullMemory.cc \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  OperatingSystems/Memories/MainMemories/NullMemory/NullMemory.h \
  Messages/IPv4Address.h
$O/OperatingSystems/NetworkServices/NetworkService/NetworkService.o: OperatingSystems/NetworkServices/NetworkService/NetworkService.cc \
  Messages/IPvXAddress.h \
  OperatingSystems/NetworkServices/NetworkService/NetworkService.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPvXAddressResolver.h \
  Messages/SIMCAN_MPI_Message.h \
  OperatingSystems/NetworkServices/TCP_Services/TCP_ServerSideService.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  OperatingSystems/NetworkServices/TCP_Services/TCP_ClientSideService.h \
  Messages/IPv4Address.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h
$O/OperatingSystems/NetworkServices/NullNetworkService/NullNetworkService.o: OperatingSystems/NetworkServices/NullNetworkService/NullNetworkService.cc \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/IPv4Address.h \
  OperatingSystems/NetworkServices/NullNetworkService/NullNetworkService.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/IPvXAddress.h
$O/OperatingSystems/NetworkServices/TCP_Services/TCP_ClientSideService.o: OperatingSystems/NetworkServices/TCP_Services/TCP_ClientSideService.cc \
  Messages/IPv4Address.h \
  OperatingSystems/NetworkServices/TCP_Services/TCP_ClientSideService.h \
  Messages/TCPSocketMap.h \
  Messages/Compat.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  OperatingSystems/NetworkServices/NetworkService/NetworkService.h \
  Messages/IPv6Address.h \
  Messages/IPvXAddressResolver.h \
  Messages/INETDefs.h
$O/OperatingSystems/NetworkServices/TCP_Services/TCP_ServerSideService.o: OperatingSystems/NetworkServices/TCP_Services/TCP_ServerSideService.cc \
  Messages/IPvXAddress.h \
  OperatingSystems/NetworkServices/NetworkService/NetworkService.h \
  Messages/TCPSocket.h \
  Messages/INETDefs.h \
  Messages/IPv6Address.h \
  OperatingSystems/NetworkServices/TCP_Services/TCP_ServerSideService.h \
  Messages/IPv4Address.h \
  Messages/Compat.h \
  Messages/TCPSocketMap.h
$O/OperatingSystems/ServiceRedirectors/ServiceRedirector/ServiceRedirector.o: OperatingSystems/ServiceRedirectors/ServiceRedirector/ServiceRedirector.cc \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/IPvXAddress.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  OperatingSystems/ServiceRedirectors/ServiceRedirector/ServiceRedirector.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h
$O/OperatingSystems/VirtualFileSystems/VirtualFileSystem/VirtualFileSystem.o: OperatingSystems/VirtualFileSystems/VirtualFileSystem/VirtualFileSystem.cc \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  OperatingSystems/VirtualFileSystems/VirtualFileSystem/VirtualFileSystem.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Base/FileConfigManager.h \
  Messages/IPv4Address.h \
  Base/cfgVFS.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Base/cfgMPI.h \
  Messages/INETDefs.h \
  Base/cfgServers.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Base/cfgPreloadFS.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h
$O/OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/DataBlockManager.o: OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/DataBlockManager.cc \
  Messages/SIMCAN_File.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_Message_m.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_BlockList_Message.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  SMS/SMS_RAID_0.h \
  OperatingSystems/VolumeManagers/BlockManagers/DataBlockManager/DataBlockManager.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h
$O/OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/NullBlockManager.o: OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/NullBlockManager.cc \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_File.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  OperatingSystems/VolumeManagers/BlockManagers/NullBlockManager/NullBlockManager.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/Compat.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  SMS/SIMCAN_Request.h \
  SMS/SMS_RAID_0.h
$O/OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/DataBlockScheduler.o: OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/DataBlockScheduler.cc \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  OperatingSystems/VolumeManagers/BlockSchedulers/DataBlockScheduler/DataBlockScheduler.h \
  Messages/SIMCAN_BlockList_Message.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  Messages/SIMCAN_File.h \
  Messages/IPv4Address.h \
  SMS/SMS_Branch.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h
$O/OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/NullBlockScheduler.o: OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/NullBlockScheduler.cc \
  Base/include/SIMCAN_debug.h \
  OperatingSystems/VolumeManagers/BlockSchedulers/NullBlockScheduler/NullBlockScheduler.h \
  Messages/IPv6Address.h \
  Messages/SIMCAN_MPI_Message.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_NET_Message.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_NET_Message_m.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_CPU_Message.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/SIMCAN_MPI_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/SIMCAN_App_CPU_Message_m.h \
  Base/cSIMCAN_Base.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/SIMCAN_File.h \
  Messages/IPv4Address.h
$O/SMS/SIMCAN_Request.o: SMS/SIMCAN_Request.cc \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  Messages/Compat.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h
$O/SMS/SMS_Branch.o: SMS/SMS_Branch.cc \
  Messages/SIMCAN_File.h \
  SMS/SMS_Branch.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  Messages/Compat.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  SMS/SIMCAN_Request.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_debug.h
$O/SMS/SMS_MainMemory.o: SMS/SMS_MainMemory.cc \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  SMS/SplittingMessageSystem.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  Messages/IPvXAddress.h \
  SMS/SMS_MainMemory.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_MEM_Message.h \
  Messages/SIMCAN_Message_m.h \
  Messages/SIMCAN_App_MEM_Message_m.h \
  Messages/IPv4Address.h
$O/SMS/SMS_NFS.o: SMS/SMS_NFS.cc \
  Messages/Compat.h \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_Message_m.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  SMS/SMS_NFS.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h
$O/SMS/SMS_PFS.o: SMS/SMS_PFS.cc \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  SMS/SIMCAN_Request.h \
  Messages/TCPSocketMap.h \
  SMS/SMS_PFS.h \
  Messages/Compat.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Base/include/SIMCAN_types.h \
  Messages/INETDefs.h \
  SMS/SplittingMessageSystem.h
$O/SMS/SMS_RAID_0.o: SMS/SMS_RAID_0.cc \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_Message.h \
  Messages/SIMCAN_App_IO_Message.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  SMS/SplittingMessageSystem.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Messages/SIMCAN_File.h \
  SMS/SIMCAN_Request.h \
  SMS/SMS_RAID_0.h \
  Messages/SIMCAN_BlockList_Message_m.h \
  Messages/TCPSocketMap.h \
  Messages/SIMCAN_BlockList_Message.h \
  Messages/Compat.h
$O/SMS/SplittingMessageSystem.o: SMS/SplittingMessageSystem.cc \
  Messages/TCPSocketMap.h \
  SMS/SIMCAN_Request.h \
  Messages/Compat.h \
  Messages/SIMCAN_Message_m.h \
  Messages/IPv4Address.h \
  Base/include/SIMCAN_debug.h \
  Messages/IPv6Address.h \
  SMS/SplittingMessageSystem.h \
  Messages/INETDefs.h \
  Base/include/SIMCAN_types.h \
  Messages/IPvXAddress.h \
  Messages/SIMCAN_App_IO_Message_m.h \
  Messages/TCPSocket.h \
  Messages/SIMCAN_App_IO_Message.h \
  Messages/SIMCAN_Message.h
$O/Util/ResourcesMeter.o: Util/ResourcesMeter.cc \
  Util/ResourcesMeter.h
