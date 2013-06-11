import sys
import gdb

# Update module path.
dir = '/sourcecode/arm-workspace/dbus-test/out/share/glib-2.0/gdb'
if not dir in sys.path:
    sys.path.insert(0, dir)

from gobject import register
register (gdb.current_objfile ())
