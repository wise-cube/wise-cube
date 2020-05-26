from flask import Flask, render_template, request
import database

app = Flask(__name__)


@app.route('/')
@app.route('/guide/<title>/<txt_q>', methods=['GET', 'POST'])
@app.route('/game/<question>/<a>/<b>/<c>/<d>/<txt_a>', methods=['GET', 'POST'])
def homepage():
    if request.method == 'GET':
        ty = request.args.get('t')
        title="prova titolo"
        txt_q="prova testo"
        txt_a="prova testo risp"
        a="prova a"
        b="prova b"
        c="prova d"
        d="prova D"
        question="prova domanda"
        if ty == "txt":
            return render_template("guide.html", title=title, txt_q=txt_q)
        else:
            return render_template("game.html", question=question, a=a, b=b, c=c, d=d, txt_a=txt_a)
if __name__ == '__main__':
    app.run()
