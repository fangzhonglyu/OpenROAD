# gcd_nangate45 IO placement
from openroad import Design, Tech
import helpers
import ppl_aux

tech = Tech()
tech.readLef("Nangate45/Nangate45.lef")
design = Design(tech)
design.readDef("gcd.def")

ppl_aux.set_io_pin_constraint(design, direction="INPUT", region="top:*")
ppl_aux.set_io_pin_constraint(design, direction="OUTPUT", region="bottom:*")

ppl_aux.place_pins(design, hor_layers="metal3", ver_layers="metal2", random=True)

def_file = helpers.make_result_file("random3.def")
design.writeDef(def_file)
helpers.diff_files("random3.defok", def_file)
