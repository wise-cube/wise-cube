# General Configuration

# Application name
APPLICATION = wise-cube
# Default BOARD if it is not overwritten
BOARD ?= esp8266-esp-12x
# Default RIOTBASE directoty 
RIOTBASE ?= $(CURDIR)/../../../../RIOT
# Default debug stats
DEVELHELP ?= 0
LOG_LEVEL = LOG_INFO
QUIET ?= 1

# Debugging tools, set DEBUG to 1 to enable them
DEBUG?=1
ifneq (1, $(DEBUG))
    DEVELHELP ?= 0
    LOG_LEVEL = LOG_INFO
    QUIET ?= 1
endif



# Generic Board and sensor requirements
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_i2c
USEMODULE += periph_gpio_irq
USEMODULE += pn532
USEMODULE += mpu9150
USEMODULE += esp_wifi
# Wifi settings
WIFI_SSID ?= "---redacted---"
WIFI_PASS ?= "---redacted---"

# Mqtt/ Mqtt-sn settings
BROKER_HOST ?= "---redacted---"
CLIENT_ID ?= "cube_1"
USE_MQTTSN ?= 1
# Nfc reader settings
PN532_MODE = i2c
# Mpu reader


# Utility modules
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += i2c_scan
USEMODULE += sntp
# Mpu modules
USEMODULE += netdev_default


# Additional Packages
USEPKG += jsmn





ifneq (1, $(USE_MQTTSN))
   include ./Makefile.paho
else
	include ./Makefile.emcute
  CFLAGS += -DEMCUTE_T_RETRY=5U
  CFLAGS += -DEMCUTE_N_RETRY=1U
endif




# Define ESP flags

CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
CFLAGS += -DPN532_SUPPORT_I2C


# Define flag USE_MQTT in c when USE_MQTT is true
ifeq (0, $(USE_MQTT))
  CFLAGS += -DUSE_MQTT
endif

USEMODULE += cube_led

EXTERNAL_MODULE_DIRS += $(CURDIR)/cube_led



# Note: For some Reason this import should be done before defining the cflags
include $(RIOTBASE)/Makefile.include

