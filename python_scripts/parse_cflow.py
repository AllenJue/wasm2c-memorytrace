# Path to the Cflow output file
# cflow_output_file = "/u/allenjue/cs380s/Final_Project/wabt/wasm2c/benchmarks/dhrystone/dhrystone_flow.txt"
cflow_output_file = "/u/allenjue/cs380s/Final_Project/wabt/wasm2c/examples/fibonacci/fib_flow.txt"

# Dictionary to store caller-callee relationships
caller_callee_map = {}

def parse_callees(lines, parent_function=None, parent_indent=0):
    """Parse callees based on indentation."""
    while lines:
        line = lines[0]
        stripped_line = line.strip()
        if not stripped_line:
            lines.pop(0)
            continue  # Skip empty lines

        # Detect the indentation of the current line
        current_indent = len(line) - len(stripped_line)

        if current_indent < parent_indent:
            # Indentation decreased, return to previous level
            return
        
        # Process the current line
        lines.pop(0)  # Consume the line
        if "()" not in stripped_line:
            continue  # Skip lines without a function

        # Extract the function name (before "()")
        caller_end = stripped_line.find("()")
        function_name = stripped_line[:caller_end].strip()

        # Extract parameter list (inside "<...>")
        signature_start = stripped_line.find("<")
        signature_end = stripped_line.find(">")
        signature = stripped_line[signature_start + 1:signature_end] if signature_start != -1 and signature_end != -1 else ""

        # If the parent function is defined, add this as its callee
        if parent_function and "var_p" in signature:
            if parent_function not in caller_callee_map:
                caller_callee_map[parent_function] = []
            caller_callee_map[parent_function].append(function_name)
        
        # Recur to parse the callees of this function, indent seems to go up by two spaces everytime
        parse_callees(lines, parent_function=function_name, parent_indent=current_indent + 2)

# Read the file and parse the lines
with open(cflow_output_file, "r") as file:
    lines = file.readlines()

parse_callees(lines)
for caller, callees in caller_callee_map.items():
    print(caller, ':', ','.join(callees), sep='')
