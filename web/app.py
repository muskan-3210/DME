from flask import Flask, render_template_string, jsonify
import subprocess, re, os

app = Flask(__name__)

EXE = os.path.join(os.path.dirname(__file__), "..", "build", "Release", "dsa_engine.exe")

SECTIONS = [
    "DATA STRUCTURES",
    "ALGORITHMS",
    "CORE SYSTEMS",
    "SIMULATIONS",
]

HTML = """
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>DSA Master Engine</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body { font-family: 'Segoe UI', sans-serif; background: #0d1117; color: #c9d1d9; }

  header {
    background: linear-gradient(135deg, #0f3460 0%, #1a1a2e 100%);
    padding: 32px 40px 24px;
    border-bottom: 3px solid #e94560;
  }
  header h1 { font-size: 2rem; color: #fff; letter-spacing: 1px; }
  header p  { color: #aaa; margin-top: 6px; font-size: 0.95rem; }

  .badge {
    display: inline-block; padding: 3px 10px; border-radius: 12px;
    font-size: 0.78rem; font-weight: 600; margin-top: 10px;
    background: #238636; color: #fff;
  }

  nav {
    display: flex; gap: 12px; padding: 18px 40px;
    background: #161b22; border-bottom: 1px solid #30363d;
    flex-wrap: wrap;
  }
  nav button {
    padding: 7px 18px; border-radius: 6px; border: 1px solid #30363d;
    background: #21262d; color: #c9d1d9; cursor: pointer;
    font-size: 0.88rem; transition: all .15s;
  }
  nav button:hover, nav button.active {
    background: #0f3460; border-color: #e94560; color: #fff;
  }

  #run-btn {
    margin-left: auto; background: #e94560; border-color: #e94560;
    color: #fff; font-weight: 600; padding: 7px 22px;
  }
  #run-btn:hover { background: #c73652; }
  #run-btn:disabled { background: #555; border-color: #555; cursor: not-allowed; }

  main { padding: 28px 40px; max-width: 1200px; margin: 0 auto; }

  .section-card {
    background: #161b22; border: 1px solid #30363d;
    border-radius: 10px; margin-bottom: 20px; overflow: hidden;
  }
  .section-card h2 {
    padding: 14px 20px; background: #0f3460;
    color: #fff; font-size: 1rem; letter-spacing: 0.5px;
    border-bottom: 2px solid #e94560;
  }
  .section-card pre {
    padding: 18px 20px; font-family: 'Consolas', 'Courier New', monospace;
    font-size: 0.85rem; line-height: 1.7; white-space: pre-wrap;
    word-break: break-word; color: #c9d1d9;
  }

  .pass  { color: #3fb950; font-weight: 600; }
  .label { color: #58a6ff; }
  .value { color: #e3b341; }
  .sep   { color: #e94560; }

  #status {
    padding: 10px 20px; background: #161b22;
    border: 1px solid #30363d; border-radius: 8px;
    margin-bottom: 20px; font-size: 0.88rem;
  }
  #status.running { color: #e3b341; }
  #status.done    { color: #3fb950; }
  #status.error   { color: #f85149; }

  .spinner {
    display: inline-block; width: 12px; height: 12px;
    border: 2px solid #e3b341; border-top-color: transparent;
    border-radius: 50%; animation: spin .7s linear infinite;
    margin-right: 8px; vertical-align: middle;
  }
  @keyframes spin { to { transform: rotate(360deg); } }

  footer {
    text-align: center; padding: 24px; color: #555;
    font-size: 0.82rem; border-top: 1px solid #21262d;
    margin-top: 40px;
  }
</style>
</head>
<body>

<header>
  <h1>DSA Master Engine</h1>
  <p>Advanced C++ system — 20+ data structures &bull; 30+ algorithms &bull; 4 simulations</p>
  <span class="badge">C++17 &nbsp;|&nbsp; CMake &nbsp;|&nbsp; FAANG-level</span>
</header>

<nav>
  <button class="active" onclick="showAll(this)">All</button>
  <button onclick="filter('DATA STRUCTURES', this)">Data Structures</button>
  <button onclick="filter('ALGORITHMS', this)">Algorithms</button>
  <button onclick="filter('CORE SYSTEMS', this)">Core Systems</button>
  <button onclick="filter('SIMULATIONS', this)">Simulations</button>
  <button id="run-btn" onclick="runEngine()">&#9654; Run Engine</button>
</nav>

<main>
  <div id="status" style="display:none"></div>
  <div id="output"></div>
</main>

<footer>DSA Master Engine &mdash; Built with engineering-first thinking &mdash; 2026</footer>

<script>
let allSections = [];

function colorize(text) {
  return text
    .replace(/(PASS:.*)/g, '<span class="pass">$1</span>')
    .replace(/(---[^-\n]+---)/g, '<span class="sep">$1</span>')
    .replace(/(\w[\w\s]*=)(\d+[\d.]*)/g, '<span class="label">$1</span><span class="value">$2</span>')
    .replace(/(score=[\d.]+)/g, '<span class="value">$1</span>')
    .replace(/(match=\d+%)/g, '<span class="pass">$1</span>')
    .replace(/(\$\d+)/g, '<span class="value">$1</span>');
}

function render(sections) {
  const out = document.getElementById('output');
  out.innerHTML = '';
  sections.forEach(s => {
    const card = document.createElement('div');
    card.className = 'section-card';
    card.dataset.section = s.title;
    card.innerHTML = `<h2>${s.title}</h2><pre>${colorize(s.content)}</pre>`;
    out.appendChild(card);
  });
}

function showAll(btn) {
  document.querySelectorAll('nav button').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  render(allSections);
}

function filter(name, btn) {
  document.querySelectorAll('nav button').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  render(allSections.filter(s => s.title === name));
}

function setStatus(msg, cls) {
  const el = document.getElementById('status');
  el.style.display = 'block';
  el.className = cls;
  el.innerHTML = msg;
}

async function runEngine() {
  const btn = document.getElementById('run-btn');
  btn.disabled = true;
  btn.textContent = 'Running...';
  setStatus('<span class="spinner"></span> Running DSA Master Engine...', 'running');

  try {
    const res = await fetch('/run');
    const data = await res.json();
    if (data.error) {
      setStatus('Error: ' + data.error, 'error');
    } else {
      allSections = data.sections;
      render(allSections);
      setStatus('All demos completed successfully &nbsp; &#10003;', 'done');
    }
  } catch(e) {
    setStatus('Failed to connect to server: ' + e.message, 'error');
  }

  btn.disabled = false;
  btn.innerHTML = '&#9654; Run Engine';
}

// auto-run on page load
window.onload = runEngine;
</script>
</body>
</html>
"""

def parse_output(raw):
    raw = raw.replace('\r\n', '\n')
    # strip the outer header/footer
    lines = raw.splitlines()
    sections = []
    current_title = None
    current_lines = []
    section_names = {"DATA STRUCTURES", "ALGORITHMS", "CORE SYSTEMS", "SIMULATIONS"}

    for line in lines:
        stripped = line.strip().strip('=').strip()
        if stripped in section_names:
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
    return render_template_string(HTML)

@app.route("/run")
def run():
    exe = os.path.abspath(EXE)
    if not os.path.exists(exe):
        return jsonify({"error": f"Executable not found at {exe}. Build the project first."}), 404
    try:
        result = subprocess.run(
            [exe], capture_output=True, text=True,
            encoding="utf-8", errors="replace", timeout=30
        )
        sections = parse_output(result.stdout)
        if not sections:
            return jsonify({"error": "No output parsed. Raw: " + result.stdout[:500]}), 500
        return jsonify({"sections": sections})
    except subprocess.TimeoutExpired:
        return jsonify({"error": "Engine timed out after 30 seconds"}), 500
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    print("Starting DSA Master Engine web UI...")
    print("Open http://localhost:5000 in your browser")
    app.run(debug=False, port=5000)
