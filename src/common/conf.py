from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import scoped_session

HOSTNAME='192.168.1.205'
PORT=5000
SQLALCHEMY_DATABASE_URI = 'sqlite:///wise-cube.db'
SECRET_KEY = b'_5#y2L"F4Q8z\n\xec]/'
SQL_ENGINE = create_engine(SQLALCHEMY_DATABASE_URI, echo=True)
GLOBAL_SESSION = sessionmaker(bind=SQL_ENGINE)


Base = declarative_base()
FlaskSession = scoped_session(GLOBAL_SESSION)
HandlerSession = scoped_session(GLOBAL_SESSION)

