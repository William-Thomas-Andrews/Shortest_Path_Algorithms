FROM python:3.13-slim

# Install dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    graphviz \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside container
WORKDIR /usr/src/shortest_path

# Copy current directory contents into container
COPY . .

# Install Flask inside container
RUN pip install --no-cache-dir flask

# Expose port 8000 for Flask
EXPOSE 8000

# Run Flask app
CMD ["python", "app.py"]