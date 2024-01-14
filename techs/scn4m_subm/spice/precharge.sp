.SUBCKT tri_gate bl br en_bar vdd gnd

M1000 vdd en_bar bl vdd p w=1.6u l=0.4u
M1001 vdd en_bar br vdd p w=1.6u l=0.4u
M1002 bl en_bar br vdd p w=1.6u l=0.4u

.ENDS