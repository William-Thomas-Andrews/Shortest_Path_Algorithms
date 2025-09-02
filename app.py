from flask import Flask, send_from_directory
import subprocess

app = Flask(__name__)

# Serve HTML
@app.route('/')
def index():
    return send_from_directory('.', 'test.html')

# Generate new graph
@app.route('/generate_0')
def generate_0():
    # Run C++ program to update g.gv
    subprocess.run(["g++", "hello_world_0.cpp", "-o", "hello_world_0"], check=True)
    subprocess.run(["./hello_world_0"], check=True)

    # subprocess.run(["mv", "file1.png", "file0.png"], check=True)

    # Generate PNG from updated g.gv
    subprocess.run(["neato", "-n2", "-Tpng", "g.gv", "-o", "file0.png"], check=True)

    return "OK"

# Generate new graph
@app.route('/generate_1')
def generate_1():
    # Run C++ program to update g.gv
    subprocess.run(["g++", "hello_world_1.cpp", "-o", "hello_world_1"], check=True)
    subprocess.run(["./hello_world_1"], check=True)

    # subprocess.run(["mv", "file1.png", "file2.png"], check=True)

    # Generate PNG from updated g.gv
    subprocess.run(["neato", "-n2", "-Tpng", "g.gv", "-o", "file1.png"], check=True)

    return "OK"

# Generate new graph
@app.route('/generate_2')
def generate_2():
    # Run C++ program to update g.gv
    subprocess.run(["g++", "hello_world_2.cpp", "-o", "hello_world_2"], check=True)
    subprocess.run(["./hello_world_2"], check=True)

    # subprocess.run(["mv", "file1.png", "file2.png"], check=True)

    # Generate PNG from updated g.gv
    subprocess.run(["neato", "-n2", "-Tpng", "g.gv", "-o", "file2.png"], check=True)

    return "OK"


# Serve static files (image)
@app.route('/<path:path>')
def static_files(path):
    return send_from_directory('.', path)

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=8000)