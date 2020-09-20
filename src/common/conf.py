from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import scoped_session

import os

HOSTNAME='192.168.1.205'
PORT=5000
#SQLALCHEMY_DATABASE_URI = 'sqlite:///wise-cube.db'

SQLALCHEMY_DATABASE_URI = os.environ.get('SQLALCHEMY_DATABASE_URI', 'sqlite:///wise-cube.db')
if not os.environ.get('IS_DOCKER', None):
    SQLALCHEMY_DATABASE_URI = SQLALCHEMY_DATABASE_URI.replace('@db','@localhost')

SECRET_KEY = b'_5#y2L"F4Q8z\n\xec]/'
SQL_ENGINE = create_engine(SQLALCHEMY_DATABASE_URI, echo=False)
LocalSession = sessionmaker(bind=SQL_ENGINE)
ScopedSession = scoped_session(LocalSession)

Base = declarative_base()


