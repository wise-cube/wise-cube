APPLICATION = wise-cube
# If no BOARD is found in the environment, use this default:
BOARD ?= esp8266-esp-12x

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1
# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

DEBUG=1
WIFI_SSID ?= "---redacted---"
WIFI_PASS ?= "---redacted---"
BROKER_HOST ?= "---redacted---"
CLIENT_ID ?= "cube_1"

USE_MQTTSN ?= 1

USEMODULE += pn532
PN532_MODE ?= i2c


FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_i2c

USEMODULE += periph_gpio_irq
USEMODULE += xtimer

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += i2c_scan

USEMODULE += netdev_default
USEMODULE += mpu9150

USEPKG += jsmn
USEMODULE += esp_wifi

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

ifeq ($(PN532_MODE),i2c)
  CFLAGS += -DPN532_SUPPORT_I2C
endif
ifeq ($(PN532_MODE),spi)
  CFLAGS += -DPN532_SUPPORT_SPI
endif

ifeq (0, $(USE_MQTT))
  CFLAGS += -DUSE_MQTT
endif
