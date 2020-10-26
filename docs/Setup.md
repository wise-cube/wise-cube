In oder to deploy the wise cube infrastructure we need the following things:

- Install `docker` and `docker-compose`

- Add ipv6 support to docker by creating `/etc/docker/daemon.json` and adding the following lines

  ```
  {
    "ipv6": true,
    "fixed-cidr-v6": "2001:db8:1::/64"
  }
  ```

- In order to deploy the backend move into the `src` folder and run 

  ```bash
  $ docker-compose up --build
  ```

- move into `src/riot-cube` and revise the `buid.sh` file, make sure that you set the current address of the broker host and your username, then attach the ESP8266 to the serial port and run 

  ```bash
  $ ./build.sh
  ```

  to flash the riot firmware for the cube