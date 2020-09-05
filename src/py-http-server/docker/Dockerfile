# pull official base image
FROM python:3.8.0-alpine# set work directory
WORKDIR /usr/src/ # set environment variables
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1RUN apk update && apk add postgresql-dev gcc python3-dev musl-dev# install dependencies
RUN pip install --upgrade pip

# copy project
COPY . /usr/src/

# install dependencies
RUN pip install --upgrade pip
COPY ./requirements.txt /usr/src/app/requirements.txt
RUN export LDFLAGS="-L/usr/local/opt/openssl/lib"
RUN pip install -r requirements.txt

RUN ls -la 

ENTRYPOINT ["run.sh"]
