"""
Opposed-flow premixed strained flame
====================================

This script simulates a lean hydrogen-oxygen flame stabilized in a strained
flowfield, with an opposed flow consisting of equilibrium products.

Requires: cantera >= 3.0, matplotlib >= 2.0

.. tags:: Python, combustion, 1D flow, premixed flame, strained flame
"""

from pathlib import Path
import matplotlib.pyplot as plt
import cantera as ct
import numpy as np
import time

mech = 'gri30.yaml'
gas = ct.Solution(mech)

phi = 0.8
Tu = 300 # Initial Temperature
Po = ct.one_atm # Atmospheric / Initial pressure

#gas.set_equivalence_ratio(phi, fuel="CH4:1", oxidizer="O2:0.233,N2:0.767", basis='mass')
oxidant_composition = 'O2:0.21, N2:0.79' 
gas.set_equivalence_ratio(phi, fuel="CH4:1", oxidizer="O2:0.21,N2:0.79", basis='mole')
gas.TP = Tu, Po


phi=0.8
gas.set_equivalence_ratio(phi, fuel="CH4:1", oxidizer="O2:0.21,N2:0.79", basis='mole')
W=gas.molecular_weights
ich4 = gas.species_index('CH4')
gas.TP = Tu, Po
width = 0.03
flame = ct.FreeFlame(gas, width=width)

# Define tolerances for the solver
flame.set_refine_criteria(ratio=3.0, slope=0.1, curve=0.1)

flame.soret_enabled = False
flame.transport_model = 'multicomponent'
#flame.transport_model = 'AVBP'
# Define logging level
loglevel = 0

#opposed_flame.show_solution()
start = time.time()
flame.solve(loglevel=loglevel, auto=True)
end = time.time()
t_taken = str(round(end-start,2))
z= flame.grid
T = flame.T

size = np.size(z)-1
grad = np.zeros(size)
for i in range(size):
  grad[i] = (T[i+1]-T[i])/(z[i+1]-z[i])

thickness = (max(T) -min(T)) / max(grad)

print('laminar flame thickness = ', thickness)
print('laminar flame speed = ', flame.velocity[0])


# %%
# Temperature and Heat Release Rate
# ---------------------------------
fig, ax1 = plt.subplots()

ax1.plot(flame.grid, flame.heat_release_rate / 1e6, color='C4')
ax1.set_ylabel('heat release rate [MW/m³]', color='C4')
ax1.set(xlabel='flame coordinate [m]')

ax2 = ax1.twinx()
ax2.plot(flame.grid, flame.T, color='C3')
ax2.set_ylabel('temperature [K]', color='C3')
plt.show()
