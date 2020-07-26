from flask_sqlalchemy_session import flask_scoped_session
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
from flask import Flask


HOSTNAME='192.168.1.205'
PORT=5000
SQLALCHEMY_DATABASE_URI = 'sqlite:///wise-cube.db'

ENGINE = None
if ENGINE is None:
    ENGINE = create_engine(SQLALCHEMY_DATABASE_URI, echo=True)

FlaskApp = Flask(__name__, static_folder='static', static_url_path='/')
FlaskApp.secret_key = b'_5#y2L"F4Q8z\n\xec]/'

Session = sessionmaker(bind=ENGINE)
ScopedSession = flask_scoped_session(Session, FlaskApp)
