from flask import Flask
from flask_executor import Executor
from flask_shell2http import Shell2HTTP

#Flask application instance
app = Flask(__name__)

executor = Executor(app)
shell2http = Shell2HTTP(app=app, executor=executor, base_url_prefix="/scripts/")

def my_callback_fn(context, future):
    #optional user-defined callback function
    print(context, future.result())

#shell2http.register_command(endpoint="saythis", command_name="echo", callback_fn=my_callback_fn, decorators=[])
shell2http.register_command(endpoint="runtestscript", command_name="./scripts/testscript.sh")

if __name__ == '__main__':
    app.run(host='0.0.0.0')
    app.testing = True
    