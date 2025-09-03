from flask import Flask, send_from_directory, jsonify
import subprocess
import os
import logging
import time

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = Flask(__name__)

# Serve HTML
@app.route('/')
def index():
    return send_from_directory("static", "test.html")

@app.route("/test.html")
def serve_test():
    return send_from_directory("static", "test.html")

def run_algorithm_script(script_name, algo_name):
    try:
        logger.info(f"Starting {algo_name} algorithm...")
        logger.info(f"Current working directory: {os.getcwd()}")
        logger.info(f"Contents of scripts directory: {os.listdir('scripts') if os.path.exists('scripts') else 'scripts/ not found'}")
        
        # Check if script exists
        script_path = os.path.join("scripts", script_name)
        if not os.path.exists(script_path):
            raise FileNotFoundError(f"Script {script_path} not found")
        
        # Make sure script is executable
        os.chmod(script_path, 0o755)
        logger.info(f"Running script: {script_path}")
        
        # Run the script with more verbose output
        result = subprocess.run(
            [f"./{script_name}"], 
            cwd="scripts",
            check=True,
            capture_output=True,
            text=True,
            timeout=120,
            env=dict(os.environ, PYTHONUNBUFFERED="1")  # Force unbuffered output
        )
        
        logger.info(f"{algo_name} script completed")
        logger.info(f"Script stdout: '{result.stdout}'")
        logger.info(f"Script stderr: '{result.stderr}'")
        
        # Check if display0.png was created/updated in static directory
        display_path = os.path.join(os.getcwd(), "static", "display0.png")
        logger.info(f"Checking for image at: {display_path}")
        
        if os.path.exists(display_path):
            # Get file modification time to verify it's recent
            mod_time = os.path.getmtime(display_path)
            current_time = time.time()
            time_diff = current_time - mod_time
            logger.info(f"Image found, modified {time_diff:.1f} seconds ago")
            
            if time_diff < 30:  # Modified within last 30 seconds (increased from 10)
                logger.info("display0.png updated successfully in static directory")
                return {"status": "success", "message": f"{algo_name} completed"}
            else:
                logger.warning(f"display0.png exists but wasn't recently modified ({time_diff:.1f}s ago)")
        else:
            logger.error(f"display0.png not found at expected path: {display_path}")
            # List contents of static directory for debugging
            static_dir = os.path.join(os.getcwd(), "static")
            if os.path.exists(static_dir):
                static_contents = os.listdir(static_dir)
                logger.info(f"Contents of static directory: {static_contents}")
            else:
                logger.error("static/ directory doesn't exist!")
        
        return {"status": "success", "message": f"{algo_name} completed", "warning": "Image may not have been updated"}
        
    except subprocess.TimeoutExpired:
        logger.error(f"{algo_name} timed out after 120 seconds")
        return {"status": "error", "message": f"{algo_name} timed out"}
    
    except subprocess.CalledProcessError as e:
        logger.error(f"{algo_name} failed: {e.stderr}")
        return {"status": "error", "message": f"{algo_name} failed: {e.stderr}"}
    
    except FileNotFoundError as e:
        logger.error(f"Script not found: {e}")
        return {"status": "error", "message": f"Script not found: {e}"}
    
    except Exception as e:
        logger.error(f"Unexpected error in {algo_name}: {str(e)}")
        return {"status": "error", "message": f"Unexpected error: {str(e)}"}

# Generate new graph with Dijkstra's algorithm
@app.route('/generate_0')
def generate_0():
    result = run_algorithm_script("run_d.sh", "Dijkstra's Algorithm")
    return jsonify(result)

# Generate new graph with A* algorithm
@app.route('/generate_1')
def generate_1():
    result = run_algorithm_script("run_a.sh", "A* Algorithm")
    return jsonify(result)

# Generate new graph with Multi-threaded A* algorithm
@app.route('/generate_2')
def generate_2():
    result = run_algorithm_script("run_ma.sh", "Multi-threaded A* Algorithm")
    return jsonify(result)

# Health check endpoint
@app.route('/health')
def health_check():
    """
    Check if all required components are available
    """
    checks = {
        "scripts_dir": os.path.exists("scripts"),
        "run_d_script": os.path.exists("scripts/run_d.sh"),
        "run_a_script": os.path.exists("scripts/run_a.sh"),
        "run_ma_script": os.path.exists("scripts/run_ma.sh"),
        "static_dir": os.path.exists("static"),
        "test_html": os.path.exists("static/test.html"),
        "display0_png": os.path.exists("static/display0.png")
    }
    
    all_good = all(checks.values())
    status_code = 200 if all_good else 500
    
    return jsonify({
        "status": "healthy" if all_good else "unhealthy",
        "checks": checks,
        "current_directory": os.getcwd(),
        "directory_contents": os.listdir(".")
    }), status_code

# Serve static files (images, CSS, JS, etc.)
@app.route('/static/<path:filename>')
def serve_static(filename):
    return send_from_directory('static', filename)

# Serve display images specifically from static directory
@app.route('/display<int:num>.png')
def serve_display_image(num):
    filename = f"display{num}.png"
    static_path = os.path.join("static", filename)
    logger.info(f"Attempting to serve {filename} from static directory")
    logger.info(f"Looking for file at: {os.path.abspath(static_path)}")
    
    if os.path.exists(static_path):
        logger.info(f"File found, serving: {filename}")
        return send_from_directory('static', filename)
    else:
        logger.error(f"Display image not found: {static_path}")
        # List contents of static directory
        if os.path.exists("static"):
            contents = os.listdir("static")
            logger.info(f"Static directory contents: {contents}")
        return jsonify({"error": f"Display image not found: {filename}"}), 404

# Serve other files from root directory
@app.route('/<path:path>')
def static_files(path):
    # Skip display images as they're handled above
    if path.startswith('display') and path.endswith('.png'):
        return serve_display_image(int(path[7:-4]))
    
    if os.path.exists(path):
        directory = os.path.dirname(path) or '.'
        filename = os.path.basename(path)
        return send_from_directory(directory, filename)
    else:
        logger.warning(f"File not found: {path}")
        return jsonify({"error": f"File not found: {path}"}), 404

@app.errorhandler(404)
def not_found_error(error):
    return jsonify({"error": "Resource not found"}), 404

@app.errorhandler(500)
def internal_error(error):
    return jsonify({"error": "Internal server error"}), 500

if __name__ == '__main__':
    # Log startup information
    logger.info(f"Starting Flask app in directory: {os.getcwd()}")
    logger.info(f"Directory contents: {os.listdir('.')}")
    
    # Check if required directories exist
    if not os.path.exists("scripts"):
        logger.error("scripts/ directory not found!")
    if not os.path.exists("static"):
        logger.error("static/ directory not found!")
    
    app.run(host="0.0.0.0", port=8000, debug=False)