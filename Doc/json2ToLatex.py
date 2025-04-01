import json

def escape_underscores(s):
    """Escape underscores for LaTeX by replacing '_' with '\_'."""
    return s.replace("_", r"\_")

def json_to_latex(data, indent=0, highlight_last_in_list=False):
    """
    Recursively convert JSON data into LaTeX nested itemize environments.
    
    For every dictionary, the last key-value pair is printed such that if its value is a string,
    it is wrapped in blue color (using \textcolor{blue}{...}). Similarly, for a list that is the
    value of a dictionary’s last key, the last element is printed in blue if it is a string.
    """
    latex_str = ""
    spacing = " " * indent

    if isinstance(data, dict):
        latex_str += f"{spacing}\\begin{{itemize}}\n"
        items = list(data.items())
        for i, (key, value) in enumerate(items):
            is_last = (i == len(items) - 1)
            # Escape underscores in key and print it in bold.
            key_escaped = escape_underscores(key)
            latex_str += f"{spacing}  \\item \\textbf{{{key_escaped}}}:"
            # If the value is a container, recursively process it.
            if isinstance(value, (dict, list)):
                # For a dictionary, we want its last item highlighted.
                flag = is_last
                latex_str += "\n" + json_to_latex(value, indent + 4, highlight_last_in_list=flag)
            else:
                # For a simple value, escape underscores.
                value_str = escape_underscores(str(value))
                if is_last:
                    value_str = f"\\textcolor{{blue}}{{{value_str}}}"
                latex_str += " " + value_str + "\n"
        latex_str += f"{spacing}\\end{{itemize}}\n"
    elif isinstance(data, list):
        latex_str += f"{spacing}\\begin{{itemize}}\n"
        for i, item in enumerate(data):
            is_last = (i == len(data) - 1)
            latex_str += f"{spacing}  \\item "
            if isinstance(item, (dict, list)):
                # If processing a nested container from a dictionary’s last key, flag it.
                flag = highlight_last_in_list and is_last
                latex_str += "\n" + json_to_latex(item, indent + 4, highlight_last_in_list=flag)
            else:
                item_str = escape_underscores(str(item))
                if highlight_last_in_list and is_last:
                    item_str = f"\\textcolor{{blue}}{{{item_str}}}"
                latex_str += item_str + "\n"
        latex_str += f"{spacing}\\end{{itemize}}\n"
    else:
        latex_str += f"{spacing}{escape_underscores(str(data))}\n"
    return latex_str

# Load JSON data from a file (adjust the filename if needed)
with open("sample.json", "r") as f:
    data = json.load(f)

# Build the complete LaTeX document.
latex_document = (
    "\\documentclass{article}\n"
    "\\usepackage[utf8]{inputenc}\n"
    "\\usepackage{xcolor}\n"  # needed for color
    "\\begin{document}\n\n" +
    json_to_latex(data) +
    "\n\\end{document}"
)

# Write the output to a .tex file.
with open("output.tex", "w") as f:
    f.write(latex_document)

print("LaTeX file created as output.tex")

