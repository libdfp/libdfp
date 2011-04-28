# gdb_start_address.py created by Ryan S. Arnold, April 2011.  No
# attribution required or necessary to use/reuse/copy/modify this
# function/script.

import re
class StartAddress(gdb.Function):
	"""Returns the start address of a library or executable from info
	proc mappings."""

	def __init__(self):
		super (StartAddress, self).__init__ ("start_address")

	def invoke(self, library):
		mappings = gdb.execute("info proc mappings", to_string=True)
		lines = mappings.split('\n')
		to_mappings = ""
		for line in lines:
			if (to_mappings != "true"):
				if (re.search("Start Addr", line)):
					to_mappings = "true"
				continue
			else:

				# The first match is the .text segment. Make sure to
				# match on "/libdfp-1.0.7.so" when passed "libdfp" and
				# not on "/libdfp" in the following example:
				# /home/ryanarn/libdfp/build64_power6/libdfp-1.0.7.so
				if (re.search("/" + library.string() + "[^/]*$", line)):
					chunks = line.split()
					return int(str(chunks[0]),16)
		return 0x0

StartAddress ()
