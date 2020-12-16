APPLICATION = wise-cube
# If no BOARD is found in the environment, use this default:
BOARD ?= native
# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 0
# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

DEBUG=0
LOG_LEVEL = LOG_INFO

WIFI_SSID ?= "---redacted---"
WIFI_PASS ?= "---redacted---"
BROKER_HOST ?= "---redacted---"
CLIENT_ID ?= "cube_1"

USE_MQTTSN ?= 1

USEMODULE += xtimer

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += netdev_default

USEPKG += jsmn

USEMODULE += sntp

ifneq (1, $(USE_MQTTSN))
   include ./Makefile.paho
else
	include ./Makefile.emcute
endif

include $(RIOTBASE)/Makefile.include

ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
  
endif

ifeq (0, $(USE_MQTT))
  CFLAGS += -DUSE_MQTT
endif
