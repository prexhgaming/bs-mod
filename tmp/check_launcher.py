import re, os, glob
import xml.etree.ElementTree as ET

errs = []

files = ["app/src/main/res/layout/activity_game.xml", "app/src/main/res/values/colors.xml",
         "app/src/main/res/values/strings.xml", "app/src/main/res/values/themes.xml",
         "app/src/main/res/values/dimens.xml", "app/src/main/res/menu/menu_launcher.xml",
         "app/src/main/AndroidManifest.xml"]
files += glob.glob("app/src/main/res/drawable/bg_*.xml") + glob.glob("app/src/main/res/drawable/ic_play.xml")
for f in files:
    try:
        ET.parse(f)
    except Exception as e:
        errs.append("XML %s: %s" % (f, e))

layout = open("app/src/main/res/layout/activity_game.xml").read()
ids = re.findall(r'android:id="@\+id/([A-Za-z0-9_]+)"', layout)


def camel(s):
    p = s.split('_')
    return p[0] + ''.join(w[:1].upper() + w[1:] for w in p[1:])


bindings = set(camel(i) for i in ids)
ktsrc = open("app/src/main/java/top/niunaijun/blackboxa/view/game/GameActivity.kt").read()
no_imports = "\n".join(l for l in ktsrc.splitlines() if not l.startswith("import "))
used = set(re.findall(r'binding\.([A-Za-z0-9_]+)', no_imports))
missing = set(u for u in used if u not in bindings and u != "root")
if missing:
    errs.append("binding refs with no view id: %s" % sorted(missing))


def names(kind, folder_pat):
    out = set()
    for f in glob.glob(folder_pat):
        try:
            t = ET.parse(f).getroot()
        except Exception:
            continue
        for el in t:
            if el.tag == kind and el.get('name'):
                out.add(el.get('name'))
    return out


strings = names('string', "app/src/main/res/values/strings.xml")
colors = names('color', "app/src/main/res/values/colors.xml")
dimens = names('dimen', "app/src/main/res/values/dimens.xml")
styles = names('style', "app/src/main/res/values/themes.xml")


def drawable_exists(n):
    for d in glob.glob("app/src/main/res/drawable*"):
        if os.path.exists(os.path.join(d, n + ".xml")) or os.path.exists(os.path.join(d, n + ".9.png")):
            return True
    for d in glob.glob("app/src/main/res/mipmap*"):
        for ext in (".png", ".xml"):
            if os.path.exists(os.path.join(d, n + ext)):
                return True
    return False


# Kotlin sources touched this session
kots = ["app/src/main/java/top/niunaijun/blackboxa/view/game/GameActivity.kt",
        "app/src/main/java/top/niunaijun/blackboxa/view/game/GameViewModel.kt",
        "app/src/main/java/top/niunaijun/blackboxa/data/GameRepository.kt"]
kt = "\n".join(open(f).read() for f in kots)

xml_refs = layout
for prefix, pool, kind in [
    ("@string/", strings, "string"), ("@color/", colors, "color"),
    ("@dimen/", dimens, "dimen"), ("@style/", styles, "style")]:
    for n in sorted(set(re.findall(re.escape(prefix) + r'([A-Za-z0-9_.]+)', xml_refs))):
        if n not in pool:
            errs.append("missing %s (xml): %s" % (kind, n))

for n in sorted(set(re.findall(r'R\.string\.([A-Za-z0-9_]+)', kt))):
    if n not in strings:
        errs.append("missing string (kotlin): %s" % n)
for n in sorted(set(re.findall(r'R\.color\.([A-Za-z0-9_]+)', kt))):
    if n not in colors:
        errs.append("missing color (kotlin): %s" % n)
for n in sorted(set(re.findall(r'R\.dimen\.([A-Za-z0-9_]+)', kt))):
    if n not in dimens:
        errs.append("missing dimen (kotlin): %s" % n)
for n in sorted(set(re.findall(r'R\.drawable\.([A-Za-z0-9_]+)', kt))):
    if not drawable_exists(n):
        errs.append("missing drawable (kotlin): %s" % n)
for n in sorted(set(re.findall(r'R\.menu\.([A-Za-z0-9_]+)', kt))):
    if not os.path.exists("app/src/main/res/menu/%s.xml" % n):
        errs.append("missing menu (kotlin): %s" % n)

print("ids: %d | binding refs: %d | strings: %d" % (len(ids), len(used), len(strings)))
print("\n".join(errs) if errs else "OK - no problems found")
