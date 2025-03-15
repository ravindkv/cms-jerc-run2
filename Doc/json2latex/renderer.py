# json2latex/renderer.py

import json
from jinja2 import Environment, FileSystemLoader
from .filters import latex_escape

def render_template(json_path, template_path, output_path):
    # Load JSON data
    with open(json_path, 'r') as f:
        data = json.load(f)
    
    # Set up Jinja2 environment and register the latex_escape filter
    env = Environment(loader=FileSystemLoader('.'))
    env.filters['latex_escape'] = latex_escape

    # Load the template file
    template = env.get_template(template_path)
    
    # Render template with JSON data; here the JSON data is passed as 'data'
    rendered = template.render(data=data)
    
    # Write the rendered LaTeX file to output
    with open(output_path, 'w') as f:
        f.write(rendered)
    
    print(f"Generated {output_path}")

