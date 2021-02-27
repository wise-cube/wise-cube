APPLICATION = wise-cube
BOARD ?= native 
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Debug Stuff
DEVELHELP ?= 0
QUIET = 1
DEBUG = 1
LOG_LEVEL = LOG_INFO

# Shell
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps 
WERROR = 0

# Networking PHY
ifneq (,$(findstring esp,$(BOARD)))
  include Makefile.esp.include
  CFLAGS += -DESP_WIFI_SSID=\"wireless_sonaglia\"
  CFLAGS += -DESP_WIFI_PASS=\"wurstelecrauti\"
  USEMODULE += esp_wifi
else
  include Makefile.native.include
  
endif

# Networking GNRC
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_icmpv6_echo

USEMODULE += netstats_ipv6
USEMODULE += gnrc_udp
USEMODULE += gnrc_icmpv6_error



# Mqtt
USEMODULE += asymcute
CFLAGS += -DBROKER_HOST=\"$(BROKER_HOST)\"
CFLAGS += -DBROKER_PORT=$(BROKER_PORT)
CFLAGS += -DCLIENT_ID=\"$(CUBE_ID)\"
CFLAGS += -DPUB_TOPIC=\"/to_cube/$(CUBE_ID)\"
CFLAGS += -DSUB_TOPIC=\"/from_cube/$(CUBE_ID)\"


# Compiler flags
CFLAGS += -DBOARD=\"$(BOARD)\"


include $(RIOTBASE)/Makefile.include

