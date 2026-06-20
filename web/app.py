from flask import Flask, render_template, jsonify
import subprocess, os

app = Flask(__name__)

EXE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "build", "Release", "dsa_engine.exe"))

SECTION_NAMES = ["DATA STRUCTURES", "ALGORITHMS", "CORE SYSTEMS", "SIMULATIONS"]

def parse_output(raw):
    raw = raw.replace('\r\n', '\n').replace('\r', '\n')
    sections = []
    current_title = None
    current_lines = []

    for line in raw.splitlines():
        stripped = line.strip().strip('=').strip()
        if stripped in SECTION_NAMES:
            if current_title:
                sections.append({"title": current_title, "content": "\n".join(current_lines).strip()})
            current_title = stripped
            current_lines = []
        elif current_title:
            current_lines.append(line)

    if current_title and current_lines:
        sections.append({"title": current_title, "content": "\n".join(current_lines).strip()})

    return sections

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/run")
def run():
    if not os.path.exists(EXE):
        return jsonify({"error": f"Executable not found: {EXE}"}), 404
    try:
        result = subprocess.run(
            [EXE], capture_output=True, text=True,
            encoding="utf-8", errors="replace", timeout=30
        )
        sections = parse_output(result.stdout)
        if not sections:
            return jsonify({"error": "No output parsed. Raw: " + result.stdout[:300]}), 500
        return jsonify({"sections": sections})
    except subprocess.TimeoutExpired:
        return jsonify({"error": "Engine timed out after 30 seconds"}), 500
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    print(f"EXE path: {EXE}")
    print("Open http://localhost:5000 in your browser")
    app.run(debug=False, port=5000)
