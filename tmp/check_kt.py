import re

src = open("app/src/main/java/top/niunaijun/blackboxa/view/game/GameActivity.kt").read()

# strip strings and comments so braces inside literals do not skew the count
clean = re.sub(r'"""[\s\S]*?"""', '""', src)
clean = re.sub(r'"(\\.|[^"\\])*"', '""', clean)
clean = re.sub(r'//[^\n]*', '', clean)
clean = re.sub(r'/\*[\s\S]*?\*/', '', clean)

for op, cl in (("{", "}"), ("(", ")"), ("[", "]")):
    print("%s%s balance: %d" % (op, cl, clean.count(op) - clean.count(cl)))

funcs = re.findall(r'\bprivate fun ([A-Za-z0-9_]+)', src) + re.findall(r'\bfun ([A-Za-z0-9_]+)\(', src)
dupes = set(f for f in funcs if funcs.count(f) > 1)
print("functions:", len(funcs), "| duplicates:", sorted(dupes) if dupes else "none")
print("declared:", ", ".join(sorted(set(funcs))))
