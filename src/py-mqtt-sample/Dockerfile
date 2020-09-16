
# set base image (host OS)
FROM python:3.8

# set the working directory in the container
WORKDIR /src

# copy the content of the local src directory to the working directory
COPY requirements.txt .

# install dependencies
RUN pip install -r requirements.txt

# command to run on container start
#ENTRYPOINT "/docker-entrypoint.sh"
CMD [ "python3", "./client.py" ]
