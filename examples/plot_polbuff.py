#!/usr/bin/env python
#
# Python script demonstrating how to read in
# and plot yarg_rt polbuff files

import matplotlib.pyplot as plt
import numpy as np
import h5py as h5

#

def read_polbuff(file_name):

    # Read polbuff file

    with h5.File(file_name, 'r') as f:

        S_d = f['stokes_direct'][...]
        S_i = f['stokes_indirect'][...]

        n_x = f.attrs['n_x']
        n_y = f.attrs['n_x']

        x_min = f.attrs['x_min']
        y_min = f.attrs['y_min']

        dx = f.attrs['dx']
        dy = f.attrs['dy']

    # Extract data

    # Direct photons

    I_d = np.transpose(S_d[:,:,0])
    Q_d = np.transpose(S_d[:,:,1])
    U_d = np.transpose(S_d[:,:,2])
    V_d = np.transpose(S_d[:,:,2])

    # Indirect (scattered) photons

    I_i = np.transpose(S_i[:,:,0])
    Q_i = np.transpose(S_i[:,:,1])
    U_i = np.transpose(S_i[:,:,2])
    V_i = np.transpose(S_i[:,:,3])

    x = x_min + dx*(0.5 + np.arange(n_x))
    y = y_min + dy*(0.5 + np.arange(n_x))

    return {
        'I_d': I_d,
        'Q_d': Q_d,
        'U_d': U_d,
        'V_d': V_d,
        'I_i': I_i,
        'Q_i': Q_i,
        'U_i': U_i,
        'V_i': V_i,
        'x_min': x_min,
        'y_min': y_min,
        'dx': dx,
        'dy': dy,
        'n_x': n_x,
        'n_y': n_y}
        
# Read the polbuff

p = read_polbuff('polbuff.h5')

extent = [
    p['x_min'],
    p['x_min']+p['dx']*p['n_x'],
    p['y_min'],
    p['y_min']+p['dy']*p['n_y']]

# Create the figure

fig, ax = plt.subplots(2, 2, figsize=[10,10])

ax[0][0].imshow(p['I_d'], extent=extent)#, title='I (direct)')
ax[0][1].imshow(p['I_i'], extent=extent)#, title='I (indirect)')
ax[1][0].imshow(p['Q_i'], extent=extent)#, title='Q (indirect)')
ax[1][1].imshow(p['U_i'], extent=extent)#, title='U (indirect)')

fig.tight_layout()

fig.savefig('polbuff.pdf')





