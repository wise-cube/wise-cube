# General Configuration

# Application name
APPLICATION = wise-cube
# Default BOARD if it is not overwritten
BOARD ?= esp8266-esp-12x
# Default RIOTBASE directoty 
RIOTBASE ?= $(CURDIR)/../../../../RIOT
# Toggle to enable/disable debug environments
DEBUG ?= 1

#Select additional modules

# Cube messaging module, there are 3 values mqtt for this config
# emcute -> use ipv6 and emcute module for mqttsn
# paho -> use ipv4 and paho package for mqtt
# none -> disable messaging
CUBE_MESSAGING ?= emcute

# Define NATIVE inside compiler, in order to differentiate 
# build for native / esp environments
ifeq ($(BOARD), native)
  VIRTUAL = 1
endif

ifeq ($(VIRTUAL), 1)
  CFLAGS += -DVIRTUAL
  CFLAGS += -DSERVER_ADDR=\"$(VIRTUAL_SERVER_ADDR)\"
else
  CFLAGS += -DSERVER_ADDR=\"$(SERVER_ADDR)\"
endif

# Enable debugging tools
ifeq (1, $(DEBUG))
  DEVELHELP = 1
  LOG_LEVEL = LOG_DEBUG
  QUIET ?= 0
  USEMODULE += shell_commands
  USEMODULE += ps
else
  DEVELHELP = 0
  LOG_LEVEL = LOG_INFO
  QUIET ?= 1
endif



CFLAGS += -DGNRC_NETIF_SINGLE

USEMODULE += netdev_default
USEMODULE += shell
USEMODULE += sntp




ifeq ($(CUBE_MESSAGING),none)
  $(info "CUBE_MESSAGING is disabled")
else ifeq ($(CUBE_MESSAGING),emcute)
  USEMODULE += sock_dns
  USEMODULE += cube_messaging
  USEMODULE += auto_init_gnrc_netif
  USEMODULE += gnrc_ipv6_default
  USEMODULE += gnrc_sock_udp
  USEMODULE += gnrc_icmpv6_echo
  USEMODULE += gnrc_ipv6_nib_dns
  CFLAGS += -DEMCUTE
  CFLAGS += -DCUBE_MESSAGING
else ifeq ($(CUBE_MESSAGING),tcp)
  USEMODULE += sock_dns
  USEMODULE += cube_messaging
  USEMODULE += auto_init_gnrc_netif
  USEMODULE += gnrc_ipv6_default
  USEMODULE += gnrc_sock_udp
  CFLAGS += -DTCP
  CFLAGS += -DCUBE_MESSAGING
else ifeq ($(CUBE_MESSAGING),paho)
  CFLAGS += -DPAHO
  CFLAGS += -DCUBE_MESSAGING
else
  $(error "CUBE_MESSAGING is not set or invalid")
endif


# # Generic Board and sensor requirements
# FEATURES_REQUIRED += periph_gpio
# FEATURES_REQUIRED += periph_i2c
# USEMODULE += periph_gpio_irq
# USEMODULE += pn532

# USEMODULE += esp_wifi
# # Wifi settings
# WIFI_SSID ?= "---redacted---"
# WIFI_PASS ?= "---redacted---"

# # Mqtt/ Mqtt-sn settings
# BROKER_HOST ?= "---redacted---"
# CLIENT_ID ?= "cube_1"
# USE_MQTTSN ?= 1
# # Nfc reader settings
# PN532_MODE = i2c
# # Mpu reader


# # Utility modules
# USEMODULE += xtimer


# USEMODULE += ps
# USEMODULE += i2c_scan

# # Mpu modules



# # Additional Packages
# USEPKG += jsmn





# ifneq (1, $(USE_MQTTSN))
#    include ./Makefile.paho
# else
# 	include ./Makefile.emcute
#   CFLAGS += -DEMCUTE_T_RETRY=5U
#   CFLAGS += -DEMCUTE_N_RETRY=1U
# endif




# # Define ESP flags


# CFLAGS += -DPN532_SUPPORT_I2C


# # Define flag USE_MQTT in c when USE_MQTT is true
# ifeq (0, $(USE_MQTT))
#   CFLAGS += -DUSE_MQTT
# endif




# USEMODULE += cube_led
# USEMODULE += cube_mpu
# USEMODULE += cube_mqtt
USEMODULE += cube_core
USEMODULE += cube_messaging
# USEMODULE += cube_mqtt

# EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_*
# EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_led
# EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_nfc
# EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_mqtt
# EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_button
EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_core
EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_mqtt
EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_messaging

include $(RIOTBASE)/Makefile.include
# Note: For some Reason this import should be done before defining the cflags


