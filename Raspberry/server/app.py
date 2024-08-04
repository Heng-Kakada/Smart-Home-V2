from flask import Flask, request

app = Flask(__name__)

@app.route('/')
def index():
    return "Hello World"

def run_server():
    app.run(debug=True, use_reloader=False)

if __name__ == '__main__':
    run_server()