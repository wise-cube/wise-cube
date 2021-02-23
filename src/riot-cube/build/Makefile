APPLICATION = wise-cube

# If no BOARD is found in the environment, use this default:
BOARD ?= native 
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 0
QUIET = 0
DEBUG = 0
LOG_LEVEL = LOG_INFO

# Shell
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps 


# Networking PHY
ifneq (,$(findstring esp,$(BOARD)))
  include Makefile.esp.include
  CFLAGS += -DESP_WIFI_SSID=\"wireless_sonaglia\"
  CFLAGS += -DESP_WIFI_PASS=\"wurstelecrauti\"
  USEMODULE += esp_wifi
else
  include Makefile.native.include
  USEMODULE += netdev_tap
endif



# Networking ROUTING 
USEMODULE += netdev_default
USEMODULE += lwip_netdev
USEMODULE += lwip

# Networking ROUTING ipv4
USEMODULE += ipv6_addr
USEMODULE += lwip_ipv6
USEMODULE += lwip_ipv6_autoconfig

# Networking TRANSPORT
USEMODULE += sock_async_event
USEMODULE += sock_ip
USEMODULE += sock_udp
USEMODULE += sock_tcp

# Mqtt
USEPKG += paho-mqtt
CFLAGS += -DBROKER_HOST=\"$(BROKER_HOST)\"
CFLAGS += -DBROKER_PORT=$(BROKER_PORT)
CFLAGS += -DCLIENT_ID=\"$(CUBE_ID)\"
CFLAGS += -DPUB_TOPIC=\"/to_cube/$(CUBE_ID)\"
CFLAGS += -DSUB_TOPIC=\"/from_cube/$(CUBE_ID)\"


# Compiler flags
CFLAGS += -DBOARD=\"$(BOARD)\"

include $(RIOTBASE)/Makefile.include

