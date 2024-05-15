import sys

data = []

def parse_input(line):
    line = line.strip()
    if line.startswith("__Instr_onEnterFunc@@@"):
        func_name = line.split("@@@")[1]
        data.append({
            "type": "enter",
            "func_name": func_name,
            "args": {}
        })
    elif line.startswith("__Instr_beforeExit@@@"):
        func_name = line.split("@@@")[1]
        return_ptr = line.split("@@@")[2]
        data.append({
            "type": "exit",
            "func_name": func_name,
            "return_ptr": return_ptr,
        })
        # for i in range(0, len(data)):
        #     if data[i]["type"] == "enter" and "return_ptr" not in data[i]:
        #         data[i]["return_ptr"] = return_ptr
    elif line.startswith("__Instr_onArg@@@"):
        splitted = line.split("@@@")
        nth = int(splitted[1])
        args = splitted[2]
        assert data[-1]["type"] == "enter"
        data[-1]["args"][nth] = args

for line in sys.stdin:
    parse_input(line)

def func_with_args(func_name, args, return_ptr=None):
    max_arg = max(args.keys() if args else [-1])
    arg_str = ", ".join([f"{args[i] if i in args else '@FREE'}" for i in range(max_arg + 1)])
    return f"{func_name}({arg_str}) -> {return_ptr}"


def generate_layout(data):
    current_level = 0
    for i in data:
        if i["type"] == "enter":
            print("|  " * current_level + "|- " + func_with_args(i["func_name"], i["args"], ))#i["return_ptr"]))
            current_level += 1
        elif i["type"] == "exit":
            current_level -= 1

generate_layout(data)
