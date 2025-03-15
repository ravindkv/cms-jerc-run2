# json2latex/filters.py

def latex_escape(text):
    """Escape underscores for LaTeX."""
    if not isinstance(text, str):
        return text
    return text.replace('_', r'\_')

