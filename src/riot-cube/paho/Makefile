APPLICATION = paho-mqtt-example

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
WIFI_SSID ?= "AirPort Network"
WIFI_PASS ?= "netgear2014"


USEMODULE += periph_gpio_irq
USEMODULE += xtimer
USEMODULE += mpu9150
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps
USEMODULE += saul_default

USEPKG += paho-mqtt
USEPKG += jsmn

FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_i2c


# Networking with lwip
USEMODULE += netdev_default

LWIP_IPV4 ?= 1

ifneq (0, $(LWIP_IPV4))
	USEMODULE += ipv4_addr
	USEMODULE += lwip_arp
	USEMODULE += lwip_ipv4
	USEMODULE += lwip_dhcp_auto
	USEMODULE += lwip_sock_udp
	CFLAGS += -DETHARP_SUPPORT_STATIC_ENTRIES=1
	LWIP_IPV6 ?= 0
else
	LWIP_IPV6 ?= 1
endif
	
ifneq (0, $(LWIP_IPV6))
	USEMODULE += ipv6_addr
	USEMODULE += lwip_ipv6_autoconfig
endif
    
USEMODULE += lwip_netdev
USEMODULE += lwip lwip_sock_ip
USEMODULE += lwip_tcp lwip_sock_tcp
USEMODULE += lwip_sock_async
    
USEMODULE += sock_async_event
   
####
include $(RIOTBASE)/Makefile.include

ifneq (,$(filter arch_esp,$(FEATURES_USED)))
  CFLAGS += -DESP_WIFI_SSID=\"$(WIFI_SSID)\"
  CFLAGS += -DESP_WIFI_PASS=\"$(WIFI_PASS)\"
  CFLAGS += -DBROKER_HOST=\"$(IP)\"
endif
