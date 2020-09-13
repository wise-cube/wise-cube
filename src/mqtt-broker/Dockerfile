FROM eclipse-mosquitto:latest

COPY  ./conf /mosquitto/config
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD [ "mosquitto", "-v", "-c", "/mosquitto/config/mosquitto.conf" ]