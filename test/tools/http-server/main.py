import argparse
import json
import socket
import threading
import time
from typing import Any
from typing import Optional

from flask import Flask
from flask import request
from flask import Response
from werkzeug.serving import make_server

app = Flask(__name__)

s: Optional[socket.socket] = None

running = True


def send_json_data(data: dict[str, Any]) -> None:
    assert s is not None
    serialized_data = json.dumps(data) + "\0"
    s.sendall(serialized_data.encode())


@app.route("/", defaults={"path": ""}, methods=["GET", "POST"])
@app.route("/<path:path>", methods=["GET", "POST"])
def handle_request(path: str) -> Response:
    assert isinstance(path, str)
    req_data = {
        "endpoint": path,
        "method": request.method,
        "headers": dict(request.headers),
        "body": request.get_json(force=True, silent=True)
    }
    send_json_data(req_data)
    return Response("OK", status=200)


@app.route("/shutdown", methods=["POST"])
def shutdown() -> Response:
    global running
    running = False
    return Response("OK", status=200)


def main() -> None:
    print("program started")
    parser = argparse.ArgumentParser()
    parser.add_argument("--socket-port", type=int, help="TCP socket port")
    args = parser.parse_args()

    global s
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        print("connecting socket...")
        s.connect(("localhost", args.socket_port))
        print("connected")

        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as test_socket:
            test_socket.bind(("localhost", 0))
            webserver_port = test_socket.getsockname()[1]

        s.sendall(f"{webserver_port}\0".encode())

        server = make_server("localhost", webserver_port, app)
        thread = threading.Thread(target=server.serve_forever)
        print("starting HTTP server...")
        thread.start()
        while running:
            time.sleep(0.1)

        print("shutting down...")
        server.shutdown()


if __name__ == "__main__":
    main()
